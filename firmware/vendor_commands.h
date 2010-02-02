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

#ifndef VENDOR_COMMANDS_H
#define VENDOR_COMMANDS_H

// allow other compilers to include this header 
#ifdef SDCC
#include <fx2types.h>
typedef BYTE uint8;
typedef WORD uint16;
typedef DWORD uint32;
#else
// for gcc/win32 etc
#include <nitro/types.h>
#endif

#define bmSETWRITE 8
/** 
 * command codes for rdwr_data_t.command
 **/
enum NITRO_COMMAND {
   COMMAND_READ,
   COMMAND_WRITE=bmSETWRITE,
   COMMAND_GET=1,
   COMMAND_SET=bmSETWRITE|1
};

// structure for passing data from rdwr vendor command to rdwr handlers
#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

/**
 * This is the payload data for the vendor command to initiate a read, write,
 * get, or set.
 **/
typedef struct {
  /**
   * The command.  A NITRO_COMMAND.
   **/
  uint8 command;

  /**
   * The terminal address.
   **/
  uint16 term_addr;

  /**
   * The register address
   **/
  uint32 reg_addr;

  /**
   * Total transfer length
   **/
  uint32 transfer_length;
  
} 
#ifdef __GNUG__
 __attribute__((__packed__))
#endif
rdwr_data_header;

typedef struct { 
  /**
   * The header data is copied directly from the vendor command data.
   **/
  rdwr_data_header h;
  /**
   * Used by the main loop to signal that a read or write is in progress.
   * Handler functions should not modify this variable.
   **/
  uint8 in_progress;
  
  /**
   * For read handlers, this is the number of bytes availabe in the read
   * endpoint.
   *
   * For write handlers, this is the number of bytes written to the endpoint.
   **/
  uint16 bytes_avail;

  /**
   * Total progress of the current transaction for reads or writes.
   **/
  union {
   uint32 bytes_written;
   uint32 bytes_read;
  };

  /**
   * If an error occurs, a handler can set aborted=TRUE to cause the main
   * loop to discard any additional OUT data and not attempt to send any
   * more IN data.
   **/
  uint8 aborted;
} 
#ifdef __GNUG__
 __attribute__((__packed__))
#endif
rdwr_data_t;
#ifdef WIN32
#pragma pack(pop)
#endif


enum NITRO_VC { 

 // implemented by fx2 core
 VC_RDWR_RAM=0xa0,
 
 /**
  * type 0xc0
  *  returns sizeof(rdwr_data_t) bytes 
  *  returns the rdwr_data structure
  **/

 VC_RDWR_STAT=0xb1,

/**
 * type 0x40 to initiate a read or a write
 *
 * value = ep_addr
 * index = rdwr_addr
 * length = 5
 *  send 1 byte (0=read 1 = write, 2=get, 3=set) followed by 4 bytes for length (send 4 bytes little-endian!)
 **/
VC_HI_RDWR=0xb4,

/**
 * type 0x40
 * Cause the device to renumerate
 **/
VC_RENUM=0xb5,


/**
 * type 0x40 -> set the serial
 * type 0xc0 <- get the serial
 * length = 8
 * \return 8 byte serial number 
 **/
VC_SERIAL=0xb6

};

#endif
