// Filename: image_buffer.h
//
// Provide interface to control a Link Sprite 2MP camera.
// Copyright (C) 2020  Kevin Somervill
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
// Date: Thu 14 May 2020 05:07:31 PM EDT
#ifndef _IMAGE_BUFFER_H
#define _IMAGE_BUFFER_H


#include <cstdint>

#include <string>
using std::string;

namespace blip {

class image_buffer {
  private:
    size_t _len;
    uint8_t * _data;

  public:

    image_buffer();
    image_buffer(uint8_t*, size_t);
    ~image_buffer();

    uint8_t * data();
    //void create_buffer();
    //void destroy_buffer();
    void save_to_file(string);
    size_t size();
};

};//end namespace blip
#endif //defined(_IMAGE_BUFFER_H)

