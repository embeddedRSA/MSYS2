/*
 * rgb565.c
 *
 * Created: 05-04-2020 01:54:04
 *  Author: aaron
 */ 

#include "rgb565.h"
#include <string.h>

static rgb_t rgbColor;

uint16_t rgb565ToInt(rgb_t *color)
{
	uint16_t colorInt = 0;
	colorInt |= ((uint16_t)color->red) << 11;
	colorInt |= ((uint16_t)color->green) << 5;
	colorInt |= (uint16_t)color->blue;
		
	return colorInt;
}

rgb_t* rgb565FromInt(uint16_t color)
{
	memset(&rgbColor,0,sizeof(rgbColor));
	rgbColor.red = (uint8_t)(color>>11);
	rgbColor.green = (uint8_t)((color&0x07FF)>>5);
	rgbColor.blue = (uint8_t)(color&0x1F);
	
	return &rgbColor;
}

bool rgb565IsColorValid(rgb_t *color)
{
	return ((color->red < 32) && (color->blue < 32) && (color->green < 64));
}