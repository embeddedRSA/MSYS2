/*
 * MPU_6050_driver.h
 *
 * Created: 26/03/2020 15:01:58
 *  Author: ryana
 */ 


#ifndef MPU_6050_DRIVER_H_
#define MPU_6050_DRIVER_H_

#define ADDR_0 0b1101000 //set to low
#include "D:\Onedrive AU\OneDrive - Aarhus universitet\6.s\msys2\MSYS2\i2c\i2c_driver\i2c_driver\i2c_driver.h"
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
	uint16_t accel_x;
	uint16_t accel_y;
	uint16_t accel_z;
	uint16_t temp;
	uint16_t gyro_x;
	uint16_t gyro_y;
	uint16_t gyro_z;
}GA_data_struct;

typedef void (*void_function_t)(void);
typedef void (*void_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_bool_PAR)(bool);
typedef uint8_t (*uint8_RET_void_PAR)(void);
typedef uint8_t (*uint8_RET_uint8_bool_PAR)(uint16_t,bool);

typedef struct
{
	uint8_RET_void_PAR start;
	void_function_t enter_write;
	void_function_t enter_read;
	void_RET_uint8_PAR select_register;
	void_RET_uint8_PAR write;
	uint8_RET_bool_PAR read;
}GA_t;
GA_t* get_GA_interface(i2c_t* i2c_interface);
/**
uint8_t start_transfer(void);
void enter_GA_write(void);
void enter_GA_read(void);
void select_GA_register(reg_addr_t reg_addr);
void write_GA(uint8_t data);
uint8_t read_GA(bool is_last);
**/

//call before enabling interrupt
/**
void init_acce(void);
void init_gyro(void);
**/
#endif /* MPU_6050_DRIVER_H_ */