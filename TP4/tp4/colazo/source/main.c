#include <string.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*#include "fsl_debug_console.h"*/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "fsl_port.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"




/* Task priorities. */

#define h_periodic_priority (configMAX_PRIORITIES - 1)
#define l_periodic_priority (configMAX_PRIORITIES - 2)
#define h_aperiodic_priority (configMAX_PRIORITIES - 3)


static void red_task(void *pvParameters);
static void temperature_task(void *pvParameters);
static void aperiodic_task(void *pvParameters);

void aperiodic_task(void *pvParameters){
	int random;
	int delay;
	for(;;){
		int random = rand() % 2000;
		vTaskDelay(random);
		printf("Hello! I generated %d \r\n",random);

		for(delay=0;delay<1000000;delay++){

		}
	}
}

 void red_task(void *pvParameters) {
 for (;;) {
            puts("Hello world. I am Red task\r\n");
            LED_RED_TOGGLE();
            vTaskDelay(100);

  }
}


 void temperature_task(void *pvParameters){
	 int temp;
	 for(;;){

		 temp = rand() % 100;
		 printf("The temperature is %d\r\n", temp);
		 vTaskDelay(100);


	 }
 }









/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */




 BOARD_InitPins();
 BOARD_BootClockRUN();
 BOARD_InitDebugConsole();

  /* Add your code here */

  // Enable the clock to the PORT module that the LED is on.
 CLOCK_EnableClock(kCLOCK_PortB);
 CLOCK_EnableClock(kCLOCK_PortE);

 // Setup the red and green LED pin as GPIO
 PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
 PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);








 //Initialize the RGB to on/off condition
 LED_RED_INIT(LOGIC_LED_OFF);
 LED_GREEN_INIT(LOGIC_LED_OFF);


 LED_RED_OFF();
 LED_GREEN_OFF();
 //Turn off RED Led

  /* Create RTOS task */
 vTraceEnable(TRC_START);


 xTaskCreate(temperature_task, "Temperature Task",configMINIMAL_STACK_SIZE, NULL, h_periodic_priority, NULL);
 xTaskCreate(red_task, "Red Task",configMINIMAL_STACK_SIZE, NULL, l_periodic_priority, NULL);
 xTaskCreate(aperiodic_task, "Aperiodic Task",configMINIMAL_STACK_SIZE, NULL, h_aperiodic_priority, NULL);

 vTaskStartScheduler();


 for(;;) { /* Infinite loop to avoid leaving the main function */
 __asm("NOP"); /* something to use as a breakpoint stop while looping */
 __asm("NOP");
  }
}
