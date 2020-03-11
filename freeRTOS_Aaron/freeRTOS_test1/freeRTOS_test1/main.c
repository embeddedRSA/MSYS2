/******************************************************
FreeRTOS demo program.
Implementing 2 tasks, each blinking a LED.

Target = "Arduino Mega2560" + "PR I/O Shield"

Henning Hargaard 25.2.2018
*******************************************************/
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

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

int main(void)
{
	initLEDport();
	xTaskCreate( vLEDFlashTask1, ( signed char * ) "LED1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vLEDFlashTask2, ( signed char * ) "LED2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	vTaskStartScheduler();
	while(1)
	{}
}

