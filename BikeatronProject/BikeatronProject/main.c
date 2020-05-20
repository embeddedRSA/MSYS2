/*
 * BikeatronProject.c
 *
 * Created: 20-05-2020 11:06:01
 * Author : aaron
 */ 

#define F_CPU 16000000
#define LIGHT_TRESH 180
#include <stdint.h>
#include "Drivers/screen/screen.h"
#include "Drivers/screen/rgb565.h"
#include "Drivers/touch/touch.h"
#include "Drivers/tacho/tacho.h"
#include "Drivers/temp/temp.h"
#include "Drivers/rtc/rtc.h"
#include "Drivers/i2c/i2c.h"
#include "Drivers/adc/adc.h"
#include "Drivers/light_sensor/lightsensor.h"
#include "Drivers/light_controller/lightcontroller.h"
#include "Drivers/ga/ga.h"
#include <util/delay.h>
#include <avr/interrupt.h>

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

static uint16_t timerOverflows;
static uint16_t timerCount;
static uint8_t checkpointCnt;
static uint8_t revolutionsForCalc;

static lcdDriverInterface_t* screen;
static touchDriverInterface_t* touch;
static speedSensorInterface_t* speedSensor;
static DHT_t* tempSensor;
static RTC_t* myClock;
static i2c_t* myI2C;
static ADC_t* myADC;
static LightSensor_t* myLightSensor;
static lightDriver_t* myLights;
static GA_t* myGyro;

static GUIstate_t currentState = TEMPERATURE;
static GUIstate_t previousState = VELOCITY;

static uint16_t GUIstateColors[NUMBER_OF_COLORS][NUMBER_OF_STATES];

static uint16_t rcnt = 0;
static uint16_t lcnt = 0;

static bool lightFlag = false;

static void GUI_sm(void);
static void updateGUI_sm();
static void setUpGUIColors(void);
static void drawGrid(void);

int main(void)
{
	setUpGUIColors();
	screen = lcdDriver_getDriver();
	touch = touchDriver_getDriver();
	speedSensor = speedSensor_getDriver(65);
	tempSensor = getDHTInterface();
	myADC	= get_ADC_interface();
	myLights = get_lightDriverInterface();
	myLightSensor = get_lightSensor_interface(myADC);
	myI2C = get_i2c_interface();
	//myClock = get_RTC_interface(myI2C);
	//myClock->setDateTime(20,5,20,3,16,45,30);
	//myGyro = get_GA_interface(myI2C);
	
	myI2C->init(10000,false);
	myADC->initADC(AVCC,0);
	myLightSensor->init(0, LIGHT_TRESH);
	myLights->init();
	//myGyro->reset();
	//myGyro->accerelSettings(1);
	sei();
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
		if (myLightSensor->getLightStatus() && !lightFlag)
		{
			myLights->setBack(100);
			myLights->setFront(100);
			lightFlag = true;
		}
		else if(lightFlag)
		{
			myLights->setBack(0);
			myLights->setFront(0);
			lightFlag = false;
		}
    }
	
	return 0;
}

ISR(INT3_vect) //PE4
{
	speedSensor->updateMilestoneCount();
	revolutionsForCalc++; //Counts up the revolutions for speed calculation.
}


//TIMER 2 interrupts every second to measure how many revolutions the wheel has made.
ISR(TIMER2_OVF_vect)
{
	// After 62500 interrupts overflows the timer counts another timer to get other times.
	// Because of CPU clock and prescaling 8bit timer overflows every 16.063 us. Delay=prescaler*(OCRn+1)/fcpu
	timerOverflows++;
	
	if (timerOverflows == 62500)	// 16.063us*62500=1.004s
	{
		timerOverflows = 0; //reset timer
		PORTB |=(1<<PB5); //debug
		
		speedSensor->updateRevolutionCount(revolutionsForCalc);
		revolutionsForCalc = 0; //Resetting after getting value for KHM calculation
		timerCount=0; //Resetting before getting value.
		checkpointCnt++;
		if (previousState == currentState)
		{
			updateGUI_sm();
		}
	}
	
	if (checkpointCnt>60) //Save milestone to EEPROM every minute
	{
		checkpointCnt = 0;
		speedSensor->saveMilestoneCount();
	}

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
			screen->printString("Time");
			
			screen->setCursor(210,20);
			screen->printString("km/t");
			
			screen->setCursor(50,100);
			screen->printString("Temp.");
			
			screen->setCursor(100,175);
			screen->printString("Km count");
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

static void updateGUI_sm(void)
{
	switch(currentState)
	{
		uint8_t temp,none,hour,min,sec,year,month,day;
		case ALL:
		// DATE
		screen->setCursor(35,50);
		//myClock->getDateTime(&year,&month,&day,&hour,&min,&sec);
		screen->printInteger(hour);
		screen->printString(":");
		screen->printInteger(min);
		screen->printString(":");
		screen->printInteger(sec);
		// SPEED
		screen->setCursor(210,50);
		screen->printFloat(speedSensor->getSpeedInKmh());
		
		//TEMP
		screen->setCursor(60,130);
		tempSensor->getTempHumid(&temp,&none);
		screen->printInteger(temp);
		
		//STEEPNESS
		screen->setCursor(210,110);
		screen->printInteger(day);
		screen->printString("-");
		screen->printInteger(month);
		screen->printString("-");
		screen->printInteger(year);
		
		//MILESTONES
		screen->setCursor(130,205);
		screen->printFloat(speedSensor->getTripDistance());
		break;
		
		case MILESTONE:
		screen->setCursor(80,140);
		screen->printFloat(speedSensor->getTripDistance());
		break;
		
		case TEMPERATURE:
		screen->setCursor(80,140);
		tempSensor->getTempHumid(&temp,&none);
		screen->printInteger(temp);
		break;
		
		case VELOCITY:
		screen->setCursor(80,140);
		screen->printFloat(speedSensor->getSpeedInKmh());
		break;
		
		case STEEPNESS:
		screen->setCursor(80,120);
		int16_t pitchroll[2];
		//myGyro->gatherData();
		//myGyro->getPitchRoll(pitchroll);
		screen->printInteger(pitchroll[0]);
		screen->printString(",");
		screen->printInteger(pitchroll[1]);
		break;
		
		
		default:
		currentState = TEMPERATURE;
		break;
	}
}