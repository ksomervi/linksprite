// Filename: ui_port_control.cpp
// Author: Kevin Somervill
// Date: Thu Apr 23 21:07:15 EDT 2015

#include "ui_port_control.h"

#ifdef _WIN32
#define __USE_W32_SOCKETS
#include <Windows.h> //QueryDosDevice()
#endif

Ui_Port_Control::Ui_Port_Control(int x, int y, int w, int h) :
  Ui_Logging_Group(x, y, w, h, "Camera Configuration") {
  resizable(0);
  align(Fl_Align(FL_ALIGN_TOP_LEFT));
  box(FL_ENGRAVED_BOX);
  begin();
  _cport_ch = new Fl_Choice(x+WIDGET_MARGIN, y+20,
                            140, 25, "Port Name");
  _cport_ch->tooltip("Filename for camera serial port.");
  _cport_ch->align(Fl_Align(FL_ALIGN_TOP_LEFT));

  int x_loc = _cport_ch->x() + _cport_ch->w() + 5;
  int y_loc = _cport_ch->y();
  int width = 100;
  int height = 25;
  _cbaud_ch = new Fl_Choice(x_loc, y_loc, width, height,
                              "Baud");
  _cbaud_ch->align(Fl_Align(FL_ALIGN_TOP_LEFT));
  //FIXME: this is a temporary hack for the baud rates.
  _cbaud_ch->add("115200", 0, 0);
  _cbaud_ch->add("38400", 0, 0);
  _cbaud_ch->add("9600", 0, 0);
  _cbaud_ch->value(0);

  _cport_ctrl_b = new Fl_Light_Button(x+w-85, _cport_ch->y(),
      80, BUTTON_H, "Open");
  _cport_ctrl_b->callback(cport_ctrl_cb, (void*)this);
  _cport_ctrl_b->selection_color(FL_RED);
  add(_cport_ch);
  add(_cport_ctrl_b);
  end();

  _port.index = -1;
  _port.name = NULL;
  _ls = NULL;
  _ls_ctrl = NULL;
}

int Ui_Port_Control::initialize() {
  _log("initializing serial port control...\n");
  int rv = 0;

  rv = scan_ports();
  if (0 == rv) {
    _cport_ch->add("NONE");
    _cport_ch->value(0);
    _cport_ctrl_b->deactivate();
  }
  else {
    _cport_ch->value(rv-1);
  }
  return rv;
}

int Ui_Port_Control::scan_ports() {
  _log("# Checking available ports\n");
  communication_manager *cm = new communication_manager();
  _ports = cm->get_available_ports();
  delete cm;
  size_t sz = _ports.size();
  for (size_t i=0; i<sz; i++) {
    sprintf (_scratch_buf, "# %s available\n", _ports[i]);
    _log(_scratch_buf);
    _cport_ch->add(_ports[i]);
  }

  _log("# Done checking available ports\n");
  return sz;
}

void Ui_Port_Control::resize(int x, int y, int w, int h) {

}

/*
void Ui_Port_Control::log_buffer(Ui_Text_Display * d) {
  _log_buf = d;
}

void Ui_Port_Control::_log(const char * s) {
  if (_log_buf != NULL) {
    _log_buf->append(s);
  }
}
*/

void Ui_Port_Control::aux_cport_ctrl_cb (Fl_Widget *w, void *data) {
  Ui_Port_Control *pc = (Ui_Port_Control*)(data);
  pc->_log(_cport_ch->text());
  if (_ls->is_open()) {
    pc->_log(": Closing serial port\n");
    _ls->close();
    _cport_ch->activate();
    _cbaud_ch->activate();
    _cport_ctrl_b->label("Open");
    _ls_ctrl->deactivate();
  }
  else {
    pc->_log(": Opening serial port\n");

    if (_ls->open(_cport_ch->text(), atoi(_cbaud_ch->text()))) {
      sprintf(_scratch_buf, "%s: port opened %d baud\n",
          _ls->port_name(), _ls->baudrate());
      pc->_log(_scratch_buf);
      _cport_ch->deactivate();
      _cbaud_ch->deactivate();
      _cport_ctrl_b->label("Close");
      _ls_ctrl->activate();
    }
    else {
      pc->_log(_cport_ch->text());
      sprintf(_scratch_buf, "%s", " Failed to open port!\n");
      pc->_log(_scratch_buf);
    }
  }
}

void Ui_Port_Control::cport_ctrl_cb (Fl_Widget * w, void *data) {
  ((Ui_Port_Control*)data)->aux_cport_ctrl_cb(w, data);

}//end cport_ctrl_cb()

void Ui_Port_Control::ls_control_handle(Ui_Linksprite_Control *lsc) {
  _ls_ctrl = lsc;
  _ls = _ls_ctrl->linksprite_handle();
}//end register_ls_control()
