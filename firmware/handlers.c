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


#include <fx2macros.h>

#include "firmware.h"
#include "vendor_commands.h"
#include <handlers.h>
#include <terminals.h>

#include "dummy.h"
#include "fx2term.h"
#include <m24xx.h>


io_handler code io_handlers[] = {
 DECLARE_FX2_HANDLER(TERM_FX2),
 DECLARE_FX2SFR_HANDLER(TERM_FX2_SFR),
 DECLARE_M24XX_HANDLER(TERM_FX2_PROM),
 DECLARE_NULL_HANDLER
};
