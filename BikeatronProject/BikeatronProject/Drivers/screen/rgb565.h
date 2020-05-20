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

uint16_t rgb3byteToInt(uint8_t red,uint8_t green, uint8_t blue);


#endif /* RGB565_H_ */