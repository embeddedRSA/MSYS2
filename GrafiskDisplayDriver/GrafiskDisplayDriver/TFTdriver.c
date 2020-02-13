/************************************************************
  File name: "TFTdriver.c"

  Driver for "ITDB02 320 x 240 TFT display module, Version 2"
  mounted at "ITDB02 Arduino Mega2560 Shield".
  Display controller = ILI 9341.
  
  Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

  Connections:
  DB15-DB8:   PORT A
  DB7-DB0:    PORT C

  RESETx:     PORT G, bit 0
  CSx:        PORT G, bit 1
  WRx:        PORT G, bit 2
  RS (=D/Cx): PORT D, bit 7

  Henning Hargaard, February 14, 2019
************************************************************/  
#include <avr/io.h> 
#include <avr/cpufunc.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "TFTdriver.h"

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

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

static void setBusPortsHigh(void)
{
	WR_PORT |= bit<<WR_BIT; //Set WR high, to be able to trigger falling edge
	DC_PORT |= bit<<DC_BIT; //Set RS high, to be able to trigger falling edge
	CS_PORT |= bit<<CS_BIT; //Set CS high, to be able to trigger falling edge
}

// ILI 9341 data sheet, page 238
void WriteCommand(unsigned int command)
{
	uint8_t bit = 0b00000001;
	
	//Set command
	DATA_PORT_HIGH = (uint8_t)(command>>8);
	DATA_PORT_LOW = (uint8_t)(command&(0x00FF));
	
	DC_PORT &= ~(bit<<DC_BIT); //Trigger falling edge, no waiting because other controller is faster than us.
	CS_PORT &= ~(bit<<CS_BIT);
	
	//Write pulse
	WR_PORT &= ~(bit<<WR_BIT);
	WR_PORT |= bit<<WR_BIT;
	
	CS_PORT |= bit<<CS_BIT; //CS set high
	WR_PORT &= ~(bit<<WR_BIT);
	WR_PORT |= bit<<WR_BIT; // clean up, wr back to high	
}


// ILI 9341 data sheet, page 238
void WriteData(unsigned int data)
{
	uint8_t bit = 0b00000001;
		
	//Set data
	DATA_PORT_HIGH = (uint8_t)(data>>8);
	DATA_PORT_LOW = (uint8_t)(data&(0x00FF));
		
	DC_PORT |= bit<<DC_BIT; //Trigger rising edge, no waiting because other controller is faster than us.
	CS_PORT &= ~(bit<<CS_BIT); // Chip select active low, trigger falling edge
		
	//Write pulse
	WR_PORT &= ~(bit<<WR_BIT);
	WR_PORT |= bit<<WR_BIT;
		
	CS_PORT |= bit<<CS_BIT; //CS set high
	WR_PORT &= ~(bit<<WR_BIT);
	WR_PORT |= bit<<WR_BIT; // clean up.
}

// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////

// Initializes (resets) the display
void DisplayInit()
{
}

void DisplayOff()
{
}

void DisplayOn()
{
}

void SleepOut()
{
}

void MemoryAccessControl(unsigned char parameter)
{
}

void InterfacePixelFormat(unsigned char parameter)
{
}

void MemoryWrite()
{
}

// Red 0-31, Green 0-63, Blue 0-31
void WritePixel(unsigned char Red, unsigned char Green, unsigned char Blue)
{
}

// Set Column Address (0-239), Start > End
void SetColumnAddress(unsigned int Start, unsigned int End)
{
}

// Set Page Address (0-319), Start > End
void SetPageAddress(unsigned int Start, unsigned int End)
{
}

// Fills rectangle with specified color
// (StartX,StartY) = Upper left corner. X horizontal (0-319) , Y vertical (0-239).
// Height (1-240) is vertical. Width (1-320) is horizontal.
// R-G-B = 5-6-5 bits.
void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue)
{
}