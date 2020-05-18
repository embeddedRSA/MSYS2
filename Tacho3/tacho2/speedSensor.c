/*
 * speedSensor.c
 *
 * Created: 24-04-2020 17:53:35
 *  Author: Anders
 */ 

#include "speedSensor.h"
#include <stdio.h>
#include <avr/eeprom.h>
#include <stdbool.h>

static uint8_t revPerSec[4];
static uint32_t milestoneCount;

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
	}
	return &myInterface;
}

static void initSpeedSensor(float wheelDiameter)
{
	float wheelD=(wheelDiameter/200); //Calculations to meters centered ( /100 & /2)
	revLength=(wheelD*3.1415); //One revolution gives meters
	milestoneCount = eeprom_read_dword(0);
	if (milestoneCount == 0xFFFFFFFF)
	{
		milestoneCount = 0;
	}
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


static float getSpeedKMH(void) //WORKS TESTED 
{
	//char buffer[10];
	
	float KMH = ((float)sumRevolutions()/4)*revLength*3.6; // Revolutions per second times revolution length = m/s. time 3.6 = km/h
	
	return KMH; 
}

static float getTripDistance(void)  //WORKS TESTED 
{ 
	
	float KMD = ((revLength*(float)milestoneCount)/1000); //Total KM distance driven
	return KMD;
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
	eeprom_write_dword(0,milestoneCount);
}