/*
 * speedSensor.h
 *
 * Created: 24-04-2020 17:53:52
 *  Author: Anders & Aaron 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef SPEEDSENSOR_H_
#define SPEEDSENSOR_H_

typedef void(*void_function_t)(void);
typedef void(*void_uint8_function_t)(uint8_t);
typedef float(*float_void_function_t)(void);

typedef struct
{
	void_uint8_function_t updateRevolutionCount;
	
	void_function_t updateMilestoneCount;
	void_function_t saveMilestoneCount;
	
	float_void_function_t getSpeedInKmh;
	float_void_function_t getTripDistance;
	
	
}speedSensorInterface_t;

speedSensorInterface_t* speedSensor_getDriver(float wheelDiameter);


#endif /* SPEEDSENSOR_H_ */