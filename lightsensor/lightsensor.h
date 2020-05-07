/*
 * lightsensor.h
 *
 * Created: 07/05/2020 11:26:58
 *  Author: ryana
 */ 


#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_
#include <stdbool.h>
#include "../ADC/ADC_MEGA2560.h"

typedef bool (*bool_RET_void_PAR)(void);
typedef void (*void_RET_uint8_uint16_PAR)(uint8_t, uint16_t);

typedef struct
{
	void_RET_uint8_uint16_PAR init;
	bool_RET_void_PAR getLightStatus;
	
}LightSensor_t;

 LightSensor_t *get_lightSensor_interface(ADC_t * p_ADC_Interface);

#endif /* LIGHTSENSOR_H_ */