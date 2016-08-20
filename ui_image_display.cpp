// Filename: ui_image_display.cpp
// Author: Kevin Somervill
// Date: Tue Apr 14 20:51:28 EDT 2015

#include "ui_image_display.h"
// COMPILE: fltk-config --use-images --compile load-jpeg.cxx
Ui_Image_Display::Ui_Image_Display(int x, int y, int w, int h) :
  Fl_Box(x, y, w, h) {
  box(FL_ENGRAVED_BOX);
  color(FL_WHITE);
  Fl_JPEG_Image* _image_tmp = new Fl_JPEG_Image("test_img.jpg");
  if (_image_tmp->w() > 0) {
    _image = (Fl_JPEG_Image*)_image_tmp->copy(this->w(), this->h());
  }
  delete _image_tmp;
  image(_image);
}

