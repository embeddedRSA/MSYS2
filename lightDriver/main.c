/*
 * lightDriver.c
 *
 * Created: 29-04-2020 08:48:27
 * Author : Anders
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "lightDriver.h"

lightDriver_t *LD_Obj;

int main(void)
{
	
	LD_Obj=get_lightDriverInterface();
	
	//Init the lightDriver.
	LD_Obj->init();
			
    while (1) 
    {
		LD_Obj=get_lightDriverInterface();
		LD_Obj->setFront(0);
		LD_Obj->setBack(0);
		_delay_ms(4000);
		LD_Obj->setFront(50);
		LD_Obj->setBack=50;
		_delay_ms(4000);
		LD_Obj->setFront=100;
		LD_Obj->setBack=100;
		_delay_ms(4000);
				
    }
}

