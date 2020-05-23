/*
 * i2c test.c
 *
 * Created: 21/05/2020 22:23:11
 * Author : ryana
 */ 

#include <avr/io.h>
#include "../../i2c_driver/i2c_driver.h"
#include "uart.h"
#define F_SCL 122000
#define UARTADDR UART0
i2c_t* i2c_obj;
int main(void)
{
	
	
	InitUART(UARTADDR, 9600, 8,0);
	SendString(UARTADDR,"\n \n \n");
	SendInteger(UARTADDR,32000);
	SendString(UARTADDR,"\n");
	SendString(UARTADDR,"creating objects \n");
	
i2c_obj = get_i2c_interface();
i2c_obj->init(F_SCL,false);
	SendString(UARTADDR,"\n");
	SendInteger(UARTADDR,i2c_obj->init(F_SCL,false));
	SendString(UARTADDR,"\n");
	SendString(UARTADDR,"\n");
	SendInteger(UARTADDR,	TWSR & 0b00000011);
	SendString(UARTADDR,"\n");

while (1)
{
i2c_obj->selectmode(0b11,I2C_WRITE_MODE);

}

return 0;
}

