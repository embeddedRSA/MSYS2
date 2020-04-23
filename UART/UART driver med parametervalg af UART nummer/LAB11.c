/********************************************
* MSYS, LAB 11                              *
* Test program for Mega2560 UART driver.    *
* Using UART0, UART1, UART2 or UART3        *
* (defined by "myUART")                     *
*                                           *
* Henning Hargaard 4/11 2015                *
*********************************************/
#include <avr/io.h>
#include "uart.h"
#include "led.h"

// Select the UART to test (UART0, UART1, UART2 or UART3)
#define myUART UART0

int main()
{
  char tegn;
  char TestStreng[] = "This string is stored in SRAM memory\r\n";

  // Initialize LED port
  initLEDport();
  // Initialize UART
  // Important : Also other Baud Rates, Data Bits and Parity should be tested
  // Corresponding changes has to be made in the terminal program used
  // for the test
  InitUART(myUART, 9600, 8, 'N');

  while (1)
  {
    // Testing SendChar
    writeAllLEDs(1);
    SendChar(myUART, 'A');
    SendChar(myUART, 'B');
    SendChar(myUART, 'C');
    SendChar(myUART, '\r');
    SendChar(myUART, '\n');

    // Testing ReadChar() : Read and echo
    writeAllLEDs(2);
    tegn = ReadChar(myUART);
    SendChar(myUART, tegn);
    SendChar(myUART, '\r');
    SendChar(myUART, '\n');

    // Testing CharReady() : Wait, read and echo
    writeAllLEDs(3);	
    while ( !CharReady(myUART) )
    {}
    SendChar(myUART, ReadChar(myUART) );
    SendChar(myUART, '\r');
    SendChar(myUART, '\n');

    // Sending string stored in Flash (constant string)
    writeAllLEDs(4);
    SendString(myUART, "This string is stored as a constant in flash memory\r\n");

    // Sending string stored in SRAM (modify-able string)
    writeAllLEDs(5);
    SendString(myUART, TestStreng);

    // Testing SendInteger()
    writeAllLEDs(6);
    SendInteger(myUART, 12345); //Positive number
    SendChar(myUART, '\r');
    SendChar(myUART, '\n');
    SendInteger(myUART, -1000); //negative number
    SendChar(myUART, '\r');
    SendChar(myUART, '\n');
  }
}