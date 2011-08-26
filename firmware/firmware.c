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


#ifdef DEBUG_FIRMWARE 
#include <serial.h>
#include <stdio.h>
#else
#define printf(...)
#endif

#include "firmware.h"
#include "vendor_commands.h"
#include "handlers.h"


volatile __bit dosud=FALSE;
volatile __bit dosuspend=FALSE;
volatile __bit new_vc_cmd;

// custom functions
extern void main_loop();
extern void main_init();


void main() {

 SETCPUFREQ(CLK_48M); // required for sio0_init 

#ifdef DEBUG_FIRMWARE
 // main_init can still set this to whatever you want.
 sio0_init(57600); // needed for printf if debug defined 
#endif

 main_init();

 // set up interrupts.
 USE_USB_INTS();
 
 ENABLE_SUDAV();
 ENABLE_USBRESET();
 ENABLE_HISPEED(); 
 // NOTE causes some Hydro boards to not enumerate?
// ENABLE_SUSPEND();
// ENABLE_RESUME();

 // this board has no WAKEUP
 WAKEUPCS &= ~0x03;

 EA=1;

// use RENUMERATE_UNCOND() if need to change device descriptor.
#ifdef RENUM_UNCOND 
 RENUMERATE_UNCOND(); 
#else
 RENUMERATE();
#endif
 
 while(TRUE) {

     main_loop();

     if (dosud) {
       dosud=FALSE;
       new_vc_cmd=0;
       handle_setupdata();
     }

     if (dosuspend) {
        dosuspend=FALSE;
        do {
           printf ( "I'm going to Suspend.\n" );
           WAKEUPCS |= bmWU|bmWU2; // make sure ext wakeups are cleared
           SUSPEND=1;
           PCON |= 1;
           SYNCDELAY(); SYNCDELAY();
        } while ( !remote_wakeup_allowed && REMOTE_WAKEUP()); 
        printf ( "I'm going to wake up.\n");

        // resume
        // trm 6.4
        if ( REMOTE_WAKEUP() ) {
            delay(5);
            USBCS |= bmSIGRESUME;
            delay(15);
            USBCS &= ~bmSIGRESUME;
        }

     }

 } // end while

} // end main

void resume_isr() __interrupt RESUME_ISR {
 CLEAR_RESUME();
}
  
void sudav_isr() __interrupt SUDAV_ISR {
 // if the vendor command is a custom vendor
 // command for our firmware, make sure we
 // cancel any pending reads/writes etc, to 
 // avoid possible lock situations.
 // let all other vendor commands fall through.
 printf ( "SUDAV\n" );
 if (SETUPDAT[1] == VC_HI_RDWR || 
    (SETUPDAT[1] == VC_SERIAL && SETUPDAT[0] == 0x40) // sets on serial are unsafe, gets are safe
 ) {
  new_vc_cmd=1; // deprecated, needs to use aborted
  cancel_i2c_trans=TRUE; // deprecated, needs to move to i2c interrupts
  rdwr_data.aborted=TRUE;
  dosud=TRUE;
  printf ( "Unsafe\n" );
 } else if (rdwr_data.in_progress && !rdwr_data.autocommit) {
   printf ( "Handle in interrupt..\n" );
   handle_setupdata();
 } else {
    printf ( "Safe\n" );
    dosud=TRUE;
 }
 CLEAR_SUDAV();
}
void usbreset_isr() __interrupt USBRESET_ISR {
 in_packet_max=FULLSPD_EP6_SIZE;
 handle_hispeed(FALSE);
 printf ( "usb reset\n" );
 CLEAR_USBRESET();
}
void hispeed_isr() __interrupt HISPEED_ISR {
 in_packet_max=HISPD_EP6_SIZE;
 handle_hispeed(TRUE);
 printf ( "usb_hispeed\n" );
 CLEAR_HISPEED();
}

void suspend_isr() __interrupt SUSPEND_ISR {
 dosuspend=TRUE;
 CLEAR_SUSPEND();
}
