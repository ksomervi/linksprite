// Filename: ui_text_display.cpp
// Author: Kevin Somervill
// Date: Sat May 23 12:23:41 EDT 2015

#include "ui_text_display.h"

Ui_Text_Display::Ui_Text_Display(int x, int y, int w, int h, char *l=0)
  : Fl_Group(x, y, w, h, l) {
  s_buff = new Fl_Text_Buffer();
  begin();
  s_disp = new Fl_Text_Display(x, y, w, h);
  s_disp->buffer(s_buff);
  end();
}

void Ui_Text_Display::append (const char *s) {
  s_buff->append(s);
  s_disp->insert_position(s_buff->length());
  s_disp->show_insert_position();
}

void Ui_Text_Display::clear () {
  s_buff->text("");
}

