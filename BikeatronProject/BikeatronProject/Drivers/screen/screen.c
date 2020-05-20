/*
 * screen.c
 *
 * Created: 05-04-2020 01:52:52
 *  Author: aaron
 */ 

#include "screen.h"
#include "letters.h"
#include "rgb565.h"
#include "../../Utils/HAL_Definitions.h"
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
	uint16_t color;
	uint16_t x_position;
	uint16_t y_position;
}cursor_t;

static cursor_t myCursor;
static uint16_t backgroundColor = WHITE;

static lcdDriverInterface_t myInterface;
static bool initialized;

static void driverInit();
static void displayOff();
static void displayOn();
static void setBackgroundColor(uint16_t color);
static void setCursorColor(uint16_t color);
static void setCursor(uint16_t x_pos, uint16_t y_pos);
static void sleepOut();
static void memoryAccessControl(uint8_t parameter);
static void interfacePixelFormat(uint8_t parameter);

static void memoryWrite();
static void setColumnAddress(uint16_t Start, uint16_t End);
static void setPageAddress(uint16_t Start, uint16_t End);
static void drawRectangle(uint16_t Width, uint16_t Height);

static void printCharById(uint8_t char_id);
static void printString(char str[]);
static void printInteger(int16_t num);
static void printFloat(float num);

lcdDriverInterface_t* lcdDriver_getDriver()
{
	if(!initialized)
	{
		driverInit();
		myInterface.displayON = displayOn;
		myInterface.displayOFF = displayOff;
		myInterface.setBackgroundColor = setBackgroundColor;
		myInterface.setCursorColor = setCursorColor;
		myInterface.setCursor = setCursor;
		myInterface.drawRectangle = drawRectangle;
		myInterface.printString = printString;
		myInterface.printInteger = printInteger;
		myInterface.printFloat = printFloat;
		
		backgroundColor = WHITE;
		initialized = true;
	}
	return &myInterface;
}

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

// ILI 9341 data sheet, page 238
static void writeCommand(uint8_t command)
{
	uint8_t bit = 0b00000001;
	
	DATA_PORT_LOW =command;
	
	DC_PORT &= ~(bit<<DC_BIT); //Trigger falling edge, no waiting because other controller is faster than us.
	CS_PORT &= ~(bit<<CS_BIT);
	
	//Write pulse
	WR_PORT &= ~(bit<<WR_BIT); //WRX low
	WR_PORT |= bit<<WR_BIT; //WRX high triggers read signal
	
}


// ILI 9341 data sheet, page 238
static void writeData(uint16_t data)
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
}

static void driverInit()
{
	uint8_t bit = 0b00000001;
	
	//Set control bits to output
	DDRG |= (bit<<CS_BIT)|(bit<<RST_BIT)|(bit<<WR_BIT);
	DDRD |= (bit<<DC_BIT);
	
	//Set data ports to output
	DDRA |= 0xFF;
	DDRC |= 0xFF;
	
	// set control pins start position high
	DC_PORT |= bit<<DC_BIT;
	WR_PORT |= bit<<WR_BIT;
	CS_PORT	|= bit<<CS_BIT;
	RST_PORT|= bit<<RST_BIT;
	
	//Reset grafik display
	RST_PORT &= ~(bit<<RST_BIT);
	_delay_ms(500);
	RST_PORT|= bit<<RST_BIT;
	_delay_ms(130);
	
	sleepOut();
	// Display on
	displayOn();
	// Set bit BGR (scanning direction)
	memoryAccessControl(0b00001000);
	// 16 bits (2 bytes) per pixel
	interfacePixelFormat(0b00000101);
	
	memset(&myCursor,0,sizeof(myCursor));
}

static void displayOff()
{
	uint16_t command = 0b00101000;
	writeCommand(command);
}

static void displayOn()
{
	uint16_t command = 0b00101001;
	writeCommand(command);
}

static void sleepOut()
{
	uint16_t command = 0b00010001;
	writeCommand(command);
	
}

static void memoryAccessControl(uint8_t parameter)
{
	uint16_t command = 0b00110110;
	writeCommand(command);
	writeData((uint16_t)parameter);
}

static void interfacePixelFormat(uint8_t parameter)
{
	uint16_t command = 0b00111010;
	writeCommand(command);
	writeData((uint16_t)parameter);
}

static void memoryWrite()
{
	uint16_t command = 0b00101100;
	writeCommand(command);
}

static void setColumnAddress(uint16_t Start, uint16_t End)
{
	uint16_t command = 0b00101010;
	writeCommand(command);
	writeData((Start>>8));
	writeData((Start));
	writeData((End>>8));
	writeData((End));
}

// Set Page Address (0-319), Start > End
static void setPageAddress(uint16_t Start, uint16_t End)
{
	uint16_t command = 0b00101011;
	writeCommand(command);
	writeData((Start>>8));
	writeData((Start));
	writeData((End>>8));
	writeData((End));
}

static void setBackgroundColor(uint16_t color)
{
	backgroundColor = color;
}

static void setCursorColor(uint16_t color)
{
	myCursor.color = color;
}

static void setCursor(uint16_t x_pos, uint16_t y_pos)
{
	myCursor.x_position = x_pos;
	myCursor.y_position = y_pos;
}

static void drawRectangle(uint16_t Width, uint16_t Height)
{
	uint32_t i = 0;
	uint32_t size = (uint32_t)Width*(uint32_t)Height;
	setPageAddress(myCursor.x_position,(myCursor.x_position+Width));
	setColumnAddress(myCursor.y_position,(myCursor.y_position+Height-1));
	memoryWrite();
	
	for(i = 0; i<size; i++)
	{
		writeData(myCursor.color);
	}
	writeCommand(0);
}

static void printString(char str[])
{
	uint8_t i = 0;
	for(i = 0; i<strlen(str); i++)
	{
		printCharById(((uint8_t)str[i])-32);
		if(myCursor.x_position < 280)
		{
			myCursor.x_position += 13;
		}
		else if (myCursor.y_position < 210)
		{
			myCursor.y_position +=13;
			myCursor.x_position = 10;
		}
	}
}

static void printInteger(int16_t num)
{
	char buf[10];
	itoa(num,buf,10);
	printString(buf);
}

static void printFloat(float num)
{
	uint16_t roundnum = num;
	printInteger(roundnum);
	printString(".");
	uint16_t dec = (num*100)-(roundnum*100);
	if (dec<10)
	{
		printInteger(0);
	}
	printInteger(dec);
}



static void printCharById(uint8_t char_id)
{
	setPageAddress(myCursor.x_position,(myCursor.x_position+8));
	setColumnAddress(myCursor.y_position,(myCursor.y_position+12));
	memoryWrite();

	uint8_t bit = 0b10000000;
	uint8_t i = 0;
	uint8_t j = 0;
	for(i = 0; i<8; i++)
	{
		for (j = 0; j<13; j++)
		{
			if(pgm_read_byte(&letters[char_id][12-j])&(bit>>i))
			{
				writeData(myCursor.color);
			}
			else
			{
				writeData(backgroundColor);
			}
		}
		j = 0;
	}
	writeCommand(0);
}