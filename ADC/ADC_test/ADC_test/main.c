/*
 * ADC_test.c
 *
 * Created: 30/04/2020 10:53:08
 * Author : ryana
 */ 

#include <avr/io.h>
#include <math.h>
//#include "../../ADC_MEGA2560.h"
#include "../../../UART/uart.h"
#include "../../../lightsensor/lightsensor.h"
#define F_CPU 16000000
#define LS_THRESHOLD 4500
#include <util/delay.h>


int main(void)
{
	ADC_t * ADC_Interface;
	LightSensor_t * LS_interface;
	ADC_Interface	=	get_ADC_interface();
	LS_interface	=	get_lightSensor_interface(ADC_Interface);

	//init
	ADC_Interface->initADC(AVCC,0);
	LS_interface->init(7,LS_THRESHOLD);
	InitUART(UART0, 9600, 8,0);
	SendString(UART0,"UART started \n");
		
    while (1) 
    {
	_delay_ms(1000);
	SendString(UART0,"\n");
	SendInteger(UART0 , LS_interface->getLightStatus());
	_delay_ms(100);		
	SendString(UART0,"\n 0:");
	_delay_ms(100);
	SendInteger(UART0 , ADC_Interface->getADC_mV(7));

    }

	
	
}

