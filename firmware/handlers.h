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


#ifndef HANDLERS_H
#define HANDLERS_H

#include <fx2types.h>

#include "vendor_commands.h"

/**
 * API for IO handlers.
 *
 **/


/**
 * The RDWR vendor command populates the rdwr_data struct and calls an optional
 * init function for each terminal.  Handlers are in charge of the term_addr
 * and reg_addr of the rdwr_data structure.  A handler may increment the
 * reg_addr each time a byte is written for instance.
 **/


/**
 *
 * READ handlers: void read_handler(WORD length);
 *  length is the number of maximum number of bytes that can be read from the
 *  terminal/register into the read endpoint.  After reading bytes into the
 *  endpoint, rdwr_data.bytes_avail should contains the number of bytes in the 
 *  endpoint.  The endpoint is then committed with the corresponding byte count
 *  and available for the Host to read.  A read handler need not read all the 
 *  bytes into the endpoint.  Handlers should not block.  If not bytes are
 *  read, the 0 length packets are not committed.  Read handlers should NOT 
 *  commit the packets.
 *
 *  Currently, read/write handlers should not be NULL.  The dummy terminal
 *  handlers can be used in place of a null read or write handler when reading
 *  or writing from the terminal is not supported.
 **/
typedef void (*io_handler_read_func)(WORD);

/**
 * WRITE handlers: WORD write_handler():
 *  The number of bytes available in the write buffer are stored in
 *  rdwr_data.bytes_avail.  The write_handler must handle the available bytes
 *  without blocking. The handler can return FALSE instead of TRUE to allow
 *  another pass at the same data if the write call would block.
 *  
 *  \return BOOL return TRUE to indicate that the data has been written o
 *      FALSE to have the same data available to process the next time the
 *      function is called.
 **/
typedef BOOL (*io_handler_write_func)();


/**
 * Optional code to be run before a read/write transaction.  function can be
 * NULL in read/write structure.
 **/
typedef void (*io_handler_init_func)();

/**
 * After a transaction is finished, the ack is sent back to the host.  This
 * function takes a pointer to checksum and status.  The function can be NULL
 * In the read/write structure.  Status must be 0 for a successful transaction
 * and checksum is the 16 bit sum (ignoring carry) of all bytes transferred.  A
 * NULL ack function causes a default ack of status/checksum both 0 to be sent.
 * 0 checksum only causes drivers to fail if the host is verifying checksums.
 **/
typedef WORD (*io_handler_status_func)();

/**
 * Same as io_handler_status.  Function can be NULL;
 **/
typedef WORD (*io_handler_chksum_func)();


/**
 *  rdwr vendor command handles a NULL-terminated array of these structures 
 *  when setting up the read/write transfer.
 **/
typedef struct {
 WORD term_addr;
 io_handler_read_func read_handler;
 io_handler_write_func write_handler;
 io_handler_init_func init_handler;
 io_handler_status_func status_handler;
 io_handler_chksum_func chksum_handler;
} io_handler;


extern xdata rdwr_data_t rdwr_data;


#endif
