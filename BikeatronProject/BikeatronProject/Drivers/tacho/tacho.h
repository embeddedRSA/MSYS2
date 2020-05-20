/*
 * tacho.h
 *
 * Created: 24-04-2020 17:53:52
 *  Author: Anders & Aaron 
 */ 
#include <avr/io.h>
#include "../../Utils/functionPointers.h"

#ifndef TACHO_H_
#define TACHO_H_

typedef struct
{
	void_uint8_f_t updateRevolutionCount;
	
	void_void_f_t updateMilestoneCount;
	void_void_f_t saveMilestoneCount;
	
	float_void_f_t getSpeedInKmh;
	float_void_f_t getTripDistance;
	
	
}speedSensorInterface_t;

speedSensorInterface_t* speedSensor_getDriver(float wheelDiameter);


#endif /* TACHO_H_ */