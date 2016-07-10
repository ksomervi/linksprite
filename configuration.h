// Filename: configuration.h
// Author: Kevin Somervill
// Date: Sun Apr 26 11:26:56 EDT 2015
#ifndef CONSOLE_APP_CONFIGURATION_H
#define CONSOLE_APP_CONFIGURATION_H
#include <getopt.h>

#define CONSOLE_DEFAULT_BAUD 115200

class configuration {
  private:
    char * _port_name;
    int    _baud;
    bool   _con;
    bool   _help;
    bool   _list;

  public:

    configuration();
    char * serialport(void);
    void serialport(const char *);
    int baudrate(void);
    void baudrate(int);
    void help(bool);
    bool help(void);
    void console(bool);//unused - later this will be to integrate the UI
    bool console(void);//unused - later this will be to integrate the UI
    bool parse_options (int, char **);
    void list(bool);
    bool list(void);

};

#endif //!define(CONSOLE_APP_CONFIGURATION_H)
