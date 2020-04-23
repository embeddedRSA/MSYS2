/*
 * i2c_test.c
 *
 * Created: 16/04/2020 21:34:07
 * Author : ryana
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "D:\Onedrive AU\OneDrive - Aarhus universitet\6.s\msys2\MSYS2\i2c\i2c_test\i2c_test\drivers\MPU_6050_driver.h"
#include "D:\Onedrive AU\OneDrive - Aarhus universitet\6.s\msys2\MSYS2\i2c\i2c_test\i2c_test\drivers\i2c_driver.h"

i2c_t	*i2c_obj;
GA_t	*GA_obj;

ISR(TWI_vect)
{
switch (i2c_obj->get_status())
{
	case 0x
}
}


int main(void)
{
	
	i2c_obj	=	get_i2c_interface();
	GA_obj	=	get_GA_interface(i2c_obj);

	{
//code
}
    while (1) 
    {
	
    }
}

