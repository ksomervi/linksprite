// Filename: linksprite.cpp
//
// Provide interface to control a Link Sprite 2MP camera.
// Copyright (C) 2105  Kevin Somervill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
// USA.
//
// Author: Kevin Somervill <kevin@somervill.org>
// Date: Thu Apr  9 15:05:44 EDT 2015
#ifndef _LINKSPRITE_H
#define _LINKSPRITE_H

#include <map>
using std::map;
using std::pair;

#include <string>
using std::string;

#include "serialport/communication_manager.h"

namespace blip {

#ifndef INVALID_SIZE
#define INVALID_SIZE -1
#endif

class linksprite {
public:
  class format_option {
  public:
    unsigned char value;
    string desc;
    format_option();
    format_option(unsigned char, string);
  };

private:
  communication_manager * _cm;

  map< int, format_option > _img_sizes;

  void _init_image_sizes();

  unsigned char _rxbuf[32];//used for debugging
  //unsigned char _rxlog[];//used for debugging
  //unsigned char _txbuf[32];
  bool _validate_msg(unsigned char * exp, unsigned char * act, int n);
  int _buffered_image_size;

  void _print_buf(const char *, unsigned char *, int);

public:
  // Constructor
  linksprite();

  // Interface control
  bool open(const char *, int);
  void close();
  bool is_open();
  char * port_name();
  int baudrate();

  // Reset the camera. Wait 2-3 seconds before taking a picture.
  void reset();
  unsigned int read_image_size();
  void take_image();
  int download_image(string);
  void set_image_size();
  void set_image_size(uint8_t);

  //void quiet(bool);
  //bool quiet();

};
};

/*
//Camera commands
void reset_camera(communication_manager *);
unsigned int read_image_size(communication_manager *);
void take_image(communication_manager *);
int download_image(communication_manager *);
void set_image_size(communication_manager *);
*/

// Length of image: 56 00 34 01 00
// Ack: 76 00 34 00 04 00 Byte2 Byte1 Byte0
//

#endif //defined(_linksprite_H)

