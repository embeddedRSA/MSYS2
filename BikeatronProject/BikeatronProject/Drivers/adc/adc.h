#ifndef ADC_H_
#define ADC_H_
#include <stdint.h> 

typedef enum
{
 AREF,
 AVCC,
 INTERNAL_1V1,
 INTERNAL_2V56
}ADC_REF;


typedef void (*void_RET_ADC_REF_float_PAR)(ADC_REF reff_vol, float p_aRef);
typedef int16_t (*int16_RET_uint8_PAR)(uint8_t);
typedef uint16_t (*uint16_RET_uint8_PAR)(uint8_t);

typedef struct
{
	void_RET_ADC_REF_float_PAR initADC;
	uint16_RET_uint8_PAR getFullADC;
	int16_RET_uint8_PAR getADC_mV;			
}ADC_t;


ADC_t* get_ADC_interface();



#endif /*ADC_H_*/