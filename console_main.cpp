// Filename: console_main.cpp
// Synopsis: Simple test program to evaluate the link sprite 2 MP camera. The
// camera connects to the computer via a TTL serial port. In this case, I'm
// using the SparkFun FTDI breakout board.
// Author: Kevin Somervill
// Date: Thu Apr  9 15:28:04 EDT 2015
//

#include <ctime>

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
using blip::image_buffer;

#define LSCON_VERSION  "1.3rc"

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

void download_image(linksprite *, bool);
int read_frame(linksprite *);
void set_image_geometry(linksprite *);
void start_console(configuration *);
string timestamp();

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
  cout << "lscon - Version " << LSCON_VERSION << endl;
  cout << "Available commands:" << endl
    << "  ?|h   show this Help message" << endl
    << "  c     Close port session" << endl
    << "  d|D   Download image" << endl
    //<< "  e     Edit port settings" << endl
    //<< "  m|M   Mode set" << endl
    << "  l     read Length of image (in bytes)" << endl
    << "  o     Open port session" << endl
    << "  p     Set packet size" << endl
    << "  P     Poll serial port" << endl
    << "  q     Query status" << endl
    << "  r|R   Reset the camera" << endl
    << "  s     Set image size" << endl
    << "  t|T   Take image" << endl
    << "  v|V   Toggle verbose output" << endl
    << "  x     eXit" << endl
    << " The rest are yet to be implemented" << endl;
}

void start_console(configuration *cfg) {
  bool running = true;
  char cmd;
  int rv = 0;
  uint8_t buf[32];
  linksprite * ls = new linksprite();
  bool verbose = false;
  const char *verbose_state[3] = {"Off", "On"};

  cout << "Opening port: " << cfg->serialport()
    << " at " << cfg->baudrate() << " baud\n";
  if (ls->open(cfg->serialport(), cfg->baudrate())) {
    cout << "console started " << timestamp() << " ..." << endl;
  }
  else {
    cout << "Failed to open port!" << endl;
    //return;
  }

  while (running) {
    cout << "[" << timestamp() << "]> ";
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
          << "  - Baudrate: " << ls->baudrate() << endl
          << " Packet size: " << ls->packet_size() << endl
          << " Verbose: " << verbose_state[verbose] << endl;
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
        download_image(ls, verbose);
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
        rv = LS_DEFAULT_PKT_SZ;
        cout << "Enter packet size [multiple of 8]: ";
        cin >> rv;
        ls->packet_size(rv);
        break;

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
        if (verbose) {
          cout << "Taking image..." << endl;
        }
        ls->take_image();
        break;

      case 'v':
      case 'V':
        verbose = (verbose == false);
        ls->log_to_console(verbose);
        if (verbose) {
          cout << "Verbose on" << endl;
        }
        else {
          cout << "Verbose off" << endl;
        }

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

void download_image(linksprite *ls, bool verbose=false) {
  int size = ls->read_image_size();
  image_buffer *ibuf = new image_buffer(new uint8_t[size], size);

  size = ls->download_image(ibuf->data(), ibuf->size());
  if (size > 0) {
    if (verbose) {
      cout << "Downloaded " << std::dec << size << " bytes" << endl;
    }
    string filename = string("ls_img-") + timestamp() + ".jpg";
    ibuf->save_to_file(filename);
    cout << "  saved image to: " << filename << endl;
    ibuf = nullptr;
  }
  else {
    cout << "Failed to download image!" << endl;
  }
  delete ibuf;
}//end download_image()

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
  df->data = nullptr;
  delete df;
  return rd;
}//end read_frame

string timestamp() {
  time_t timer;
  struct tm * timeinfo;
  char tbuf[48];

  time(&timer);
  timeinfo = localtime(&timer);
  int seconds = (((timeinfo->tm_hour*60) + timeinfo->tm_min)*60) + timeinfo->tm_sec;
  
  sprintf(tbuf, "%d-%02d-%02d.%05d", timeinfo->tm_year+1900, 
      timeinfo->tm_mon+1, timeinfo->tm_mday, seconds);
  return string(tbuf);
}
