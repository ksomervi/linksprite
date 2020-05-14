// Filename: ui_image_display.cpp
// Author: Kevin Somervill
// Date: Tue Apr 14 20:51:28 EDT 2015

#include "ui_image_display.h"
#include <FL/fl_ask.h>
#include <cerrno>

#include <iostream>
using std::cerr;
using std::endl;

Ui_Image_Display::Ui_Image_Display(int x, int y, int w, int h) :
  Fl_Box(x, y, w, h) {
  box(FL_ENGRAVED_BOX);
  color(FL_WHITE);
}

void Ui_Image_Display::load_image(string path) {
  Fl_JPEG_Image* _image_tmp = new Fl_JPEG_Image(path.c_str());
  switch ( _image_tmp->fail() ) {
    case Fl_Image::ERR_NO_IMAGE:
    case Fl_Image::ERR_FILE_ACCESS:
      fl_alert("%s: %s", path.c_str(), strerror(errno));
      break;

    case Fl_Image::ERR_FORMAT:
      fl_alert("Error: couldn't decode image %s", path.c_str());
      break;

    default:
      if (_image_tmp->w() > 0) {
        _image = (Fl_JPEG_Image*)_image_tmp->copy(this->w(), this->h());
        image(_image);
      }
  }
  delete _image_tmp;
}
void Ui_Image_Display::set_image(uint8_t *data) {
  cerr << "set image" << endl;
  Fl_JPEG_Image* _image_tmp = new Fl_JPEG_Image(NULL, data);
  switch ( _image_tmp->fail() ) {
    case Fl_Image::ERR_NO_IMAGE:
    case Fl_Image::ERR_FILE_ACCESS:
      fl_alert("Error: %s!", strerror(errno));  // shows actual os error to user
      break;

    case Fl_Image::ERR_FORMAT:
      fl_alert("Error: couldn't decode image!");
      break;

    default:
      if (_image_tmp->w() > 0) {
        _image = (Fl_JPEG_Image*)_image_tmp->copy(this->w(), this->h());
        image(_image);
        cerr << "copied image" << endl;
      }
      else {
        cerr << "image width == 0" << endl;
      }
  }
  delete _image_tmp;
}

