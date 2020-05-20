/*
 * temp.c
 *
 * Created: 13-05-2020 09:39:30
 *  Author: Anders
 */ 

#include "temp.h"

static uint8_t init=0;

//"Object"
static DHT_t DHTinterface;

//Static functions so only this .c file can see them
static void getDataDHT(uint8_t* temp, uint8_t* humid);

//Constructor
DHT_t *getDHTInterface(void)
{
	if (!init)
	{
		DHTinterface.getTempHumid=getDataDHT;
	}
	return &DHTinterface;
};


/**
-------------function description-----------------------------------------------------------
static void getDataDHT(uint8_t *temp, uint8_t *humid)
	Data from both Temperature and humidity integer part only. 
	RET: Void
----------------description-----------------------------------------------------------------
This function is used to get the temperature and humidity as unsigned 8 bit integers 
since the temperature reading of the DHT11 cant go below 0 C. 
array in the function contains:
Index 0 of array is integer part of humidity
Index 1 of array is decimal part of humidity 
Index 2 of array is integer part of temperature
Index 3 of array is decimal part of temperature
Index 4 of array is the check sum  
-------------function description end-------------------------------------------------------
**/
static void getDataDHT(uint8_t* temp, uint8_t* humid)
{

	uint8_t dataRecived, j;
	int8_t i;
	uint8_t data[5]; //To store all received data
	uint8_t array[5]; //The save and checked data after CRC

	
	//Request data, the microcontroller sends start pulse
	DDRB |=(1<<DHT_PIN); //Output
	PORTB &=~(1<<DHT_PIN); //Pull low
	_delay_ms(20); //wait as stated in datasheet 
	PORTB |=(1<<DHT_PIN); //Pull high
	
	//Get response form the DHT11
	DDRB &=~(1<<DHT_PIN); //Input
	while(PINB &(1<<DHT_PIN));
	while((PINB &(1<<DHT_PIN))==0);
	while(PINB &(1<<DHT_PIN));
	
	
	for (j=0; j<5; j++)
	{
	
		//Getting data for each 8 bit data received 
		for (i=0; i<8; i++)
		{
		
			while ((PINB & (1<<DHT_PIN))==0); //Wait and check received bit
		
			_delay_us(30); //Wait 30 us as stated in datasheet 
			if (PINB & (1<<DHT_PIN)) //If the signal is longer than 30us then its HIGH else its logic low
			{
				dataRecived=(dataRecived<<1)|(0x01);
			}
			else
			{
				dataRecived=(dataRecived<<1);
			}
		
			while(PINB &(1<<DHT_PIN));
		
		}
		
		data[j]=dataRecived; //getting the data and indexing it in array
		
	}
	
	//CRC checkup 
	if ((data[0]+data[1]+data[2]+data[3]) !=data[4])
	{
		//If the CRC is wrong dont update values
	}
	else //Pass data into array 
	{
		for (i=0; i < 4; i++)
		{
			array[i]=data[i]; //transfer data
		}
			*temp=array[2];	//gets integer part of temperature byte 2 in array
			*humid=array[0]; //gets integer part of humidity byte 0 in array
	}
	
}