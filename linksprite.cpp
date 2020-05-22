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

#include <unistd.h>

#include "serialport/communication_manager.h"
#include "linksprite.h"

namespace blip {

  /*
#ifdef WIN32
#include <windows.h>
#endif


  void uSleep(int waitTime) {
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
      QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < waitTime);
  }
  */

linksprite::format_option::format_option() : value(), desc() {
  //value = v;
  //desc = s;
}

linksprite::format_option::format_option(unsigned char v, string s) {
  value = v;
  desc = s;
}

// Private functions
void linksprite::_init_image_geometries() {
  _img_geometries.clear();
  _img_geometries.push_back(format_option(0x22, string("160x120")));
  _img_geometries.push_back(format_option(0x11, string("320x240")));
  _img_geometries.push_back(format_option(0x00, string("640x480")));
  _img_geometries.push_back(format_option(0x1D, string("800x600")));
  _img_geometries.push_back(format_option(0x1C, string("1024x768")));
  _img_geometries.push_back(format_option(0x1B, string("1280x960")));
  _img_geometries.push_back(format_option(0x21, string("1600x1200")));
}//end _init_image_sizes()

bool linksprite::_validate_msg(unsigned char *e, unsigned char *a, int n) {
  for (int i=0; i<n; i++) {
    if (e[i] != a[i]) {
      if (_log_to_console) {
        cout << "Byte[" << i << "]: expected '0x" << std::hex << e[i]
          << "' but received '0x" << std::hex << a[i] << "'" << endl;
      }
      return false;
    }
  }
  return true;
}//end _validate_msg()

linksprite::linksprite() {
  _cm = new communication_manager();
  //_img_buffer = new image_buffer();
  _packet_size = LS_DEFAULT_PKT_SZ;
  _init_image_geometries();
  //_mlog = NULL;
  _log_to_console = true;
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

int32_t linksprite::baudrate() {
  return _cm->baudrate();
}//end baudrate()

void linksprite::_dbg_msg(const char *m) {
  cout << m << endl;
}
void linksprite::_dbg_msg(const char *name, unsigned char *b, size_t len) {
  cout << name << ":";
  std::ios_base::fmtflags old_flags = cout.flags(std::ios::hex);
  for (size_t i=0; i<len; i++) {
    cout << " " << (uint32_t)b[i];
  }
  cout << endl;
  cout.flags(old_flags);
}//end _dbg_msg()

void linksprite::reset() {
  //TODO unsigned char vbuf[] = {
  //TODO 0xd, 0xa, 0x49, 0x6e, 0x69, 0x74, 0x20, 0x65, 0x6e, 0x64, 0xd, 0xa};
  unsigned char txbuf[4] = {0x56, 0, 0x26, 0x00};
  _log_tx_msg(txbuf, 4);
  if (_log_to_console) {
    _dbg_msg((char*)"Resetting camera...");
    _dbg_msg((char*)"tx", txbuf, 4);
  }
  _cm->send(txbuf, 4);
  usleep(1000);
  // Expected response:
  // rx: d a 49 6e 69 74 20 65 6e 64 d a
  _cm->receive(_rxbuf, 12);
  if (_log_to_console) {
    _dbg_msg((char*)"rx", _rxbuf, 12);
  }
}//end reset()

size_t linksprite::read_image_size() {
  //TODO unsigned char vbuf[] = {0x76, 0x0, 0x34, 0x0, 0x4, 0x0};
  unsigned char txbuf[5] = {0x56, 0, 0x34, 0x01, 0};
  if (_log_to_console) {
    _dbg_msg((char*)"Read image size...");
    _dbg_msg((char*)"tx", txbuf, 5);
  }

  _cm->send(txbuf, 5);
  _log_tx_msg(txbuf, 5);
  usleep(1000);
  _cm->receive(_rxbuf, 9);
  // Expected response:
  // rx: 76 0 34 0 4 0 0 4e ae
  if (_log_to_console) {
    _dbg_msg((char*)"rx", _rxbuf, 9);
  }
  size_t img_sz = ((uint32_t)_rxbuf[6] << 16) & 0x00FF0000;
  img_sz += ((uint32_t)_rxbuf[7] << 8) & 0x0000FF00;
  img_sz += (uint32_t)_rxbuf[8] & 0x000000FF;
  return img_sz;
}//end read_image_size()

vector< linksprite::format_option > linksprite::get_geometry_options() {
  return _img_geometries;
}//end get_geometry_options()

void linksprite::set_image_geometry(unsigned char sz) {
  unsigned char txbuf[5] = {0x56, 0, 0x54, 0x01, 0x11};
  txbuf[4] = sz;
  if (_log_to_console) {
    _dbg_msg((char*)"tx", txbuf, 5);
  }
  _cm->send(txbuf, 5);
  _log_tx_msg(txbuf, 5);
  _cm->receive(_rxbuf, 5);
  if (_log_to_console) {
    _dbg_msg((char*)"rx", _rxbuf, 5);
  }
}//end set_image_size()

void linksprite::take_image() {
  unsigned char txbuf[5] = {0x56, 0, 0x36, 0x01, 0x00};
  if (_log_to_console) {
    _dbg_msg((char*)"Take image...");
    _dbg_msg((char *)"tx", txbuf, 5);
  }
  _cm->send(txbuf, 5);
  _log_tx_msg(txbuf, 5);
  usleep(1000);
  // Expected response:
  // rx: 76 0 36 0 0
  _cm->receive(_rxbuf, 5);
  if (_log_to_console) {
    _dbg_msg((char *)"rx", _rxbuf, 5);
  }
  //_buffered_image_size = INVALID_SIZE;
  //_img_buffer->size() = INVALID_SIZE;
}//end take_image()

int linksprite::download_image(uint8_t *ibuf, size_t len) {
  size_t sz = 0;
  size_t expected_sz = 0;
  //uint8_t * data_buf = NULL;
  uint8_t * data_p = NULL;
  uint8_t * data_last = NULL;
  size_t i = 0;
  size_t expected_packets;
  size_t packet_count = 0;
  unsigned char* _rdbuf = NULL;

  unsigned char cmd_buf[] = {
    0x56, 0x00, 0x32, 0x0c, 0x00, 0x0a, // [0-5] - Command header
    0x00,                               // [6] - unused
    0x00, 0x00, 0x00,                   // [7-9] - Address of read
    0x00,                               // [10] - unused
    0x00, 0x00, LS_DEFAULT_PKT_SZ,      // [11-13] - Size of returned to be
                                        // read, multiple of 8
    0x00, 0x0a};                        // [14-15] - Trailer
  bool end_of_image = false;

  //_packet_size = ( (size_t)(cmd_buf[11]<<16) & 0x00FF0000)
               //| ( (size_t)(cmd_buf[12]<<8)  & 0x0000FF00)
               //| ( (size_t)(cmd_buf[13])     & 0x000000FF);

   
  cmd_buf[11] = (_packet_size>>16) & 0xFF;
  cmd_buf[12] = (_packet_size>>8) & 0xFF;
  cmd_buf[13] = (_packet_size) & 0xFF;

  //expected_sz = this->read_image_size();
  expected_sz = len;

  expected_packets = expected_sz / _packet_size;
  if (expected_sz % _packet_size) {
    expected_packets++;
  }

  //data_buf = new uint8_t[expected_sz];
  data_p = ibuf;
  data_last = ibuf;

  _rdbuf = new uint8_t[_packet_size];

  if (_log_to_console) {
    _dbg_msg((char*)"Downloading image ...");
    cout << "image size: "
      << std::dec << expected_sz
      << " bytes (0x" << std::hex << expected_sz << ")"
      << endl;
    cout << "  - Packet read size: " << std::dec << _packet_size << endl;
    cout << "  - Reading " << expected_packets << " packets" << endl;
  }

  while (end_of_image == false) {
    cmd_buf[ADDR2_OFFSET] = (sz>>16) & 0xFF;
    cmd_buf[ADDR1_OFFSET] = (sz>>8) & 0xFF;
    cmd_buf[ADDR0_OFFSET] = (sz) & 0xFF;

    if ((_log_to_console) and (cmd_buf[ADDR0_OFFSET] == 0)) {
      cout << ".";
      cout.flush();
    }
    //Send the read data command
    _cm->send(cmd_buf, 16);
    _log_tx_msg(cmd_buf, 16);
    //Receive the packet frame header
    _cm->receive(_rxbuf, 5);
    // Expected response:
    // rx: 76 0 32 0 0
    packet_count++;

    i = 0;
    while ((i < _packet_size) and (end_of_image == false)) {
      //Receive the data bytes
      _cm->receive(data_p, 1);
      _rdbuf[i] = *data_p;
      i++;
      if ((*data_last == 0xFF) and (*data_p == 0xD9)) {
        end_of_image = true;
        sz += i;
      }
      if (sz > expected_sz) {
        cout << "ERROR: (sz > expected_sz)!" << endl;
        cout << "sz: " << std::dec << sz
          << " (0x" << std::hex << sz << ")" << endl;
        end_of_image = true;
        sz = -1;
      }
      data_last = data_p;
      data_p++;
    }//end while ((i < _packet_size) and (end_of_image == false))
    if (end_of_image == true) {
      if (_log_to_console) {
        //Show last data packet
        _dbg_msg((char*)"\nlast data packet...");
        _dbg_msg((char *)"rd", _rdbuf, i);
      }
      //Clear zeros in packet
      do {
        _cm->receive(_rdbuf, 1);
      } while (_rdbuf[0] == 0);
      //Receive the packet frame trailer
      _cm->receive(&_rxbuf[1], 4);
      // Expected response:
      // rx: 76 0 32 0 0
      if (_log_to_console) {
        _dbg_msg((char *)"rx", _rxbuf, 5);
      }
    }//end if (end_of_image == true)
    else {
      sz += i;
      //Receive the packet frame
      _cm->receive(_rxbuf, 5);
    }
  }//end while (end_of_image == false)

  cout << "Downloaded " << std::dec << packet_count << " packets" << endl;
  if (packet_count != expected_packets) {
    cout << "  Expected " << expected_packets << " packets!" << endl;
  }

  if (sz != expected_sz) {
    //ibuf = new image_buffer(data_buf, sz);
  //}
  //else {
    cout << "sz != expected_sz: " << sz << " != " << expected_sz << endl;
  }

  delete[] _rdbuf;

  return sz;
}//end download_image()

//Currently unused, stay tuned.
uint32_t linksprite::read_frame(data_frame *f) {
  uint32_t sz = 0;
  uint8_t cmd_buf[] = {
    0x56, 0x00, 0x32, 0x0c, 0x00, 0x0a, // [0-5] - Command header
    0x00,                               // [6] - unused
    0x00, 0x00, 0x00,                   // [7-9] - Address of read
    0x00,                               // [10] - unused
    0x00, 0x00, 0x20,                   // [11-13] - Size of returned to be
                                        // read, multiple of 8
    0x00, 0x0a};                        // [14-15] - Trailer
  bool end_of_image = false;
  uint8_t * _rdbuf = NULL;
  size_t i;

  // Frame: offset, length, *data
  int adr = f->offset;
  _rdbuf = new uint8_t[_packet_size];
  /*
  if (_log_stream) {
    _log_stream << "p_size: " << p_size << endl;
  }
  */
  cmd_buf[CMD_PKT_SZ_OFFSET] = _packet_size;

  uint8_t *data_p = f->data;
  uint8_t *data_last = data_p;

  int frames = f->length / _packet_size;
  if (f->length % _packet_size != 0) {
    frames++;
  }

  while ((frames>0) and (end_of_image == false)) {
    cmd_buf[ADDR2_OFFSET] = (adr>>16) & 0xFF;
    cmd_buf[ADDR1_OFFSET] = (adr>>8) & 0xFF;
    cmd_buf[ADDR0_OFFSET] = (adr) & 0xFF;

    //Send the read data command
    _cm->send(cmd_buf, 16);
    _log_tx_msg(cmd_buf, 16);
    //Receive the packet frame
    _cm->receive(_rxbuf, 5);
    // Expected response:
    // rx: 76 0 32 0 0

    i = 0;
    while ((i < _packet_size) and (end_of_image == false)) {
      //Receive the data bytes
      _cm->receive(data_p, 1);
      _rdbuf[i] = *data_p;
      i++;
      if ((*data_last == 0xFF) and (*data_p == 0xD9)) {
        end_of_image = true;
        sz += i;
      }
      data_last = data_p;
      data_p++;
    }//end while ((i < p_size) and (end_of_image == false))

    frames--;

    if (end_of_image == true) {
      if (_log_to_console) {
        //Show last data packet
        _dbg_msg((char *)"\nlast data packet...");
        _dbg_msg((char *)"rx", _rdbuf, i);
      }
      //Clear zeros in packet
      do {
        _cm->receive(_rxbuf, 1);
      } while (_rxbuf[0] == 0);
      //Receive the packet frame
      _cm->receive(&_rxbuf[1], 4);
      _dbg_msg((char *)"rx", _rxbuf, 5);
    }
    else {
      sz += i;
      //Receive the packet frame
      _cm->receive(_rxbuf, 5);
      // Expected response:
      // rx: 76 0 32 0 0
    }
  }//end while (end_of_image == false)

  cout << "requested " << f->length << " bytes, read " << sz << " bytes"
    << endl;
  _dbg_msg((char *)"data", f->data, f->length);

  return sz;
}

size_t linksprite::packet_size() {
  return _packet_size;
}

void linksprite::packet_size(size_t sz) {
  size_t tmp = sz & 0x00FFFFF8;

  if (sz % LS_MIN_PKT_SZ) {
    cout << "Correcting packet size to be 8-byte aligned!" << endl;
    cout << "Adjusted to " << tmp;
  }
  _packet_size = tmp;
}

//image_buffer * linksprite::get_image_buffer() {
  //return _img_buffer;
//}//end image_buffer()

void linksprite::log_to_console(bool b) {
  _log_to_console = b;
}//end log_to_console(bool)

bool linksprite::log_to_console() {
  return _log_to_console;
}//end log_to_console()

void linksprite::_log_tx_msg(unsigned char *s, size_t sz) {
  _tx_msize = sz;
  for (size_t i=0; i<sz; i++) {
    _txbuf[i] = s[i];
  }
}

uint8_t linksprite::tx_buffer(unsigned char* d) {
  memcpy(d, _txbuf, _tx_msize);
  return _tx_msize;
}

//uint8_t linksprite::poll_comm(uint8_t *b, size_t max) {
uint8_t linksprite::poll_comm(uint8_t *b) {
  return (_cm->poll(b, true));
}

}; //end namespace blip
