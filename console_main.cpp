// Filename: console_main.cpp
// Synopsis: Simple test program to evaluate the link sprite 2 MP camera. The
// camera connects to the computer via a TTL serial port. In this case, I'm
// using the SparkFun FTDI breakout board.
// Author: Kevin Somervill
// Date: Thu Apr  9 15:28:04 EDT 2015
//

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <fstream>
using std::ofstream;

#include<ios>

#include <string>
using std::string;

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "configuration.h"
#include "linksprite.h"
using blip::linksprite;

#define LSCON_VERSION  "1.2rc"

void usage(void) {
    cout << "Usage: lscon -p <serialport> [OPTIONS]\n"
      << "\n"
      << "Options:\n"
      << "  -h, --help               Print this help message\n"
      << "  -l                       list available ports and exit\n"
      << "  -p, --port=serialport    Serial port to the LinkSprite camera\n"
      << "  -b, --baud=baudrate      Baudrate (bps) of camera (default 115200)\n"
      << " lscon - Version " << LSCON_VERSION
      << "\n";
}//end usage(void)

int read_frame(linksprite*);
void save_image_to_file(string, uint8_t*, size_t);
void set_image_geometry(linksprite *);
void start_console(configuration *);

int main(int argc, char *argv[]) {
    configuration * cfg = new configuration();

    cfg->parse_options(argc, argv);

    if (cfg->list()) {
      communication_manager *cm = new communication_manager();
      port_list ports = cm->get_available_ports();
      size_t sz = ports.size();

      for (size_t i=0; i<sz; i++) {
        cout << " - " << ports[i] << endl;
      }
      delete cm;
    }
    else if (cfg->help() || (cfg->serialport()==NULL)) {
        usage();
    }
    else {
      start_console(cfg);
    }

    exit(EXIT_SUCCESS);
}// end main

void show_console_help() {
  cout << "Available commands:" << endl
    << "  ?|h   show this Help message" << endl
    << "  c     Close port session" << endl
    << "  d|D   Download image" << endl
    //<< "  e     Edit port settings" << endl
    //<< "  m|M   Mode set" << endl
    << "  l     read Length of image (in bytes)" << endl
    << "  o     Open port session" << endl
    << "  q     Query port status" << endl
    << "  r|R   Reset the camera" << endl
    << "  s     Set image size" << endl
    << "  t|T   Take image" << endl
    << "  v|V   Show version" << endl
    << "  x     eXit" << endl
    << " The rest are yet to be implemented" << endl;
}

