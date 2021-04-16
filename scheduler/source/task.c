/*********************************FILE__HEADER*********************************\
* File:			task.c						 		  								
* Author:		Daniel Brodsky					  								
* Date:			16/04/2021							   								
* Version:		1.0	(Pre-Review)						
* Reviewer:		Olga						   								
* Description:	Implementation of a task element inside of a task scheduler.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <assert.h>				/*	assert			*/
#include <stddef.h>				/*	size_t, NULL	*/
#include <stdlib.h>				/*	malloc, free	*/

#include "UID.h"				/*	UIDCreate, UIDIsEqual, UIDGetBadUID		*/
#include "utils.h"				/*	status_ty, boolean_ty					*/
#include "operation_func.h"		/*	operation_func_ty definition			*/

/***************************** Struct__Definition *****************************/
struct task
{
	oper_ret_ty func;	/*	the function that will be excuted by the task	  */
	size_t interval;	/*	how often should the task run, in seconds		  */	
	time_t time_to_run;	/*	on what time should the function be first excuted */
	ilrd_uid_ty uid;	/*	UID which is exclusively created for each task	  */
	void *param;
};
/******************************************************************************/
/************************* Functions__Implementations *************************/
/******************************************************************************/
task_ty *TaskCreate(operation_func_ty func, size_t interval,
					 						time_t time_to_run, void *param)
{
	assert(func); /* a task without a valid function to excute is meaningless */
	
	task_ty *new_task = (task_ty *)malloc(task_ty);
	if (NULL == new_task)
	{
		return (NULL);
	}
	
	task->uid = UIDCreate();
	if (UIDIsEqual(task->uid, UIDGetBadUID))
	{
		free(new_task);
		new_task == NULL;
		
		return (NULL);
	}
	
	task->interval = interval;
	task->time_to_run = time_to_run;
	task->param = param;
	
	return (new_task);	
	
}
/******************************************************************************/
void TaskDestroy(task_ty *task)
{
	if (NULL != task)
	{
		free(task);
		task = NULL;
	}
}
/******************************************************************************/
oper_ret_ty TaskRun(task_ty *task)
{
	assert(task);
	
	return (task->func);
}
/******************************************************************************/

time_t TaskGetTimeToRun(const task_ty *task)
{
	assert(task);
	
	return (task->time_to_run);
}
/******************************************************************************/

ilrd_uid_ty TaskGetUid(const task_ty *task)
{
	assert(task);
	
	return (task->uid);
}
/******************************************************************************/

void TaskSetTimeToRun(task_ty *task, time_t new_time)
{
	assert(task);
	
	task->time_to_run = task->interval + new_time;
}
/******************************************************************************/
