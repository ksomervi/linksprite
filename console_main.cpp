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

void usage(void) {
    cout << "Usage: lscon -p <serialport> [OPTIONS]\n"
      << "\n"
      << "Options:\n"
      << "  -h, --help                   Print this help message\n"
      << "  -p, --port=serialport        Serial port to the LinkSprite camera\n"
      << "  -b, --baud=baudrate          Baudrate (bps) of camera (default 115200)\n"
      << "\n";
}//end usage(void)

void start_console(configuration *);

int main(int argc, char *argv[]) {
    configuration * cfg = new configuration();

    cfg->parse_options(argc, argv);

    if (cfg->help() || (cfg->serialport()==NULL)) {
        usage();
        exit(EXIT_SUCCESS);
    }

    start_console(cfg);

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
    << "  x     eXit" << endl
    << " The rest are yet to be implemented" << endl;
}

void start_console(configuration *cfg) {
  bool running = true;
  char cmd;
  int rv = 0;
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
        rv = ls->download_image(string("ls_img.jpg"));
        cout << "Downloaded " << std::dec << rv << " bytes" << endl;
        break;

      case 'o':
        if (ls->is_open()) {
          cout << "Opening port: " << cfg->serialport() 
            << " at " << cfg->baudrate() << " baud" << endl;

          ls->open(cfg->serialport(), cfg->baudrate());
        }
        else {
          cout << "Port "  << cfg->serialport() << " is already open."
            << endl;
        }
        break;

      case 'l':
      case 'L':
        rv = ls->read_image_size();
        cout << "size: 0x" << std::hex << rv 
          << " " << std::dec << rv << endl;
        break;

      case 'r':
      case 'R':
        ls->reset();
        break;

      case 's':
        ls->set_image_size();
        break;

      case 't':
      case 'T':
        cout << "Taking image..." << endl;
        ls->take_image();
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

