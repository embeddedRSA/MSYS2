/*
 * HAL_Definitions.h
 *
 * Created: 05-04-2020 02:24:28
 *  Author: aaron
 */ 


#ifndef HAL_DEFINITIONS_H_
#define HAL_DEFINITIONS_H_

#include <avr/io.h>
#define F_CPU 16000000

/* LCD SCREEN ILI9341*/
/*******************************************/

// Data port definitions:
#define DATA_PORT_HIGH	PORTA
#define DATA_PORT_LOW	PORTC

// Control port definitions:
#define WR_PORT		PORTG
#define WR_BIT		2
#define DC_PORT		PORTD
#define DC_BIT		7  //"DC" signal is at the shield called RS
#define CS_PORT		PORTG
#define CS_BIT		1
#define RST_PORT	PORTG
#define RST_BIT		0

/* TOUCH SCREEN XPT*/
/*******************************************/

#define TOUCH_CLK_PORT	PORTH
#define TOUCH_CLK_BIT	3
#define TOUCH_CS_PORT	PORTE
#define TOUCH_CS_BIT	3
#define TOUCH_DIN_PORT	PORTG
#define TOUCH_DIN_BIT	5
#define TOUCH_DOUT_PORT PORTE
#define TOUCH_DOUT_PIN	PINE
#define TOUCH_DOUT_BIT	5
#define TOUCH_IRQ_PORT	PORTE
#define TOUCH_IRQ_BIT	4


#endif /* HAL_DEFINITIONS_H_ */