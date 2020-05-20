/*
 * rtc.c
 *
 * Created: 12-04-2020 10:46:21
 *  Author: Anders
 */

#include "rtc.h"
#include "../i2c/i2c.h"

static uint8_t init=0;

//For i2c "object"
static i2c_t* i2c;

//"Object for RTC"
static RTC_t RTCinterface;

//Static functions so only this .c file can see them
static void ds1307_init(i2c_t* p_i2c);
static uint8_t ds1307_dec2bcd(uint8_t val);
static uint8_t ds1307_bcd2dec(uint8_t val);
static void ds1307_setDateAndTime(uint8_t year, uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour, uint8_t minute, uint8_t second);
static void ds1307_getDateAndTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

//Constructor
RTC_t *get_RTC_interface(void)
{
	if (!init)
	{
		RTCinterface.initRTC=ds1307_init;
		RTCinterface.setDateTime=ds1307_setDateAndTime;
		RTCinterface.getDateTime=ds1307_getDateAndTime;
	}
	return &RTCinterface;
};


//Initialize the i2c communication for the RTC
static void ds1307_init(i2c_t* p_i2c)
{
	i2c=p_i2c;
	//init i2c communication
	i2c->init(10000,0); 
}

//Decimal uint8_t to BCD
static uint8_t ds1307_dec2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}

//BCD to decimal uint8_t
static uint8_t ds1307_bcd2dec(uint8_t val)
{
	return val - 6 * (val >> 4);
}

/**
-------------function description-----------------------------------------------------------
static void ds1307_setDateAndTime(uint8_t year, uint8_t month, uint8_t day, uint8_t weekDay, 
uint8_t hour, uint8_t minute, uint8_t second)
	To set the time and date for the RTC.  
	RET: Void
----------------description-----------------------------------------------------------------
To set the time and date for the DS1307 RTC in its timekeeper registers, 
done via the i2c driver. 
-------------function description end-------------------------------------------------------
**/
static void ds1307_setDateAndTime(uint8_t year, uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour, uint8_t minute, uint8_t second)
{
	//First check if any input is over limit
	if (second < 0 || second > 59 ||
	minute < 0 || minute > 59 ||
	hour < 0 || hour > 23 ||
	day < 1 || day > 31 ||
	weekDay < 1 || weekDay > 7 ||
	month < 1 || month > 12 ||
	year < 0 || year > 99)
	{
		year=0;
		month=0;
		day=0;
		weekDay=0;
		hour=0;
		minute=0;
		second=0;
	}
	else
	{
		//Write date and time
		i2c->start();		
		i2c->selectmode((DS1307_ADDR),I2C_WRITE_MODE); //Write select
		i2c->write(0x00); //stop oscillator		
		i2c->write(ds1307_dec2bcd(second));	//Write the data
		i2c->write(ds1307_dec2bcd(minute));
		i2c->write(ds1307_dec2bcd(hour));
		i2c->write(ds1307_dec2bcd(weekDay));
		i2c->write(ds1307_dec2bcd(day));
		i2c->write(ds1307_dec2bcd(month));
		i2c->write(ds1307_dec2bcd(year));
		i2c->write(0x00); //start oscillator
		i2c->stop();
	}
	
}

/**
-------------function description-----------------------------------------------------------
static void ds1307_getDateAndTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, 
uint8_t *minute, uint8_t *second)
	To get the latest time and date form the RTC.  
	RET: Void
----------------description-----------------------------------------------------------------
To get the latest time and date form the DS1307 RTC in its timekeeper registers, 
done via the i2c driver. Weekday "day" is not used in the function.  
-------------function description end-------------------------------------------------------
**/
static void ds1307_getDateAndTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second)
{
	i2c->start();
	i2c->selectmode((DS1307_ADDR),I2C_WRITE_MODE); //Write select
	i2c->write(0x00); //stop oscillator
	i2c->stop();

	i2c->start(); //start again before reading 
	i2c->selectmode((DS1307_ADDR),I2C_READ_MODE); //I2C read
	*second = ds1307_bcd2dec(i2c->read(false) & 0x7F);
	*minute = ds1307_bcd2dec(i2c->read(false));
	*hour = ds1307_bcd2dec(i2c->read(false));
	i2c->read(false); //We dont need to read the weekday
	*day = ds1307_bcd2dec(i2c->read(false));
	*month = ds1307_bcd2dec(i2c->read(false));
	*year = ds1307_bcd2dec(i2c->read(true));
	i2c->stop();
}