/*
 * speedSensor.h
 *
 * Created: 24-04-2020 17:53:52
 *  Author: Anders
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef SPEEDSENSOR_H_
#define SPEEDSENSOR_H_


unsigned int timerOverflows; 
unsigned int timerCount;
uint32_t revolutionsTrip;
uint8_t revolutionsForCalc;
uint8_t revPerSec[4];

void initSpeedSensor(float wheelDiameter);
float getSpeedKMH(void);
float getTripDistance(void); 
void updateRevForCalc(uint8_t revs);


#endif /* SPEEDSENSOR_H_ */