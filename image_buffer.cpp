/** \file image_buffer.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2020-05-14
 */

#include "image_buffer.h"

#include <fstream>
using std::ofstream;

#include <ios>

namespace blip {

image_buffer::image_buffer() {
  _len = 0;
  _data = nullptr;
}

image_buffer::image_buffer(uint8_t *d, size_t l) {
  _len = l;
  _data = d;
}

image_buffer::~image_buffer() {
  _len = 0;
  if (_data) {
    delete[] _data;
    _data = nullptr;
  }
}

size_t image_buffer::size() {
  return _len;
}

uint8_t * image_buffer::data() {
  return _data;
}

void image_buffer::save_to_file(string fname) {
  ofstream img_file;
  img_file.open(fname.c_str(), std::ios::out|std::ios::binary);
  img_file.write((const char*)_data, _len);
  img_file.close();
}//end save_to_file()

}; //end namespace blip
