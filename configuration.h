#ifndef CONSOLE_APP_CONFIGURATION_H
/*
 *
 */
#define CONSOLE_APP_CONFIGURATION_H
#include <getopt.h>

#define CONSOLE_DEFAULT_BAUD 115200

class configuration {
  private:
    char * _port_name;
    int    _baud;
    bool   _con;
    bool   _help;

  public:

    configuration();
    char * serialport(void);
    void serialport(const char *);
    int baudrate(void);
    void baudrate(int);
    void help(bool);
    bool help(void);
    void console(bool);
    bool console(void);
    bool parse_options (int, char **);

};

#endif //!define(CONSOLE_APP_CONFIGURATION_H)
