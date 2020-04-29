/*
 * MPU_6050_driver.c
 *
 * Created: 26/03/2020 15:00:51
 * Author : ryana
 */ 

#include <avr/io.h>
#include "MPU_6050_driver.h"
#include "../UART/uart.h"
#include <math.h>

#define PI_DIV_180 57.2957795130823
//private member
static i2c_t* i2c;
static uint8_t currentAddr=1;
static GA_data_struct data_struct;
static GA_t GA_interface;
static bool initialized = false;

static uint8_t start_s(void);
static void stop_s(void);
static void enterWrite_s(void);
static void enterRead_s(void);
static void selectRegister_s(reg_addr_t reg_addr);
static void write_s(uint8_t data);
static uint8_t read_s(bool is_last);
static void getPitchRoll_s(double *pitchRoll);
static void getAccelXYZ_s(int16_t* XYZ);


GA_t* get_GA_interface(i2c_t* i2c_interface)
{
	
	if (!initialized)
	{
		i2c=i2c_interface;
		
			GA_interface.start					=	start_s;
			GA_interface.stop					=	stop_s;
			GA_interface.enterWrite				=	enterWrite_s;
			GA_interface.enterRead				=	enterRead_s;
			GA_interface.selectRegister			=	selectRegister_s;
			GA_interface.write					=	write_s;
			GA_interface.read					=	read_s;
			GA_interface.getPitchRoll 			=	getPitchRoll_s;
			GA_interface.getAccelXYZ			=	getAccelXYZ_s;			
	}
	return &GA_interface; 
}

static uint8_t start_s(void)
{
	i2c->start();
	return 1;
	/**
	if ((i2c->getBusy)==false)
	{
		//i2c->setBusy(true);
		i2c->start();
		return 1;
	}
	else
	{
		//if busy
		return 0;
	}
	**/
}
static void stop_s(void)
{
	i2c->stop();
}
static void enterWrite_s(void)
{
	
	i2c->selectmode(ADDR_0,I2C_WRITE_MODE);
	
}
static void enterRead_s(void)
{

	i2c->selectmode(ADDR_0,I2C_READ_MODE);
	
}
static void selectRegister_s(reg_addr_t reg_addr)
{
	currentAddr=reg_addr;
	i2c->write(reg_addr);
}
static void write_s(uint8_t data)
{
	i2c->write(data);
}
static uint8_t read_s(bool is_last)
{
	int16_t temp=i2c->read(is_last);
	
	switch (currentAddr)
	{
		//accelerometer
		case ACCEL_XOUT_H:
			data_struct.accel_x=temp<<8;
		break;
		
		case ACCEL_XOUT_L:
		data_struct.accel_x|=temp;
		break;
		
		case ACCEL_YOUT_H:
			data_struct.accel_y=temp<<8;
		break;
		
		case ACCEL_YOUT_L:
			data_struct.accel_y|=temp;
		break;
		
		case ACCEL_ZOUT_H:
			data_struct.accel_z=temp<<8;
		break;
		
		case ACCEL_ZOUT_L:
			data_struct.accel_z|=temp;
		break;
		
		//temperature
		case TEMP_OUT_H:
			data_struct.temp=temp<<8;
		break;
		
		case TEMP_OUT_L:
			data_struct.temp|=temp;
		break;
		
		//Gyro
		case GYRO_XOUT_H:
			data_struct.gyro_x=temp<<8;
		break;
		
		case GYRO_XOUT_L:
			data_struct.gyro_x|=temp;
		break;
		
		case GYRO_YOUT_H:
			data_struct.gyro_y=temp<<8;
		break;
		
		case GYRO_YOUT_L:
			data_struct.gyro_y|=temp;
		break;
			
		case GYRO_ZOUT_H:
			data_struct.gyro_z=temp<<8;
		break;
				
		case GYRO_ZOUT_L:
			data_struct.gyro_z|=temp;
		break;
		
		default:
		//de nada
		break;
	}
	
	currentAddr++;
	//SendInteger(UART0,currentAddr);
	//SendString(UART0,"\n");
return temp;
}

static void getAccelXYZ_s(int16_t* XYZ)
{
XYZ[0]=(int16_t)((data_struct.accel_x)/8.192);
XYZ[1]=(int16_t)((data_struct.accel_y)/8.192);
XYZ[2]=(int16_t)((data_struct.accel_z)/8.192);
}

static void getPitchRoll_s(double *pitchRoll)
{	
		pitchRoll[0] = (atan(((data_struct.accel_y)/16384.0) / sqrt(pow(((data_struct.accel_x)/16384.0), 2) + pow(((data_struct.accel_z)/16384), 2))) * PI_DIV_180) - 0.58;
	pitchRoll[1] = (atan((-1.0*(data_struct.accel_x)/16384.0) / sqrt(pow(((data_struct.accel_y)/16384.0), 2) + pow(((data_struct.accel_z)/16384), 2))) * PI_DIV_180) + 0.58;
}



//https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/