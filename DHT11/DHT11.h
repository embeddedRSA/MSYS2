/*
 * DHT11.h
 *
 * Created: 13-05-2020 09:39:52
 *  Author: Anders
 */ 
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

#define DHT_PIN 4 

#ifndef DHT11_H_
#define DHT11_H_

typedef void(*void_ret_uint8ptr_uint8ptr_PAR)(uint8_t*,uint8_t*);


typedef struct  
{
	void_ret_uint8ptr_uint8ptr_PAR getTempHumid;
	
} DHT_t;

DHT_t *getDHTInterface(void);

#endif /* DHT11_H_ */