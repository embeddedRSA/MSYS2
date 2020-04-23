/*
 * i2c_driver.c
 *
 * Created: 26/03/2020 09:18:07
 * Author : ryana
 */ 
#include "D:\Onedrive AU\OneDrive - Aarhus universitet\6.s\msys2\MSYS2\i2c\i2c_driver\i2c_driver\i2c_driver.h"
#include <avr/io.h>
#include <util/delay.h>


static i2c_t i2c_interface;
static bool int_is_enabled=false;
static bool initialized=false;
static bool busy_flag=false;


static uint8_t i2c_init(uint16_t SCL_f ,bool enable_interrupt);

static void i2c_disable(void);

static void i2c_select_mode(uint8_t SLA_x);

static void i2c_start();

static void i2c_stop();

static void i2c_write(uint8_t data);

static uint8_t i2c_read (bool isLast);

static bool set_busy(bool status);

static bool get_busy(void);

/**
-------------function description-----------------------------------------------------------
i2c_t* get_i2c_interface(void)

	RET: pointer to object struct

----------------description-----------------------------------------------------------------
sets struct pointer functions, can be called several times to return pointer, or saved as a pointer in main
-------------function description end-------------------------------------------------------
**/
i2c_t* get_i2c_interface(void)
{
	
 if (!initialized)
 {
	i2c_interface.init				=	i2c_init;
	i2c_interface.disable			=	i2c_disable;
	i2c_interface.start				=	i2c_start;
	i2c_interface.stop				=	i2c_stop;
	i2c_interface.selectmode		=	i2c_select_mode;
	i2c_interface.write				=	i2c_write;
	i2c_interface.read				=	i2c_read;
	i2c_interface.set_busy_flag		=	set_busy;
	i2c_interface.get_busy_flag		=	get_busy;
 }
	return &i2c_interface;
}


/**
-------------function description-----------------------------------------------------------
static uint8_t i2c_init(uint16_t SCL_f ,bool enable_interrupt)

	SCL_f: frequency that should be approximated

	enable_interrupt : 'true' to enable interrupt

	RET: returns 0 if SCL_f impossible, else return TWCR

----------------description-----------------------------------------------------------------
Init function for i2C, will set frequency by use of prescaler x4 or x1,
this is also where polling is enabled by setting enable_interrupt = false.
is implemented 
-------------function description end-------------------------------------------------------
**/
static uint8_t i2c_init(uint16_t SCL_f ,bool enable_interrupt) //freq is given in Hz
{
	
	//no internal pull up implemented so far
if (SCL_f <= SCL_MAX)
	{
		//enable TWI clock module 
	PRR0&=~(1<<PRTWI);
	
	//enable TWI
	//TWCR|=(1<<TWEN);	
	
	//enable interrupt
	int_is_enabled=enable_interrupt;
	if (int_is_enabled)
	{
		TWCR|=(1<<TWIE);
	}
	
	//constants pre calculated from mathcad 
	uint16_t TWBR_1= (uint16_t)((F_CPU/(8*SCL_f))-2);

	uint16_t TWBR_4= (uint16_t)((F_CPU/(512*SCL_f))-(1/32));

	uint8_t validation = 0xFF;

	if(TWBR_1>0 && TWBR_1<=validation )
		{
		//set prescaler
	
		TWSR&=~(1<<TWPS0);
		TWSR&=~(1<<TWPS1);
		//typecast to proper format and insert
		TWBR=(uint8_t) TWBR_1;
		return  (uint8_t) TWBR_1;
		}
	else if(TWBR_4>0 && TWBR_4<=validation )
		{
		TWSR|=(1<<TWPS0);
		TWSR&=~(1<<TWPS1);
		TWBR=(uint8_t) TWBR_4;
		return  (uint8_t) TWBR_4;
		}
	else
		{
		return 0; //return 0 error
		}
	}
else
	{
	return 0;
	}
}

/**
-------------function description-----------------------------------------------------------
static void i2c_disable(void)

RET: void
----------------description-----------------------------------------------------------------
Disables any i2c activity, activity, 'i2c_init()' does not need to be followed before starting new transfer
-------------function description end-------------------------------------------------------
**/
static void i2c_disable(void)
{
		//TWCR|=(1<<TWEN);	
}



/**
-------------function description-----------------------------------------------------------
static void i2c_start(void)

RET: void
----------------description-----------------------------------------------------------------
Starts i2c, must be set before read or write (address and R/W are set with i2c_select_mode())
can be called several times without calling stop, a new mode may even be selected. 
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void i2c_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA)||(1<<TWEN); //TWINT clears FLAG, TWSTA sends start bit 

	if (!int_is_enabled)
	{
			while ((TWCR & (1<<TWINT)) == 0) //poll
			{}
	}
}

/**
-------------function description-----------------------------------------------------------
static void i2c_stop()

RET: void
----------------description-----------------------------------------------------------------
stops i2c, no polling needed
-------------function description end-------------------------------------------------------
**/
static void i2c_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

/**
-------------function description-----------------------------------------------------------
static uint8_t i2c_select_mode(uint8_t SLA_x)

SLA_x: selects SLA+mode 

RET: void
----------------description-----------------------------------------------------------------
is called after 'i2c_start()' selects address and R/W.
THe adress is  7 bit address (x) and mode is R/W (y), an input could be: 'xxxxxxxy'
W=0, R=1
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void i2c_select_mode(uint8_t SLA_x) //SLA_x is SLA+R/W
{
	//must send address now
	TWDR=SLA_x;
	TWCR = (1<<TWINT)|(1<<TWEN);
	if (!int_is_enabled)
	{
		while ((TWCR & (1<<TWINT)) == 0) //poll
		{}
	}
	
}

/**
-------------function description-----------------------------------------------------------
static void i2c_write(uint8_t data)

data: data to be sent to  slave 
RET: void
----------------description-----------------------------------------------------------------
Write may be called as manny time as needed after selecting mode 'i2c_select_mode()' with once. 
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	if (!int_is_enabled)
	{
		while ((TWCR & (1<<TWINT)) == 0)
		{}
	}

}
/**
-------------function description-----------------------------------------------------------
static uint8_t i2c_read (uint8_t isLast)
isLast: set 'false' when Being called more than once, last call should be 'true'
RET: TWDR register containing read data, if int_is_enable!=true, else retuns 0
----------------description-----------------------------------------------------------------
used for reading message sent, can read data longer than one byte by looping and finishing wit isLast=true
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static uint8_t i2c_read (bool isLast)
{
	if (!isLast ) //If we want to read more than 1 byte
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	else             //If we want to read only one byte
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	if (!int_is_enabled)
	{
	while ((TWCR & (1<<TWINT)) == 0)
	{}
	return TWDR;
	}

	return 0;
}

static bool set_busy(bool status)
{	
	busy_flag=status;
	return  busy_flag;
}


static bool get_busy(void)
{
	return busy_flag;
}