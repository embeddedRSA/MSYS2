/*
 * DHT11.h
 *
 * Created: 13-05-2020 09:39:52
 *  Author: Anders
 */ 
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "../../Utils/functionPointers.h"
#define DHT_PIN 4 

#ifndef DHT11_H_
#define DHT11_H_


typedef struct  
{
	void_2_uint8ptr_f_t getTempHumid;
	
} DHT_t;

DHT_t *getDHTInterface(void);

#endif /* DHT11_H_ */