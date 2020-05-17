/*
 * ds1307.h
 *
 * Created: 12-04-2020 10:45:52
 *  Author: Anders
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "i2c_driver.h"

//definitions
#define DS1307_ADDR 0x68 //device address


#ifndef DS1307_H
#define DS1307_H

typedef void(*void_ret_ObjPtr_PAR)(i2c_t*);
typedef void(*void_ret_uint8_7PAR)(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
typedef void(*void_ret_uint8ptr_6PAR)(uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*);


typedef struct
{
	void_ret_ObjPtr_PAR initRTC;
	void_ret_uint8_7PAR setDateTime;
	void_ret_uint8ptr_6PAR getDateTime;
	
} RTC_t;

RTC_t *get_RTC_interface(void);
  

#endif
