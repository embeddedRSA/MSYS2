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
* - File              : Self_programming.h
* - Compiler          : avr-gcc
*
* - Supported devices : All devices with bootloaders support.
*
* - AppNote           : AVR106 - C functions for reading and writing
*                       to flash memory.
*
* - Description       : The file contains functions for easy reading and writing
*                       of Flash memory on parts having the "Self-programming"
*                       feature. The user functions are as follows:
*
*                       ReadFlashByte()	
*                       ReadFlashPage()	
*                       WriteFlashByte()
*                       WriteFlashPage()	
*                       RecoverFlash()	
*
*                       The remaining functions contained in this file are used
*                       by the functions listed above.
*
*****************************************************************************/
#include <avr/io.h>

#ifndef __SELF_PROGRAMMING_H
#define __SELF_PROGRAMMING_H


/******************************************************************************
* Defines for configuration
******************************************************************************/

#define __FLASH_RECOVER  // This define enables FLASH buffering to avoid
                              // data loss if reset or power break when writing
#define ADR_FLASH_BUFFER 0xEF00    // Defines the FLASH page address used for buffering
                              // if enabled. Will use Flash page 0 for buffering
#define ADR_LIMIT_LOW   0     // Defines the low address limit for FLASH write

#define BOOTSECTORSIZE 0x2000 // 4096 words
#define ADR_LIMIT_HIGH  FLASHEND-BOOTSECTORSIZE// Defines the high address limit for FLASH write

/******************************************************************************
* Definition of PAGESIZE
******************************************************************************/

#if defined(__AVR_ATmega88__)   || defined(__AVR_AT90Mega88__)   || \
    defined(__AVR_ATmega88PA__) || defined(__AVR_AT90Mega88PA__) || \
	defined(__AVR_ATmega88PB__) || defined(__AVR_AT90Mega88PB__) || \
	defined(__AVR_ATmega88A__)  || defined(__AVR_AT90Mega88A__)  || \
    defined(__AVR_ATmega8__)    || defined(__AVR_AT90Mega8__)    || \
	defined(__AVR_ATmega8A__)   || defined(__AVR_AT90Mega8A__)   || \
    defined(__AVR_ATmega8515__) || defined(__AVR_AT90Mega8515__) || \
    defined(__AVR_ATmega8535__) || defined(__AVR_AT90Mega8535__)
#define PAGESIZE 64
#endif

#if defined(__AVR_ATmega162__)  || defined(__AVR_AT90Mega162__)  || \
    defined(__AVR_ATmega163__)  || defined(__AVR_AT90Mega163__)  || \
	defined(__AVR_ATmega164A__) || defined(__AVR_AT90Mega164A__) || \
	defined(__AVR_ATmega164P__) || defined(__AVR_AT90Mega164P__) || \
	defined(__AVR_ATmega164PA__)|| defined(__AVR_AT90Mega164PA__)|| \
	defined(__AVR_ATmega165A__) || defined(__AVR_AT90Mega165A__) || \
	defined(__AVR_ATmega165P__) || defined(__AVR_AT90Mega165P__) || \
	defined(__AVR_ATmega165PA__)|| defined(__AVR_AT90Mega165PA__)|| \
    defined(__AVR_ATmega168__)  || defined(__AVR_AT90Mega168__)  || \
	defined(__AVR_ATmega168A__) || defined(__AVR_AT90Mega168A__) || \
	defined(__AVR_ATmega168PA__)|| defined(__AVR_AT90Mega168PA__)|| \
	defined(__AVR_ATmega168PB__)|| defined(__AVR_AT90Mega168PB__)|| \
    defined(__AVR_ATmega169__)  || defined(__AVR_AT90Mega169__)  || \
	defined(__AVR_ATmega169A__) || defined(__AVR_AT90Mega169A__) || \
	defined(__AVR_ATmega169P__) || defined(__AVR_AT90Mega169P__) || \
	defined(__AVR_ATmega169PA__)|| defined(__AVR_AT90Mega169PA__)|| \
    defined(__AVR_ATmega161__)  || defined(__AVR_AT90Mega161__)  || \
    defined(__AVR_ATmega16__)   || defined(__AVR_AT90Mega16__)   || \
    defined(__AVR_ATmega32__)   || defined(__AVR_AT90Mega32__)   || \
	 defined(__AVR_ATmega32A__) || defined(__AVR_AT90Mega32A__)  || \
    defined(__AVR_ATmega323__)  || defined(__AVR_AT90Mega323__)  || \
	defined(__AVR_ATmega324A__) || defined(__AVR_AT90Mega324A__) || \
	defined(__AVR_ATmega324P__) || defined(__AVR_AT90Mega324P__) || \
	defined(__AVR_ATmega324PA__)|| defined(__AVR_AT90Mega324PA__)|| \
	defined(__AVR_ATmega325__)  || defined(__AVR_AT90Mega325__)  || \
	defined(__AVR_ATmega3250__) || defined(__AVR_AT90Mega3250__) || \
	defined(__AVR_ATmega328__)  || defined(__AVR_AT90Mega328__)  || \
	defined(__AVR_ATmega328P__) || defined(__AVR_AT90Mega328P__) || \
	defined(__AVR_ATmega328PB__)|| defined(__AVR_AT90Mega328PB__)|| \
	defined(__AVR_ATmega329__)  || defined(__AVR_AT90Mega329__)  || \
	defined(__AVR_ATmega329A__) || defined(__AVR_AT90Mega329A__) || \
	defined(__AVR_ATmega329P__) || defined(__AVR_AT90Mega329P__) || \
	defined(__AVR_ATmega329PA__)|| defined(__AVR_AT90Mega329PA__)|| \
    defined(__AVR_ATmega3290__) || defined(__AVR_AT90Mega3290__) || \
	defined(__AVR_ATmega3290A__)|| defined(__AVR_AT90Mega3290A__)|| \
	defined(__AVR_ATmega3290P__)|| defined(__AVR_AT90Mega3290P__)|| \
	defined(__AVR_ATmega3290PA__)|| defined(__AVR_AT90Mega3290PA__) 
