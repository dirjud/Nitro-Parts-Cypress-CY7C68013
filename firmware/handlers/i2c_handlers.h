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
#include <handlers.h>

// These are generic i2c handlers for various address and data widths.
// The name of the function indicates the number of address and data bits
// the given function supports.

void i2c_a8_d8_get(WORD len);
BOOL i2c_a8_d8_set();

#define DECLARE_I2C88_HANDLER(term) \
    DECLARE_HANDLER(term,0,0,i2c_a8_d8_get,i2c_a8_d8_set,0,0,0)
