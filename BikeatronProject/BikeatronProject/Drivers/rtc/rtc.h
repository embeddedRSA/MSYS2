/*
 * rtc.h
 *
 * Created: 12-04-2020 10:45:52
 *  Author: Anders
 */ 
#include <avr/io.h>
#include <stdbool.h>
#include "../i2c/i2c.h"

//definitions
#define DS1307_ADDR 0x68 //device address


#ifndef RTC_H_
#define RTC_H_

typedef void(*void_ret_uint8_7PAR)(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
typedef void(*void_ret_uint8ptr_6PAR)(uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*);

typedef struct
{
	void_ret_uint8_7PAR setDateTime;
	void_ret_uint8ptr_6PAR getDateTime;
	
} RTC_t;

RTC_t *get_RTC_interface(i2c_t*);
  

#endif /*RTC_H_*/
