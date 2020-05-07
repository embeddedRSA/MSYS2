
//ex2.h file-------------------------------------------

typedef struct
{
uint8_t(*getData)(void);
void(*setData)(uint8_t);
}ex2_t

ex2_t *getEx2InterFace( ex_t* p_ex2Interface);

//ex2.c------------------------------------------------
#include "ex.h" //other interface
#include "ex2.h"
static ex_t* m_exInterfacePtr; //'private members'
static ex2_t ex2Interface;
static m_data;
static char m_isInitiated = 0;

static uint8_t s_getData(void)
{
  return m_data;
}

static void s_setData(uint8_t p_data)
{
  m_data=p_data;
  m_exInterfacePtr->setData(m_data); //also use interface
}

ex2_t *getExInterFace( ex_t* p_exInterfacePtr)
{
  if (isInitiated) {
    //nothing
  }
  else
  {
    m_exInterfacePtr = p_exInterfacePtr; //set interface
    ex2Interface.getData=s_getData; //set function pointers
    ex2Interface.setData=s_setData;
    m_isInitiated=1; //is now set
  }
  return &ex2Interface;
}

//main.c-----------------------------------------------
#include "ex.h" //other interface
#include "ex2.h"

ex_t*  ex_intPtr; //global
ex2_t* ex2_intPtr;

int int main(void)
{
ex_intPtr  = getExInterFace();
ex2_intPtr = getEx2InterFace(ex_intPtr);
ex2_intPtr->setData(9);//function call in main
return 0;
}
