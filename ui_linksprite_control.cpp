// Filename: ui_linksprite_control.cpp
// Author: Kevin Somervill
// Date: Wed Apr 15 19:55:09 EDT 2015

#include "ui_linksprite_control.h"

Ui_Linksprite_Control::Ui_Linksprite_Control(int x, int y, int w, int h) :
  Ui_Logging_Group(x, y, w, h) {
  resizable(0);
  box(FL_ENGRAVED_BOX);

  _ls = new linksprite();

  int x_loc = x+WIDGET_MARGIN;
  int y_loc = y+WIDGET_MARGIN;
  int width = BUTTON_W;
  int height = BUTTON_H;

  _conlog_checkbutton = new Fl_Check_Button(x_loc, y_loc, 80, 25, "Debug");
  _conlog_checkbutton->tooltip("Print linksprite debug messages to the console.");
  _conlog_checkbutton->value(_ls->log_to_console());
  _conlog_checkbutton->callback(console_log_cb, (void*)this);

  width = 100;
  x_loc = x+w-(width+55)-WIDGET_MARGIN;
  _isize_ch = new Fl_Choice(x_loc, y_loc, width, height,
                            "Image Size");
  //_isize_ch->down_box(FL_BORDER_BOX);

  x_loc += _isize_ch->w()+WIDGET_MARGIN;
  width = w-x_loc;
  Fl_Button* _iset_b = new Fl_Button(x_loc, y_loc, width, height, "Set");
  _iset_b->tooltip("Set image size.");

  Fl_Button* _icapture_b = new Fl_Button(10, 105,
      BUTTON_W, BUTTON_H, "Capture");
  _icapture_b->tooltip("Capture image.");
  _icapture_b->callback(image_capture_cb, (void*)this);

  Fl_Button* _idownload_b = new Fl_Button(10, 135,
      BUTTON_W, BUTTON_H, "Download");
  _idownload_b->tooltip("Download buffered image to file.");
  _idownload_b->callback(download_cb, (void*)this);
  _ifname_in = new Fl_Input(105, 105, 235, BUTTON_H);
  _iprogress = new Fl_Progress(105, 135, 235, BUTTON_H);
  _iprogress->selection_color((Fl_Color)4);

  add(_conlog_checkbutton);
  add(_isize_ch);
  add(_iset_b);
  add(_icapture_b);
  add(_idownload_b);
  add(_ifname_in);
  add(_iprogress);

  end();
}


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

void Ui_Linksprite_Control::download_cb (Fl_Widget * w, void *data){
  Ui_Linksprite_Control *lsc = (Ui_Linksprite_Control*)data;
  linksprite *lsh = lsc->linksprite_handle();
  int rv = lsh->download_image();
  if (rv > 0) {
    sprintf(lsc->_scratch_buf, "Downloaded %d bytes\n", rv);
  }
  else{
    sprintf(lsc->_scratch_buf, "Failed to download image!\n");
  }

  lsc->_log(lsc->_scratch_buf);
}//end download_cb()

int Ui_Linksprite_Control::initialize() {
  int rv = 0;
  _log("initializing camera control...\n");
  vector < linksprite::format_option > geoms = _ls->get_geometry_options();
  for (size_t i = 0; i<geoms.size(); i++) {
    sprintf (_scratch_buf, "adding dimension:%s [0x%x]\n",
        geoms[i].desc.c_str(), geoms[i].value);
    _isize_ch->add(geoms[i].desc.c_str(),0,0);
  }
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

  lsc->_log("capture image...\n");
  ls->take_image();
}//end image_capture_cb()

/*
void Ui_Linksprite_Control::_log(const char * s) {
  if (_log_buf != NULL) {
    _log_buf->append(s);
  }
}

*/
