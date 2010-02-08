
# When starting your own firmware, simply copy this file and change the path to
# point to nitrofx2.mk.  Copy config.mk locally and change it according to your
# own needs

DEPS=$(DEPS) fx2_terminals.h
include nitrofx2.mk

fx2_terminals.h: ../terminals.py
	di --header $@ $<
