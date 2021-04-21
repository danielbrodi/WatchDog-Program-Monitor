/*********************************FILE__HEADER*********************************\
* File:			scheduler.c						 		  								
* Author:		Daniel Brodsky					  								
* Date:			16/04/2021							   								
* Version:		1.0	(Reviewed)			
* Reviewer:		Olga						   								
* Description:	Implementation of a task scheduler.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <assert.h>				/*	assert			*/
#include <stddef.h>				/*	size_t, NULL	*/
#include <stdlib.h>				/*	malloc, free	*/
#include <time.h> 				/*	time_t, time() 	*/
#include <unistd.h>				/*	sleep			*/

#include "pqueue.h"				/*	priority queue structure API wrapper	*/
#include "UID.h"				/*	UIDIsEqual, UIDGetBadUID, ilrd_uid_ty	*/
#include "utils.h"				/*	status_ty, boolean_ty					*/
#include "task.h"				/*	scheduler's tasks implementation		*/
#include "operation_func.h"		/*	operation_func_ty definition			*/
#include "scheduler.h"

/***************************** Macros Definitions *****************************/
#define VOID_PTR_TO_TASK_PTR(void_ptr) ((task_ty *)void_ptr)
#define TIME_TO_INT(time) (int)(long)(time)
#define CURRENT_TIME time(0)

/**************************** Forward Declarations ****************************/
static int SortTasks(const void *task1, const void *task2);
static boolean_ty MatchUIDs(const void *task, const void *uid);

/***************************** Struct__Definition *****************************/
struct scheduler
{
	p_queue_ty *tasks;				/*	a queue which stores the
										scheduler's tasks and is sorted 
										by their expected time to run.	*/
											
