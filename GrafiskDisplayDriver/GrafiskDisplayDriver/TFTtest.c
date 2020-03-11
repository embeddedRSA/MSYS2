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
#include "stdbool.h"

int main(void)
{
  // Initialize the display
  DisplayInit();
  // All pixels white (background)
  FillRectangle(0,239,320,240,30,60,30);
  // Draw red parts of danish flag
  /*
  FillRectangle(0,140,100,100,31,0,0);
  FillRectangle(0,0,100,100,31,0,0);
  FillRectangle(140,0,320-140,100,31,0,0);
  FillRectangle(140,140,320-140,100,31,0,0);      
  
  FillRectangle(0,239,320,240,0,0,0);
	*/
  DisplayOn();
   while(1)
  {  
	  FillRectangle(0,239,320,240,30,60,30);
	  _delay_ms(1000);
	  FillRectangle(0,239,320,240,0,0,0); 		
  } 
}
