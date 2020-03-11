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
#include "stdbool.h"
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

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}rgbData_t;

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

// ILI 9341 data sheet, page 238
static void WriteCommand(unsigned int command)
{
	uint8_t bit = 0b00000001;
	
	//Set command
	DATA_PORT_HIGH = (uint8_t)(command>>8);
	DATA_PORT_LOW = (uint8_t)(command&(0x00FF));
	
	DC_PORT &= ~(bit<<DC_BIT); //Trigger falling edge, no waiting because other controller is faster than us.
	CS_PORT &= ~(bit<<CS_BIT);
	
	//Write pulse
	WR_PORT &= ~(bit<<WR_BIT); //WRX low
	WR_PORT |= bit<<WR_BIT; //WRX high triggers read signal
	
	CS_PORT |= bit<<CS_BIT; //CS set high
	WR_PORT &= ~(bit<<WR_BIT); //WRX low
	
	//clean up??
}


// ILI 9341 data sheet, page 238
static void WriteData(unsigned int data)
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
	
	//clean up??
}

// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////

// Initializes (resets) the display
void DisplayInit()
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
	
	// Exit sleep mode
	SleepOut();
	// Display on
	DisplayOn();
	// Set bit BGR (scanning direction)
	MemoryAccessControl(0b00001000);
	// 16 bits (2 bytes) per pixel
	InterfacePixelFormat(0b00000101);
	
}

void DisplayOff()
{
	uint16_t command = 0b00101000;
	WriteCommand(command);
}

void DisplayOn()
{
	uint16_t command = 0b00101001;
	WriteCommand(command);
}

void SleepOut()
{
	uint16_t command = 0b00010001;
	WriteCommand(command);
	
}

void MemoryAccessControl(unsigned char parameter)
{
	uint16_t command = 0b00110110;
	WriteCommand(command);
	WriteData(parameter);
}

void InterfacePixelFormat(unsigned char parameter)
{
	uint16_t command = 0b00111010;
	WriteCommand(command);
	WriteData(parameter);
	
}

void MemoryWrite()
{
	uint16_t command = 0b00101100;
	WriteCommand(command);
}

static bool isDataOk(rgbData_t* data)
{
	return ((data->red < 32) && (data->blue < 32) && (data->green < 64));
}

static uint16_t rgbDataToInt(rgbData_t* data)
{
	uint16_t dataInt = 0;
	dataInt |= (uint16_t)data->blue;
	dataInt |= ((uint16_t)data->green) << 5;
	dataInt |= ((uint16_t)data->red) << 11;
	
	return dataInt;
}

// Red 0-31, Green 0-63, Blue 0-31 // side 21 Lesson slides // side 78 datasheet
void WritePixel(unsigned char Red, unsigned char Green, unsigned char Blue)
{
	rgbData_t data;
	data.red = Red;
	data.green = Green;
	data.blue = Blue;
	
	if(isDataOk(&data))
	{
		WriteData(rgbDataToInt(&data));
	}
}

// Set Column Address (0-239), Start > End
void SetColumnAddress(unsigned int Start, unsigned int End)
{
	uint16_t command = 0b00101010;
	WriteCommand(command);
	WriteData((Start>>8));
	WriteData((Start));
	WriteData((End>>8));
	WriteData((End));
}

// Set Page Address (0-319), Start > End
void SetPageAddress(unsigned int Start, unsigned int End)
{
	uint16_t command = 0b00101011;
	WriteCommand(command);
	WriteData((Start>>8));
	WriteData((Start));
	WriteData((End>>8));
	WriteData((End));
}

// Fills rectangle with specified color
// (StartX,StartY) = Upper left corner. X horizontal (0-319) , Y vertical (0-239).
// Height (1-240) is vertical. Width (1-320) is horizontal.
// R-G-B = 5-6-5 bits.
void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	uint32_t i = 0;
	SetPageAddress(StartX,(StartX+Width));
	SetColumnAddress(StartY,(StartY+Height));
	MemoryWrite();
	
	for(i = 0; i<(Width*Height); i++)
	{
		WritePixel(Red,Green,Blue);
	}
}

void setBrightness(uint16_t parameter)
{
	if (parameter<=0xFF)
	{
		uint16_t command = 0b01010001;
		WriteCommand(command);
		WriteData(parameter);
	}
	else
	{
		//de nada.
	}
}

void enableBacklightControl(bool is_on)
{
	uint16_t command = 0b01010011;
	uint16_t parameter = 0;
	if (is_on)
	{
		parameter = 0b00100100;
	}
	else
	{
		parameter = 0b00000100;
	}
	WriteCommand(command);
	WriteData(parameter);

}

void enableCABC(bool enable)
{
	uint16_t command = 0b01010101;
	uint16_t parameter = 0;
	if (enable)
	{
		parameter = 0b00000010;
	}
	else
	{
		parameter = 0b00000000;
	}
	WriteCommand(command);
	WriteData(parameter);
}
