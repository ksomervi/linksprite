// Filename: ui_logging_group.cpp
// Author: Kevin Somervill
// Date: Fri May 22 18:00:58 EDT 2015

#include "ui_logging_group.h"

Ui_Logging_Group::Ui_Logging_Group(int x, int y, int w, int h, const char* L) :
  Fl_Group(x, y, w, h, L) {

}

void Ui_Logging_Group::log_buffer(Ui_Text_Display * td) {
  _log_buf = td;
}

void Ui_Logging_Group::_log(const char * s) {
  if (_log_buf != NULL) {
    _log_buf->append(s);
  }
}
