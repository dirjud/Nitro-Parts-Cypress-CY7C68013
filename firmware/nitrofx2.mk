# set fx2dir to the directory that this makefile is included in
FX2DIR:=$(dir $(lastword $(MAKEFILE_LIST)))
FX2LIBDIR:=$(FX2DIR)../lib/fx2lib

DEPS = terminals.h build/vidpid.asm

include config.mk
include $(FX2LIBDIR)/lib/fx2.mk

terminals.h: ../terminals.py
	di --header terminals.h $<

build/vidpid.asm: Makefile
	echo "VID=$(VID)" >  $@ 
	echo "PID=$(PID)" >> $@ 
