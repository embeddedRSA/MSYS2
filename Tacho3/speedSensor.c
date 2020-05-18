/*
 * speedSensor.c
 *
 * Created: 24-04-2020 17:53:35
 *  Author: Anders
 */ 

#include "speedSensor.h"
#include <stdio.h>

static float revLength; //Keeping the one revolution travel distance in meters
static float sumint8(uint8_t* revs);

void initSpeedSensor(float wheelDiameter)
{
	float wheelD=(wheelDiameter/200); //Calculations to meters centered ( /100 & /2)
	revLength=(wheelD*3.1415); //One revolution gives meters
	
	//Timer2 is used for keeping time of rpm measurement.
	// Timer2: Normal mode, No prescaling
	TCCR2A = 0b00000000;
	TCCR2B = 0b00000001;
	// Timer2 overflow interrupt enable
	TIMSK2 |= 0b00000001;
	
	// INT4:Rising edge
	EICRB = 0b00000011;
	// Enable extern interrupts INT4 
	EIMSK |= 0b00010000;
	
	// Global interrupt enable
	sei();
}


float getSpeedKMH(void) //WORKS TESTED 
{
	//char buffer[10];
	
	float KMH = (sumint8(revPerSec)/4)*revLength*3.6; // Revolutions per second times revolution length = m/s. time 3.6 = km/h
	
	return KMH; 
}

float getTripDistance(void)  //WORKS TESTED 
{ 
	
	float KMD = ((revLength*(float)revolutionsTrip)/1000); //Total KM distance driven
	return KMD;
}

void updateRevForCalc(uint8_t revs)
{
	static uint8_t cnt = 0;
	revPerSec[cnt] = revs;
	cnt = (cnt+1) % 4;
}

static float sumint8(uint8_t* revs)
{
	float retVal = 0;
	uint8_t i = 0;
	for (i = 0; i<4; i++)
	{
		retVal += (float)revs[i];
	}
	
	return retVal;
}