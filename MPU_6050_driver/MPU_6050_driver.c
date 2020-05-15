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
#define ACCEL_SETTING_REG 28
#define GYRO_SETTING_REG 27
#define MS_SQR		9.80665
#define PI_DIV_180	57.2957795130823

//private member
static GA_t				s_GA_interface;
static i2c_t*			s_i2c;
static bool				s_initialized		=	false;
static uint8_t			s_currentAddr		=	1;
static GA_data_struct	s_data_struct;
static uint8_t			s_gyroRange;
static uint8_t			s_accelRange;
static double			s_scalefact_accel	=	0;
static double			s_scalefact_gyro	=	0;
static uint8_t	s_start(void);
static void		s_stop(void);
static void		s_enterWrite(void);
static void		s_enterRead(void);
static void		s_selectRegister(reg_addr_t p_reg_addr);
static void		s_write(uint8_t p_data);
static uint8_t	s_read(bool p_is_last);
static void		s_getPitchRoll(int16_t *p_pitchRoll);
static void		s_getAccelXYZ(int32_t* p_XYZ);
static void		s_getGyroXYZ(int32_t* p_XYZ);
static void		s_gatherData(void);
static void		s_gyroSettings(uint8_t p_range);
static void		s_accerelSettings(uint8_t p_range);
static void		s_reset(void);

GA_t* get_GA_interface(i2c_t* p_i2c_interface)
{
	
	if (!s_initialized)
	{
		s_i2c=p_i2c_interface;
		
			s_GA_interface.reset					=	s_reset;
			s_GA_interface.gyroSettings				=	s_gyroSettings;
			s_GA_interface.accerelSettings			=	s_accerelSettings;
			s_GA_interface.getPitchRoll 			=	s_getPitchRoll;
			s_GA_interface.getAccelXYZ				=	s_getAccelXYZ;
			s_GA_interface.getGyroXYZ				=	s_getGyroXYZ;
			s_GA_interface.gatherData				=	s_gatherData;
			/**
			s_GA_interface.start					=	s_start;
			s_GA_interface.stop						=	s_stop;
			s_GA_interface.enterWrite				=	s_enterWrite;
			s_GA_interface.enterRead				=	s_enterRead;
			s_GA_interface.selectRegister			=	s_selectRegister;
			s_GA_interface.write					=	s_write;
			s_GA_interface.read						=	s_read;
			**/
			
			s_initialized=true;		
	}
	return &s_GA_interface; 
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
		s_gyroRange = p_range;
		s_start();
		s_enterWrite();
		s_selectRegister(GYRO_SETTING_REG);
		s_write(s_gyroRange<<3);
		s_stop();
		
		switch (s_gyroRange)
		{
		case 0:
			s_scalefact_gyro = 131.0;	
		break;
		case 1:
			s_scalefact_gyro = 65.5;	
		break;
		case 2:
			s_scalefact_gyro = 32.8;	
		break;
		case 3:
			s_scalefact_gyro = 16.4;
		break;
		
		default:
		//do nothing 
		break;
			
		}
	}
}

