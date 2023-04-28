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
#include <string.h>

#define A_Duration_Time 100
#define B_Duration_Time 200
#define C_Duration_Time 400
#define D_Duration_Time 500
#define Cleaner_Duration_Time 300

#define A_period 400
#define B_period 500
#define C_period 300
#define D_period 600
#define Cleaner_period 1000

#define TIME_DELAY_MS 100
/*Mutex是一種保護共享資源的方法，可以確保同一時間只有一個線程或任務可以訪問這些資源。
mutex 是一種特殊的 Semaphore，它只有兩種狀態：
鎖定和解鎖。當 mutex 被鎖定時，其他任務就無法訪問被保護的資源，直到 mutex 被解鎖為止。
*/
//Semaphore是一種同步機制，用於控制多個線程或任務對共享資源的訪問。
/*
 configSUPPORT_DYNAMIC_ALLOCATION 和 configUSE_MUTEXES 必須同時在 FreeRTOSConfig.h 中設置為 1， 方能確保 xSemaphoreCreateMutex() 可用
 */

static void taskA( void *p );
static void taskB( void *p );
static void taskC( void *p );
static void taskD( void *p );
static void taskTime( void *p );
static void taskCleaner( void *p );

static int count=0;
SemaphoreHandle_t xSemaphore_Toilet=NULL;
SemaphoreHandle_t xSemaphoreMutex_Count=NULL;
char lastperson[50];
int first=0;
int cleaning=0;
void demo( void )
{
	xSemaphore_Toilet = xSemaphoreCreateCounting(2,2) ;//(MaxCount,InitialCount)有2間隔間，當一個線程或任務成功獲取了 semaphore， semaphore 的計數器會減去1
	xSemaphoreMutex_Count = xSemaphoreCreateMutex();

    xTaskCreate(taskTime, "Time", configMINIMAL_STACK_SIZE, NULL,2, NULL);
	xTaskCreate( taskA,					/* The function that implements the task. */
				"A", 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
				NULL, 						/* The parameter passed to the task - just to check the functionality. */
				1, 							/* The priority assigned to the task. */
				NULL );
    xTaskCreate(taskB, "B", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(taskC, "C", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(taskD, "D", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(taskCleaner, "Cleaner", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	vTaskStartScheduler();

	while(1);
}

void use_toilet(char* name, int duration, int period) {

	const TickType_t Duration = pdMS_TO_TICKS(duration);
	const TickType_t Period = pdMS_TO_TICKS(period);

  while (1) {
			if(first==0){
				printf("%s is the first one who want get in to toilet,check the toilet-semaphore\n", name);
				first=1;
			}
			while(cleaning);
			printf("%s waiting for toilet\n",name);
			if (xSemaphoreTake(xSemaphore_Toilet, portMAX_DELAY) == pdTRUE) {

				printf("%s start using toilet for %d ms...\n", name,duration);

				xSemaphoreTake(xSemaphoreMutex_Count, portMAX_DELAY);
				count++;
				xSemaphoreGive(xSemaphoreMutex_Count);

				vTaskDelay(Duration);

				printf("%s leave the toilet.\n", name);

				xSemaphoreTake(xSemaphoreMutex_Count, portMAX_DELAY);
				count--;
				xSemaphoreGive(xSemaphoreMutex_Count);


				xSemaphoreGive(xSemaphore_Toilet);//釋放廁所

				strcpy(lastperson,name); //紀錄最後離開的人
				vTaskDelay(Period);
			}
  	  }
}

void clean_toilet() {

  const TickType_t Period = pdMS_TO_TICKS(Cleaner_period);
  printf("Cleaner is waiting..\n");

  while (1) {
	  	  //printf("cleaner is here\n");
	  	  if (xSemaphoreTake(xSemaphore_Toilet, portMAX_DELAY) == pdTRUE){
				if(xSemaphoreTake(xSemaphore_Toilet, (TickType_t)0)==pdTRUE){

					xSemaphoreTake(xSemaphoreMutex_Count, portMAX_DELAY);
					cleaning=1;
					count=0;
					first=0;
					xSemaphoreGive(xSemaphoreMutex_Count);

					printf("%s is the last one get out the toilet,release the toilet-semaphore\n",lastperson);
					printf("All Toilet are closed for cleaning!!\n");

					vTaskDelay(pdMS_TO_TICKS(Cleaner_Duration_Time));
					printf("Cleaning is completed.\n");
					cleaning=0;

					xSemaphoreGive(xSemaphore_Toilet);
					xSemaphoreGive(xSemaphore_Toilet);

				}else{
					xSemaphoreGive(xSemaphore_Toilet);
					//printf("Cleaner is waiting..\n");
					vTaskDelay(pdMS_TO_TICKS(Cleaner_Duration_Time));
					continue;
				}
	  	  }
      vTaskDelay(Period);
  }
}

static void taskTime( void *p )
{
	TickType_t startTime = xTaskGetTickCount();
	while(1){
		printf("\n---%d ms-----------------------------------------------------.\n", xTaskGetTickCount()-startTime);
		vTaskDelay(pdMS_TO_TICKS(TIME_DELAY_MS));
		fflush(stdout);  //clean buffer
	}
}


static void taskA( void *p )
{
	//printf( "Student A is the first one who want get in to tilet,check the toilet-semaphore\n" );
	while (1) {
		use_toilet("studentA",A_Duration_Time,A_period);

    }
}


static void taskB( void *p )
{
    while (1) {
    	use_toilet("studentB",B_Duration_Time,B_period);
    }

}

static void taskC( void *p )
{
	while (1) {
		use_toilet("studentC",C_Duration_Time,C_period);

    }
}
static void taskD( void *p )
{
	while (1) {
	use_toilet("studentD",D_Duration_Time,D_period);
	}
}

static void taskCleaner( void *p )
{
	  while (1) {
		  clean_toilet();
	  }
}



