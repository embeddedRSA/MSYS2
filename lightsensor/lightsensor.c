/*
 * lightsensor.c
 *
 * Created: 07/05/2020 11:27:11
 *  Author: ryana
 */ 

#include "lightsensor.h"
static LightSensor_t LightSensorInterface;
static ADC_t *ADC_interface;
static bool initialized = false;

static uint8_t pin_no;
uint16_t light_threshold;


static void s_init(uint8_t p_pin_no, uint16_t p_threshold);
static bool s_getLightStatus(void);
 
 LightSensor_t *get_lightSensor_interface(ADC_t * p_ADC_Interface)
 {
	if (!initialized)
	{	ADC_interface								=	p_ADC_Interface;
		
		LightSensorInterface.init					=	s_init;
		LightSensorInterface.getLightStatus			=	s_getLightStatus;
		
		initialized									=	true;
	}
	return &LightSensorInterface;	 
 }
 
 static void s_init(uint8_t p_pin_no, uint16_t p_threshold)
 {
	light_threshold		=	p_threshold;
	pin_no				=	p_pin_no;
 
 }
 
 static bool s_getLightStatus(void)
 {
	if(ADC_interface->getADC_mV(pin_no)>=light_threshold)
	{
	return false;
	}
	else
	{
	return true;
	}
 }