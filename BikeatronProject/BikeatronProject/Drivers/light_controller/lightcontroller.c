/*
 * lightDriver.c
 *
 * Created: 29-04-2020 08:49:04
 *  Author: Anders
 */ 

#include "lightcontroller.h"
#include <avr/io.h>
static uint8_t init=0;

//Private variables 
static lightDriver_t lD_interface;

//Prototypes
static void initLightDriver(void);
static void setFrontlight(unsigned char percent);
static void setBacklight(unsigned char percent);
 
 
 lightDriver_t* get_lightDriverInterface(void)
 {
	 if (!init)
	 {
		 lD_interface.init=initLightDriver;
		 lD_interface.setFront=setFrontlight;
		 lD_interface.setBack=setBacklight;
	 }
	 return &lD_interface;
 }

static void initLightDriver(void)
{
		DDRH |= (1<<PH4);	//Sets portH4 as output.
		DDRH |= (1<<PH5);	//Sets portH5 as output.
		TCCR4B=0b00000001;	//Sets timer 4 to no prescaling
		TCCR4A = 0b00101001;	//Compare match channel B 5:4, Compare match channel C 3:2, PWM, phase correct 8-bit. 

}

static void setFrontlight(unsigned char percent)
{
	uint8_t percentA;
	
	if(percent>100){	//checkup 
	percentA=100;}
	else
	percentA=percent;
	
	uint16_t frontLight=((255*percentA)/100);	//percentage of full OCR4B TOP value.
	
	OCR4B=(uint8_t)frontLight;	//setting output compare.
}

static void setBacklight(unsigned char percent)
{
	uint8_t percentB;
		
	if(percent>100){	//checkup
	percentB=100;}
	else
	percentB=percent;
	
	uint16_t BackLight=((255*percentB)/100);	//percentage of full OCR4C TOP value.
	
	OCR4C=(uint8_t)BackLight;	//setting output compare.	
}
