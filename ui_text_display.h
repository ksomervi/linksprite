// Filename: ui.h
// Author: Kevin Somervill
// Date: Sat Aug 20 18:39:30 EDT 2016
#ifndef UI_TEXT_DISPLAY_H
#define UI_TEXT_DISPLAY_H

#define __USE_W32_SOCKETS

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

#include <cstdio>              // printf

#define STS_SCROLL_W 20

class Ui_Text_Display : public Fl_Group {
  private:
    Fl_Text_Buffer * s_buff;
    Fl_Text_Display * s_disp;
    //Fl_Scroll * s_scroll;

  public:
    Ui_Text_Display(int, int, int, int, char *);
    void append (const char *);
    void clear ();
    //void resize ();
};

#endif //defined(UI_TEXT_DISPLAY_H)
