// Filename: ui.cpp
// Author: Kevin Somervill
// Date: Sat Aug 20 18:44:58 EDT 2016

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Window.H>

#include "ui.h"

int main() {
  // initialize image lib
  fl_register_images();
  // make a window
  Ui_Main_Window * w = new Ui_Main_Window(MAIN_WIN_W, MAIN_WIN_H,
      "Link Sprite Camera Controller");
  w->initialize();
  w->show();
  return(Fl::run());
}
