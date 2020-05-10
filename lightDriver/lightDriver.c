/*
 * lightDriver.c
 *
 * Created: 29-04-2020 08:49:04
 *  Author: Anders
 */ 

#include "lightDriver.h"
#include <avr/io.h>

//Private variables 
static lightDriver_t lD_interface;

//Prototypes
static void initLightDriver(void);
static void setFrontlight(unsigned char percent);
static void setBacklight(unsigned char percent);
 
 
 lightDriver_t* get_lightDriverInterface(void)
 {
	 lD_interface.init=initLightDriver;
	 lD_interface.setFront=setFrontlight;
	 lD_interface.setBack=setBacklight;
	 
	 return &lD_interface;
 }

static void initLightDriver(void)
{
		DDRH |= (1<<PH4);	//Sets portH4 as output.
		DDRH |= (1<<PH5);	//Sets portH5 as output.
		TCCR4B=0b00000101;	//Sets timer 4 to no prescaling and last WGM42.
		TCCR4A = 0b00101001;	//Set on compare match channel B 5:4, set on compare match channel C 3:2, fast PWM mode and 8-bit. 
}

static void setFrontlight(unsigned char percent)
{
	uint8_t percentA=(percent>0) ? percent:1;	//Checkup if percentA >0 if not its 1
	uint16_t frontLight=((255*percentA)/100);	//percentage of full OCR4B TOP value.
	
	OCR4B=(uint8_t)frontLight;	//setting output compare.
}

static void setBacklight(unsigned char percent)
{
	uint8_t percentB=(percent>0) ? percent:1;	//Checkup
	uint16_t BackLight=((255*percentB)/100);	//percentage of full OCR4C TOP value.
	
	OCR4C=(uint8_t)BackLight;	//setting output compare.	
}
