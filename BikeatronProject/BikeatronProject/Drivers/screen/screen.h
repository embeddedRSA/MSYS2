/*
 * screen.h
 *
 * Created: 05-04-2020 01:53:06
 *  Author: aaron
 */ 


#ifndef SCREEN_H_
#define SCREEN_H_


#include "rgb565.h"
#include "../../Utils/functionPointers.h"

typedef struct  
{
	void_void_f_t displayON;
	void_void_f_t displayOFF;
	void_uint16_f_t setBackgroundColor;
	void_uint16_f_t setCursorColor;
	void_2_uint16_f_t setCursor;
	void_2_uint16_f_t drawRectangle;
	void_string_f_t printString;
	void_int16_f_t printInteger;
	void_float_f_t printFloat;
	
}lcdDriverInterface_t;

lcdDriverInterface_t* lcdDriver_getDriver();

#endif /* SCREEN_H_ */