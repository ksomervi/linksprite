# Makefile 
#

HOST=$(shell uname -o)

CXXFLAGS = -Wall -g -O3 

ifeq "$(HOST)" "Cygwin"
CC = i686-pc-mingw32-gcc
CXX = i686-pc-mingw32-g++

CXXFLAGS += -DWIN32

LDFLAGS = -static-libstdc++ -static-libgcc
FL_LDFLAGS = -mwindows -lfltk -lole32 -luuid -lcomctl32 $(LDFLAGS)

else
CC = gcc
CXX = g++

LDFLAGS = -static-libgcc -lcm
#FL_LDFLAGS = -lfltk -lX11 -ldl -luuid $(LDFLAGS)
FL_LDFLAGS = $(shell fltk-config --ldstaticflags --use-images) $(LDFLAGS)
# --use-images adds /usr/lib64/libfltk_images.a -lpng -lz -ljpeg

endif

CFLAGS = $(CXXFLAGS) -std=c99 

LSC_SRC = console_main.cpp 
LSC_SRC += configuration.cpp linksprite.cpp
LSC_OBJ = $(subst cpp,o,$(LSC_SRC))

UI_SRC = ui.cpp 
UI_SRC += ui_main_window.cpp
UI_SRC += ui_linksprite_control.cpp
UI_SRC += ui_port_control.cpp
UI_SRC += ui_text_display.cpp
UI_SRC += linksprite.cpp
UI_OBJ = $(subst .cpp,.o,$(UI_SRC))

INCDIRS = -I/usr/local/include 
LIBDIRS = -L./serialport -L/usr/local/lib

SUBDIRS = serialport

CLEANDIRS = $(SUBDIRS:%=clean-%)

TARGETS = lscon lsui

.SILENT:

.cpp.o:
	echo "# Compiling $@ ..."
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $<

.PHONY: default all neat clean subdirs $(SUBDIRS)

default: lsui

all: $(TARGETS)

lscon: $(LSC_OBJ) $(SUBDIRS)
	echo "# Linking $@ ..."
	$(CXX) -o $@ $(LSC_OBJ) $(LIBDIRS) $(LDFLAGS)

lsui: $(UI_OBJ) $(SUBDIRS)
	echo "# Linking $@ ..."
	$(CXX) $(LIBDIRS) -o $@ $(UI_OBJ) $(FL_LDFLAGS)

neat: 
	-rm -f *~ *.o

clean: neat $(CLEANDIRS)
	echo "# Cleaning up targets ..."
	-rm -f ${TARGETS}{,.exe}

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

