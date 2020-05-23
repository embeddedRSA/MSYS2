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
static i2c_t s_i2c_interface;
static bool s_int_is_enabled=false;
static bool s_initialized=false;
static bool s_busy_flag=false;


//prototypes
		
static uint16_t s_init(uint32_t SCL_f ,bool enable_interrupt);

static void s_disable(void);

static void s_selectMode(uint8_t p_SLA_x,I2C_MODE_t p_mode);

static void s_start(void);

static void s_stop(void);

static void s_write(uint8_t p_data);

static uint8_t s_read(bool p_isLast);

static bool s_setBusy(bool p_status);

static bool s_getBusy(void);

static uint8_t s_getStatus(void);

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
	
 if (!s_initialized)
 {
	s_i2c_interface.init				=	s_init;
	s_i2c_interface.disable				=	s_disable;
	s_i2c_interface.start				=	s_start;
	s_i2c_interface.stop				=	s_stop;
	s_i2c_interface.selectmode			=	s_selectMode;
	s_i2c_interface.write				=	s_write;
	s_i2c_interface.read				=	s_read;
	s_i2c_interface.setBusy				=	s_setBusy;
	s_i2c_interface.getBusy				=	s_getBusy;
	s_i2c_interface.getStatus			=	s_getStatus;
	s_initialized=true;
	
 }
	return &s_i2c_interface;
}


/**
-------------function description-----------------------------------------------------------
static uint16_t s_init(uint32_t p_SCL_f ,bool p_enable_interrupt) //freq is given in Hz

	SCL_f: frequency that should be approximated

	enable_interrupt : 'true' to enable interrupt

	RET: returns 0 if SCL_f impossible, else return TWCR

----------------description-----------------------------------------------------------------
Init function for i2C, will set frequency by use of prescaler x4 or x1,
this is also where polling is enabled by setting enable_interrupt = false.
is implemented 
-------------function description end-------------------------------------------------------
**/
static uint16_t s_init(uint32_t p_SCL_f ,bool p_enable_interrupt) //freq is given in Hz
{
	
	//no internal pull up implemented so far
if (!(p_SCL_f <= SCL_MAX))
	{
		p_SCL_f=SCL_MAX;
	}
		//enable TWI clock module 
	PRR0&=~(1<<PRTWI);
	
	//enable TWI
	TWCR|=(1<<TWEN);	
	
	//enable interrupt
	s_int_is_enabled=p_enable_interrupt;
	if (s_int_is_enabled)
	{
		TWCR|=(1<<TWIE);
	}
	
	//constants pre calculated from mathcad 
			
		uint8_t TWBR_1= (uint8_t)((F_CPU/((double)8.0*p_SCL_f))-(double)2.0);

	//set prescaler to 1 -> 0b00
		
	TWSR&=~(1<<TWPS1);
		TWSR&=~(1<<TWPS0);
	//typecast to proper format and insert
	TWBR = TWBR_1;//TWBR_1;
	return TWBR;
	
}



/**
-------------function description-----------------------------------------------------------
static void s_disable(void)

RET: void
----------------description-----------------------------------------------------------------
Disables any i2c activity, activity, 'init_s()' does not need to be followed before starting new transfer
-------------function description end-------------------------------------------------------
**/
static void s_disable(void)
{
		//TWCR|=(1<<TWEN);	
}



/**
-------------function description-----------------------------------------------------------
static void s_start(void)

RET: void
----------------description-----------------------------------------------------------------
Starts i2c, must be set before read or write (address and R/W are set with selectMpde_s())
can be called several times without calling stop, a new mode may even be selected. 
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void s_start(void)
{
	TWCR=0;
	TWCR |= (1<<TWSTA)|(1<<TWEN)|(1<<TWINT); //TWSTA sends start bit 

	if (!s_int_is_enabled)
	{
		while ((TWCR & (1<<TWINT)) == 0)
		{}
			
	}
}

/**
-------------function description-----------------------------------------------------------
static void s_stop()

RET: void
----------------description-----------------------------------------------------------------
stops i2c, no polling needed
-------------function description end-------------------------------------------------------
**/
static void s_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

/**
-------------function description-----------------------------------------------------------
static uint8_t s_selectMode(uint8_t SLA_addr,I2C_MODE_t p_mode)

SLA_addr: Slave SLA address 7bit
p_mode: I2C_READ_MODE/I2C_WRITE_MODE   

RET: void
----------------description-----------------------------------------------------------------
is called after 'start_s()' selects address and R/W.
THe adress is  7 bit addre ss (x) and mode is I2C_READ_MODE or I2C_WRITE_MODE
-------------function description end-------------------------------------------------------
**/
static void s_selectMode(uint8_t p_SLA_addr,I2C_MODE_t p_mode) //SLA_x is SLA+R/W
{
	//must send address now
	if(p_mode==I2C_WRITE_MODE)
	{
		TWDR=((p_SLA_addr<<1)&(~0b1));
	}
	else if(p_mode==I2C_READ_MODE)
	{
		TWDR=((p_SLA_addr<<1)|(0b1));
	}
	TWCR |= (1<<TWINT);
	TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	if (!s_int_is_enabled)
	{
	  while ((TWCR & (1<<TWINT)) == 0) //poll
	  {} 
		
	}
	
}

/**
-------------function description-----------------------------------------------------------
static void s_write(uint8_t data)

data: data to be sent to  slave 
RET: void
----------------description-----------------------------------------------------------------
Write may be called as manny time as needed after selecting mode 's_selectMode()' with once. 
Will poll if interrupt is disabled
-------------function description end-------------------------------------------------------
**/
static void s_write(uint8_t p_data)
{
	TWDR = p_data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	if (!s_int_is_enabled)
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
static uint8_t s_read (bool p_isLast)
{
	if (!p_isLast ) //If we want to read more than 1 byte
	{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	}

	else             //If we want to read only one byte
	{
		TWCR = (1<<TWINT) | (1<<TWEN);
	}
	
	if (!s_int_is_enabled)
	{
	  while ((TWCR & (1<<TWINT)) == 0)
	  {}
	{}
	return TWDR;
	}

	return 0;
}

/**
-------------function description-----------------------------------------------------------
static bool s_setBusy(bool p_status)
bool p_status: set flag

RET: returns flag status
----------------description-----------------------------------------------------------------
may be used as a primitive mutex when using several i2c devices in isr
-------------function description end-------------------------------------------------------
**/
static bool s_setBusy(bool p_status)
{	
	s_busy_flag		=	p_status;
	return  s_busy_flag;
}

/**
-------------function description-----------------------------------------------------------
bool s_getBusy(void)
RET: returns status
----------------description-----------------------------------------------------------------
is used before  s_setBusy to read current flag status.
-------------function description end-------------------------------------------------------
**/
static bool s_getBusy(void)
{
	return s_busy_flag;
}

/**
-------------function description-----------------------------------------------------------
static uint8_t s_getStatus(uint8_t isLast)
RET: TWSR status code
----------------description-----------------------------------------------------------------
retiurns TWSR status code, is used for debugging purposes or ISR state machhines
-------------function description end-------------------------------------------------------
**/
static uint8_t s_getStatus(void)
{
	return (TWSR)&(0b11111000);
}