// Filename: ui_linksprite_control.cpp
// Author: Kevin Somervill
// Date: Wed Apr 15 19:55:09 EDT 2015

#include "ui_linksprite_control.h"

#include <FL/Fl_Native_File_Chooser.h>

Ui_Linksprite_Control::Ui_Linksprite_Control(int x, int y, int w, int h) :
  Ui_Logging_Group(x, y, w, h) {
  resizable(0);
  box(FL_ENGRAVED_BOX);

  _ls = new linksprite();

  int x_loc = x+WIDGET_MARGIN;
  int y_loc = y+WIDGET_MARGIN;
  int width = BUTTON_W;
  int height = BUTTON_H;

  _conlog_checkbutton = new Fl_Check_Button(x_loc, y_loc, 70, 25, "Debug");
  _conlog_checkbutton->tooltip("Print linksprite debug messages to the console.");
  _conlog_checkbutton->value(_ls->log_to_console());
  _conlog_checkbutton->callback(console_log_cb, (void*)this);

  width = 110;
  x_loc = x+w-(width+55)-WIDGET_MARGIN;
  _isize_ch = new Fl_Choice(x_loc, y_loc, width, height, "Image Size");

  x_loc += _isize_ch->w()+WIDGET_MARGIN;
  width = w-x_loc;
  Fl_Button* _iset_b = new Fl_Button(x_loc, y_loc, width, height, "Set");
  _iset_b->tooltip("Set image size.");

  x_loc = x+WIDGET_MARGIN;
  y_loc += BUTTON_H + WIDGET_MARGIN;
  Fl_Button* _icapture_b = new Fl_Button(x_loc, y_loc,
      BUTTON_W, BUTTON_H, "Capture");
  _icapture_b->tooltip("Capture image.");
  _icapture_b->callback(image_capture_cb, (void*)this);

  x_loc += _icapture_b->w() + WIDGET_MARGIN;
  width = w - x_loc;
  _iprogress = new Fl_Progress(x_loc, y_loc, width, BUTTON_H);
  _iprogress->selection_color((Fl_Color)4);
  _iprogress->minimum(0.0);
  _iprogress->value(0.0);

  x_loc = x+WIDGET_MARGIN;
  y_loc += BUTTON_H + WIDGET_MARGIN;// 110??
  _isave_b = new Fl_Button(x_loc, y_loc, BUTTON_W, BUTTON_H, "Save Image");
  _isave_b->callback(save_img_cb, (void*)this);
  _isave_b->deactivate();

  x_loc += _isave_b->w() + WIDGET_MARGIN;
  Fl_Button* _iload_b = new Fl_Button(x_loc, y_loc, 
      BUTTON_W, BUTTON_H, "Load Image");
  _iload_b->callback(load_img_cb, (void*)this);


  add(_conlog_checkbutton);
  add(_isize_ch);
  add(_iset_b);
  add(_icapture_b);
  add(_iprogress);
  add(_isave_b);
  add(_iload_b);

  end();

  _img_buf = nullptr;
}//end Ui_Linksprite_Control()


void Ui_Linksprite_Control::console_log_cb(Fl_Widget * w, void *data) {
  Ui_Linksprite_Control *lsc = (Ui_Linksprite_Control*)data;
  linksprite *lsh = lsc->linksprite_handle();
  bool v = (bool)lsc->_conlog_checkbutton->value();
  if (v) {
    lsc->_log("enabling linksprite debug messages...\n");
  }
  else {
    lsc->_log("disabling linksprite debug messages...\n");
  }
  lsh->log_to_console(v);
}//end console_log_cb()


void Ui_Linksprite_Control::load_img_cb (Fl_Widget * w, void *data){
  Ui_Linksprite_Control *lsc = (Ui_Linksprite_Control*)data;
  Fl_Native_File_Chooser fnfc(Fl_Native_File_Chooser::BROWSE_FILE);
  fnfc.title("Load image file...");
  fnfc.directory(".");
  fnfc.filter("*.jpg");

  if (fnfc.show() == 0) {
    lsc->image_display()->load_image(fnfc.filename());
    lsc->image_display()->redraw();
  }
}//end load_img_cb()

void Ui_Linksprite_Control::save_img_cb (Fl_Widget * w, void *data){
  Ui_Linksprite_Control *lsc = (Ui_Linksprite_Control*)data;
  Fl_Native_File_Chooser fnfc(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  fnfc.title("Save image to file...");
  fnfc.directory(".");
  fnfc.filter("*.jpg");

  if (fnfc.show() == 0) {
    lsc->_img_buf->save_to_file(fnfc.filename());
  }
}//end save_img_cb()


int Ui_Linksprite_Control::initialize() {
  int rv = 0;
  _log("initializing camera control...\n");
  vector < linksprite::format_option > geoms = _ls->get_geometry_options();
  for (size_t i = 0; i<geoms.size(); i++) {
    sprintf (_scratch_buf, "adding dimension:%s [0x%x]\n",
        geoms[i].desc.c_str(), geoms[i].value);
    _isize_ch->add(geoms[i].desc.c_str(),0,0);
  }
  _ls->packet_size(256);
  _isize_ch->value(0);
  return rv;
}

void Ui_Linksprite_Control::resize(int x, int y, int w, int h) {
  /* Do nothing */
}

linksprite* Ui_Linksprite_Control::linksprite_handle() {
  return _ls;
}

void Ui_Linksprite_Control::image_capture_cb (Fl_Widget *w, void *data) {
  Ui_Linksprite_Control *lsc = (Ui_Linksprite_Control*)data;
  linksprite *ls = lsc->linksprite_handle();

  lsc->_log("Capturing image...\n");
  ls->take_image();
  lsc->_iprogress->maximum(1.0);
  lsc->_iprogress->value(0.25);
  lsc->_iprogress->redraw();

  int len = ls->read_image_size();
  sprintf(lsc->_scratch_buf, "Image size: %d bytes\n", len);
  lsc->_iprogress->value(0.5);
  lsc->_iprogress->redraw();
  lsc->_log(lsc->_scratch_buf);

  if (lsc->_img_buf) {
    delete lsc->_img_buf;
  }

  lsc->_img_buf = new image_buffer(new uint8_t[len], len);

  int dl_sz = ls->download_image(lsc->_img_buf->data(), len);
  lsc->_iprogress->value(1.0);
  lsc->_iprogress->redraw();
  if (dl_sz == len) {
    sprintf(lsc->_scratch_buf, "Downloaded %d bytes\n", dl_sz);
    lsc->_log(lsc->_scratch_buf);

    lsc->image_display()->set_image(lsc->_img_buf->data());
    lsc->image_display()->redraw();
    lsc->_log("Updated image display\n");
    lsc->_isave_b->activate();
  }
  else{
    lsc->_log("Failed to download image!\n");
  }
}//end image_capture_cb()

    
Ui_Image_Display * Ui_Linksprite_Control::image_display() {
  return _img_display;
}//end image_display()


void Ui_Linksprite_Control::image_display(Ui_Image_Display *id) {
  _img_display = id;
}//end image_display()
