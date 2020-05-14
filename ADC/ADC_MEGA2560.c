

#include "ADC_MEGA2560.h"
#include <avr/io.h>
#include <stdbool.h>

static ADC_t s_ADC_interface;
static bool s_initialized = false;
static ADC_REF s_ref_type;
static float s_Ref_V=0;


static void s_initADC(ADC_REF reff_vol, float p_aRef);
static void s_startADC();
static uint8_t s_get_ADCL(void);
static uint8_t s_get_ADCH(void);
static uint16_t s_getFullADC(uint8_t p_pin_no);
static int16_t s_getADC_mV(uint8_t p_pin_no);
static void s_setADCPin(uint8_t p_pin_no);


ADC_t* get_ADC_interface()
{
	if (!s_initialized)
	{
		s_ADC_interface.initADC				=	s_initADC;
		s_ADC_interface.getFullADC			=	s_getFullADC;
		s_ADC_interface.getADC_mV				=	s_getADC_mV;
		s_initialized							=	true;	
	}
	return &s_ADC_interface;
}

static void s_initADC(ADC_REF p_reff_vol, float p_aRef)
{
	PRR0=PRR0 & ~(1<<PRADC);//enable ADC
	
	ADMUX=0b00000000; //reset ADC registers
	ADCSRA=0b00000000;
	ADCSRB=0b00000000;
	//ADMUX|=1<<ADLAR; // set to left adjust
	ADCSRA|=0b00000111; //set prescaler 128	
	ADCSRA|=1<<ADEN;
	
	//ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	//selecting ref
	s_ref_type=p_reff_vol;
	switch (s_ref_type) {
		case AREF:
		ADMUX&=0b00111111;
		s_Ref_V=p_aRef;
		break;

		case AVCC:
		ADMUX&=0b01111111;
		ADMUX|=0b01000000;
		break;

		case INTERNAL_1V1:
		ADMUX&=0b10111111;
		ADMUX|=0b10000000;
		break;

		case INTERNAL_2V56:
		ADMUX|=0b11000000;
		break;

		default:
		break;
	}
		
}

static void s_startADC(void)
{
	//start	
	ADCSRA|=1<<ADSC;
}


static void s_setADCPin(uint8_t p_pin_no)
{
// inputchannel ADC 0-15 single ended;
if(p_pin_no<8)
{
	ADMUX|=p_pin_no;		//set mux0-4
}
else if ((p_pin_no>7) && (p_pin_no<16))
{
	ADCSRB|=1<<MUX5;		//set mux5
	ADMUX|=(p_pin_no-8);	//set mux0-4
}	
}


static uint8_t s_get_ADCL(void)
{
	return (uint8_t) ADCL;
}

static uint8_t s_get_ADCH(void)
{
	return (uint8_t) ADCH;
}

static uint16_t s_getFullADC(uint8_t p_pin_no)
{
	s_setADCPin(p_pin_no);
	s_startADC();
	
	while ((ADCSRA & (1<<ADSC) )!=0)
	{
		//de nada
	}
	uint16_t	LSB =  s_get_ADCL();
	uint16_t	MSB =  s_get_ADCH()& 0b00000011;
	uint16_t retVal	  = (MSB<<8|LSB);
	return retVal;
}

static int16_t s_getADC_mV(uint8_t p_pin_no)
 {
	 uint16_t temp,out_f;
	 temp= (uint16_t)((s_getFullADC(p_pin_no)/1024.0)*1000);  // 10 BIT res and scale to mV
	 
	 switch (s_ref_type) {
		 case AREF:
		 out_f= temp*s_Ref_V;
		 break;

		 case AVCC:
		 out_f= temp*5.0;
		 break;

		 case INTERNAL_1V1:
		 out_f= temp*1.1;
		 break;

		 case INTERNAL_2V56:
		 out_f= temp*2.56;
		 break;

		 default:
		 out_f=0;
		 break;
	 }
	return (int16_t)out_f;
 }