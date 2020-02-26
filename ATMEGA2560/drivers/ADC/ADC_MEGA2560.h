



typedef unsigned char V_REF
{
  unsigned char AREF_INTERNAL;
  unsigned char AVCC;
  unsigned char INTERNAL_1V1;
  unsigned char INTERNAL_2V56;
};

void initADC(uint8_t pin_no,uint8_t reff_vol);

void startADC(uint8_t pin_no);

void getADCMSB();
void getADCLSB();
