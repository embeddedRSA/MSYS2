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
#define  MS_SQR 9.80665
#define PI_DIV_180 57.2957795130823

//private member
static i2c_t*			i2c;
static uint8_t			currentAddr		=	1;
static GA_data_struct	data_struct;
static GA_t				GA_interface;
static bool				initialized		=	false;
static uint8_t			gyroRange;
static uint8_t			accelRange;

static uint8_t	s_start(void);
static void		s_stop(void);
static void		s_enterWrite(void);
static void		s_enterRead(void);
static void		s_selectRegister(reg_addr_t reg_addr);
static void		s_write(uint8_t data);
static uint8_t	s_read(bool is_last);
static void		s_getPitchRoll(int16_t *pitchRoll);
static void		s_getAccelXYZ(int16_t* XYZ);
static void		s_getGyroXYZ(int16_t* XYZ);
static void		s_gatherData(void);
static void		s_gyroSettings(uint8_t p_range);
static void		s_accerelSettings(uint8_t p_range);
static void		s_reset(void);

GA_t* get_GA_interface(i2c_t* i2c_interface)
{
	
	if (!initialized)
	{
		i2c=i2c_interface;
		
			GA_interface.start					=	s_start;
			GA_interface.stop					=	s_stop;
			GA_interface.enterWrite				=	s_enterWrite;
			GA_interface.enterRead				=	s_enterRead;
			GA_interface.selectRegister			=	s_selectRegister;
			GA_interface.write					=	s_write;
			GA_interface.read					=	s_read;
			GA_interface.getPitchRoll 			=	s_getPitchRoll;
			GA_interface.getAccelXYZ			=	s_getAccelXYZ;
			GA_interface.getGyroXYZ				=	s_getGyroXYZ;
			GA_interface.gatherData				=	s_gatherData;
			GA_interface.gyroSettings			=	s_gyroSettings;
			GA_interface.accerelSettings		=	s_accerelSettings;
			GA_interface.reset					=	s_reset;
			initialized=true;		
	}
	return &GA_interface; 
}

static void s_gatherData(void)
{
	int i=0;
	s_start();
	s_enterWrite();
	s_selectRegister(ACCEL_XOUT_H);
	s_start();
	s_enterRead();
	
	for(i=0;i<14;++i)
	{
		if(i==13)
		{
			s_read(true);
		}
		else
		{
			s_read(false);
		}
	}
	s_stop();
}

static void s_gyroSettings(uint8_t p_range)
{
	if (p_range<=3)
	{
		gyroRange = p_range;
		s_start();
		s_enterWrite();
		s_selectRegister(27);
		s_write(accelRange<<3);
		s_stop();
	}
}

static void s_accerelSettings(uint8_t p_range)
{
//4g right now
	if (p_range<=3)
	{
		accelRange = p_range;
		s_start();
		s_enterWrite();
		s_selectRegister(28);
		s_write(accelRange<<3);
		s_stop();
	}

}



static void s_reset(void)
{
	s_start();
	s_enterWrite();
	s_selectRegister(0x6B);
	s_write(0x00);
	s_stop();
}


static uint8_t s_start(void)
{
	i2c->start();
	return 1;

}
static void s_stop(void)
{
	i2c->stop();
}
static void s_enterWrite(void)
{
	
	i2c->selectmode(ADDR_0,I2C_WRITE_MODE);
	
}
static void s_enterRead(void)
{

	i2c->selectmode(ADDR_0,I2C_READ_MODE);
	
}
static void s_selectRegister(reg_addr_t reg_addr)
{
	currentAddr=reg_addr;
	i2c->write(reg_addr);
}
static void s_write(uint8_t data)
{
	i2c->write(data);
}
static uint8_t s_read(bool is_last)
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

static void s_getAccelXYZ(int16_t* XYZ)
{
XYZ[0]=(int16_t)((data_struct.accel_x)/8.192);
XYZ[1]=(int16_t)((data_struct.accel_y)/8.192);
XYZ[2]=(int16_t)((data_struct.accel_z)/8.192);
}

static void s_getGyroXYZ(int16_t* XYZ)
{
	XYZ[0]=(int16_t)((data_struct.gyro_x)/8.192);
	XYZ[1]=(int16_t)((data_struct.gyro_y)/8.192);
	XYZ[2]=(int16_t)((data_struct.gyro_z)/8.192);
}

static void s_getPitchRoll(int16_t *pitchRoll)
{	
	pitchRoll[0] = (int16_t)((atan2(((data_struct.accel_y)/8192.0),((data_struct.accel_z)/8192.0)))*PI_DIV_180);
	pitchRoll[1] = (int16_t)((atan2((-1.0 * (data_struct.accel_x)/8192.0) , sqrt(((data_struct.accel_y)/8192.0) * ((data_struct.accel_y)/8192.0) + ((data_struct.accel_z)/8192.0) * ((data_struct.accel_z)/8192.0))))*PI_DIV_180);
  //pitchRoll[0] = (int16_t)(((atan(((data_struct.accel_y)/16384.0) / sqrt(pow(((data_struct.accel_x)/16384.0), 2) + pow(((data_struct.accel_z)/16384.0), 2))) * PI_DIV_180) - 0.58)*1000);
  //pitchRoll[1] = (int16_t)(((atan((-1.0*(data_struct.accel_x)/16384.0) / sqrt(pow(((data_struct.accel_y)/16384.0), 2) + pow(((data_struct.accel_z)/16384.0), 2))) * PI_DIV_180) + 0.58)*1000);
}



//https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/