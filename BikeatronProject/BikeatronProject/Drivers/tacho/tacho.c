/*
 * speedSensor.c
 *
 * Created: 24-04-2020 17:53:35
 *  Author: Anders
 */ 

#include "tacho.h"
#include <stdio.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <avr/interrupt.h>

static uint8_t revPerSec[4];
static uint32_t milestoneCount;
static float kmCount;
float EEMEM eepromKmCount;

static speedSensorInterface_t myInterface;
static bool initialized = false;
static float revLength; //Keeping the one revolution travel distance in meters
static uint16_t sumRevolutions(void);

static void initSpeedSensor(float wheelDiameter);
static void updateMilestoneCount(void);
static void eepromSave(void);
static void updateRevolutionCount(uint8_t revs);

static float getSpeedKMH(void);
static float getTripDistance(void);

speedSensorInterface_t* speedSensor_getDriver(float wheelDiameter)
{
	if (!initialized)
	{
		initSpeedSensor(wheelDiameter);
		myInterface.getSpeedInKmh = getSpeedKMH;
		myInterface.getTripDistance = getTripDistance;
		myInterface.saveMilestoneCount = eepromSave;
		myInterface.updateMilestoneCount = updateMilestoneCount;
		myInterface.updateRevolutionCount = updateRevolutionCount;
		initialized = true;
	}
	return &myInterface;
}

static void initSpeedSensor(float wheelDiameter)
{
	float wheelD=(wheelDiameter/200); //Calculations to meters centered ( /100 & /2)
	revLength=(wheelD*3.1415); //One revolution gives meters
	kmCount = eeprom_read_float(&eepromKmCount);
	//eeprom_write_float(&eepromKmCount,0.0);
	if (kmCount > 10000)
	{
		kmCount = 0;
	}
	milestoneCount = 0;
	//Timer2 is used for keeping time of rpm measurement.
	// Timer2: Normal mode, No prescaling
	TCCR2A = 0b00000000;
	TCCR2B = 0b00000001;
	// Timer2 overflow interrupt enable
	TIMSK2 |= 0b00000001;
	
	// INT3:Rising edge
	EICRA = 0b11000000;
	// Enable extern interrupts INT3 
	EIMSK |= 0b00001000;
}


static float getSpeedKMH(void) //WORKS TESTED 
{
	//char buffer[10];
	
	float KMH = ((float)sumRevolutions()/4)*revLength*3.6; // Revolutions per second times revolution length = m/s. time 3.6 = km/h
	
	return KMH; 
}

static float getTripDistance(void)  //WORKS TESTED 
{ 
	
	float KMD = ((revLength*milestoneCount)/1000)+kmCount; //Total KM distance driven
	return KMD;
	
	//return (float) milestoneCount;
}

static void updateMilestoneCount()
{
	milestoneCount++;
}

static void updateRevolutionCount(uint8_t revs)
{
	static uint8_t cnt = 0;
	revPerSec[cnt] = revs;
	cnt = (cnt+1) % 4;
}

static uint16_t sumRevolutions(void)
{
	uint16_t retVal = 0;
	uint8_t i = 0;
	for (i = 0; i<4; i++)
	{
		retVal += revPerSec[i];
	}
	
	return retVal;
}


static void eepromSave(void)
{
	eeprom_update_float(&eepromKmCount,getTripDistance());
}