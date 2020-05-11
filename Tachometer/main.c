/*
 * tachometer.c
 *
 * Created: 24-04-2020 17:49:23
 * Author : Anders
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>

#include "speedSensor.h"
#include "uart.h"

//For printout only 
int meterTotal;
int kmTotal;

// Select the UART to test (UART0, UART1, UART2 or UART3)
#define myUART UART0

int main(void)
{
	initSpeedSensor(65); //65 cm wheel diameter
	
	//uart setup
	InitUART(myUART, 9600, 8, 'N');
	
	while (1)
	{
		getTripDistance(&kmTotal, &meterTotal);
		
		_delay_ms(500);
			
		PORTB &= ~(1<<PB5); //debug 
		
		SendString(myUART, "KMH: ");
		SendInteger(myUART, getSpeedKMH());
		SendChar(myUART,'\n'); //newline
		SendChar(myUART,'\r'); //newline
		
		SendString(myUART, "KHM_TOTAL: ");
		SendInteger(myUART, kmTotal);
		SendChar(myUART,'\n'); //newline
		SendChar(myUART,'\r'); //newline
		
		SendString(myUART, "M_TOTAL: ");
		SendInteger(myUART, meterTotal);
		SendChar(myUART,'\n'); //newline
		SendChar(myUART,'\r'); //newline
		
		//space
		SendChar(myUART,'\n'); //newline
		SendChar(myUART,'\r'); //newline
	}
}


ISR(INT4_vect)
{
	revolutionsTrip++; //Counts up the revolutions for calculating trip length
	revolutionsForCalc++; //Counts up the revolutions for speed calculation.
}


//TIMER 2 interrupts every second to measure how many revolutions the wheel has made.
ISR(TIMER2_OVF_vect)
{
	// After 62500 interrupts overflows the timer counts another timer to get other times. 
	// Because of CPU clock and prescaling 8bit timer overflows every 16.063 us. Delay=prescaler*(OCRn+1)/fcpu
	timerOverflows++;
	
	if (timerOverflows == 62500)	// 16.063us*62500=1.004s
	{
		timerOverflows = 0; //reset timer
		timerCount++;
		
		PORTB |=(1<<PB5); //debug
	}
	
	if (timerCount==3)	//after 3 seconds
	{
		revKHM=revolutionsForCalc;
		revolutionsForCalc=0; //Resetting after getting value for KHM calculation  
		timerCount=0; //Resetting before getting value.
	}

}

