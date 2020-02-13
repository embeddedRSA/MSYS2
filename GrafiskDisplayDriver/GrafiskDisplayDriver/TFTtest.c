/*
 * TFTtest.c
 * Test program for the TFTdriver
 *
 * Created: 15-02-2018 
 * Author : Henning Hargaard
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"

int main(void)
{
  // Initialize the display
  DisplayInit();
  // All pixels white (background)
  FillRectangle(0,0,320,240,31,63,31);
  // Draw red parts of danish flag
  FillRectangle(0,140,100,100,31,0,0);
  FillRectangle(0,0,100,100,31,0,0);
  FillRectangle(140,0,320-140,100,31,0,0);
  FillRectangle(140,140,320-140,100,31,0,0);      
  while(1)
  {
    DisplayOn();
    _delay_ms(1000);
    DisplayOff();
    _delay_ms(1000);		
  } 
}
