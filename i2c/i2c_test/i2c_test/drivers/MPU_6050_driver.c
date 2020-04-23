/*
 * MPU_6050_driver.c
 *
 * Created: 26/03/2020 15:00:51
 * Author : ryana
 */ 

#include <avr/io.h>
#include "MPU_6050_driver.h"
#include "D:\Onedrive AU\OneDrive - Aarhus universitet\6.s\msys2\MSYS2\i2c\i2c_driver\i2c_driver\i2c_driver.h"
//private member
static i2c_t* i2c;
static uint8_t currentAddr=0;
static GA_data_struct data_struct;
static GA_t GA_interface;
static bool initialized = false;

static  uint8_t start_transfer(void);
static void enter_GA_write(void);
static void enter_GA_read(void);
static void select_GA_register(reg_addr_t reg_addr);
static void write_GA(uint8_t data);
static uint8_t read_GA(bool is_last);


GA_t* get_GA_interface(i2c_t* i2c_interface)
{
	
	if (!initialized)
	{
		i2c=i2c_interface;
		
			GA_interface.start					=	start_transfer;
			GA_interface.enter_write			=	enter_GA_write;
			GA_interface.enter_read				=	enter_GA_read;
			GA_interface.select_register		=	select_GA_register;
			GA_interface.write					=	write_GA;
			GA_interface.read					=	read_GA;
		
	}
	return &GA_interface; 
}

static uint8_t start_transfer(void)
{
	if (i2c->get_busy_flag()==false)
	{
		i2c->set_busy_flag(true);
		return 1;
		i2c->start();
	}
	else
	{
		//if busy
		return 0;
	}
	
}
static void enter_GA_write(void)
{
	i2c->selectmode((ADDR_0<<1)|0b1);
	
}
static void enter_GA_read(void)
{

	i2c->selectmode((ADDR_0<<1)&(~0b1));	
	
}
static void select_GA_register(reg_addr_t reg_addr)
{
	currentAddr=reg_addr;
	i2c->write(reg_addr);
}
static void write_GA(uint8_t data)
{
	i2c->write(data);
	currentAddr++;
}
static uint8_t read_GA(bool is_last)
{
	int8_t temp=i2c->read(is_last);

	switch (currentAddr)
	{
		//accelerometer
		case ACCEL_XOUT_H:
			data_struct.accel_x=temp<<8;
		break;
		
		case ACCEL_XOUT_L:
			data_struct.accel_x=temp;
		break;
		
		case ACCEL_YOUT_H:
			data_struct.accel_y=temp<<8;
		break;
		
		case ACCEL_YOUT_L:
			data_struct.accel_y=temp;
		break;
		
		case ACCEL_ZOUT_H:
			data_struct.accel_z=temp<<8;
		break;
		
		case ACCEL_ZOUT_L:
			data_struct.accel_z=temp;
		break;
		
		//temperature
		case TEMP_OUT_H:
			data_struct.temp=temp<<8;
		break;
		
		case TEMP_OUT_L:
			data_struct.temp=temp;
		break;
		
		//Gyro
		case GYRO_XOUT_H:
			data_struct.gyro_x=temp<<8;
		break;
		
		case GYRO_XOUT_L:
			data_struct.gyro_x=temp;
		break;
		
		case GYRO_YOUT_H:
			data_struct.gyro_y=temp<<8;
		break;
		
		case GYRO_YOUT_L:
			data_struct.gyro_y=temp;
		break;
			
		case GYRO_ZOUT_H:
			data_struct.gyro_z=temp<<8;
		break;
				
		case GYRO_ZOUT_L:
			data_struct.gyro_z=temp;
		break;
		
		default:
		//de nada
		break;
	}
	
currentAddr++;
return temp;
	
}

