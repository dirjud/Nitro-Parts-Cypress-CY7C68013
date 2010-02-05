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

#include "i2c_handlers.h"
#include <i2c.h>
#include <firmware.h>
#include <handlers.h>

void i2c_a8_d8_get(WORD len) {
  // Does a single bytes get regardless of the 'len' field and packs the
  // result in a 16b work in the return fifo.  If the 'len' is greater
  // than 2, remaining data returned will be junk.
  
  BYTE buf[1];
  buf[0] = LSB(rdwr_data.h.reg_addr);
  
  // First set the address to read from
  if(!i2c_write(rdwr_data.h.term_addr, 1, buf, 0, NULL)) { 
    rdwr_data.aborted = TRUE;
    return;
  }
  
  // Now read a single byte
  if(!i2c_read(rdwr_data.h.term_addr, 1, EP6FIFOBUF)) {
    rdwr_data.aborted = TRUE;
    return;
  }
  EP6FIFOBUF[1] = 0;
  
  rdwr_data.aborted = FALSE;
  rdwr_data.bytes_avail = len;
  rdwr_data.h.reg_addr += 1;
}

BOOL i2c_a8_d8_set () {
  // Does a single byte set regardless of the amount of data requested
  // to be set.
  
  BYTE buf[1];
  buf[0] = LSB(rdwr_data.h.reg_addr);
  if(!i2c_write(rdwr_data.h.term_addr, 1, buf, 1, EP2FIFOBUF)) { 
    rdwr_data.aborted = TRUE;
  }
  rdwr_data.aborted = FALSE;
  return TRUE;
}



void i2c_a8_d16_get(WORD len) {
  BYTE buf[1];
  buf[0] = LSB(rdwr_data.h.reg_addr);

  // First set the address to read from
  if(!i2c_write(rdwr_data.h.term_addr, 1, buf, 0, NULL)) { 
    printf("I2C addr write FAILED\n");
    rdwr_data.aborted = TRUE;
    return;
  }

  // Now read all the bytes
  if(!i2c_read(rdwr_data.h.term_addr, len, EP6FIFOBUF)) {
    printf("I2C read Failed\n");
    rdwr_data.aborted = TRUE;
    return;
  }

  rdwr_data.aborted = FALSE;
  rdwr_data.bytes_avail = len;
  
}

BOOL i2c_a8_d16_set () {
    BYTE buf[1];

    buf[0] = LSB(rdwr_data.h.reg_addr);
    if(!i2c_write(rdwr_data.h.term_addr, 1, buf, rdwr_data.bytes_avail, EP2FIFOBUF)) { 
      rdwr_data.aborted = TRUE;
    }
    rdwr_data.aborted = FALSE;
    return TRUE;
}
