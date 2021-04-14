/***********************************************
* File: task.h				    			 	*
* Date: 13/04/2021							    *
* Version: 1.2 								    *
* Description:  Task API     	    	        * 
************************************************/
#ifndef	__TASK_H__
#define	__TASK_H__

#include <stddef.h>     /*  size_t  */
#include <time.h>       /*  time_t  */

#include "operation_func.h"
#include "UID.h"

typedef struct task task_ty;

/* Creates an empty task and returns pointer to it */
/* Returns NULL on failure */
/* Complexity: O(1) */
task_ty *TaskCreate(operation_func_ty func, size_t interval, time_t time_to_run,
																void *param);

/* Frees task                                         */
/* No operations will be performed if pointer is NULL */
/* Complexity: O(1)                                   */
void TaskDestroy(task_ty *task)

/* Runs the task's operation function */
/* Function returns enum:             */
/* -1 FAILURE                         */
/*  0 DONE                            */
/*  1 NOT DONE                        */
/* Complexity: O(1)                   */
oper_ret_ty TaskRun(task_ty *task);

/* Returns time to run from task */
/* Complexity: O(1) */
time_t TaskGetTimeToRun(const task_ty *task);

/* Returns UID from task */
/* Complexity: O(1) */
ilrd_uid_ty TaskGetUid(const task_ty *task);

/* Sets time_to_run of task */
/* Complexity: O(1) */
void TaskSetTimeToRun(task_ty *task, time_t new_time);

#endif /* __TASK_H__ */
