/*
 * RTCtest.c
 *
 * Created: 01-05-2020 13:07:44
 * Author : Anders
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>


#include "ds1307.h"
#include "UART.h"
#include "i2c_driver.h"

#define myUART UART0
i2c_t *i2c_object;
RTC_t *RTC_object;

int main(void)
{
	
	//uart setup
	InitUART(myUART, 9600, 8, 'N');
	
	i2c_object=get_i2c_interface();
	RTC_object=get_RTC_interface();
	
	//init ds1307
	RTC_object->initRTC(i2c_object);
	
	uint8_t year = 0;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;

	//remember to set the weekday 1-7 correct!
	RTC_object->setDateTime(20, 12, 31, 4, 23, 59, 35);
	
    while (1) 
    {
		//get the time and date
		RTC_object->getDateTime(&year, &month, &day, &hour, &minute, &second);
		
		 _delay_ms(1000);
		 //write time and date over the uart every second
		 SendChar(myUART,' ');
		 SendChar(myUART,' ');
		 //displays time hrs:min:sec
		 SendChar(myUART,'T');
		 SendChar(myUART,'i');
		 SendChar(myUART,'m');
		 SendChar(myUART,'e');
		 SendChar(myUART,':');
		 SendInteger(myUART, hour);
		 SendChar(myUART,':');
		 SendInteger(myUART, minute);
		 SendChar(myUART,':');
		 SendInteger(myUART, second);
		 
		 //space
		 SendChar(myUART,' ');
		 SendChar(myUART,' ');
		 SendChar(myUART,'\n'); //newline
		 SendChar(myUART,'\r'); //newline
		 
		 //displays date dd-mm-yy
		 SendChar(myUART,'D');
		 SendChar(myUART,'a');
		 SendChar(myUART,'t');
		 SendChar(myUART,'e');
		 SendChar(myUART,':');
		 SendInteger(myUART, day);
		 SendChar(myUART,'-');
		 SendInteger(myUART, month);
		 SendChar(myUART,'-');
		 SendInteger(myUART, year);
		 
		 SendChar(myUART,'\n'); //newline
		 SendChar(myUART,'\r'); //newline
    }
}

