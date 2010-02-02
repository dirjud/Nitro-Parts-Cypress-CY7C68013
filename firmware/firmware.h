/**
 * Copyright (C) 2009 Ubixum, Inc. 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **/


#include <string.h>

#include <fx2regs.h>
#include <fx2macros.h>
#include <fx2ints.h>
#include <autovector.h>
#include <setupdat.h>
#include <i2c.h>
#include <eputils.h>

#define SYNCDELAY() SYNCDELAY4;

#ifdef DEBUG_FIRMWARE
#include <stdio.h>
#define REENTRANT __reentrant
#else
#define printf(...)
#define REENTRANT
#endif

void reset_endpoints();


extern volatile xdata WORD in_packet_max;
extern volatile bit new_vc_cmd;

#define PROM_ADDR 0x51 // prom for serial number
#define PROM_SERIAL_OFFSET 0xff9b // 0xffff-100 = last 100 bytes of the 16k prom


