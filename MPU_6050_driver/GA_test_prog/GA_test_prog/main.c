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
	SendString(UARTADDR,"\n");	
	
//Set to +- 4G
	GA_obj->reset();
	GA_obj->accerelSettings(3);
	GA_obj->gyroSettings(3);
	

	int32_t acceldata[3];
	int32_t gyrodata[3];
	int16_t dataPR[2];
    while (1) 
    {
	_delay_ms(250);
	
	GA_obj->gatherData();
   
	//char outstring[10];
	GA_obj->getGyroXYZ(gyrodata);
	GA_obj->getAccelXYZ(acceldata);
	GA_obj->getPitchRoll(dataPR);


SendString(UARTADDR,"GYRO X: ");
SendInteger(UARTADDR, gyrodata[0]);
SendString(UARTADDR,"deg/s Y: ");
SendInteger(UARTADDR, gyrodata[1]);
SendString(UARTADDR,"deg/s Z: ");
SendInteger(UARTADDR, gyrodata[2]);
SendString(UARTADDR,"deg/s \n");



/*
SendString(UARTADDR,"Accel X: ");
SendInteger(UARTADDR, acceldata[0]);
SendString(UARTADDR,"mG Y: ");
SendInteger(UARTADDR, acceldata[1]);
SendString(UARTADDR,"mG Z: ");
SendInteger(UARTADDR, acceldata[2]);
SendString(UARTADDR,"mG \n");
*/
/*
SendString(UARTADDR,"\n");
SendString(UARTADDR,"Pitch Roll: X: ");
SendInteger(UARTADDR, dataPR[0]);
SendString(UARTADDR,"deg Y: ");
SendInteger(UARTADDR, dataPR[1]);
SendString(UARTADDR,"deg\n");
*/
/**
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
*/

    }
	return 0;
}