#define PAGESIZE 128
#endif

#if defined(__AVR_ATmega64__)   || defined(__AVR_AT90Mega64__)   || \
    defined(__AVR_ATmega640__)  || defined(__AVR_AT90Mega640__)  || \
	defined(__AVR_ATmega644__)  || defined(__AVR_AT90Mega644__)  || \
	defined(__AVR_ATmega644A__) || defined(__AVR_AT90Mega644A__) || \
	defined(__AVR_ATmega644P__) || defined(__AVR_AT90Mega644P__) || \
	defined(__AVR_ATmega644PA__)|| defined(__AVR_AT90Mega644PA__)|| \
	defined(__AVR_ATmega645__)  || defined(__AVR_AT90Mega645__)  || \
	defined(__AVR_ATmega6450__) || defined(__AVR_AT90Mega6450__) || \
    defined(__AVR_ATmega649__)  || defined(__AVR_AT90Mega649__)  || \
	defined(__AVR_ATmega649A__) || defined(__AVR_AT90Mega649A__) || \
	defined(__AVR_ATmega649PA__)|| defined(__AVR_AT90Mega649PA__)|| \
	defined(__AVR_ATmega6490__) || defined(__AVR_AT90Mega6490__) || \
	defined(__AVR_ATmega6490A__)|| defined(__AVR_AT90Mega6490A__)|| \
	defined(__AVR_ATmega6490PA__)|| defined(__AVR_AT90Mega6490PA__)|| \
    defined(__AVR_ATmega128__)  || defined(__AVR_AT90Mega128__)  || \
	defined(__AVR_ATmega128A__) || defined(__AVR_AT90Mega128A__) || \
    defined(__AVR_ATmega1280__) || defined(__AVR_AT90Mega1280__) || \
    defined(__AVR_ATmega1281__) || defined(__AVR_AT90Mega1281__) || \
	defined(__AVR_ATmega1284__) || defined(__AVR_AT90Mega1284__) || \
	defined(__AVR_ATmega1284P__)|| defined(__AVR_AT90Mega128P__) || \
    defined(__AVR_ATmega2560__) || defined(__AVR_AT90Mega2560__) || \
    defined(__AVR_ATmega2561__) || defined(__AVR_AT90Mega2561__)
#define PAGESIZE 256
#endif

/******************************************************************************
* Definition of datatypes
* All functions uses integer types for passing Flash addresses.
******************************************************************************/

#ifdef __HAS_ELPM__
typedef volatile unsigned long MyAddressType;        // Used for passing address to functions
typedef volatile unsigned char __farflash* MyFlashCharPointer; // Used for reading Flash
typedef volatile unsigned int __farflash* MyFlashIntPointer;  // Used for reading Flash
#else
typedef volatile unsigned int  MyAddressType;       // Used for passing address to functions
typedef const unsigned char __flash* MyFlashCharPointer;    // Used for reading Flash
typedef const unsigned int  __flash* MyFlashIntPointer;
#endif

/******************************************************************************
* Definition of SPM control register. Labeled SPMCR on some devices, SPMSCR
* on others.
******************************************************************************/
#if defined(__AVR_ATmega8__)    || defined(__AVR_AT90Mega8__)    || \
	defined(__AVR_ATmega8A__)   || defined(__AVR_AT90Mega8A__)   || \
	defined(__AVR_ATmega16__)   || defined(__AVR_AT90Mega16__)   || \
    defined(__AVR_ATmega161__)  || defined(__AVR_AT90Mega161__)  || \
    defined(__AVR_ATmega162__)  || defined(__AVR_AT90Mega162__)  || \
    defined(__AVR_ATmega163__)  || defined(__AVR_AT90Mega163__)  || \
    defined(__AVR_ATmega32__)   || defined(__AVR_AT90Mega32__)   || \
    defined(__AVR_ATmega323__)  || defined(__AVR_ATMega323__)    || \
    defined(__AVR_ATmega64__)   || defined(__AVR_AT90Mega64__)   || \
    defined(__AVR_ATmega8__)    || defined(__AVR_AT90Mega8__)    || \
    defined(__AVR_ATmega8535__) || defined(__AVR_AT90Mega8535__) || \
    defined(__AVR_ATmega8515__) || defined(__AVR_AT90Mega8515__)
