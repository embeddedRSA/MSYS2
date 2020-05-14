/*
 * i2c_driver.c
 *
 * Created: 26/03/2020 09:18:07
 * Author : ryana
 */ 
#include "i2c_driver.h"
#include <avr/io.h>
#include <util/delay.h>

//private vars
static i2c_t i2c_interface;
static bool int_is_enabled=false;
static bool initialized=false;
static bool busy_flag=false;


//prototypes
		
static uint16_t init_s(uint32_t SCL_f ,bool enable_interrupt);

static void disable_s(void);

static void selectMode_s(uint8_t SLA_x,I2C_MODE_t p_mode);

static void start_s();

static void stop_s();

static void write_s(uint8_t data);

static uint8_t read_s(bool isLast);

static bool setBusy_s(bool status);

static bool getBusy_s(void);

static uint8_t getStatus_s(void);

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
	i2c_interface.init				=	init_s;
	i2c_interface.disable			=	disable_s;
	i2c_interface.start				=	start_s;
	i2c_interface.stop				=	stop_s;
	i2c_interface.selectmode		=	selectMode_s;
	i2c_interface.write				=	write_s;
	i2c_interface.read				=	read_s;
	i2c_interface.setBusy			=	setBusy_s;
	i2c_interface.getBusy			=	getBusy_s;
	i2c_interface.getStatus			=	getStatus_s;
	initialized=true;
	
 }
	return &i2c_interface;
}


/**
-------------function description-----------------------------------------------------------
static uint8_t init_s(uint16_t SCL_f ,bool enable_interrupt)

	SCL_f: frequency that should be approximated

	enable_interrupt : 'true' to enable interrupt

	RET: returns 0 if SCL_f impossible, else return TWCR

----------------description-----------------------------------------------------------------
Init function for i2C, will set frequency by use of prescaler x4 or x1,
this is also where polling is enabled by setting enable_interrupt = false.
is implemented 
-------------function description end-------------------------------------------------------
**/
static uint16_t init_s(uint32_t SCL_f ,bool enable_interrupt) //freq is given in Hz
{
	
	//no internal pull up implemented so far
if (SCL_f <= 400000)
	{
		//enable TWI clock module 
	PRR0&=~(1<<PRTWI);
	
	//enable TWI
	TWCR|=(1<<TWEN);	
	
	//enable interrupt
	int_is_enabled=enable_interrupt;
	if (int_is_enabled)
	{
		TWCR|=(1<<TWIE);
	}
	
	//constants pre calculated from mathcad 
			
		uint8_t TWBR_1= (uint8_t)((F_CPU/(8.0*SCL_f))-2.0);


		uint8_t validation = 0xFF;

		if(TWBR_1>0 && TWBR_1<=validation )
		{
			//set prescaler

			TWSR&=~(1<<TWPS0);
			TWSR&=~(1<<TWPS1);
			//typecast to proper format and insert
			TWBR = TWBR_1;
			return TWBR;
		}

		else
		{
			return 0; //return 0 error
		}
	
	
	
	
//	if(SCL_f==400000)
//	{
//		TWBR_1= 3;
//	}
//	else if(SCL_f>=333333)
//	{
//		TWBR_1= 4;
//	}
//	else if(SCL_f>=5000)
//	{
//		TWBR_1= 398;
//	}
//	else if(SCL_f>=2000)
//	{
//		TWBR_1= 1998;
//	}
//	else if(SCL_f>=1000)
//	{
//		TWBR_1= 998;
//	}

	TWSR&=~(1<<TWPS0);
	TWSR&=~(1<<TWPS1);
	
	
	TWBR = TWBR_1;
	return TWBR_1;
	}
else
	{
	return 69;
	}
}

/**
-------------function description-----------------------------------------------------------
static void disable_s(void)

RET: void
----------------description-----------------------------------------------------------------
Disables any i2c activity, activity, 'init_s()' does not need to be followed before starting new transfer
-------------function description end-------------------------------------------------------
**/
static void disable_s(void)
{
		//TWCR|=(1<<TWEN);	
}



/**
-------------function description-----------------------------------------------------------
static void start_s(void)

RET: void
----------------description-----------------------------------------------------------------
Starts i2c, must be set before read or write (address and R/W are set with selectMpde_s())
can be called several times without calling stop, a new mode may even be selected. 
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void start_s(void)
{
	TWCR=0;
	TWCR |= (1<<TWSTA)|(1<<TWEN)|(1<<TWINT); //TWSTA sends start bit 

	if (!int_is_enabled)
	{
		while ((TWCR & (1<<TWINT)) == 0)
		{}
			
	}
}

/**
-------------function description-----------------------------------------------------------
static void stop_s()

RET: void
----------------description-----------------------------------------------------------------
stops i2c, no polling needed
-------------function description end-------------------------------------------------------
**/
static void stop_s()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

/**
-------------function description-----------------------------------------------------------
static uint8_t selectMode_s(uint8_t SLA_addr,I2C_MODE_t p_mode)

SLA_addr: Slave SLA address 7bit
p_mode: I2C_READ_MODE/I2C_WRITE_MODE   

RET: void
----------------description-----------------------------------------------------------------
is called after 'start_s()' selects address and R/W.
THe adress is  7 bit address (x) and mode is I2C_READ_MODE or I2C_WRITE_MODE
-------------function description end-------------------------------------------------------
**/
static void selectMode_s(uint8_t SLA_addr,I2C_MODE_t p_mode) //SLA_x is SLA+R/W
{
	//must send address now
	if(p_mode==I2C_WRITE_MODE)
	{
		TWDR=((SLA_addr<<1)&(~0b1));
	}
	else if(p_mode==I2C_READ_MODE)
	{
		TWDR=((SLA_addr<<1)|(0b1));
	}
	TWCR |= (1<<TWINT);
	TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	if (!int_is_enabled)
	{
	  while ((TWCR & (1<<TWINT)) == 0) //poll
	  {} 
		
	}
	
}

/**
-------------function description-----------------------------------------------------------
static void write_s(uint8_t data)

data: data to be sent to  slave 
RET: void
----------------description-----------------------------------------------------------------
Write may be called as manny time as needed after selecting mode 'selectMode_s()' with once. 
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void write_s(uint8_t data)
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
static uint8_t read_s(uint8_t isLast)
isLast: set 'false' when Being called more than once, last call should be 'true'
RET: TWDR register containing read data, if int_is_enable!=true, else retuns 0
----------------description-----------------------------------------------------------------
used for reading message sent, can read data longer than one byte by looping and finishing wit isLast=true
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static uint8_t read_s (bool isLast)
{
	if (!isLast ) //If we want to read more than 1 byte
	{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	}

	else             //If we want to read only one byte
	{
		TWCR = (1<<TWINT) | (1<<TWEN);
	}
	
	if (!int_is_enabled)
	{
	  while ((TWCR & (1<<TWINT)) == 0)
	  {}
	{}
	return TWDR;
	}

	return 0;
}

static bool setBusy_s(bool status)
{	
	busy_flag=status;
	return  busy_flag;
}


static bool getBusy_s(void)
{
	return busy_flag;
}

static uint8_t getStatus_s(void)
{
	return (TWSR)&(0b11111000);
}