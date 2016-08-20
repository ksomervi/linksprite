// Filename: ui_linksprite_control.h
// Author: Kevin Somervill
// Date: Tue Apr 14 20:51:28 EDT 2015
#ifndef UI_LINKSPRITE_CONTROL_H
#define UI_LINKSPRITE_CONTROL_H

#include <FL/Fl.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Progress.H>

#include "ui_defines.h"
#include "ui_logging_group.h"
#include "ui_text_display.h"

#include "linksprite.h"
using blip::linksprite;

#define UI_MAX_PORTS 15

/*
#ifdef _WIN32
#define PORT_NAME_BASE   "COM"
#else
#define PORT_NAME_BASE   "/dev/tty"
#endif
*/

class Ui_Linksprite_Control : public Ui_Logging_Group {

  private:
  /*
    class _port_desc {
      public:
      int index;
      char * name;
    };

    // Serial port configuration
    //Fl_Light_Button * _cport_ctrl_b;
    //Fl_Choice * _cport_ch;
    //Fl_Choice * _cbaud_ch;
    */
    linksprite *_ls;

    // Image parameters
    Fl_Choice * _isize_ch;
    Fl_Check_Button * _conlog_checkbutton;
    Fl_Input  * _ifname_in;
    Fl_Progress * _iprogress;

    char _scratch_buf[256];
    //int _ports[UI_MAX_PORTS];
    //Ui_Text_Display * _log_buf;

    //void _log(const char*);

  protected:
    // Callback routines
    //inline void aux_cport_ctrl_cb (Fl_Widget *w, void *data);
    static void image_capture_cb (Fl_Widget * w, void *data);
    static void download_cb (Fl_Widget * w, void *data);
    void set_geometry_cb (Fl_Widget * w, void *data);
    static void console_log_cb(Fl_Widget * w, void *data);

  public:
    Ui_Linksprite_Control(int, int, int, int);
    int initialize();
    void resize(int, int, int, int);
    linksprite * linksprite_handle();

};

#endif //!defined(UI_LINKSPRITE_CONTROL_H)
