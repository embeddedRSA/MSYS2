/*
 * speedSensor.c
 *
 * Created: 24-04-2020 17:53:35
 *  Author: Anders
 */ 

#include "speedSensor.h"
#include <stdio.h>

static double revLength; //Keeping the one revolution travel distance in meters

void initSpeedSensor(uint8_t wheelDiameter)
{
	double wheelD=(wheelDiameter/(double)100); //Calculations to meters
	revLength=((double)wheelD*(double)3.1415); //One revolution gives meters
	
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


int getSpeedKMH(void) //WORKS TESTED 
{
	//char buffer[10];
	
	double KM=(revKHM*revLength)/(double)1000; //Getting the distance in km for revolutions made in 3s 
	double KMH=KM/(double)0.000833333; //The decimal number for 3 seconds in hours 
	
	//itoa((int)KHM,buffer,10); //10 means decimal conversion put in buffer string
	
	
	return (int)KMH; 
}

void getTripDistance(int *km, int *md)  //WORKS TESTED 
{
	//char buffer[10]; 
	
	double KMD=((revLength*revolutionsTrip)/(double)1000); //Total KM distance driven
	double M=(revLength*revolutionsTrip); //Total M distance driven 
	
	*km=(int)KMD;
	*md=(int)M;
	
	
	//itoa((int)KMD,buffer,10); 
	
	//return (int)KMD; 	

}




