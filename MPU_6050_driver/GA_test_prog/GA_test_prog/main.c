/*
 * i2c_test.c
 *
 * Created: 16/04/2020 21:34:07
 * Author : ryana
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 
#include "../../MPU_6050_driver.h"
#include "../../../i2c/i2c_driver/i2c_driver.h"
#include "../../../UART/uart.h"
#include <util/delay.h>

//#define sei()


i2c_t	*i2c_obj;
GA_t	*GA_obj;
/**
ISR(TWI_vect)
{
switch (i2c_obj->get_status())
{
	case 0x:
}
}
**/
#define UARTADDR UART0

int main(void)
{
	int i;

	for (i=0; i<2;++i)
	{
		_delay_ms(1000);
	}
	
	
	InitUART(UARTADDR, 9600, 8,0);
	SendString(UARTADDR,"\n \n \n");
	SendInteger(UARTADDR,32000);
	SendString(UARTADDR,"\n");
	SendString(UARTADDR,"creating objects \n");
	
	i2c_obj=get_i2c_interface();
	
	GA_obj=get_GA_interface(i2c_obj);
	
	SendString(UARTADDR,"init i2c: ");
	SendInteger(UARTADDR,i2c_obj->init(10000,false));
	
	
	
	
	SendString(UARTADDR,"\n start transfer: ");
	GA_obj->start();
	SendInteger(UARTADDR,(TWSR)&(0b11111000));
	SendString(UARTADDR,"\n enter write: ");
	GA_obj->enterWrite();
	SendInteger(UARTADDR,(TWSR)&(0b11111000));
	SendString(UARTADDR,"\n select reg: ");
	GA_obj->selectRegister(0x6B);
	SendInteger(UARTADDR,(TWSR)&(0b11111000));
	SendString(UARTADDR,"\n about to start: ");
	GA_obj->write(0x00);
	GA_obj->stop();
	SendString(UARTADDR,"\n is reset \n ");
	

//Set to +- 4G

	SendString(UARTADDR,"\n start transfer: ");
	GA_obj->start();
	SendInteger(UARTADDR,(TWSR)&(0b11111000));
	SendString(UARTADDR,"\n enter write: ");
	GA_obj->enterWrite();
	SendInteger(UARTADDR,(TWSR)&(0b11111000));
	SendString(UARTADDR,"\n select reg: ");
	GA_obj->selectRegister(28);
	SendInteger(UARTADDR,(TWSR)&(0b11111000));
	SendString(UARTADDR,"\n about to start: ");
	GA_obj->write(0b00001000);
	GA_obj->stop();
	SendString(UARTADDR,"\n set to 4g \n ");
	
	


    while (1) 
    {
	_delay_ms(250);

	//SendString(UARTADDR,"\n start transfer: ");
	GA_obj->start();
	
	//SendInteger(UARTADDR,(TWSR)&(0b11111000));
	//SendString(UARTADDR,"\n enter write: ");
	GA_obj->enterWrite();
	//SendInteger(UARTADDR,(TWSR)&(0b11111000));
	//SendString(UARTADDR,"\n select reg: ");
	GA_obj->selectRegister(ACCEL_XOUT_H);
	//SendInteger(UARTADDR,(TWSR)&(0b11111000));
	//SendString(UARTADDR,"\n about to start: ");
	GA_obj->start();
	//SendInteger(UARTADDR,(TWSR)&(0b11111000));
	//SendString(UARTADDR,"\n enter read: ");
	GA_obj->enterRead();
	//SendInteger(UARTADDR,(TWSR)&(0b11111000));
	
	
	//uint16_t data[13];
	for(i=0;i<14;++i)
	{
		if(i==13)
		{
			GA_obj->read(true);
		}
		else
		{
			GA_obj->read(false);
		}
	}
	GA_obj->stop();
    int16_t data[3];
	//char outstring[10];
	GA_obj->getAccelXYZ(data);
/**
	/SendString(UARTADDR,"\n");
	
	SendInteger(UARTADDR,data[0]);

	SendString(UARTADDR,",");
	SendInteger(UARTADDR,data[1]);	
	SendString(UARTADDR,",");
	SendInteger(UARTADDR,data[2]);	
	SendString(UARTADDR,"\n");
**/
SendString(UARTADDR,"\n");

if (data[2]>3800 ||data[2]<(-3800))
{
	SendString(UARTADDR,"8======D~~~ ~ ~~  ");
} 
else if (data[2]>3000 ||data[2]<(-3000))
{
	SendString(UARTADDR,"8=====D");	
}
else if(data[2]>2000 ||data[2]<(-2000))
{
	SendString(UARTADDR,"8===D");	
}
else if (data[2]>800 ||data[2]<(-800))
{
	
	SendString(UARTADDR,"8==D");	
}
else
{
	SendString(UARTADDR,"8=D");	
}

    }
	return 0;
}

