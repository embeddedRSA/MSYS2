/*
 * lcdDriverc.c
 *
 * Created: 05-04-2020 01:52:52
 *  Author: aaron
 */ 

#include "lcdDriver.h"
#include "HAL_Definitions.h"
#include "rgb565.h"
#include <util/delay.h>
#include <string.h>

typedef struct
{
	rgb_t color;
	uint16_t x_position;
	uint16_t y_position;
}cursor_t;

static cursor_t myCursor;


static lcdDriverInterface_t myInterface;
static bool initialized;

static void driverInit();
static void displayOff();
static void displayOn();
static void setCursorColor(rgb_t* color);
static void setCursor(uint16_t x_pos, uint16_t y_pos);
static void sleepOut();
static void memoryAccessControl(uint8_t parameter);
static void interfacePixelFormat(uint8_t parameter);

static void memoryWrite();
static void writePixel(rgb_t *color);
static void setColumnAddress(uint16_t Start, uint16_t End);
static void setPageAddress(uint16_t Start, uint16_t End);
static void drawRectangle(uint16_t Width, uint16_t Height);

lcdDriverInterface_t* lcdDriver_getDriver()
{
	if(!initialized)
	{
		driverInit();
		myInterface.displayON = displayOn;
		myInterface.displayOFF = displayOff;
		myInterface.setCursorColor = setCursorColor;
		myInterface.setCursor = setCursor;
		myInterface.drawRectangle = drawRectangle;
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

static void writePixel(rgb_t *color)
{
	if(rgb565IsColorValid(color))
	{
		writeData(rgb565ToInt(color));
	}
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

static void setCursor(uint16_t x_pos, uint16_t y_pos)
{
	myCursor.x_position = x_pos;
	myCursor.y_position = y_pos;
}

static void setCursorColor(rgb_t* color)
{
	myCursor.color = *color;
}


static void drawRectangle(uint16_t Width, uint16_t Height)
{
	uint32_t i = 0;
	setPageAddress(myCursor.x_position,(myCursor.x_position+Width));
	setColumnAddress(myCursor.y_position,(myCursor.y_position+Height));
	memoryWrite();
	
	for(i = 0; i<((uint32_t)Width*(uint32_t)Height); i++)
	{
		writePixel(&myCursor.color);
	}
	writeCommand(0);
}