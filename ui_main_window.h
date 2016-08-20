// Filename: ui_main_window.h
// Author: Kevin Somervill
// Date: Sat Aug 20 18:41:06 EDT 2016
#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#ifdef _WIN32
#define __USE_W32_SOCKETS
#endif

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>

#include <cstdio> // printf

#include "ui_port_control.h"
#include "ui_linksprite_control.h"
#include "ui_text_display.h"
#include "ui_image_display.h"
#include "ui_defines.h"

class Ui_Main_Window : public Fl_Double_Window {
  private:
    Ui_Linksprite_Control * _ls_ctrl;
    Ui_Port_Control * _pc;
    //Ui_Text_Display * _text;
    Ui_Text_Display * _status;
    Ui_Image_Display * _disp;

  protected:
    // Callback routines
    //inline void aux_cport_ctrl_cb (Fl_Widget *w, void *data);
    //static void cport_ctrl_cb (Fl_Widget * w, void *data);

  public:
    Ui_Main_Window (int W, int H, const char*L);

    void initialize ();
};

#endif //defined(UI_MAIN_WINDOW_H)

