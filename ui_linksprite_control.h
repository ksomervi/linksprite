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
#include "ui_image_display.h"
#include "ui_logging_group.h"
#include "ui_text_display.h"

#include "linksprite.h"
using blip::linksprite;
using blip::image_buffer;

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
    linksprite *_ls;

    // Image parameters
    Fl_Choice * _isize_ch;
    Fl_Check_Button * _conlog_checkbutton;
    Fl_Progress * _iprogress;

    Ui_Image_Display* _img_display;
    Fl_Button* _isave_b;

    char _scratch_buf[256];
    image_buffer * _img_buf;

  protected:
    // Callback routines
    static void image_capture_cb (Fl_Widget * w, void *data);
    static void load_img_cb (Fl_Widget * w, void *data);
    static void save_img_cb (Fl_Widget * w, void *data);
    void set_geometry_cb (Fl_Widget * w, void *data);
    static void console_log_cb(Fl_Widget * w, void *data);

  public:
    Ui_Linksprite_Control(int, int, int, int);
    int initialize();
    void resize(int, int, int, int);
    linksprite * linksprite_handle();

    void image_display(Ui_Image_Display *);
    Ui_Image_Display * image_display();

};

#endif //!defined(UI_LINKSPRITE_CONTROL_H)
