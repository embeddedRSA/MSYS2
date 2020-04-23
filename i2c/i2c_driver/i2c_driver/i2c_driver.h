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
#define SCL_MAX (400*10^3)
#define F_CPU 16000000


typedef void (*void_function_t)(void);
typedef void (*void_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_uint8_PAR)(uint8_t);
typedef uint8_t (*uint8_RET_bool_PAR)(bool);
typedef uint8_t (*uint8_RET_uint8_bool_PAR)(uint16_t,bool);
typedef bool	(*bool_RET_void_PAR)(void);
typedef void	(*void_RET_bool_PAR)(bool);
typedef bool	(*bool_RET_bool_PAR)(bool);


typedef struct
{
uint8_RET_uint8_bool_PAR init;
	
void_function_t disable;

void_function_t start;	

void_function_t stop;

void_RET_uint8_PAR selectmode;

void_RET_uint8_PAR write;

uint8_RET_bool_PAR read;

bool_RET_bool_PAR set_busy_flag;

bool_RET_void_PAR get_busy_flag;

}i2c_t;




i2c_t* get_i2c_interface(void); 
/**

static uint8_t i2c_init(uint16_t SCL_f ,bool enable_interrupt);

static void i2c_disable(void);

static void i2c_select_mode(uint8_t SLA_x);

static void i2c_start();

static void i2c_stop();

static void i2c_write(uint8_t data);

static uint8_t i2c_read (bool isLast);

static bool set_busy(bool status);

static bool get_busy(void);
**/

#endif /* I2C_DRIVER_H_ */