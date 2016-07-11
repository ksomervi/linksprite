// Filename: linksprite.h
//
// Provide interface to control a Link Sprite 2MP camera.
// Copyright (C) 2015  Kevin Somervill
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

#include <vector>
using std::vector;

#include <iostream>
using std::ostream;

#include <string>
using std::string;

#include "serialport/communication_manager.h"
//#include "ostream_log.h"

namespace blip {

#ifndef INVALID_SIZE
#define INVALID_SIZE -1
#endif

#define LS_MIN_PKT_SZ 8
#define LS_DEFAULT_PKT_SZ 32
#define CMD_PKT_SZ_OFFSET 13 //byte in command array buffer

#define ADDR2_OFFSET  7
#define ADDR1_OFFSET  8
#define ADDR0_OFFSET  9

class linksprite {
public:
  class format_option {
  public:
    unsigned char value;
    string desc;
    format_option();
    format_option(unsigned char, string);
  };

  class data_frame {
    public:
      int offset;
      int length;
      uint8_t *data;
  };

private:
  communication_manager * _cm;
  vector< format_option > _img_geometries;

  //base_log * _mlog;

  void _init_image_geometries();

  unsigned char _rxbuf[32];//used for debugging
  unsigned char _txbuf[32];
  bool _validate_msg(unsigned char * exp, unsigned char * act, int32_t n);

  int32_t _packet_size;
  int32_t _buffered_image_size;
  uint8_t * _img_buffer;
  uint8_t _tx_msize;

  void _log_tx_msg(unsigned char *, size_t);
  void _dbg_msg(const char*);
  void _dbg_msg(const char *, unsigned char *, size_t);

  bool _log_to_console;

public:
  // Constructor
  linksprite();

  // Interface control
  bool open(const char *, int32_t);
  void close();
  bool is_open();
  char * port_name();
  int32_t baudrate();

  // Reset the camera. Wait 2-3 seconds before taking a picture.
  void reset();
  void set_image_geometry(uint8_t);
  void take_image();
  uint32_t read_image_size();
  int32_t download_image();
  uint32_t read_frame(data_frame*);
  uint8_t packet_size();
  void packet_size(uint8_t);
  //uint8_t poll_comm(uint8_t*, size_t);
  uint8_t poll_comm(uint8_t*);
  uint8_t * image_buffer();
  vector< format_option > get_geometry_options();

  void log_to_console(bool);
  bool log_to_console();

  uint8_t tx_buffer(unsigned char *);


};//end class linksprite
};//end namespace blip

// Length of image: 56 00 34 01 00
// Ack: 76 00 34 00 04 00 Byte2 Byte1 Byte0
//

#endif //defined(_linksprite_H)

