/*
 * rgb565.c
 *
 * Created: 05-04-2020 01:54:04
 *  Author: aaron
 */ 

#include "rgb565.h"

uint16_t rgb3byteToInt(uint8_t red,uint8_t green, uint8_t blue)
{
	uint16_t colorInt = 0;
	colorInt |= ((uint16_t)(red>>3)) << 11;
	colorInt |= ((uint16_t)(green>>2)) << 5;
	colorInt |= ((uint16_t)(blue>>3));
	
	return colorInt;
}