// Filename: ui_image_display.h
// Author: Kevin Somervill
// Date: Tue Apr 14 20:51:28 EDT 2015
#ifndef UI_IMAGE_DISPLAY_H
#define UI_IMAGE_DISPLAY_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Window.H>

class Ui_Image_Display: public Fl_Box {
  private:
    Fl_JPEG_Image * _image;

  public:
    Ui_Image_Display(int, int, int, int);

};
#endif //!define(UI_IMAGE_DISPLAY_H)
