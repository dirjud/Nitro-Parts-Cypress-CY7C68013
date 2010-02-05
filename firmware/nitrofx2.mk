# set fx2dir to the directory that this makefile is included in
FX2DIR:=$(dir $(lastword $(MAKEFILE_LIST)))
FX2LIBDIR:=$(FX2DIR)../lib/fx2lib

DEPS = terminals.h build/vidpid.asm firmwarever.h

include config.mk
include $(FX2LIBDIR)/lib/fx2.mk

FIRMWARE_VERSION?=1

terminals.h: ../terminals.py
	di --header terminals.h $<

firmwarever.h: config.mk
	echo "#ifndef FIRMWARE_VERSION" > $@
	echo "#define FIRMWARE_VERSION $(FIRMWARE_VERSION)" >> $@
	echo "#endif" >> $@ 

build/vidpid.asm: config.mk 
	echo "VID=$(VID)" >  $@ 
	echo "PID=$(PID)" >> $@ 
