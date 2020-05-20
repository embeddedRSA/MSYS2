/*
 * touch.h
 *
 * Created: 23-04-2020 11:00:30
 *  Author: aaron
 */ 


#ifndef TOUCH_H_
#define TOUCH_H_

#include "../../Utils/functionPointers.h"

typedef struct
{
	uint16_void_f_t readTouchX;
	uint16_void_f_t readTouchY;
	uint16_void_f_t readPressure;
	
}touchDriverInterface_t;

touchDriverInterface_t* touchDriver_getDriver();



#endif /* TOUCH_H_ */