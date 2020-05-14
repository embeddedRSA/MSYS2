/*
 * DHT11New.c
 *
 * Created: 13-05-2020 09:39:10
 * Author : Anders
 */ 
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include "DHT11.h"
#include "uart.h"

//making object
DHT_t *tempHumidObj;

// Select the UART to test (UART0, UART1, UART2 or UART3)
#define myUART UART0

int main(void)
{
	
	//uart setup
	InitUART(myUART, 9600, 8, 'N');
	
	
	uint8_t temperature, humidity;

	
	tempHumidObj=getDHTInterface(); //returns pointer to static struct 
	
    /* Replace with your application code */
    while (1) 
    {
		
		tempHumidObj->getTempHumid(&temperature, &humidity);
		
				
				_delay_ms(2000);

				SendChar(myUART,'T');
				SendChar(myUART,':');
				SendInteger(myUART, temperature);

				SendChar(myUART,' '); //space
				SendChar(myUART,' '); //space
				SendChar(myUART,' '); //space
				SendChar(myUART,' '); //space
				
				SendChar(myUART,'H');
				SendChar(myUART,':');
				SendInteger(myUART, humidity);
				
				
				SendChar(myUART,' '); //space
				SendChar(myUART,' '); //space
				SendChar(myUART,' '); //space
				SendChar(myUART,' '); //space
		
    }
}

