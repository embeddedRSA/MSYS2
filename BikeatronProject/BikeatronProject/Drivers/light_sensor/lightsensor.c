/*
 * lightsensor.c
 *
 * Created: 07/05/2020 11:27:11
 *  Author: ryana
 */ 

#include "lightsensor.h"
static LightSensor_t s_LightSensorInterface;
static ADC_t *		 s_ADC_interface;
static bool			 s_initialized = false;
static uint8_t		 s_pin_no;
static uint16_t		 s_light_threshold;


static void s_init(uint8_t p_pin_no, uint16_t p_threshold);
static bool s_getLightStatus(void);
 
 LightSensor_t *get_lightSensor_interface(ADC_t * p_ADC_Interface)
 {
	if (!s_initialized)
	{	s_ADC_interface									=	p_ADC_Interface;
		
		s_LightSensorInterface.init						=	s_init;
		s_LightSensorInterface.getLightStatus			=	s_getLightStatus;
		
		s_initialized									=	true;
	}
	return &s_LightSensorInterface;	 
 }
 
 static void s_init(uint8_t p_pin_no, uint16_t p_threshold)
 {
	s_light_threshold	=	p_threshold;
	s_pin_no			=	p_pin_no;
 
 }
 
 static bool s_getLightStatus(void)
 {
	if(s_ADC_interface->getADC_mV(s_pin_no)>=s_light_threshold)
	{
	return false;
	}
	else
	{
	return true;
	}
 }