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
uint16_t revolutionsForCalc;
uint16_t revKHM;

void initSpeedSensor(uint8_t wheelDiameter);
int getSpeedKMH(void);
void getTripDistance(int *km, int *md); 



#endif /* SPEEDSENSOR_H_ */