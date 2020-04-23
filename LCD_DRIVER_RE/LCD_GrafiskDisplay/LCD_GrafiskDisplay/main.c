/*
 * LCD_GrafiskDisplay.c
 *
 * Created: 05-04-2020 01:51:49
 * Author : aaron
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "lcdDriver/lcdDriver.h"
#include "lcdDriver/rgb565.h"

#define F_CPU 16000000
#include <util/delay.h>


static lcdDriverInterface_t* screen = NULL;
uint16_t colors[] = {RED,GREEN,BLUE};

uint16_t rand_lim(uint16_t limit);

int main(void)
{
    screen = lcdDriver_getDriver();
	
	screen->setCursorColor(rgb565FromInt(WHITE));
	screen->drawRectangle(319,239);
	screen->setCursorColor(rgb565FromInt(BLACK));
	screen->setCursor(50,50);
	screen->printTest();
	screen->displayON();
	
	
    while (1) 
    {
    }
}

