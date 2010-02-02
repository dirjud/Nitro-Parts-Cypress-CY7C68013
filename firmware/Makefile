FX2LIBDIR = ../lib/fx2lib

# possible env flags
# -D DEBUG_FIRMWARE - enable stdio & printf on sio-0 (57600 buad)
SDCCFLAGS := $(SDCCFLAGS)

###
# dscr is under 200 bytes currently
# int2jt is under 200 bytes
# 400 bytes for xram

# usbjt 4000 - 100(256) = 3f00 (must lie on page boundary) 
# dscr 3f00 - 100(256) = 3e00 
# xram 3e00 - 200(512) = 3c00 
###

BASENAME = firmware
DEPS = terminals.h vidpid.inc

SOURCES = firmware.c \
          dummy.c \
		  main.c \
		  handlers.c \
		  fx2term.c
A51_SOURCES = dscr.a51 fx2_sfr.a51

include $(FX2LIBDIR)/lib/fx2.mk

VID=0x1fe1
PID=0x8613

terminals.h: ../terminals.py
	di --header terminals.h $<

vidpid.inc:	
	echo "VID=$(VID)" > vidpid.inc
	echo "PID=$(PID)" >> vidpid.inc
