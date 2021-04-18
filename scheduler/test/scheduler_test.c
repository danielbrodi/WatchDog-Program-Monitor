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
#include "operation_func.h"

/***************************** Macros Definitions *****************************/
/* print colored output */
#define PRINT_SUCCESS printf (ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET)
#define PRINT_FAILURE printf (ANSI_COLOR_RED "FAILURE\n" ANSI_COLOR_RESET)

#define CURRENT_TIME time(0)
/**************************** Forward Declarations ****************************/
oper_ret_ty Func1(void *param);
oper_ret_ty Func2(void *param);
oper_ret_ty Func3(void *param);
static void RunTests(scheduler_ty *scheduler);
static void SchedulerCreateTest(scheduler_ty *scheduler);
static void SchedulerDestroyTest(scheduler_ty *scheduler);
static void SchedulerAddTest(scheduler_ty *scheduler);
/*static void SchedulerRemoveTest(scheduler_ty *scheduler);*/
static void SchedulerRunTest(scheduler_ty *scheduler);
/*static void SchedulerIsEmptyTest(scheduler_ty *scheduler);*/
static void SchedulerSizeTest(scheduler_ty *scheduler);
/*static void SchedulerClearTest(scheduler_ty *scheduler);*/

time_t start_time;

typedef struct input
{
	scheduler_ty *scheduler;
	size_t counter;
}input_ty;

input_ty input1, input2, input3;

static void (*TestFuncs[5]) (scheduler_ty *schedule);
/******************************* Main__Function *******************************/
int main()	
{

	/*	Initializing a new empty scheduler	*/
	scheduler_ty *new_scheduler = SchedulerCreate();
	
	/*	Initializing the starting time of the program	*/
	start_time = time(0);
	
	/*	Assigning the new scheduler and a counter to the input structure	*/
	input1.scheduler = new_scheduler;
	input2.scheduler = new_scheduler;
	input3.scheduler = new_scheduler;
	input1.counter = 0;
	input2.counter = 0;
	input3.counter = 0;
	
	/* Run tests */
	RunTests(new_scheduler);
	
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
	printf("ADDING 3 elements: ");
	SchedulerAdd(scheduler, Func1, 1, &input1);
	SchedulerAdd(scheduler, Func2, 2, &input2);
	SchedulerAdd(scheduler, Func3, 3, &input3);
	
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
}
/******************************************************************************/
static void SchedulerSizeTest(scheduler_ty *scheduler)
{
	printf("SIZE_AFTER_3_ADDS: %lu\n", SchedulerSize(scheduler));
}
/******************************************************************************/
oper_ret_ty Func1(void *param)
{
	input_ty *input = (input_ty *)param;
	++(input->counter);
	printf("Every 1 seconds: %lu\n", CURRENT_TIME-start_time);
	printf("Counter: %lu\n\n", (input->counter));
	if (10 == (input->counter))
	{
	return (DONE);
	}
	else
	{
	return (NOT_DONE);
	}
}
/******************************************************************************/
oper_ret_ty Func2(void *param)
{
	input_ty *input = (input_ty *)param;
	++(input->counter);
	printf("Every 2 seconds: %lu\n", CURRENT_TIME-start_time);
	printf("Counter: %lu\n\n", (input->counter));
	if (10 == (input->counter))
	{
	return (DONE);
	}
	else
	{
	return (NOT_DONE);
	}
}
/******************************************************************************/
oper_ret_ty Func3(void *param)
{
	input_ty *input = (input_ty *)param;
	++(input->counter);
	printf("Every 3 seconds: %lu\n", CURRENT_TIME-start_time);
	printf("Counter: %lu\n\n", (input->counter));
	if (10 == (input->counter))
	{
	return (DONE);
	}
	else
	{
	return (NOT_DONE);
	}
}
/******************************************************************************/
static void SchedulerRunTest(scheduler_ty *scheduler)
{
	printf("STATUS RUN (0 is DONE): %d", SchedulerRun(scheduler));
}
/******************************************************************************/
static void RunTests(scheduler_ty *scheduler)
{
	size_t i = 0;
	TestFuncs[0] = SchedulerCreateTest;
	TestFuncs[1] = SchedulerAddTest;
	TestFuncs[2] = SchedulerSizeTest;
	TestFuncs[3] = SchedulerRunTest;
	TestFuncs[4] = SchedulerDestroyTest;
	
	while(i<5)
	{
		(*TestFuncs[i]) (scheduler);
		++i;
	}
}
/******************************************************************************/
