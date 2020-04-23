/*
 * rbg565.h
 *
 * Created: 05-04-2020 01:54:20
 *  Author: aaron
 */ 


#ifndef RGB565_H_
#define RGB565_H_

#include <stdint.h>
#include <stdbool.h>

#define WHITE	(uint16_t)0xFFFF
#define RED		(uint16_t)0xF800
#define GREEN	(uint16_t)0x07E0
#define BLUE	(uint16_t)0x001F
#define BLACK	(uint16_t)0x0821

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}rgb_t;

bool rgb565IsColorValid(rgb_t *color);
uint16_t rgb565ToInt(rgb_t *color);
rgb_t* rgb565FromInt(uint16_t color);


#endif /* RGB565_H_ */