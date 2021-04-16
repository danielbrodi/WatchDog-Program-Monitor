/*********************************FILE__HEADER*********************************\
* File:	pqueue.c						 		  								
* Author:	Daniel Brodsky					  								
* Date:	16/04/2021							   								
* Version:	1.0	(Pre-Review)						
* Reviewer:	Olga						   								
* Description:	Implementation of a task scheduler.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <assert.h>				/*	assert			*/
#include <stddef.h>				/*	size_t, NULL	*/
#include <stdlib.h>				/*	malloc, free	*/

#include "UID.h"				/*	UIDCreate, UIDIsEqual, UIDGetBadUID		*/
#include "utils.h"				/*	status_ty, boolean_ty					*/
#include "operation_func.h"		/*	operation_func_ty definition			*/

/***************************** Macros Definitions *****************************/
#define VOID_PTR_TO_TASK_PTR(void_ptr) ((task_ty *)void_ptr)
#define TIME_TO_INT(time) (int)(long)(time)
#define CURRENT_TIME time(0)
/**************************** Forward Declarations ****************************/
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
	scheduler_ty new_scheduler = (scheduler_ty *)malloc(sizeof(scheduler_ty));
	if (NULL == new_scheduler)
	{
		return (NULL);
	}
	
	new_scheduler->tasks = PqueueCreate(SortTasks);
	if (NULL == scheduler->tasks)
	{
		free(new_scheduler)
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
		if (!SchedulerIsEmpty(scheduler))
		{
			SchedulerClear(scheduler);
		}
			
		PqueueDestroy(scheduler->pqueue);
		scheduler->pqueue = NULL;
				
		free(scheduler);
		scheduler = NULL;
	}
}
/******************************************************************************/
ilrd_uid_ty SchedulerAdd(scheduler_ty *scheduler,
			operation_func_ty operation_func,  size_t interval, void *param)
{
	task_ty new_task = NULL;
	
	assert(scheduler);
	assert(operation_func);
	
	new_task = TaskCreate(operation_func, interval, CURRENT_TIME, param);

	/*	if the task was successfully created and added to the scheduler		*/
	if (new_task && (SUCCESS == PqueueEnqueue(scheduler->tasks, new_task)))
	{
		return (TaskGetUid(new_task))
	}
	else
	{
		return (UIDGetBadUID()); /*	failed to create or to add the task	*/
	}
}
/******************************************************************************/
status_ty SchedulerRemove(scheduler_ty *scheduler, ilrd_uid_ty uid)
{
	assert(scheduler);
	
	if (!UIDIsEqual(UIDGetBadUID, uid))	/* is the received UID valid?	*/ 
	{
		ilrd_uid_ty uid_to_remove = uid;
		void *task_to_remove = PqueueErase(scheduler->tasks,
													MatchUIDs, &uid_to_remove);

		if(NULL != task_to_remove)
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
	void *task_to_run = NULL

	while(!SchedulerIsEmpty(scheduler) && scheduler->to_stop == FALSE)
	{
		task_to_run = PqueueDequeue(scheduler->tasks);

		while (0 != sleep(TaskGetTimeToRun(task_to_run) - time(0))){}

		now it's the right time for the task to run:

		status = TaskRun(task_to_run);

		swtich(status)
		{
		case 1(NOT_DONE):
		TaskSetTimeToRun(task_to_run, time(0));
		PqueueEnqueue(scheduler->pqueue, task_to_run)
		if enqueue succeedeed:
		break;
		else:
		return SCH_FAILURE; (failed to enqueue the task)

		case 0(done):
		TaskDestroy(task_to_run)
		return SCH_FAILURE;

		case -1(failed):
		TaskDestroy(task_to_run)
		return FUNC_FAILURE; (failed to complete the task)
		break;
		}

		out of the loop, which means the scheduler is empty or has been stopped:

		if SchedulerIsEmpty:
		return FINISHED
		else
		return STOPPED
	}
}
/******************************************************************************/
void SchedulerStop(scheduler_ty *scheduler)
{

}
/******************************************************************************/ 
size_t SchedulerSize(const scheduler_ty *scheduler)
{

}
/******************************************************************************/
boolean_ty SchedulerIsEmpty(const scheduler_ty *scheduler)
{

}
/******************************************************************************/
void SchedulerClear(scheduler_ty *scheduler)
{

}
/******************************************************************************/
/*	Returns a positive value if data2 has an
	earlier expected time to run than data1	*/
int SortTasks(const void *data1, const void *data2)
{
	return	(TIME_TO_INT(VOID_PTR_TO_TASK_PTR(data1)->time_to_run -
									VOID_PTR_TO_TASK_PTR(data2)->time_to_run));
}
/******************************************************************************/
/* receive UIDs addresses and cast them to uids to be able to use UIDIsEqual */
bolean_ty MatchUIDs(const void *data1, const void *data2)
{
	return UIDIsEqual(((ilrd_uid_ty *)*data1), ((ilrd_uid_ty *)*data2));
}
/******************************************************************************/
