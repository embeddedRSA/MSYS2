

void initADCSinglePin(uint8_t pin_no,V_REF reff_vol, uint8_t prescaler)
{

  ADMUX=0; //reset ADC register
  ADCRA=0;
  ADCRB=0;

switch (reff_vol) {
  case AREF_INTERNAL:
          ADMUX&=0b00111111;
  break;

  case AVCC:
          ADMUX&=0b01111111;
          ADMUX|=0b01000000;
  break;

  case INTERNAL_1V1:
          ADMUX&=0b10111111;
          ADMUX|=0b10000000;
  break:

  case INTERNAL_2V56:
          ADMUX|=0b11000000;
  break;

  default:
  break;
}
}

ADMUX|=0b00100000; //set ADLAR -- left adjust result


//setting channel
if(pin_no<8)
{
ADMUX|=pin_no;
}
if (pin_no>7||pin_no<16)
{
  ADCRB|=0b00001000;
  ADMUX|=(pin_no-8);
}

//set prescaler

switch (prescaler) {
  case 2:
    ADCRA|=0;
  break;
  case 4:
    ADCRA|=2;
  break;
  case 8:
    ADCRA|=3;
  break;
  case 16:
    ADCRA|=5;
  break
  case 32:
    ADCRA|=6;
  break;
  case 64:
    ADCRA|=7;
  break;
  case 128:
    ADCRA|=8;
  break;
  default:
    ADCRA|=0;
  break;
}









void startADC(uint8_t pin_no)
{
ADCRA=0;

}


void getADCMSB();
void getADCLSB();
