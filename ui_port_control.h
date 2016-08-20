// Filename: ui_port_control.h
// Author: Kevin Somervill
// Date: Thu Apr 23 21:08:33 EDT 2015
#ifndef UI_PORT_CONTROL_H
#define UI_PORT_CONTROL_H

#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Toggle_Button.H>

#include "linksprite.h"
using blip::linksprite;

#define UI_MAX_PORTS 15

#ifdef _WIN32
#define PORT_NAME_BASE   "COM"
#else
#define PORT_NAME_BASE   "/dev/ttyUSB"
#endif

#include "ui_defines.h"
#include "ui_text_display.h"
#include "ui_logging_group.h"
#include "ui_linksprite_control.h"

class Ui_Port_Control : public Ui_Logging_Group {
  private:
    class _port_desc {
      public:
      int index;
      char * name;
    };

    linksprite * _ls;
    Ui_Linksprite_Control *_ls_ctrl;

    Fl_Light_Button * _cport_ctrl_b;
    Fl_Choice * _cport_ch;
    Fl_Choice * _cbaud_ch;
    _port_desc _port;
    char _scratch_buf[256];
    port_list _ports;
    //Ui_Text_Display * _log_buf;

    //void _log(const char*);

  protected:
    // Callback routines
    inline void aux_cport_ctrl_cb (Fl_Widget *w, void *data);
    static void cport_ctrl_cb (Fl_Widget * w, void *data);
    int scan_ports();

  public:
    Ui_Port_Control(int, int, int, int);
    int initialize();
    void resize(int, int, int, int);
    //void log_buffer(Ui_Text_Display *);
    void ls_control_handle(Ui_Linksprite_Control *);
};

#endif //defined(UI_PORT_CONTROL_H)
