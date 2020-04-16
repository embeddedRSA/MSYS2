/*
 * lcdDriver.h
 *
 * Created: 05-04-2020 01:53:06
 *  Author: aaron
 */ 


#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "rgb565.h"

typedef void(*voidFunction_t)(void);
typedef void(*xyFunction_t)(uint16_t,uint16_t);
typedef void(*colorFunction_t)(rgb_t*);

typedef struct  
{
	voidFunction_t displayON;
	voidFunction_t displayOFF;
	colorFunction_t setCursorColor;
	xyFunction_t setCursor;
	xyFunction_t drawRectangle;
	voidFunction_t printTest;
	
}lcdDriverInterface_t;

lcdDriverInterface_t* lcdDriver_getDriver();

#endif /* LCDDRIVER_H_ */