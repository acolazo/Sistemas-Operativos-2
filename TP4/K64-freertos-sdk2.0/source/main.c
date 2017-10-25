/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include <string.h>
#include <stdlib.h>

//#include "stdio.h"
#include "printf-stdarg.h"


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
#include "heap_4.c"

/* Task priorities. */

#define task_PRIORITY (configMAX_PRIORITIES - 1)
#define MAX_VALOR 40
#define CANTIDAD_PALABRAS 3
#define config_STACK_SIZE 512

static void green_task(void *pvParameters);
static void red_task(void *pvParameters);
static void blue_task(void *pvParameters);

struct message{
	int tipo;
	char * message;
};
/*
 * Tipo 0: Cadena de tamaño variable y de caracteres pseudo-aleatorios.
 * Tipo 1: Valor que tomo el sensor de temperatura.
 */


/* TAREA TEMPERATURA */
void red_task(void *pvParameters) {
	struct message mensaje_enviar;
	uint8_t temp;

	QueueHandle_t xQueuec;
	xQueuec = (QueueHandle_t) pvParameters;

	for (;;) {

		temp = rand() % 256; //valor de 0 a 255.
		mensaje_enviar.message = (char*)pvPortMalloc(sizeof(char));
		if(mensaje_enviar.message == NULL){
			printf("ERROR ALOCANDO MEMORIA");
			vTaskSuspend(NULL);
		}


		*mensaje_enviar.message = (char) temp;
		//printf("Emisor: %s\r\n", mensaje_enviar.message);

		mensaje_enviar.tipo = 1;
		xQueueSend(xQueuec, ( void * ) &mensaje_enviar, ( TickType_t ) portMAX_DELAY);

		mensaje_enviar.message = NULL;

		LED_RED_TOGGLE();
		vTaskDelay( 1000 );
	}
}

/* TAREA GENERADOR DE CADENA */
void green_task(void *pvParameters) {
	int cantidad_letras;
	int i;
	char palabra[MAX_VALOR + 1];
	struct message mensaje_enviar;
	int boton_empujado;
	QueueHandle_t xQueuec;

	xQueuec = (QueueHandle_t) pvParameters;

	boton_empujado = 0;
	for (;;) {

		if (GPIO_ReadPinInput(BOARD_SW3_GPIO,BOARD_SW3_GPIO_PIN) == 0){
			if(boton_empujado == 0){
				LED_GREEN_TOGGLE();


				cantidad_letras = rand() % (MAX_VALOR);
				for(i=0; i<cantidad_letras; i++){
					palabra[i] = 'A' + (rand() % 26);

				}
				palabra[i]='\0';

				mensaje_enviar.message = (char*)pvPortMalloc(sizeof(char)*(cantidad_letras+1));
				if(mensaje_enviar.message == NULL){
							printf("ERROR ALOCANDO MEMORIA");
							vTaskSuspend(NULL);
						}

				strcpy (mensaje_enviar.message,palabra);
				mensaje_enviar.tipo = 0;
				xQueueSend(xQueuec, ( void * ) &mensaje_enviar, ( TickType_t ) portMAX_DELAY);
				//printf("Emisor: %s\r\n", palabra);
				boton_empujado = 1;

				mensaje_enviar.message = NULL;
			}

		}
		else{
			boton_empujado = 0;
		}
		vTaskDelay( 100 );
	}


}


/* TAREA UART */
void blue_task(void *pvParameters) {
	struct message mensaje_recibido;
	uint8_t auxiliar;
	QueueHandle_t xQueuec;

	xQueuec = (QueueHandle_t) pvParameters;

	for (;;) {

		xQueueReceive(xQueuec, &mensaje_recibido, ( TickType_t ) portMAX_DELAY);
		switch(mensaje_recibido.tipo){
		case 0:
			printf("Receptor: %s\r\n\n", mensaje_recibido.message);
			break;
		case 1:
			auxiliar = (uint8_t) *mensaje_recibido.message;
			printf("Receptor: Valor del sensor: %u\r\n\n", auxiliar);
			break;
		default:
			printf("Receptor: ERROR!\r\n\n");

		}


		vPortFree(mensaje_recibido.message);
		mensaje_recibido.message = NULL;


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
	QueueHandle_t xQueuec;
	/* Tracealyzer Code. Remove for debugging. */
	vTraceEnable(TRC_START);



	// Enable the clock to the PORT module that the LED is on.
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortE);
	CLOCK_EnableClock(kCLOCK_PortA);

	// Setup the red LED pin as GPIO
	PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_MuxAsGpio);

	//initialise the RGB to on/off condition
	LED_RED_INIT(LOGIC_LED_OFF);
	LED_GREEN_INIT(LOGIC_LED_OFF);



	for(int i=0;i<=10000000;i++);
	printf("Hello world. inside main\r\n");
	LED_RED_OFF();
	LED_GREEN_ON();


	xQueuec = xQueueCreate(CANTIDAD_PALABRAS, sizeof(struct message));



	/* Create RTOS task */

	xTaskCreate(blue_task, "EMISOR_UART", config_STACK_SIZE, xQueuec, task_PRIORITY, NULL);
	xTaskCreate(red_task, "SENSOR_TEMPERATURA", config_STACK_SIZE, xQueuec, task_PRIORITY, NULL);
	xTaskCreate(green_task, "CADENA_TEXTO", config_STACK_SIZE, xQueuec, task_PRIORITY, NULL);

	//tskIDLE_PRIORITY + 1
	vTaskStartScheduler();

	vTaskResume(red_task);
	vTaskResume(green_task);


	for(;;) { /* Infinite loop to avoid leaving the main function */
		__asm("NOP"); /* something to use as a breakpoint stop while looping */
	}
}




