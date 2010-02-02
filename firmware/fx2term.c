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
#include <fx2regs.h>
#include "firmware.h"
#include <delay.h>
#include "terminals.h"
#include "handlers.h"

extern code WORD dev_dscr; // the device descriptor.

extern BYTE asm_get_fx2_sfr(BYTE);
extern void asm_set_fx2_sfr(BYTE, BYTE);

void get_fx2(WORD len) {
    printf ( "get_fx2, len: %d\n" , len );
    if ( len == 2 ) {
        EP6FIFOBUF[1] = 0; // since most of these are just one byte...
        switch ( rdwr_data.h.reg_addr ) {
           case FX2_VERSION:
               EP6FIFOBUF[0] = *((BYTE*)(&dev_dscr) + 12);
               EP6FIFOBUF[1] = *((BYTE*)(&dev_dscr) + 13);
               break;
           default:
               if (rdwr_data.h.reg_addr >= 0xe600 && rdwr_data.h.reg_addr <= 0xfdff ) {
                   xdata BYTE* a = *((xdata BYTE**)&rdwr_data.h.reg_addr);
                   printf ( "Get %04x a: %04x *a: %02x\n " , (WORD)rdwr_data.h.reg_addr, (WORD)a, *a );
                   EP6FIFOBUF[0] = *a;
                   break;
               }
          
       }
        rdwr_data.bytes_avail=2;
    } else {
        rdwr_data.aborted=TRUE;
    }
}


BOOL set_fx2() {
    BYTE val=EP2FIFOBUF[0];
    printf ( "set_fx2, bytes_avail: %d val: %02x\n", rdwr_data.bytes_avail, val );
    if (rdwr_data.bytes_avail == 2) {
        if (rdwr_data.h.reg_addr >= 0xe600 && rdwr_data.h.reg_addr <= 0xfdff ) {
            WORD reg_addr = rdwr_data.h.reg_addr; // cast 31 bit to 16
            xdata BYTE *a = *((xdata BYTE**)&rdwr_data.h.reg_addr);
            printf ( "Set %04x a: %04x *a: %02x\n " , (WORD)rdwr_data.h.reg_addr, (WORD)a, *a );
            *a = val;
        }
        ++rdwr_data.h.reg_addr;
    } else {
        rdwr_data.aborted=TRUE;
    }
    return TRUE;
}


void get_fx2_sfr(WORD len) {
   if ( len == 2 ) {
     EP6FIFOBUF[0] = asm_get_fx2_sfr(rdwr_data.h.reg_addr);
     EP6FIFOBUF[1] = 0;
     rdwr_data.bytes_avail=2; 
   } else {
      rdwr_data.aborted=TRUE;
   }
}

BOOL set_fx2_sfr() {
    asm_set_fx2_sfr(rdwr_data.h.reg_addr, EP2FIFOBUF[0] );
    return TRUE;
}
