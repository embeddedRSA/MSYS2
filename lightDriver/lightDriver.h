/*
 * lightDriver.h
 *
 * Created: 29-04-2020 08:49:22
 *  Author: Anders
 */ 


#ifndef LIGHTDRIVER_H_
#define LIGHTDRIVER_H_

// void initLightDriver(void); //initializes the timers.
// void setFrontlight(unsigned char percent); //Takes char percent 0-100, outputs PWM on port PH4 
// void setBacklight(unsigned char percent); //Takes char percent 0-100, outputs PWM on port PH5 

typedef void (*void_RET_void_PAR) (void);
typedef void (*void_RET_unsigned_char) (unsigned char);

typedef struct
{
	void_RET_void_PAR init;
	void_RET_unsigned_char setFront;
	void_RET_unsigned_char setBack;
	
}lightDriver_t;

lightDriver_t *get_lightDriverInterface(void);

#endif /* LIGHTDRIVER_H_ */