#include <cstdio>      /* Standard input/output definitions */
#include <cstdlib> 
#include <cstring>     /* String function definitions */
#include <fcntl.h>     /* File control definitions */
#include <getopt.h>
//#include <termios.h>

#include "configuration.h"

//static
struct option loptions[] = {
      {"port",     required_argument,  0, 'p'},
      {"baud",     required_argument,  0, 'b'},
      {"console",  no_argument,        0, 'c'},
      {"help",     no_argument,        0, 'h'}
};

configuration::configuration() {
  _port_name = NULL;
  _baud = CONSOLE_DEFAULT_BAUD;
  _con = false;
  _help = false;
  _list = false;
}

void configuration::serialport (const char *port) {
  if (_port_name != NULL) {
    free(_port_name);
  }

  _port_name = (char*) malloc(strlen(port)+1);
  _port_name = strcpy(_port_name, port);
}

char * configuration::serialport (void) {
  return _port_name;
}

int configuration::baudrate(void) {
  return _baud;
}

void configuration::baudrate(int brate) {
  _baud = brate;
}

void configuration::help(bool h) {
  _help = h;
}
    
bool configuration::help(void) {
  return _help;
}
    
void configuration::list(bool l) {
  _list = l;
}

bool configuration::list(void) {
  return _list;
}

void configuration::console(bool c) {
  _con = c;
}
    
bool configuration::console(void) {
  return _con;
}
    
bool configuration::parse_options (int argc, char *argv[]) {
  bool parsing_options = true;

  if (argc==1) {
    return false;
  }

  /* parse options */
  int option_index = 0, opt;

  while(parsing_options) {
    opt = getopt_long (argc, argv, "hcp:b:l",
                       loptions, &option_index);

    switch (opt) {
      case 'b':
        this->baudrate(strtol(optarg,NULL,10));
        break;
      case 'c':
        this->console(true);
        break;
      case 'h':
        this->help(true);
        break;
      case 'l':
        this->list(true);
        break;
      case 'p':
        this->serialport(optarg);
        break;
      default:
        parsing_options = false;
    }//end switch (opt)
  }

  return true;    
} // end parse_options

