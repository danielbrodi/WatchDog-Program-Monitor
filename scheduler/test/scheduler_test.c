/*********************************FILE__HEADER*********************************\
* File:			scheduler_test.c						 		  								
* Author:		Daniel Brodsky					  								
* Date:			16/04/2021							   								
* Version:		1.0	(Pre=Review)						
* Reviewer:		Olga							   								
* Description:	Testing file of the implementation of a task scheduler.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <stddef.h>				/*	size_t			*/
#include <stdio.h>				/*	printf, NULL	*/
#include <time.h>				/*	time			*/

#include "scheduler.h"			/*	task scheduler implementation			*/
#include "task.h"				/*	scheduler's tasks implementation		*/
#include "utils.h"				/*	status_ty, boolean_ty, ANSI_COLOR_		*/
#include "operation_func.h"		/*	operation_func_ty definition			*/

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
static void SchedulerSortingTest(scheduler_ty *scheduler);
static void SchedulerClearTest(scheduler_ty *scheduler);
static void SchedulerRunTest(scheduler_ty *scheduler);
static void SchedulerDestroyTest(scheduler_ty *scheduler);

time_t start_time;

typedef struct input
{
	scheduler_ty *scheduler;
	size_t counter;
}input_ty;

input_ty input1, input2, input3;
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
/******************************************************************************/
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
static void SchedulerSortingTest(scheduler_ty *scheduler)
{
	ilrd_uid_ty ret1, ret2, ret3;
	boolean_ty is_working = TRUE;
	
	ret1 = SchedulerAdd(scheduler, Func1, 1, &input1);
	is_working *= !UIDIsEqual(ret1, UIDGetBadUID());
	
	ret2 = SchedulerAdd(scheduler, Func2, 2, &input2);
	is_working *= !UIDIsEqual(ret2, UIDGetBadUID());
	
	ret3 = SchedulerAdd(scheduler, Func3, 3, &input3);
	is_working *= !UIDIsEqual(ret3, UIDGetBadUID());
	
	is_working *= (3 == SchedulerSize(scheduler));
	
	is_working *= (SUCCESS == SchedulerRemove(scheduler, ret1));
	is_working *= (SUCCESS == SchedulerRemove(scheduler, ret2));
	is_working *= (SUCCESS == SchedulerRemove(scheduler, ret3));
	
	is_working *= SchedulerIsEmpty(scheduler);
	
	printf("Task Scheduler Add&Remove Tests: ");
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
}
/******************************************************************************/
static void SchedulerClearTest(scheduler_ty *scheduler)
{
	boolean_ty is_working = TRUE;
	
	SchedulerAdd(scheduler, Func1, 1, &input1);
	SchedulerAdd(scheduler, Func2, 2, &input2);
	SchedulerAdd(scheduler, Func3, 3, &input3);
	
	is_working *= (3 == SchedulerSize(scheduler));
	
	SchedulerClear(scheduler);
	is_working *= SchedulerIsEmpty(scheduler);
	
	printf("Task Scheduler Clear Test: ");
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
}
/******************************************************************************/
static void SchedulerRunTest(scheduler_ty *scheduler)
{
	boolean_ty is_working = TRUE;
	
	SchedulerAdd(scheduler, Func1, 1, &input1);
	SchedulerAdd(scheduler, Func2, 2, &input2);
	SchedulerAdd(scheduler, Func3, 3, &input3);
	
	is_working *= (3 == SchedulerSize(scheduler));
	
	printf (ANSI_COLOR_CYAN "\n**Scheduler Start to Run:**\n" ANSI_COLOR_RESET);
	
	is_working *= (FINISHED == SchedulerRun(scheduler));
	
	printf("Task Scheduler Run Test: ");
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
}
/******************************************************************************/

oper_ret_ty Func1(void *param)
{
	boolean_ty is_working = TRUE;
	input_ty *input = (input_ty *)param;
	
	++(input->counter);
	
	printf("Elapsed time since beginning: %lu\n", CURRENT_TIME-start_time);
	printf("1 second interval Counter: %lu\n\n", input->counter);
	
	if (10 == input->counter)
	{
/*		SchedulerStop(input->scheduler);*/
		 return (DONE); 
	}
	
	return (NOT_DONE);
}
/******************************************************************************/
oper_ret_ty Func2(void *param)
{
	input_ty *input = (input_ty *)param;
	
	++(input->counter);
	
	printf("Elapsed time since beginning: %lu\n", CURRENT_TIME-start_time);
	printf("2 seconds interval Counter: %lu\n\n", input->counter);
	
	if (10 == input->counter)
	{
		return (DONE);
	}
	
	return (NOT_DONE);
}
/******************************************************************************/
oper_ret_ty Func3(void *param)
{
	input_ty *input = (input_ty *)param;
	
	++(input->counter);
	
	printf("Elapsed time since beginning: %lu\n", CURRENT_TIME-start_time);
	printf("3 seconds interval Counter: %lu\n\n", input->counter);
	
	if (10 == input->counter)
	{
		return (DONE);
	}
	
	return (NOT_DONE);
}
/******************************************************************************/
static void RunTests(scheduler_ty *scheduler)
{
	static void (*TestFuncs[5]) (scheduler_ty *schedule);
	
	size_t i = 0;
	
	TestFuncs[0] = SchedulerCreateTest;
	TestFuncs[1] = SchedulerSortingTest;
	TestFuncs[2] = SchedulerClearTest;
	TestFuncs[3] = SchedulerRunTest;
	TestFuncs[4] = SchedulerDestroyTest;
	
	while(i < 5)
	{
		(*TestFuncs[i]) (scheduler);
		++i;
	}
}
/******************************************************************************/
