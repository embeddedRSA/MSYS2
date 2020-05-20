/*
 * tachometer.c
 *
 * Created: 24-04-2020 17:49:23
 * Author : Anders & Aaron 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <stdlib.h>

#include "speedSensor.h"
#include "uart.h"

static uint16_t timerOverflows;
static uint16_t timerCount;
static uint8_t checkpointCnt;
static uint8_t revolutionsForCalc;

static speedSensorInterface_t* speedSensor;

// Select the UART to test (UART0, UART1, UART2 or UART3)
#define myUART UART0

int main(void)
{
	checkpointCnt = 0;
	speedSensor = speedSensor_getDriver(65);
	
	//uart setup
	InitUART(myUART, 9600, 8, 'N');
	
	while (1)
	{			
		PORTB &= ~(1<<PB5); //debug 
		
		SendString(myUART, "KMH: ");
		float f = speedSensor->getSpeedInKmh();
		int r = (int)f;
		int d = (f-r)*100;
		SendInteger(myUART,r);
		SendChar(myUART,'.');
		SendInteger(myUART,d);
		
		SendString(myUART, "     KHM_TOTAL: ");
		f = speedSensor->getTripDistance();
		r = (int)f;
		d = (f-r)*100;
		SendInteger(myUART,r);
		SendChar(myUART,'.');
		SendInteger(myUART,d);
		SendChar(myUART,'\r'); //newline
		
	}
}


ISR(INT3_vect) //PE4 
{
	speedSensor->updateMilestoneCount();
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
		PORTB |=(1<<PB5); //debug
		
		speedSensor->updateRevolutionCount(revolutionsForCalc);
		revolutionsForCalc = 0; //Resetting after getting value for KHM calculation  
		timerCount=0; //Resetting before getting value.
		checkpointCnt++;
	}
	
	if (checkpointCnt>60) //Save milestone to EEPROM every minute
	{
		checkpointCnt = 0;
		speedSensor->saveMilestoneCount();
	}

}