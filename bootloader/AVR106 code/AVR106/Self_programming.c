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
* - File              : Self_programming.c
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
****************************************************************************/
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "Self_programming.h"
#include "flash.h"
void FillBuffer(unsigned char *dataPage);

/* Defining the attribute to place the selected function insu=ide the boot section*/
#define BOOTLOADER_SECTION __attribute__ ((section (".bootloader")))

/*
* Declare global struct variable in EEPROM if Flash recovery enabled.
* FlashBackup pageNumber holds Flash page address (/PAGESIZE) the data in Flash
* recovery buffer should be written to if data need to be recovered.
* FlashBackup.status tells if data need to be recovered.
*/
#ifdef __FLASH_RECOVER
EEMEM struct
{
	unsigned int  pageNumber;
	unsigned char status;
}FlashBackup = {0};
#endif

/*
* The function Returns one byte located on Flash address given by ucFlashStartAdr.
*/
unsigned char ReadFlashByte(MyAddressType flashStartAdr)
{
    return (unsigned char)*((MyFlashCharPointer)flashStartAdr);
}  // Returns data from Flash

/*!
* The function reads one Flash page from address flashStartAdr and stores data
* in array dataPage[]. The number of bytes stored is depending upon the
* Flash page size. The function returns FALSE if input address is not a Flash
* page address, else TRUE.
**/
unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)
{
	unsigned int index;
    if(!(flashStartAdr & (PAGESIZE-1)))
	{                                       // If input address is a page address
		for(index = 0; index < PAGESIZE; index++)
		{
			dataPage[index] = ReadFlashByte(flashStartAdr + index);
		}
		return TRUE;                            // Return TRUE if valid page address
    }
    else
	{
		return FALSE;                           // Return FALSE if not valid page address
    }
}

/*!
* The function writes byte data to Flash address flashAddr. Returns FALSE if
* input address is not valid Flash byte address for writing, else TRUE.
**/

unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data)
{
	MyAddressType  pageAdr;
    unsigned char eepromInterruptSettings;
    if( AddressCheck( flashAddr & ~(PAGESIZE-1) ))
	{
		eepromInterruptSettings= EECR & (1<<EERIE); // Stores EEPROM interrupt mask
        EECR &= ~(1<<EERIE);                        // Disable EEPROM interrupt
		while(EECR & (1<<EEPROM_program_enable));   // Wait if ongoing EEPROM write

		pageAdr=flashAddr & ~(PAGESIZE-1);          // Gets Flash page address from byte address

		#ifdef __FLASH_RECOVER
		FlashBackup.status=0;    // Indicate that Flash buffer does not contain data for writing
		while(EECR & (1<<EEPROM_program_enable));
		LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer
		WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer
		FlashBackup.pageNumber = (unsigned int) (pageAdr/PAGESIZE); //Stores page address of data
		FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer
                                               // contains unwritten data
		while(EECR & (1<<EEPROM_program_enable));
		#endif
    LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer
	WriteBufToFlash(pageAdr);               // Writes to Flash

    #ifdef __FLASH_RECOVER
	FlashBackup.status = 0;//Indicates that Flash recovery buffer does not contain unwritten data
                                           
		while(EECR & (1<<EEPROM_program_enable));
    #endif

		EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask
		return TRUE;                            // Return TRUE if address valid for writing
	}
  else
    return FALSE;                           // Return FALSE if address not valid for writing
}

/*
* The function writes data from array dataPage[] to Flash page address
* flashStartAdr. The Number of bytes written is depending upon the Flash page
* size. Returns FALSE if input argument ucFlashStartAdr is not a valid Flash
* page address for writing, else TRUE.
*/


unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)
{
   unsigned char eepromInterruptSettings;
    if( AddressCheck(flashStartAdr) )
	{
		eepromInterruptSettings = EECR & (1<<EERIE);   // Stores EEPROM interrupt mask
		EECR &= ~(1<<EERIE);                           // Disable EEPROM interrupt
		while(EECR & (1<<EEPROM_program_enable));      // Wait if ongoing EEPROM write

		#ifdef __FLASH_RECOVER
		FlashBackup.status=0;       // Indicate that Flash buffer does not contain data for writing
		while(EECR & (1<<EEPROM_program_enable));
		FillBuffer(dataPage);
		WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer
		FlashBackup.pageNumber=(unsigned int)(flashStartAdr/PAGESIZE);
		/* Indicates that Flash recovery buffer contains unwritten data */
		FlashBackup.status = FLASH_BUFFER_FULL_ID;
		while(EECR & (1<<EEPROM_program_enable));
		#endif
		FillBuffer(dataPage);
		WriteBufToFlash(flashStartAdr);         // Writes to Flash
		#ifdef __FLASH_RECOVER
        FlashBackup.status=0;    // Indicate that Flash buffer does not contain data for writing
        while(EECR & (1<<EEPROM_program_enable));
		#endif

		EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask
		return TRUE;                            // Return TRUE if address
                                            // valid for writing
     }
  else
    return FALSE;                           // Return FALSE if not address not
                                            // valid for writing
}

