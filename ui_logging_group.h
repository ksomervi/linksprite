// Filename: ui_logging_group.h
// Author: Kevin Somervill
// Date: Fri May 22 18:00:58 EDT 2015
#ifndef UI_LOGGING_GROUP_H
#define UI_LOGGING_GROUP_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>

#include "ui_text_display.h"

class Ui_Logging_Group : public Fl_Group {
  private:
    Ui_Text_Display * _log_buf;
    //unimplemented
    Ui_Logging_Group(const Ui_Logging_Group&);

  protected:
    void _log(const char*);

  public:
    Ui_Logging_Group(int x, int y, int w, int h, const char* =0);

    void log_buffer(Ui_Text_Display *);
};

#endif //!defined(UI_LOGGING_GROUP_H)

