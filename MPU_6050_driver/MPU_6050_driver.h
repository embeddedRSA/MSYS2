/*
 * MPU_6050_driver.h
 *
 * Created: 26/03/2020 15:01:58
 *  Author: ryana
 */ 


#ifndef MPU_6050_DRIVER_H_
#define MPU_6050_DRIVER_H_

#define ADDR_0 0b1101000 //set to low
#include "../i2c/i2c_driver/i2c_driver.h"
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

typedef void (*void_function_t)(void);
typedef void (*void_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_bool_PAR)(bool);
typedef uint8_t (*uint8_RET_void_PAR)(void);
typedef uint8_t (*uint8_RET_uint8_bool_PAR)(uint16_t,bool);
typedef void (*void_RET_doubleptr_PAR)(double*);
typedef void (*void_RET_int32ptr_PAR)(int32_t*);
typedef void (*void_RET_int16ptr_PAR)(int16_t*);


typedef struct
{
	uint8_RET_void_PAR		start;
	void_function_t			stop;
	void_function_t			enterWrite;
	void_function_t			enterRead;
	void_RET_uint8_PAR		selectRegister;
	void_RET_uint8_PAR		write;
	uint8_RET_bool_PAR		read;
	void_RET_int16ptr_PAR	getPitchRoll;
	void_RET_int32ptr_PAR	getAccelXYZ;	
	void_RET_int32ptr_PAR	getGyroXYZ;
	void_function_t			gatherData;
	void_RET_uint8_PAR		gyroSettings;
	void_RET_uint8_PAR		accerelSettings;
	void_function_t			reset;
	
	GA_data_struct*			data;
}GA_t;
GA_t* get_GA_interface(i2c_t* i2c_interface);

#endif /* MPU_6050_DRIVER_H_ */