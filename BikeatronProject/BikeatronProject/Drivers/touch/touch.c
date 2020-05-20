/*
 * touch.c
 *
 * Created: 23-04-2020 10:19:57
 *  Author: aaron
 */ 

#include "touch.h"
#include "../screen/screen.h"
#include "../screen/rgb565.h"
#include "../../Utils/HAL_Definitions.h"

#include <avr/cpufunc.h>
#include <stdbool.h>
#define F_CPU 16000000
#define READ_X_POSITION		(uint8_t)(0b10011001) /* x is y in datasheet so values are reversed, they see screen as stand not landscape*/
#define READ_Y_POSITION		(uint8_t)(0b11011001)	/* S = 1, A0-3 = 101 (Measure y position)  MODE = 1 (8bit) SER/DFR = 0 (Differential) P0-1 = 01 (FULL_POWER and no ref)*/
#define READ_PRESSURE		(uint8_t)(0b11001001)
{
	if(!initialized)
	{
		driverInit();
		myInterface.readPressure = getPressure;
		myInterface.readTouchX = getThouchX;
		myInterface.readTouchY = getThouchY;
	}
	return &myInterface;
}
	DDRE |= (bit<<TOUCH_CS_BIT);
	{
		if (command&(bit<<(7-i)))
		{
			TOUCH_DIN_PORT |= bit<<TOUCH_DIN_BIT;
		} 
		else
		{
			TOUCH_DIN_PORT  &= ~(bit<<TOUCH_DIN_BIT);
		}
		CLK();
	}
	{
		CLK();
		if (TOUCH_DOUT_PIN&(bit<<TOUCH_DOUT_BIT))
		{
			data |= bit<<(15-i);
		}
	}
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	
	TOUCH_CLK_PORT &= ~(bit<<TOUCH_CLK_BIT);
	_NOP();
	_NOP();
	_NOP();
	