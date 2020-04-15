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
	screen->setCursorColor(rgb565FromInt(RED));
	screen->displayON();
	
	
    while (1) 
    {
		_delay_ms(300);
		screen->setCursor(rand_lim(319-40),rand_lim(239-40));
		screen->setCursorColor(rgb565FromInt(colors[rand_lim(2)]));
		screen->drawRectangle(40,40);
    }
}

uint16_t rand_lim(uint16_t limit) 
{
/* return a random number between 0 and limit inclusive.
 */

    uint16_t divisor = (uint16_t)(RAND_MAX/(limit+1));
    uint16_t retval;

    do { 
        retval = (uint16_t)rand() / divisor;
    } while (retval > limit);

    return retval;
}

