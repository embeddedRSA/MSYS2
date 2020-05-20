/*
 * i2c_driver.h
 *
 * Created: 26/03/2020 10:41:17
 *  Author: ryana
 */ 

#include <stdint.h>
#include <stdbool.h>
#include "../../Utils/functionPointers.h"

#ifndef I2C_H_
#define I2C_H_
#define SCL_MAX (uint32_t)400000
#define F_CPU 16000000

typedef enum
{
	I2C_WRITE_MODE,
	I2C_READ_MODE
}I2C_MODE_t;


typedef struct
{
	uint16_uint32_bool_f_t init;
	
	void_void_f_t disable;

	void_void_f_t start;	

	void_void_f_t stop;

	void_2_uint8_f_t selectmode;

	void_uint8_f_t write;

	uint8_bool_f_t read;

	bool_bool_f_t setBusy;

	bool_void_f_t getBusy;

	uint8_void_f_t getStatus;
	
}i2c_t;


i2c_t* get_i2c_interface(void); 

#endif /* I2C_H_ */