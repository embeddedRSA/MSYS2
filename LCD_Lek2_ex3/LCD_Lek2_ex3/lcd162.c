/*-------------------------------------------------------------------
  File name: "lcd162.c"

  Driver for "LCD Keypad Shield" alphanumeric display.
  Display controller = HD44780U (LCD-II).

  Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

  Connection : PORTx (4 bit mode) :
  [LCD]        [Portx]
  RS   ------  PH 5
  RW   ------  GND
  E    ------  PH 6
  DB4  ------  PG 5
  DB5  ------  PE 3
  DB6  ------  PH 3
  DB7  ------  PH 4

  Henning Hargaard, February 5, 2018
---------------------------------------------------------------------*/
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
// Enabling us to use macro _NOP() to insert the NOP instruction
#include <avr/cpufunc.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "lcd162.h"

// library function itoa() is needed
#include <stdlib.h>

static void setRs(bool state)
{
	if(state)
	{
		PORTH |= 0b00100000;
	}
	else
	{
		PORTH &= 0b11011111;
	}
}


//*********************** PRIVATE (static) operations *********************
static void waitBusy()
{
  _delay_ms(2);
}

static void delayTicks(uint8_t ticks)
{
	uint8_t i = 0;
	for(i=0;i<ticks;i++)
	{
		_NOP(); //one tick is 62.5ns
	}
}

static void pulse_E()
{
  PORTH |= 0b01000000; //SET E HIGH
  delayTicks(4); //PW_EH
  PORTH &= 0b10111111; //ET E LOW
  delayTicks(1); //try 2 also
}

// Sets the display data pins according to the 4 lower bits of data
static void set4DataPins(unsigned char data)
{
  PORTH = (PORTH & 0b11100111) | ((data<<1) & 0b00011000);
  PORTE = (PORTE & 0b11110111) | ((data<<2) & 0b00001000);
  PORTG = (PORTG & 0b11011111) | ((data<<5) & 0b00100000);
}
// Following timing diagram page 22
static void sendInstruction(unsigned char data)
{
	setRs(false);
	delayTicks(1); // wait t_AS wait for controller ready to receive data
	set4DataPins(data >> 4); // MSB bits ready
	pulse_E();
	set4DataPins(data);
	pulse_E();
	setRs(true);
	waitBusy();
}

void testIns(unsigned char data)
{
	sendInstruction(data);
}

static void sendData(unsigned char data)
{
  	setRs(true);
  	delayTicks(1); // wait t_AS wait for controller ready to receive data
  	set4DataPins(data >> 4); // MSB bits ready
  	pulse_E(); // Signal controller to read pin data
  	set4DataPins(data);
  	pulse_E();
  	setRs(false);
	  waitBusy();
}

//*********************** PUBLIC functions *****************************

// Initializes the display, blanks it and sets "current display position"
// at the upper line, leftmost character (cursor invisible)
// Reference: Page 46 in the HD44780 data sheet
void LCDInit()
{
  // Initializing the used port
  DDRH |= 0b01111000;  // Outputs
  DDRE |= 0b00001000;
  DDRG |= 0b00100000;

  // Wait 50 ms (min. 15 ms demanded according to the data sheet)
  _delay_ms(50);
  // Function set (still 8 bit interface)
  PORTG |= 0b00100000;
  PORTE |= 0b00001000;
  pulse_E();

  // Wait 10 ms (min. 4,1 ms demanded according to the data sheet)
  _delay_ms(10);
  // Function set (still 8 bit interface)
  pulse_E();

  // Wait 10 ms (min. 100 us demanded according to the data sheet)
  _delay_ms(10);
  // Function set (still 8 bit interface)
  pulse_E();

  // Wait 10 ms (min. 100 us demanded according to the data sheet)
  _delay_ms(10);
  // Function set (now selecting 4 bit interface !)
  PORTG &= 0b11011111;
  pulse_E();

  // Function Set : 4 bit interface, 2 line display, 5x8 dots
  sendInstruction( 0b00101000 );
  // Display, cursor and blinking OFF
  sendInstruction( 0b00001000 );
  // Clear display and set DDRAM adr = 0
  sendInstruction( 0b00000001 );
  // By display writes : Increment cursor / no shift
  sendInstruction( 0b00000110 );
  // Display ON, cursor and blinking OFF
  sendInstruction( 0b00001100 );
}

// Blanks the display and sets "current display position" to
// the upper line, leftmost character
void LCDClear()
{
  sendInstruction( 0b00000001 );
}

// Sets DDRAM address to character position x and line number y
void LCDGotoXY( unsigned char x, unsigned char y )
{
	unsigned char instruc = 0b10000000;
	uint8_t ddramMap[2][16] = {{0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF},{0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F}};

	if((x < 16)&&(y<2))
	{
		instruc |= ddramMap[y][x];
		sendInstruction( instruc );
	}
}

// Display "ch" at "current display position"
void LCDDispChar(char ch)
{
	sendData(ch);
}

// Displays the string "str" starting at "current display position"
void LCDDispString(char* str)
{
	uint8_t i = 0;
	for(i=0;i<strlen(str);i++)
	{
		LCDDispChar(str[i]);
	}
}

// Displays the value of integer "i" at "current display position"
void LCDDispInteger(int i)
{
	char buff[10] = {0};
		
	itoa(i,buff,10);
	LCDDispString(buff);
}

// Loads one of the 8 user definable characters (UDC) with a dot-pattern,
// pre-defined in an 8 byte array in FLASH memory
void LCDLoadUDC(unsigned char UDCNo, const unsigned char *UDCTab)
{
	uint8_t i = 0;
	unsigned char instruc = 0b01000000; //set instruction frame
	instruc |= (UDCNo<<3); //add CGRAM global addr
	
	for (i=0; i<8; i++)		//8 rows
	{
		sendInstruction((instruc|i)); //set local addr
		sendData(UDCTab[i]); //send indexed row
	}
}

// Selects, if the cursor has to be visible, and if the character at
// the cursor position has to blink.
// "cursor" not 0 => visible cursor.
// "blink" not 0 => the character at the cursor position blinks.
void LCDOnOffControl(unsigned char cursor, unsigned char blink)
{
  // To be implemented
}

// Moves the cursor to the left
void LCDCursorLeft()
{
  // To be implemented
}

// Moves the cursor to the right
void LCDCursorRight()
{
  // To be implemented
}

// Moves the display text one position to the left
void LCDShiftLeft()
{
  // To be implemented
}

// Moves the display text one position to the right
void LCDShiftRight()
{
  // To be implemented
}

// Sets the backlight intensity to "percent" (0-100)
void setBacklight(unsigned char percent)
{
  // To be implemented
}

// Reads the status for the 5 on board keys
// Returns 0, if no key pressed
unsigned char readKeys()
{
	return 0;
  // To be implemented
}
