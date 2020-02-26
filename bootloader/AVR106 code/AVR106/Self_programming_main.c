 /* Copyright (C) 2006-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/*! \file ******************************************************************** 
*
* Atmel Corporation
*
* - File              : Self_programming_main.c
* - Compiler          : avr-gcc
*
* - Supported devices : ATmega128.
*
* - AppNote           : AVR106 - C functions for reading and writing
*                       to flash memory.
*
* - Description       : The file contains an example program using the Flash R/W
*                       functions provided with the files Self_programming.h /
*                       Self_programming.c . 
*
*****************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Self_programming.h"
#include "flash.h"
#include <avr/pgmspace.h>

 int main( void )
{
	unsigned char testBuffer1[PAGESIZE];    // Declares variables for testing
	unsigned char testBuffer2[PAGESIZE];    // Note. Each array uses PAGESIZE bytes of
										    // code stack
    static unsigned char testChar; //A warning says that the var is set but never used.Ignore it.
    int index;
    RecoverFlash();
    for(index=0; index<PAGESIZE; index++)
	{
		testBuffer1[index]=(unsigned char)index; // Fills testBuffer1 with values 0,1,2..255
	}
   
    for(;;)
	{
		cli();
		WriteFlashPage(0x2000, testBuffer1);    // Writes testbuffer1 to Flash page 2
												// Function returns TRUE		
		ReadFlashPage(0x2000, testBuffer2);     // Reads back Flash page 2 to TestBuffer2
												// Function returns TRUE
   
		WriteFlashByte(0x2004, 0x38);            // Writes 0x38 to byte address 0x204
												 // Same as byte 4 on page 2
												 // Returns TRUE
		testChar = ReadFlashByte(0x2004);        // Reads back value from address 0x204
	
		asm("nop");
    }
}
