/*
 * MPU_6050_driver.h
 *
 * Created: 26/03/2020 15:01:58
 *  Author: ryana
 */ 


#ifndef GA_H_
#define GA_H_

#define ADDR_0 0b1101000 //set to low
#include "../i2c/i2c.h"
#include "../../Utils/functionPointers.h"
typedef enum
{
	ACCEL_XOUT_H = 0x3B,
	ACCEL_XOUT_L = 0x3C,
	ACCEL_YOUT_H = 0x3D,
	ACCEL_YOUT_L = 0x3E,
	ACCEL_ZOUT_H = 0x3F,
	ACCEL_ZOUT_L = 0x40,
	
	TEMP_OUT_H = 0x41,
	TEMP_OUT_L = 0x42,
	
	GYRO_XOUT_H = 0x43,
	GYRO_XOUT_L = 0x44,
	GYRO_YOUT_H = 0x45,
	GYRO_YOUT_L = 0x46,
	GYRO_ZOUT_H = 0x47,
	GYRO_ZOUT_L = 0x48
}reg_addr_t;

typedef struct  
{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t temp;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
}GA_data_struct;

typedef struct
{
	void_int16ptr_f_t getPitchRoll;
	void_int32ptr_f_t getAccelXYZ;	
	void_int32ptr_f_t getGyroXYZ;
	void_void_f_t gatherData;
	void_uint8_f_t gyroSettings;
	void_uint8_f_t accerelSettings;
	void_void_f_t reset;
	
}GA_t;

GA_t* get_GA_interface(i2c_t* i2c_interface);

#endif /* GA_H_ */