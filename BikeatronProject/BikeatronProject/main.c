/*
 * BikeatronProject.c
 *
 * Created: 20-05-2020 11:06:01
 * Author : aaron
 */ 

#define F_CPU 16000000
#include <stdint.h>
#include "Drivers/screen/screen.h"
#include "Drivers/screen/rgb565.h"
#include "Drivers/touch/touch.h"
#include <util/delay.h>
typedef enum
{
	ALL = 0,
	MILESTONE,
	TEMPERATURE,
	VELOCITY,
	STEEPNESS,
	NUMBER_OF_STATES
}GUIstate_t;

enum
{
	BACKGROUND_COLOR = 0,
	FONT_COLOR = 1,
	NUMBER_OF_COLORS
};

static lcdDriverInterface_t* screen;
static touchDriverInterface_t* touch;

static GUIstate_t currentState = TEMPERATURE;
static GUIstate_t previousState = VELOCITY;

static uint16_t GUIstateColors[NUMBER_OF_COLORS][NUMBER_OF_STATES];

static uint16_t rcnt = 0;
static uint16_t lcnt = 0;

static float speed;
static float distance;
static uint8_t temperature;


static void GUI_sm(void);
static void setUpGUIColors(void);
static void drawGrid(void);

int main(void)
{
	setUpGUIColors();
	screen = lcdDriver_getDriver();
	touch = touchDriver_getDriver();
    while (1) 
    {
		uint16_t val = touch->readTouchX();
		
		if ((val < 2000) && (val > 1))
		{	
			lcnt = 0;
			rcnt++;
			_delay_ms(5);
			if(rcnt>10)
			{
				currentState = (currentState+1) % NUMBER_OF_STATES;
				rcnt = 0;
			}
		}
		
		else if ((val > 3000) && (val < 4000))
		{
			rcnt = 0;
			lcnt++;
			_delay_ms(5);
			if(lcnt>10)
			{
				currentState = (currentState == ALL) ? (NUMBER_OF_STATES-1) : currentState-1;
				lcnt = 0;
			}
		}
		else
		{
			rcnt = 0;
			lcnt = 0;
		}
		
		GUI_sm();
		/*
		cnt = eeprom_read_word(0);
		if (cnt == 0xFFFF)
		{
			cnt = 0;
		}
		
		eepromcnt++;
		
		if (eepromcnt > 200)
		{
			cnt++;
			eepromcnt = 0;
			eeprom_write_word(0,cnt);
		}
		*/
    }
	
	return 0;
}

static void GUI_sm()
{
	if (previousState != currentState)
	{
		screen->setCursorColor(GUIstateColors[BACKGROUND_COLOR][currentState]);
		screen->setCursor(0,0);
		screen->drawRectangle(319,239);
		screen->setCursorColor(GUIstateColors[FONT_COLOR][currentState]);
		screen->setBackgroundColor(GUIstateColors[BACKGROUND_COLOR][currentState]);
		
		switch(currentState)
		{
			case ALL:
			
			drawGrid();
			screen->setCursor(35,20);
			screen->printString("Date");
			screen->setCursor(35,50);
			screen->printString("14-05-20");
			
			screen->setCursor(210,20);
			screen->printString("km/t");
			screen->setCursor(210,50);
			screen->printFloat(25.87);
			
			screen->setCursor(50,100);
			screen->printString("Temp.");
			screen->setCursor(60,130);
			screen->printInteger(28);
			
			
			screen->setCursor(210,110);
			screen->printString("Steep");
			
			screen->setCursor(100,175);
			screen->printString("Km count");
			screen->setCursor(130,205);
			screen->printInteger(100);
			break;
			
			case MILESTONE:
			screen->setCursor(80,120);
			screen->printString(" MILESTONE  ");
			break;
			
			case TEMPERATURE:
			screen->setCursor(80,120);
			screen->printString("   TEMP     ");
			break;
			
			case VELOCITY:
			screen->setCursor(80,120);
			screen->printString("   VELO     ");
			break;
			
			case STEEPNESS:
			screen->setCursor(80,120);
			screen->printString("   STEP     ");
			break;
			
			
			default:
			currentState = TEMPERATURE;
			break;
		}
		previousState = currentState;
		_delay_ms(500);
	}
}

static void setUpGUIColors(void)
{
	GUIstateColors[BACKGROUND_COLOR][ALL] = rgb3byteToInt(181,230,29);
	GUIstateColors[FONT_COLOR][ALL] = rgb3byteToInt(8,4,8);
	
	GUIstateColors[BACKGROUND_COLOR][MILESTONE] = rgb3byteToInt(112,146,190);
	GUIstateColors[FONT_COLOR][MILESTONE] = rgb3byteToInt(255,255,255);
	
	GUIstateColors[BACKGROUND_COLOR][TEMPERATURE] = rgb3byteToInt(254,254,254);
	GUIstateColors[FONT_COLOR][TEMPERATURE] = rgb3byteToInt(255,127,39);
	
	GUIstateColors[BACKGROUND_COLOR][VELOCITY] = rgb3byteToInt(200,192,231);
	GUIstateColors[FONT_COLOR][VELOCITY] = rgb3byteToInt(253,240,9);
	
	GUIstateColors[BACKGROUND_COLOR][STEEPNESS] = rgb3byteToInt(163,73,164);
	GUIstateColors[FONT_COLOR][STEEPNESS] = rgb3byteToInt(253,200,14);
}

static void drawGrid(void)
{
	screen->setCursor(0,0);
	screen->drawRectangle(319,239);
	screen->setCursorColor(GUIstateColors[BACKGROUND_COLOR][currentState]);
	screen->setCursor(3,3);
	screen->drawRectangle(154,74);
	screen->setCursor(160,3);
	screen->drawRectangle(154,74);
	screen->setCursor(3,80);
	screen->drawRectangle(154,74);
	screen->setCursor(160,80);
	screen->drawRectangle(154,74);
	screen->setCursor(3,160);
	screen->drawRectangle(311,74);
	screen->setCursorColor(GUIstateColors[FONT_COLOR][currentState]);
}

