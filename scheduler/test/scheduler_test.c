/*********************************FILE__HEADER*********************************\
* File:			scheduler_test.c						 		  								
* Author:		Daniel Brodsky					  								
* Date:			16/04/2021							   								
* Version:		1.0	(Pre=Review)						
* Reviewer:		Olga							   								
* Description:	Testing file of the implementation of a task scheduler.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <stddef.h>	/*	size_t			*/
#include <stdio.h>	/*	printf, NULL	*/
#include <stdlib.h>	/*	rand, srand		*/
#include <time.h>	/*	time			*/

#include "scheduler.h"
#include "task.h"
#include "utils.h"	/*	status_ty, boolean_ty, ANSI_COLOR_	*/

/***************************** Macros Definitions *****************************/
/* print colored output */
#define PRINT_SUCCESS printf (ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET)
#define PRINT_FAILURE printf (ANSI_COLOR_RED "FAILURE\n" ANSI_COLOR_RESET)

/* generates random number from 1 to 100 */
#define RANDOM_NUM ((rand() % 100) + 1)

#define INT_TO_VOID_PTR(int_x) (void *)(long)(int_x)
#define VOID_PTR_TO_INT(void_ptr) (int)(long)(void_ptr)
/**************************** Forward Declarations ****************************/
static void SchedulerCreateTest(scheduler_ty *scheduler);
static void SchedulerDestroyTest(scheduler_ty *scheduler);
static void SchedulerAddTest(scheduler_ty *scheduler);
/*static void SchedulerRemoveTest(scheduler_ty *scheduler);*/
/*static void SchedulerRunTest(scheduler_ty *scheduler);*/
/*static void SchedulerIsEmptyTest(scheduler_ty *scheduler);*/
static void SchedulerSizeTest(scheduler_ty *scheduler);
/*static void SchedulerClearTest(scheduler_ty *scheduler);*/

/******************************* Main__Function *******************************/
int main()	
{
	/*	Intializing a new empty scheduler	*/
	scheduler_ty *new_scheduler = SchedulerCreate();
	
	/*	Intializing a random number generator	*/
	srand(time(0));
	
	SchedulerCreateTest(new_scheduler);
	SchedulerAddTest(new_scheduler);
	SchedulerSizeTest(new_scheduler);
/*	PqueueSortingTest(new_scheduler);*/
/*	PqueueClearTest(new_scheduler);*/
/*	PqueueEraseTest(new_scheduler);*/
	SchedulerDestroyTest(new_scheduler);
	
	return (0);
}

/************************Test Functions Implementations************************/

static void SchedulerCreateTest(scheduler_ty *scheduler)
{
	printf("\nTask Scheduler Creation Test: ");
	NULL == scheduler ? PRINT_FAILURE : PRINT_SUCCESS;
}
/******************************************************************************/
static void SchedulerDestroyTest(scheduler_ty *scheduler)
{
	SchedulerDestroy(scheduler);
	printf (ANSI_COLOR_CYAN "\nThe scheduler has been successfully deleted\n\n" 
															ANSI_COLOR_RESET);
}
/******************************************************************************/
static void SchedulerAddTest(scheduler_ty *scheduler)
{
	boolean_ty is_working = TRUE;
	int x = 3;
	
	SchedulerAdd(scheduler, 3, (void *)x)
	
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
}
/******************************************************************************/
static void SchedulerSizeTest(scheduler_ty *scheduler);
/******************************************************************************/
oper_ret_ty PrintHello(void *param)
{
	printf("Hello");
	return (done);
}
/******************************************************************************/
