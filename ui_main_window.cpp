// Filename: ui_main_window.cpp
// Author: Kevin Somervill
// Date: Sat Aug 20 18:41:26 EDT 2016

#include "ui_main_window.h"

Ui_Main_Window::Ui_Main_Window (int W, int H, const char*L=0) :
  Fl_Double_Window(W,H,L) {
  // Set the minimum size for the window
  size_range(MAIN_WIN_W, MAIN_WIN_H);
  int x = WIDGET_MARGIN;
  int y = 20;
  int w = 340;
  int h = 50;
  _pc = new Ui_Port_Control(x, y, w, h);
  add(_pc);

  y += _pc->h()+5;
  w = _pc->w();
  h = BUTTON_H*4 + WIDGET_MARGIN*5;
  _ls_ctrl = new Ui_Linksprite_Control(x, y, w, h);
  add(_ls_ctrl);

  x = _ls_ctrl->x();
  y = _ls_ctrl->y() + _ls_ctrl->h() + WIDGET_MARGIN;
  w = _ls_ctrl->w();
  h = MAIN_WIN_H - (_ls_ctrl->y() + _ls_ctrl->h() + WIDGET_MARGIN);
  _status = new Ui_Text_Display(x, y, w, h, NULL);
  add(_status);
  _status->clear();

  _pc->log_buffer(_status);
  _pc->ls_control_handle(_ls_ctrl);

  _ls_ctrl->log_buffer(_status);

 /*
  _status = new Ui_Text_Display(top_row->x(),
                                _text->h()+_text->y(),
                                this->w()-10,
                                120,
                                NULL);
                                */

  // Make the main window resizable
  resizable(_status);
  _ls_ctrl->deactivate();

  x = _pc->x() + _pc->w() + WIDGET_MARGIN;
  y = _pc->y();
  w = W - x;
  h = MAIN_WIN_H - 10;

  _disp = new Ui_Image_Display(x, y, w, h);

}

void Ui_Main_Window::initialize() {
  int rv = 0;
  // Clear the status display
  _status->clear();

  rv = _pc->initialize();
  // Initialize the serial ports
  if (rv == 0) {
    _status->append("Error: no ports found!\n");
  }
  rv = _ls_ctrl->initialize();
}