/*
* The function checks if global variable FlashBackup.status indicates that Flash recovery
* buffer contains data that needs to be written to Flash. Writes data from
* Flash recovery buffer to Flash page address given by FLASH_recovery.pageAdr.
* This function should be called at program startup if FLASH recovery option
* is enabled.
*/

unsigned char RecoverFlash() BOOTLOADER_SECTION;
unsigned char RecoverFlash()
{
	#ifdef __FLASH_RECOVER
    unsigned int index;
    if(FlashBackup.status == FLASH_BUFFER_FULL_ID) // Checks if Flash recovery buffer contains data
	{ 
		for(index=0; index < PAGESIZE; index+=2){     // Writes to Flash write buffer
        _FILL_TEMP_WORD( index, *((MyFlashIntPointer)(ADR_FLASH_BUFFER+index)) );
    }
    WriteBufToFlash((MyAddressType)FlashBackup.pageNumber * PAGESIZE);
    FlashBackup.status=0;     // Indicate that Flash buffer does not contain data for writing
    while(EECR & (1<<EEPROM_program_enable));
    return TRUE;               // Returns TRUE if recovery has taken place
  }
	#endif
	return FALSE;
}


/*
* The function checks if input argument is a valid Flash page address for
* writing. Returns TRUE only if:
* - Address points to the beginning of a Flash page
* - Address is within the limits defined in Self_programming.h
* - Address is not equal to page address used for buffering by the Flash recovery
*   functions (if enabled).
* Returns FALSE else.
*/
unsigned char AddressCheck(MyAddressType flashAdr)
{
	#ifdef __FLASH_RECOVER
 //The next line gives a warning 'pointless comparison with zero' if ADR_LIMIT_LOW is 0. Ignore it.
    if( (flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) &&
    (flashAdr != ADR_FLASH_BUFFER) && !(flashAdr & (PAGESIZE-1)) )
		return TRUE;                            // Address is a valid page address
    else
		return FALSE;                           // Address is not a valid page address
    #else
    if((flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) && !(flashAdr & (PAGESIZE-1) ) )
		return TRUE;                            // Address is a valid page address
    else
		return FALSE;                           // Address is not a valid page address
    #endif
}


/*
* The function writes Flash temporary buffer to Flash page address given by
* input argument.
*/

void WriteBufToFlash(MyAddressType flashStartAdr) BOOTLOADER_SECTION;
void WriteBufToFlash(MyAddressType flashStartAdr)
{
	#ifdef __HAS_RAMPZ__
    RAMPZ = (unsigned char)(flashStartAdr >> 16);
    #endif
    _PAGE_ERASE(flashStartAdr);
    while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed
    _PAGE_WRITE(flashStartAdr);
    while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed
    #ifdef RWWSRE
    _ENABLE_RWW_SECTION(); // Enable RWW
    #endif
}

/*
* The function reads Flash page given by flashAddr, replaces one byte given by
* flashAddr with data, and stores entire page in Flash temporary buffer.
*/

void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data) BOOTLOADER_SECTION;
void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data)
{
	unsigned int index, oddByte, pcWord;
    MyAddressType  pageAdr;
    oddByte=(unsigned char)flashAddr & 1;
    pcWord=(unsigned int)flashAddr & (PAGESIZE-2); // Used when writing FLASH temp buffer
    pageAdr=flashAddr & ~(PAGESIZE-1);        // Get FLASH page address from byte address
    for(index=0; index < PAGESIZE; index+=2)
	{
		if(index==pcWord)
		{
			if(oddByte)
			{
				 _FILL_TEMP_WORD( index, (*(MyFlashCharPointer)(flashAddr & ~1) | 
				 ((unsigned int)data<<8)));
			}                                     // Write odd byte in temporary buffer
			 else
			{
                 _FILL_TEMP_WORD( index, ( (*( (MyFlashCharPointer)flashAddr+1)<<8)  | data ) );
			}                                     // Write even byte in temporary buffer
		}
        else
	    {
			_FILL_TEMP_WORD(index, *( (MyFlashIntPointer)(pageAdr+index) ) );
        }                                       // Write Flash word directly to temporary buffer
     }
}

/*
*This function fills the temporary bufer with the data
*/
void FillBuffer(unsigned char *dataPage) BOOTLOADER_SECTION;
void FillBuffer(unsigned char *dataPage)
{
	unsigned int index;
	for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer
		_FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
	}
}