static void s_accerelSettings(uint8_t p_range)
{
//4g right now
	if (p_range<=3)
	{
		s_accelRange = p_range;
		s_start();
		s_enterWrite();
		s_selectRegister(ACCEL_SETTING_REG);
		s_write(s_accelRange<<3);
		s_stop();
		
	switch (s_accelRange)
	{
		case 0:
		s_scalefact_accel = 16384.0;
		break;
		case 1:
		s_scalefact_accel = 8192.0;
		break;
		case 2:
		s_scalefact_accel = 4096.0;
		break;
		case 3:
		s_scalefact_accel = 2048.0;
		break;
		
		default:
		//do nothing
		break;
		
	}
	
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
	s_i2c->start();
	return 1;

}
static void s_stop(void)
{
	s_i2c->stop();
}
static void s_enterWrite(void)
{
	
	s_i2c->selectmode(ADDR_0,I2C_WRITE_MODE);
	
}
static void s_enterRead(void)
{

	s_i2c->selectmode(ADDR_0,I2C_READ_MODE);
	
}
static void s_selectRegister(reg_addr_t p_reg_addr)
{
	s_currentAddr=p_reg_addr;
	s_i2c->write(p_reg_addr);
}
static void s_write(uint8_t p_data)
{
	s_i2c->write(p_data);
}
static uint8_t s_read(bool p_is_last)
{
	int16_t temp=s_i2c->read(p_is_last);
	
	switch (s_currentAddr)
	{
		//accelerometer
		case ACCEL_XOUT_H:
			s_data_struct.accel_x=temp<<8;
		break;
		
		case ACCEL_XOUT_L:
		s_data_struct.accel_x|=temp;
		break;
		
		case ACCEL_YOUT_H:
			s_data_struct.accel_y=temp<<8;
		break;
		
		case ACCEL_YOUT_L:
			s_data_struct.accel_y|=temp;
		break;
		
		case ACCEL_ZOUT_H:
			s_data_struct.accel_z=temp<<8;
		break;
		
		case ACCEL_ZOUT_L:
			s_data_struct.accel_z|=temp;
		break;
		
		//temperature
		case TEMP_OUT_H:
			s_data_struct.temp=temp<<8;
		break;
		
		case TEMP_OUT_L:
			s_data_struct.temp|=temp;
		break;
		
		//Gyro
		case GYRO_XOUT_H:
			s_data_struct.gyro_x=temp<<8;
		break;
		
		case GYRO_XOUT_L:
			s_data_struct.gyro_x|=temp;
		break;
		
		case GYRO_YOUT_H:
			s_data_struct.gyro_y=temp<<8;
		break;
		
		case GYRO_YOUT_L:
			s_data_struct.gyro_y|=temp;
		break;
			
		case GYRO_ZOUT_H:
			s_data_struct.gyro_z=temp<<8;
		break;
				
		case GYRO_ZOUT_L:
			s_data_struct.gyro_z|=temp;
		break;
		
		default:
		//de nada
		break;
	}
	
	s_currentAddr++;
	//SendInteger(UART0,currentAddr);
	//SendString(UART0,"\n");
return temp;
}

static void s_getAccelXYZ(int32_t* p_XYZ)
{
	//return mG
	p_XYZ[0]=(int32_t)((s_data_struct.accel_x)/(s_scalefact_accel/1000.0));
	p_XYZ[1]=(int32_t)((s_data_struct.accel_y)/(s_scalefact_accel/1000.0));
	p_XYZ[2]=(int32_t)((s_data_struct.accel_z)/(s_scalefact_accel/1000.0));

	
	////return mm/s^2
	//p_XYZ[0]=(int32_t)(MS_SQR*(data_struct.accel_x)/(s_scalefact_accel/1000.0));
	//p_XYZ[1]=(int32_t)(MS_SQR*(data_struct.accel_y)/(s_scalefact_accel/1000.0));
	//p_XYZ[2]=(int32_t)(MS_SQR*(data_struct.accel_z)/(s_scalefact_accel/1000.0));
}

static void s_getGyroXYZ(int32_t* p_XYZ)
{
	//return in deg/s
	p_XYZ[0]=(int32_t)((s_data_struct.gyro_x)/(s_scalefact_gyro));
	p_XYZ[1]=(int32_t)((s_data_struct.gyro_y)/(s_scalefact_gyro));
	p_XYZ[2]=(int32_t)((s_data_struct.gyro_z)/(s_scalefact_gyro));
}

static void s_getPitchRoll(int16_t *p_pitchRoll)
{	
	p_pitchRoll[0] = (int16_t)((atan2(((s_data_struct.accel_y)/s_scalefact_accel),((s_data_struct.accel_z)/s_scalefact_accel)))*PI_DIV_180);
	p_pitchRoll[1] = (int16_t)((atan2((-1.0 * (s_data_struct.accel_x)/s_scalefact_accel) , sqrt(((s_data_struct.accel_y)/s_scalefact_accel) * ((s_data_struct.accel_y)/s_scalefact_accel) 
					+ ((s_data_struct.accel_z)/s_scalefact_accel) * ((s_data_struct.accel_z)/s_scalefact_accel))))*PI_DIV_180);

//https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing

}






//https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/