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
#define READ_PRESSURE		(uint8_t)(0b11001001)static uint16_t getData(uint8_t command);static uint16_t getThouchX(void);static uint16_t getThouchY(void);static uint16_t getPressure(void);static void driverInit();static void CLK();static touchDriverInterface_t myInterface;static bool initialized = false;touchDriverInterface_t* touchDriver_getDriver()
{
	if(!initialized)
	{
		driverInit();
		myInterface.readPressure = getPressure;
		myInterface.readTouchX = getThouchX;
		myInterface.readTouchY = getThouchY;
	}
	return &myInterface;
}static void driverInit(){	uint8_t bit = 0b00000001;		/* Set CLK, CS and DIN as outputs, DIN refers to DIN of touch controller*/	DDRH |= (bit<<TOUCH_CLK_BIT);
	DDRE |= (bit<<TOUCH_CS_BIT);	DDRG |= (bit<<TOUCH_DIN_BIT);			/* Set DOUT of touch controller to be input in arduino*/	DDRE &= ~(bit<<TOUCH_DOUT_BIT);	DDRE &= ~(bit<<TOUCH_IRQ_BIT);}static uint16_t getData(uint8_t command){	uint8_t i = 0;	uint8_t bit = 0b00000001;	uint16_t data = 0;		TOUCH_CS_PORT |= bit<<TOUCH_CS_BIT;	TOUCH_CLK_PORT &= ~(bit<<TOUCH_CLK_BIT);		/* Start sending command */	TOUCH_CS_PORT &= ~(bit<<TOUCH_CS_BIT);	_NOP();	_NOP();		for (i = 0; i<8; i++)
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
	}	i = 0;	TOUCH_DIN_PORT  &= ~(bit<<TOUCH_DIN_BIT);	for (i = 0; i<16; i++)
	{
		CLK();
		if (TOUCH_DOUT_PIN&(bit<<TOUCH_DOUT_BIT))
		{
			data |= bit<<(15-i);
		}
	}	TOUCH_CS_PORT |= bit<<TOUCH_CS_BIT;		return (data>>4);}static void CLK(){	uint8_t bit = 0b00000001;	TOUCH_CLK_PORT |= bit<<TOUCH_CLK_BIT;
	_NOP();
	_NOP();
	_NOP();
	_NOP();
	
	TOUCH_CLK_PORT &= ~(bit<<TOUCH_CLK_BIT);	_NOP();
	_NOP();
	_NOP();
	_NOP();
	}static uint16_t getThouchX(void){	return getData(READ_X_POSITION);	}static uint16_t getThouchY(void){	return getData(READ_Y_POSITION);}static uint16_t getPressure(void){	return getData(READ_PRESSURE);}