	boolean_ty to_stop;				/*	a flag which determines if the 
										scheduler should be stopped.	*/
};
/******************************************************************************/
/************************* Functions__Implementations *************************/
/******************************************************************************/
scheduler_ty *SchedulerCreate(void)
{
	scheduler_ty *new_scheduler = (scheduler_ty *)malloc(sizeof(scheduler_ty));
	if (NULL == new_scheduler)
	{
		return (NULL);
	}
	
	new_scheduler->tasks = PqueueCreate(SortTasks);
	if (NULL == new_scheduler->tasks)
	{
		free(new_scheduler);
		new_scheduler = NULL;
		
		return (NULL);
	}
	
	new_scheduler->to_stop = FALSE;
	
	return (new_scheduler);
}
/******************************************************************************/
void SchedulerDestroy(scheduler_ty *scheduler)
{
	if (NULL != scheduler)
	{
		SchedulerClear(scheduler);
				
		PqueueDestroy(scheduler->tasks);
		scheduler->tasks = NULL;
				
		free(scheduler);
		scheduler = NULL;
	}
}
/******************************************************************************/
ilrd_uid_ty SchedulerAdd(scheduler_ty *scheduler,
			operation_func_ty operation_func,  size_t interval, void *param)
{
	task_ty *new_task = NULL;
	
	assert(scheduler);
	assert(operation_func);
	
	new_task = TaskCreate(operation_func, interval, CURRENT_TIME, param);

	/*	if the task was successfully created and added to the scheduler		*/
	if (new_task && (SUCCESS == PqueueEnqueue(scheduler->tasks, new_task)))
	{
		return (TaskGetUid(new_task));
	}
	else
	{
		/*	free the memory that was allocated to the task to avoid leaks	*/
		TaskDestroy(new_task);
		return (UIDGetBadUID()); /*	failed to create or to add the task	*/
	}
}
/******************************************************************************/
status_ty SchedulerRemove(scheduler_ty *scheduler, ilrd_uid_ty uid)
{
	ilrd_uid_ty uid_to_remove = uid;
	
	assert(scheduler);
	assert(!SchedulerIsEmpty(scheduler));
	
	if (!UIDIsEqual(UIDGetBadUID(), uid))	/* is the received UID valid?	*/ 
	{
		void *task_to_remove = PqueueErase(scheduler->tasks,
													MatchUIDs, &uid_to_remove);

		if(NULL != task_to_remove) /* task has been found in the scheduler	*/
		{		
			TaskDestroy(task_to_remove);
			return (SUCCESS);	/* the task was successfully removed	*/
		}
	}
	
	return (FAILURE);
}
/******************************************************************************/
run_status_ty SchedulerRun(scheduler_ty *scheduler)
{
	task_ty *task_to_run = NULL;
	oper_ret_ty ret_status = NOT_DONE;
	time_t time_to_sleep = 0;
	
	assert(scheduler);

	scheduler->to_stop = FALSE;
	
	while(!SchedulerIsEmpty(scheduler) && FALSE == scheduler->to_stop)
	{
		task_to_run = (task_ty *)PqueueDequeue(scheduler->tasks);
		time_to_sleep = TaskGetTimeToRun(task_to_run) - CURRENT_TIME;
		
		/*	check if the expected time to run of the task is yet to come
			and pause the process until the time is right	*/
		while (time_to_sleep)
		{
			time_to_sleep = sleep(time_to_sleep);
		}
		
		ret_status = TaskRun(task_to_run);

		switch(ret_status)
		{
			/* in case the task is not done yet, return it to the scheduler
				with a new time to excute it	*/
			case NOT_DONE:
				TaskSetTimeToRun(task_to_run, CURRENT_TIME);
				if (SUCCESS == PqueueEnqueue(scheduler->tasks, task_to_run))
				{
					/*	task was successfully re-enqueued with a new time	*/
					break;
				}
				else
				{
					return (SCH_FAILURE);
				}
			/* in case the task is done, keep it removed and clear the memory
				it takes and continue the run of the scheduler	*/
			case DONE:
				TaskDestroy(task_to_run);
				break;
			/* in case the excuted operation by the task has been failed,
				remove it from the memory and return a failure message	*/
			case OPER_FAILURE:
				TaskDestroy(task_to_run);
				return (FUNC_FAILURE);
		}	
	}
	
	if (SchedulerIsEmpty(scheduler))
		{
			return	(FINISHED);
		}
	else
		{
			return (STOPPED);
		}
}
/******************************************************************************/
void SchedulerStop(scheduler_ty *scheduler)
{
	assert(scheduler);
	
	scheduler->to_stop = TRUE;
}
/******************************************************************************/ 
size_t SchedulerSize(const scheduler_ty *scheduler)
{
	assert(scheduler);
	
	return (PqueueSize(scheduler->tasks));
}
/******************************************************************************/
boolean_ty SchedulerIsEmpty(const scheduler_ty *scheduler)
{
	assert(scheduler);
	
	return (PqueueIsEmpty(scheduler->tasks));
}
/******************************************************************************/
void SchedulerClear(scheduler_ty *scheduler)
{
	assert(scheduler);
	
	while (!SchedulerIsEmpty(scheduler))
	{
		TaskDestroy(PqueueDequeue(scheduler->tasks));
	}
}
/******************************************************************************/
/*	Returns a positive value if task2 has an
	earlier expected time to run than task1	*/
static int SortTasks(const void *task1, const void *task2)
{
	time_t expected_time_task1 = TaskGetTimeToRun(VOID_PTR_TO_TASK_PTR(task1));
	time_t expected_time_task2 = TaskGetTimeToRun(VOID_PTR_TO_TASK_PTR(task2));
	
	return	(TIME_TO_INT(expected_time_task2 - expected_time_task1));
}
/******************************************************************************/
/* first paramater is each element's data in the scheduler which means a task.
	second paramater is an address of a valid uid	*/
static boolean_ty MatchUIDs(const void *task, const void *uid)
{
	ilrd_uid_ty uid_of_task = TaskGetUid(task);
	ilrd_uid_ty uid_to_find = *(ilrd_uid_ty *)uid;
	
	return (UIDIsEqual(uid_of_task, uid_to_find));
}
/******************************************************************************/
