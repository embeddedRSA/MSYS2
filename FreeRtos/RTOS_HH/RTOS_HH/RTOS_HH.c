/******************************************************
FreeRTOS demo program.
Implementing 2 tasks, each blinking a LED.

Target = "Arduino Mega2560" + "PR I/O Shield"

Henning Hargaard 25.2.2018
*******************************************************/
#include <avr/io.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "led.h"
#include <queue.h>

/**
//exercise 1+2



xSemaphoreHandle xSemaphore1 = NULL;

void vLEDFlashTask1( void *pvParameters )
{
portTickType xLastWakeTime;
xLastWakeTime=xTaskGetTickCount();
  while(1)
  {
    toggleLED(0);
    vTaskDelayUntil(&xLastWakeTime,1000);
  }
}

void vLEDFlashTask2( void *pvParameters )
{
portTickType xLastWakeTime;
xLastWakeTime=xTaskGetTickCount();
  while(1)
  {
    toggleLED(1);
    vTaskDelayUntil(&xLastWakeTime,500);
  }
}



void vLEDToggle( void *pvParameters )
{
	
	while(1)
	{
		if (switchOn(2))
		{
			turnOnLED(2);
		}
		else
		{
			turnOffLED(2);
		}
			
		
	}
	
}

void vGiveTakeSwitch( void *pvParameters )
{


xSemaphoreTake(xSemaphore1, 0);
while(1)
	{
		if (switchOn(7))
		{
			xSemaphoreGive(xSemaphore1);
			while (pdTRUE!=xSemaphoreTake(xSemaphore1, 0))
			{
				//nothing
			}
		}

	}

}
void vGiveTakeLED( void *pvParameters )
{
	
	portTickType xLastWakeTime;
	xLastWakeTime=xTaskGetTickCount();
	

	int i = 0;
	while(1)
	{
		vTaskDelayUntil(&xLastWakeTime,50);
		if (pdTRUE==xSemaphoreTake(xSemaphore1, 0))
		{
		for (i=0; i<10; i++)
		{
			    turnOnLED(7);
			    vTaskDelayUntil(&xLastWakeTime,100);;
				turnOffLED(7);
				vTaskDelayUntil(&xLastWakeTime,100);;
		}
		xSemaphoreGive(xSemaphore1);
		}
	}

	
}

*/

/**
//exercise 3
**/
xQueueHandle xQueueCount = NULL;
xSemaphoreHandle xSemaphoreCount = NULL;
volatile uint8_t count= 0;

 void vDecrementer( void *pvParameters )
 {
	portTickType xLastWakeTime;
	xLastWakeTime=xTaskGetTickCount(); 
	 while(1)
	 {
		
		 if (switchOn(0))
		 {
			 xSemaphoreTake(xSemaphoreCount, 0);
			 count--;
			SendString("Decremented");
			 xQueueSend(xQueueCount,&count,0);
			 xSemaphoreGive(xSemaphoreCount);
			 vTaskDelayUntil(&xLastWakeTime,500);
			 
		 }

	 }
 }
 
 void vIncrementer( void *pvParameters )
 {
	portTickType xLastWakeTime;
	xLastWakeTime=xTaskGetTickCount();
	 	 while(1)
	 	 {
		 	 
		 	 if (switchOn(1))
		 	 {
			 	 xSemaphoreTake(xSemaphoreCount, 0);
			 	 count++;
				 SendString("incremented");
			 	 xQueueSend(xQueueCount,&count,0);
			 	 xSemaphoreGive(xSemaphoreCount);
				 vTaskDelayUntil(&xLastWakeTime,500);
		 	 }

	 	 }
 }
 
 void vCounter( void *pvParameters )
 {
	 uint8_t buffer=0;
	 while (1)
	 {
		 xQueueReceive(xQueueCount,&buffer,0);
		 //SendInteger((char)buffer);
		 writeAllLEDs(buffer);
		 
	 }
 }


int main(void)
{
	/** exercise 1+2
  initSwitchPort();
  initLEDport();
  
  vSemaphoreCreateBinary(xSemaphore1);
  
  xTaskCreate( vLEDFlashTask1, ( signed char * ) "LED1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
  xTaskCreate( vLEDFlashTask2, ( signed char * ) "LED2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
  //xTaskCreate( vLEDToggle, ( signed char * ) "LED3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	  
  xTaskCreate( vGiveTakeSwitch, ( signed char * ) "LED7", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );	
  xTaskCreate( vGiveTakeLED, ( signed char * ) "Switch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
  
  vTaskStartScheduler();
  while(1)
  {}
	  **/
	
	//exercise 3
  initSwitchPort();
  initLEDport();
  InitUART(115200,8);

  vSemaphoreCreateBinary(xSemaphoreCount);
  xQueueCount = xQueueCreate(10,sizeof(uint8_t));
  
  xTaskCreate( vDecrementer, ( signed char * ) "Dec", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
  //xTaskCreate( vIncrementer, ( signed char * ) "Inc", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
  //xTaskCreate( vCounter, ( signed char * ) "Count", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
 
  
  vTaskStartScheduler();
  while(1)
  {}	
}

