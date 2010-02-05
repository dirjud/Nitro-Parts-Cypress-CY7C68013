# FX2DIR is automatically set in nitrofx2.mk for convinience
# it has a trailing slash

# possible env flags
# -D DEBUG_FIRMWARE - enable stdio & printf on sio-0 (57600 buad)
SDCCFLAGS:=$(SDCCFLAGS)-I../../../Microchip/M24XX/fx2 -I.
BASENAME=firmware

# for any additional targets you provide locally that need built before the 
# firmware is compiled
DEPS:=$(DEPS)

# your files to build
SOURCES = $(FX2DIR)firmware.c \
          $(FX2DIR)dummy.c \
	  $(FX2DIR)main.c \
	  $(FX2DIR)handlers.c \
	  $(FX2DIR)fx2term.c \
	  $(FX2DIR)postinit.c \
	  ../../../Microchip/M24XX/fx2/m24xx.c

A51_SOURCES = $(FX2DIR)/dscr.a51 $(FX2DIR)/fx2_sfr.a51

# your vid and pid
VID=0x1fe1
PID=0x8613