void start_console(configuration *cfg) {
  bool running = true;
  char cmd;
  int rv = 0;
  uint8_t buf[32];
  linksprite * ls = new linksprite();

  cout << "Opening port: " << cfg->serialport()
    << " at " << cfg->baudrate() << " baud\n";
  if (ls->open(cfg->serialport(), cfg->baudrate())) {
    cout << "console started ..." << endl;
  }
  else {
    cout << "Failed to open port!" << endl;
    //return;
  }

  while (running) {
    cout << "]> ";
    cin >> cmd;

    switch(cmd) {
      case '?':
      case 'h':
        show_console_help();
        break;

      case 'b':
        // Set the baudrate (test of ack)
        cout << "not implemented yet" << endl;
        /*
        txbuf[2] = 0x24;
        txbuf[3] = 0x03;
        txbuf[4] = 0x01;
        txbuf[5] = 0x0D;
        //mlen = 6;
        //acklen = 5;
        */
        break;

      case 'q':
        if (ls->is_open()) {
          cout << "Camera port is open" << endl
          << "  - Device: " << ls->port_name() << endl
          << "  - Baudrate: " << ls->baudrate() << endl;
        }
        else {
          cout << "Camera port is closed" << endl;
        }
        break;

      case 'x':
        running = false;
      case 'c':
        if (ls->is_open()) {
          cout << "Closing the camera port..." << endl;
          ls->close();
        }
        else {
          cout << "Camera port is closed..." << endl;
        }
        break;

      case 'd':
      case 'D':
        rv = ls->download_image();
        if (rv>0) {
          cout << "Downloaded " << std::dec << rv << " bytes" << endl;
          save_image_to_file(string("ls_img.jpg"), ls->image_buffer(), rv);
        }
        else {
          cout << "Failed to download image!" << endl;
        }
        break;

      case 'f':
        cout << "reading frame..." << endl;
        rv = read_frame(ls);
        break;

      case 'l':
      case 'L':
        rv = ls->read_image_size();
        cout << "image size: 0x" << std::hex << rv
          << " (" << std::dec << rv << " bytes)" << endl;
        break;

      case 'o':
        if (ls->is_open()) {
          cout << "Port "  << cfg->serialport() << " is already open."
            << endl;
        }
        else {
          cout << "Opening port: " << cfg->serialport()
            << " at " << cfg->baudrate() << " baud" << endl;

          ls->open(cfg->serialport(), cfg->baudrate());
        }
        break;

      case 'p':
      case 'P':
        rv = ls->poll_comm(buf);
        if (rv > 0) {
          cout << "Polled: " << std::hex << (uint32_t)(buf[0])
            << std::dec << endl;
        }
        break;

      case 'r':
      case 'R':
        ls->reset();
        break;

      case 's':
        set_image_geometry(ls);
        break;

      case 't':
      case 'T':
        cout << "Taking image..." << endl;
        ls->take_image();
        break;

      case 'v':
      case 'V':
        cout << " lscon - Version " << LSCON_VERSION << endl;
        break;

      default:
        cout << "Unknown command: " << cmd << endl;
        show_console_help();

    };//end switch(c)

  }

  cout << "Exitting..." << endl;
  if (ls->is_open()) {
    ls->close();
  }

  delete ls;

}//end start_console()

void set_image_geometry(linksprite *ls) {
  unsigned int sz = INVALID_SIZE;
  //unsigned char s = 0x11;
  cout << "Set image geometry..." << endl;
  vector< linksprite::format_option > geoms = ls->get_geometry_options();

  do {
    for (unsigned int i=0; i<geoms.size(); i++) {
      cout << i << ": " << geoms[i].desc
        << " [cmd: 0x" << std::hex << (unsigned int)(geoms[i].value) << "]"
        << endl;
      cout << std::dec;
    }

    cout << "Select geometry: ";
    cin >> sz;
    if ((sz < 0) or (sz >= geoms.size())) {
      cout << "Invalid size: " << sz << endl;
      sz = INVALID_SIZE;
    }
  } while (sz == (unsigned int)INVALID_SIZE);
  ls->set_image_geometry(geoms[sz].value);
}//end set_image_geometry()

void download_image(linksprite *ls) {
  int rv = ls->download_image();
  string image_filename = "ls_img.jpg";
  cout << "Downloaded " << std::dec << rv << " bytes" << endl;
  save_image_to_file(image_filename, ls->image_buffer(), rv);
  cout << "Image saved to " << image_filename << endl;
}

int read_frame(linksprite *ls) {
  linksprite::data_frame * df = new linksprite::data_frame;
  cout << "Frame read offset: ";
  cin >> df->offset;
  cout << "Frame read length (packets): ";
  int pkt_cnt;
  cin >> pkt_cnt;
  df->length = pkt_cnt * ls->packet_size();
  df->data = new uint8_t[df->length];
  int rd = ls->read_frame(df);
  delete[] df->data;
  df->data = NULL;
  delete df;
  return rd;
}//end read_frame

void save_image_to_file(string fname, uint8_t *buf, size_t len) {
  ofstream img_file;
  img_file.open(fname.c_str(), std::ios::out|std::ios::binary);

  img_file.write((const char*)buf, len);
  img_file.close();
}//end save_image_to_file()
