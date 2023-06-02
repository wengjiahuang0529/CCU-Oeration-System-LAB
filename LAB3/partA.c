//
// GPIO_LED : GPIO output to control an on-board red LED
// 
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// low-active output control by GPC12


#include <stdio.h>
#include <stdlib.h>

/*NUC140*/
#include "NUC100Series.h"
#include "MCU_init.h"
//#include "SYS_init.h"
#include "Scankey.h"
#include "SYS.h"
#include "GPIO.h"
#define PLL_CLOCK           50000000

/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"



volatile uint32_t g_u32comRbytes = 0;
volatile uint32_t g_u32comRhead  = 0;
volatile uint32_t g_u32comRtail  = 0;
volatile int32_t g_bWait         = TRUE;


/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

/* hardware initail function declaration */
void GPIO_Init(void);

/* task function declaration */
static void philosopher_A( void *p );
static void philosopher_B( void *p );
static void philosopher_C( void *p );
static void philosopher_D( void *p );

/* Semaphore function declaration*/
xSemaphoreHandle Mutex;  //for scankey 
xSemaphoreHandle Chopstick[4];


void hungry(int);
void take_a_break(void);
int ScanKey_protected(void);


//by myself
void pick_chopstick(int philosopher_index);
void release_chopstick(int philosopher_index);
void eating_rice(int philosopher_index);
void thinking(int philosopher_index);
//by myself

int i;

int main(void)
{
			/* Unlock protected registers */
		SYS_UnlockReg();
		/* Lock protected registers */
		SYS_LockReg();
		GPIO_SetMode(PA,BIT12,GPIO_PMD_OUTPUT);
		GPIO_SetMode(PA,BIT13,GPIO_PMD_OUTPUT);
		GPIO_SetMode(PA,BIT14,GPIO_PMD_OUTPUT);	
		GPIO_Init();
		Mutex = xSemaphoreCreateMutex();
		
		/* create the non-sharable resource's semaphore */
		for(i = 0; i < 4; i++)
			Chopstick[i] = xSemaphoreCreateCounting(1,1);
		
		
		xTaskCreate( philosopher_A,					/* The function that implements the task. */
						NULL, 						/* The text name assigned to the task - for debug only as it is not used by the kernel. */
						configMINIMAL_STACK_SIZE, 	/* The size of the stack to allocate to the task. */
						NULL, 						/* The parameter passed to the task - just to check the functionality. */
						1, 							/* The priority assigned to the task. */
						NULL );
	
		xTaskCreate( philosopher_B,
						NULL,
						configMINIMAL_STACK_SIZE,
						NULL,
						1,
						NULL );
						
		xTaskCreate( philosopher_C,
						NULL,
						configMINIMAL_STACK_SIZE,
						NULL,
						1,
						NULL );
						
		xTaskCreate( philosopher_D,
						NULL,
						configMINIMAL_STACK_SIZE,
						NULL,
						1,
						NULL );

		vTaskStartScheduler();
		while(1);
}

static void philosopher_A( void *p )
{
	while(1)
	{
		/* Constantly check if BUTTON-1 is pressed */
		if(ScanKey_protected() == 1)
			hungry(0);	
	}
}

static void philosopher_B( void *p )
{
	while(1)
	{
		if(ScanKey_protected() == 2)
			hungry(1);
	}
}

static void philosopher_C( void *p )
{
	while(1)
	{
		if(ScanKey_protected() == 3)
			hungry(2);
	}
}

static void philosopher_D( void *p )
{
	while(1)
	{
		if(ScanKey_protected() == 4)
			hungry(3);
	}
}

/* **HINT**
* hungry:
*	--> wait for left chopstick 
*   --> take a break
*	--> wait for Right chopstick
*	--> eating rice
*	--> release right chopstick
*	--> release left chopstick
*/

void pick_chopstick(int philosopher_index)
{
    int left_chopstick_index = philosopher_index;
    int right_chopstick_index = (philosopher_index + 1) % 4;
    
    /* Pick up left chopstick */
    xSemaphoreTake(Chopstick[left_chopstick_index], portMAX_DELAY);
    printf("Success pick up left chopstick\n");
	
		printf("Take a break for 500ms");
		/*Take a break*/
		take_a_break();
    
		/* Pick up right chopstick */
    xSemaphoreTake(Chopstick[right_chopstick_index], portMAX_DELAY);
		printf("Success pick up right chopstick\n");
		
		/*start eating*/
		eating_rice(philosopher_index);
}

void release_chopstick(int philosopher_index)
{
	  int left_chopstick_index = philosopher_index;
    int right_chopstick_index = (philosopher_index + 1) % 4;
    
    /* Pick up left chopstick */
    xSemaphoreGive(Chopstick[left_chopstick_index]);
    
    /* Pick up right chopstick */
    xSemaphoreGive(Chopstick[right_chopstick_index]);
}

void eating_rice(int philo){
	printf("start eating rice for 1000ms\n");
	
	/*Eating rice*/
	GPIO_PIN_DATA(2, 12 + philo) = 0; // turn the corresponding LED on
	vTaskDelay(1000);
	GPIO_PIN_DATA(2, 12 + philo) = 1; // turn the corresponding LED off
	release_chopstick(philo);    			// release chopsticks
}

void hungry(int philo)
{
	printf("philo %d is hungry\n",philo);
	
 //this part write extra func
	pick_chopstick(philo);
	
	/*Eating rice*/
	/*
	GPIO_PIN_DATA(2, 12 + philo) = 0; // turn the corresponding LED on
	vTaskDelay(1000);
	GPIO_PIN_DATA(2, 12 + philo) = 1; // turn the corresponding LED off
	*/
}


void take_a_break()
{
	vTaskDelay(500);	
}

int ScanKey_protected()
{
	int temp;
	xSemaphoreTake(Mutex, portMAX_DELAY);
	temp = ScanKey();
	xSemaphoreGive(Mutex);
	return temp;
}

void GPIO_Init()
{
		GPIO_SetMode(PC, BIT12, GPIO_PMD_OUTPUT);
		GPIO_SetMode(PC, BIT13, GPIO_PMD_OUTPUT);
		GPIO_SetMode(PC, BIT14, GPIO_PMD_OUTPUT);
		GPIO_SetMode(PC, BIT15, GPIO_PMD_OUTPUT);
}
