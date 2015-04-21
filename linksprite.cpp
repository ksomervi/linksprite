// Filename: linksprite.cpp
// Author: Kevin Somervill <kevin@somervill.org>
// Date: Thu Apr  9 15:05:44 EDT 2015
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <fstream>
using std::ofstream;

#include <ios>

#include "serialport/communication_manager.h"
#include "linksprite.h"

namespace blip {

linksprite::format_option::format_option() : value(), desc() {
  //value = v;
  //desc = s;
}

linksprite::format_option::format_option(unsigned char v, string s) {
  value = v;
  desc = s;
}

// Private functions
void linksprite::_init_image_sizes() {
  int i = 0;
  _img_sizes[i++] = format_option(0x22, string("160x120"));
  _img_sizes[i++] = format_option(0x11, string("320x240"));
  _img_sizes[i++] = format_option(0x00, string("640x480"));
  _img_sizes[i++] = format_option(0x1D, string("800x600"));
  _img_sizes[i++] = format_option(0x1C, string("1024x768"));
  _img_sizes[i++] = format_option(0x1B, string("1280x960"));
  _img_sizes[i++] = format_option(0x21, string("1600x1200"));
}//end _init_image_sizes() 

bool linksprite::_validate_msg(unsigned char *e, unsigned char *a, int n) {
  for (int i=0; i<n; i++) {
    if (e[i] != a[i]) {
      cout << "Byte[" << i << "]: expected '0x" << std::hex << e[i] 
        << "' but received '0x" << std::hex << a[i] << "'" << endl;
      return false;
    }
  }
  return true;
}//end _validate_msg()

linksprite::linksprite() {
  _cm = new communication_manager();
  _buffered_image_size = INVALID_SIZE;
  _init_image_sizes();
}

bool linksprite::open(const char *port, int baud) {
  return _cm->open_session(port, baud);
}//end open()

bool linksprite::is_open() {
  return _cm->session_is_open();
}

void linksprite::close() {
  _cm->close_session();
}//end close()

char * linksprite::port_name() {
  return _cm->port_name();
}//end portname()

int linksprite::baudrate() {
  return _cm->baudrate();
}//end baudrate()

void linksprite::_print_buf(const char *name, unsigned char *b, int len) {
  cout << name << ":";
  for (int i=0; i<len; i++) {
    cout << " " << std::hex << (unsigned int)b[i];
  }
  cout << endl;
}//end _print_buf()

void linksprite::reset() {
  //TODO unsigned char vbuf[] = {
  //TODO 0xd, 0xa, 0x49, 0x6e, 0x69, 0x74, 0x20, 0x65, 0x6e, 0x64, 0xd, 0xa};
  unsigned char txbuf[4] = {0x56, 0, 0x26, 0x00};
  cout << "Resetting camera..." << endl;
  _print_buf((char*)"tx", txbuf, 4);
  _cm->send(txbuf, 4);
  usleep(1000);
  // Expected response:
  // rx: d a 49 6e 69 74 20 65 6e 64 d a
  _cm->receive(_rxbuf, 12);
  _print_buf((char*)"rx", _rxbuf, 12);
}//end reset()

unsigned int linksprite::read_image_size() {
  //TODO unsigned char vbuf[] = {0x76, 0x0, 0x34, 0x0, 0x4, 0x0};
  unsigned char txbuf[5] = {0x56, 0, 0x34, 0x01, 0};
  cout << "Read image size..." << endl;
  _print_buf((char*)"tx", txbuf, 5);

  _cm->send(txbuf, 5);
  usleep(1000);
  _cm->receive(_rxbuf, 9);
  // Expected response:
  // rx: 76 0 34 0 4 0 0 4e ae
  _print_buf((char*)"rx", _rxbuf, 9);
  unsigned int img_sz = ((unsigned int)_rxbuf[6] << 16) & 0x00FF0000;
  img_sz += ((unsigned int)_rxbuf[7] << 8) & 0x0000FF00;
  img_sz += (unsigned int)_rxbuf[8] & 0x000000FF;
  return img_sz;
}//end read_image_size()

void linksprite::set_image_size() {
  unsigned int sz = INVALID_SIZE;
  unsigned char s = 0x11;
  cout << "Set image size..." << endl;

  do {
    for (unsigned int i=0; i<_img_sizes.size(); i++) {
      cout << i << ": " << _img_sizes[i].desc << endl;
    }

    cout << "Select size: ";
    cin >> sz;
    if ((sz < 0) or (sz >= _img_sizes.size())) {
      cout << "Invalid size: " << sz << endl;
      sz = INVALID_SIZE;
    }
  } while (sz == (unsigned int)INVALID_SIZE);
  s = _img_sizes[sz].value;
  set_image_size(s);
}//end set_image_size()

void linksprite::set_image_size(unsigned char sz) {
  unsigned char txbuf[5] = {0x56, 0, 0x54, 0x01, 0x11};
  txbuf[4] = sz;
  _print_buf((char*)"tx", txbuf, 5);
  _cm->send(txbuf, 5);
  _cm->receive(_rxbuf, 5);
  _print_buf((char*)"rx", _rxbuf, 5);
}//end set_image_size()

void linksprite::take_image() {
  unsigned char txbuf[5] = {0x56, 0, 0x36, 0x01, 0x00};
  cout << "Take image..." << endl;
  _print_buf((char *)"tx", txbuf, 5);
  _cm->send(txbuf, 5);
  // Expected response:
  // rx: 76 0 36 0 0
  _cm->receive(_rxbuf, 5);
  _print_buf((char *)"rx", _rxbuf, 5);
  _buffered_image_size = INVALID_SIZE;
}//end take_image()

int linksprite::download_image(string fname="ls_out.jpg") {
  int sz = 0;
  int adr_h_offset = 8;
  int adr_l_offset = 9;
  ofstream img_file;
  img_file.open(fname.c_str(), std::ios::out|std::ios::binary);
  unsigned char c = 0;
  unsigned char c_last = 0;
  int i = 0;
  
  unsigned char cmd_buf[] = {0x56, 0x00, 0x32, 0x0c, 0x00, 0x0a, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0a};
  bool end_of_image = false;
        
  int rv = this->read_image_size();
  cout << "size: 0x" << std::hex << rv << " " << std::dec << rv << endl;

  cout << "Download image to " << fname << " ..." << endl;
  while (end_of_image == false) {
    cmd_buf[adr_h_offset] = sz / 0x100;
    cmd_buf[adr_l_offset] = sz % 0x100;

    if (cmd_buf[adr_l_offset] == 0) {
      cout << ".";
      cout.flush();
    }
    //Send the read data command
    _cm->send(cmd_buf, 16);
    //Receive the packet frame
    _cm->receive(_rxbuf, 5);
    // Expected response:
    // rx: 76 0 32 0 0

    i = 0;
    while ((i < 32) and (end_of_image == false)) {
      //Receive the data bytes
      _cm->receive(&c, 1);
      _rxbuf[i] = c;
      i++;
      if ((c_last == 0xFF) and (c == 0xD9)) {
        end_of_image = true;
        sz += i;
      }
      c_last = c;
    }//end while ((i < 32) and (end_of_image == false))
    //Write data to jpeg file
    img_file.write((const char*)_rxbuf, i);
    if (end_of_image == true) {
      //Show last data packet
      cout << endl << "last data packet..." << endl;
      _print_buf((char *)"rx", _rxbuf, i);
      //Clear zeros in packet
      do {
        _cm->receive(_rxbuf, 1);
      } while (_rxbuf[0] == 0);
      //Receive the packet frame
      _cm->receive(&_rxbuf[1], 4);
      _print_buf((char *)"rx", _rxbuf, 5);
    }
    else {
      sz += i;
      //Receive the packet frame
      _cm->receive(_rxbuf, 5);
    }
  }//end while (end_of_image == false)

  img_file.close();

  return sz;
}//end download_image()

}; //end namespace blip
