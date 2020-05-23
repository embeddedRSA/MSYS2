/*
 * i2c_driver.h
 *
 * Created: 26/03/2020 10:41:17
 *  Author: ryana
 */ 

#include <stdint.h>
#include <stdbool.h>

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_
#define SCL_MAX (400000)
#define F_CPU (16000000)

typedef enum
{
	I2C_WRITE_MODE,
	I2C_READ_MODE
}I2C_MODE_t;



typedef void (*void_function_t)(void);
typedef void (*void_RET_uint8_PAR)(uint8_t);
typedef void (*void_RET_uint8_I2C_MODE_t_PAR)(uint8_t,I2C_MODE_t);
typedef void	(*void_RET_bool_PAR)(bool);
typedef uint8_t (*uint8_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_bool_PAR)(bool);
typedef uint8_t (*uint8_RET_void_PAR)(void);
typedef uint16_t (*uint16_RET_uint32_bool_PAR)(uint32_t,bool);
typedef bool	(*bool_RET_void_PAR)(void);
typedef bool	(*bool_RET_bool_PAR)(bool);


typedef struct
{
uint16_RET_uint32_bool_PAR init;
	
void_function_t disable;

void_function_t start;	

void_function_t stop;

void_RET_uint8_I2C_MODE_t_PAR selectmode;

void_RET_uint8_PAR write;

uint8_RET_bool_PAR read;

bool_RET_bool_PAR setBusy;

bool_RET_void_PAR getBusy;

uint8_RET_void_PAR getStatus;

}i2c_t;


i2c_t* get_i2c_interface(void); 

#endif /* I2C_DRIVER_H_ */