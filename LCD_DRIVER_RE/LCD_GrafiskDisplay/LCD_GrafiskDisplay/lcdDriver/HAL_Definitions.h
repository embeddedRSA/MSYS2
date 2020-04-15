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

// Data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW  PORTC

// Control port definitions:
#define WR_PORT PORTG
#define WR_BIT 2
#define DC_PORT PORTD
#define DC_BIT  7  //"DC" signal is at the shield called RS
#define CS_PORT PORTG
#define CS_BIT  1
#define RST_PORT PORTG
#define RST_BIT 0

#endif /* HAL_DEFINITIONS_H_ */