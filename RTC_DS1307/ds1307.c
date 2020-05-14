#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "ds1307.h"
#include "i2c_driver.h"


static i2c_t* i2c;

//Initialize the RTC
void ds1307_init(i2c_t* p_i2c)
{
	i2c=p_i2c;
	//init i2c communication
	i2c->init(10000,0); 

}

//Decimal uint8_t to BCD
uint8_t ds1307_dec2bcd(uint8_t val)
{
	return val + 6 * (val / 10);
}

//BCD to decimal uint8_t
static uint8_t ds1307_bcd2dec(uint8_t val)
{
	return val - 6 * (val >> 4);
}


//Set the date and time
void ds1307_setDateAndTime(uint8_t year, uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour, uint8_t minute, uint8_t second)
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


void ds1307_getDateAndTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second)
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