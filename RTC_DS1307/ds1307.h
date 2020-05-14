
#ifndef DS1307_H
#define DS1307_H

#include "i2c_driver.h"

//definitions
#define DS1307_ADDR 0x68 //device address


//functions
extern void ds1307_init(i2c_t* p_i2c);
extern void ds1307_setDateAndTime(uint8_t year, uint8_t month, uint8_t day, uint8_t weekDay, uint8_t hour, uint8_t minute, uint8_t second);
extern void ds1307_getDateAndTime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

void getRTCInterface(i2c_t* p_i2c);  

#endif
