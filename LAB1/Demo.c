/*
 * demo.c
 *
 */
/* Standard includes. */
#include <stdio.h>
#include <stdbool.h>
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "list.h"
#include "queue.h"

#define A_DELAY_MS 7000
#define B_DELAY_MS 3000
#define C_DELAY_MS 2000
#define D_DELAY_MS 5000
#define TIME_DELAY_MS 1000

static void taskA( void *p );
static void taskB( void *p );
static void taskC( void *p );
static void taskD( void *p );
static void taskT( void *p );

void demo1( void )
{
	xTaskCreate( taskD,					/* The function that implements the task. */
				"D", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
				NULL, 						/* The parameter passed to the task - just to check the functionality. */
				1, 							/* The priority assigned to the task. */
				NULL );


	xTaskCreate( taskC,					/* The function that implements the task. */
				"C", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
				NULL, 						/* The parameter passed to the task - just to check the functionality. */
				2, 							/* The priority assigned to the task. */
				NULL );

	xTaskCreate( taskB,					/* The function that implements the task. */
				"B", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
				NULL, 						/* The parameter passed to the task - just to check the functionality. */
				3, 							/* The priority assigned to the task. */
				NULL );

	xTaskCreate( taskA,					/* The function that implements the task. */
				"A", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
				NULL, 						/* The parameter passed to the task - just to check the functionality. */
				4, 							/* The priority assigned to the task. */
				NULL );

	xTaskCreate( taskT,					/* The function that implements the task. */
				"Time", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
				NULL, 						/* The parameter passed to the task - just to check the functionality. */
				5, 							/* The priority assigned to the task. */
				NULL );

	vTaskStartScheduler();   //讓task 進入 FreeRTOS scheduling

	while(1);
}


static void taskT( void *p )
{
	while(1){
		printf("%d second.\n", xTaskGetTickCount()/1000);
		vTaskDelay(TIME_DELAY_MS);   //delay for 1000ms,
		fflush( stdout );  //clean buffer
	}
}


static void taskA( void *p )
{
	const char *name = "A";
	while(1){

		printf("%s!!\n", name);
		vTaskDelay(A_DELAY_MS);   //delay for 7000ms, but in windows freeRTOS simulator, it's more than 100ms.
		fflush( stdout );  //clean buffer
	}
}

static void taskB( void *p )
{
	 const char *name = "B";
	while(1){

		printf("%s!!\n", name);
		vTaskDelay(B_DELAY_MS);   //delay for 3000ms, but in windows freeRTOS simulator, it's more than 100ms.
		fflush( stdout );  //clean buffer
	}
}
static void taskC( void *p )
{
	 const char *name = "C";
	while(1){

		printf("%s!!\n", name);
		vTaskDelay(C_DELAY_MS);   //delay for 2000ms, but in windows freeRTOS simulator, it's more than 100ms.
		fflush( stdout );  //clean buffer
	}
}
static void taskD( void *p )
{
	 const char *name = "D";
	while(1){

		printf("%s!!\n", name);
		vTaskDelay(D_DELAY_MS);   //delay for 5000ms, but in windows freeRTOS simulator, it's more than 100ms.
		fflush( stdout );  //clean buffer
	}
}