#define SPMControllRegister SPMCR
#else
#define SPMControllRegister SPMCSR
#endif

/******************************************************************************
* Definition of EEPROM write enable bit. Labeled EEPE on some devices, EEWE
* on others.
******************************************************************************/
#if defined(__AVR_ATmega8__)    || defined(__AVR_AT90Mega8__)    || \
	defined(__AVR_ATmega8A__)   || defined(__AVR_AT90Mega8A__)   || \
	defined(__AVR_ATmega16__)   || defined(__AVR_AT90Mega16__)   || \
	defined(__AVR_ATmega128__)  || defined(__AVR_AT90Mega128__)  || \
	defined(__AVR_ATmega128A__) || defined(__AVR_AT90Mega128A__) || \
	defined(__AVR_ATmega161__)  || defined(__AVR_AT90Mega161__)  || \
	defined(__AVR_ATmega162__)  || defined(__AVR_AT90Mega162__)  || \
	defined(__AVR_ATmega163__)  || defined(__AVR_AT90Mega163__)  || \
	defined(__AVR_ATmega165A__) || defined(__AVR_AT90Mega165A__) || \
	defined(__AVR_ATmega165PA__)|| defined(__AVR_AT90Mega165PA__)|| \
	defined(__AVR_ATmega169__)  || defined(__AVR_AT90Mega169__)  || \
	defined(__AVR_ATmega169A__) || defined(__AVR_AT90Mega169A__) || \
	defined(__AVR_ATmega169P__) || defined(__AVR_AT90Mega169P__) || \
	defined(__AVR_ATmega169PA__)|| defined(__AVR_AT90Mega169PA__)|| \
	defined(__AVR_ATmega32__)   || defined(__AVR_AT90Mega32__)   || \
	defined(__AVR_ATmega32A__)  || defined(__AVR_AT90Mega32A__)  || \
	defined(__AVR_ATmega323__)  || defined(__AVR_ATMega323__)    || \
	defined(__AVR_ATmega325__)  || defined(__AVR_AT90Mega325__)  || \
	defined(__AVR_ATmega3250__) || defined(__AVR_AT90Mega3250__) || \
	defined(__AVR_ATmega329__)  || defined(__AVR_AT90Mega329__)  || \
	defined(__AVR_ATmega329A__) || defined(__AVR_AT90Mega329A__) || \
	defined(__AVR_ATmega329P__) || defined(__AVR_AT90Mega329P__) || \
	defined(__AVR_ATmega329PA__)|| defined(__AVR_AT90Mega329PA__)|| \
	defined(__AVR_ATmega3290__) || defined(__AVR_AT90Mega3290__) || \
	defined(__AVR_ATmega3290A__)|| defined(__AVR_AT90Mega3290A__)|| \
	defined(__AVR_ATmega3290P__)|| defined(__AVR_AT90Mega3290P__)|| \
	defined(__AVR_ATmega3290PA__)|| defined(__AVR_AT90Mega3290PA__)|| \
	defined(__AVR_ATmega64__)   || defined(__AVR_AT90Mega64__)   || \
	defined(__AVR_ATmega64A__)   || defined(__AVR_AT90Mega64A__)   || \
	defined(__AVR_ATmega645__)  || defined(__AVR_AT90Mega645__)  || \
	defined(__AVR_ATmega6450__) || defined(__AVR_AT90Mega6450__) || \
	defined(__AVR_ATmega8__)    || defined(__AVR_AT90Mega8__)    || \
	defined(__AVR_ATmega8535__) || defined(__AVR_AT90Mega8535__) || \
	defined(__AVR_ATmega8515__) || defined(__AVR_AT90Mega8515__)
#define EEPROM_program_enable EEWE
#else
#define EEPROM_program_enable EEPE
#endif
/******************************************************************************
* Other defines
******************************************************************************/

#define FLASH_BUFFER_FULL_ID  0xAA
#define TRUE 1
#define FALSE 0

/******************************************************************************
* Function prototypes
******************************************************************************/

/* User functions */
unsigned char ReadFlashByte(MyAddressType flashAdr);
unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage);
unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data);
unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage);
unsigned char RecoverFlash();

/* Functions used by user functions */
unsigned char AddressCheck(MyAddressType flashAdr);
void WriteBufToFlash(MyAddressType flashStartAdr);
void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data);

#endif /* End if __BOOT_FUNC_H */
