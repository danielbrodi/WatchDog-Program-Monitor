/************************************************
* File: scheduler.h							 	*
* Date: 13/04/2021							    *
* Version: 1.4 								    *
* Description:  Scheduler API     		        * 
************************************************/
#ifndef	__SCHEDULER_H__
#define	__SCHEDULER_H__

#include <stddef.h>     /*  size_t   */

#include "operation_func.h"
#include "UID.h"
#include "utils.h"

typedef enum
{
    FINISHED,
	STOPPED,
    SCH_FAILURE,
    FUNC_FAILURE
} run_status_ty;

typedef struct scheduler scheduler_ty;

/* Creates an empty scheduler and returns pointer to handler struct */
/* Returns NULL on failure                                          */
/* Complexity: O(1)                                                 */
scheduler_ty *SchedulerCreate(void);

/* Frees entire scheduler                             */
/* No operations will be performed if pointer is NULL */
/* Complexity: O(n)                                   */
void SchedulerDestroy(scheduler_ty *scheduler);

/* Add task to the scheduler                                    */
/* Function returns enum:                                       */
/* Return value: The inserted new UID                           */
/* 				 On failure - Bad UID {0,0,0}                   */
/* Complexity: O(n)                                             */
ilrd_uid_ty SchedulerAdd(scheduler_ty *scheduler,  
			operation_func_ty operation_func,  size_t interval, void *param);

/* Removes the task with the UID from the scheduler */
/* Returns SUCCESS or FAILURE                       */
/* Undefined if scheduler is empty                  */
/* Complexity: O(n)                                 */
status_ty SchedulerRemove(scheduler_ty *scheduler, ilrd_uid_ty uid);

/* Runs all the tasks according to their defined time intervals  */
/*  0 FINISHED                                                   */
/*  1 STOPPED                                                    */
/*  2 SCH_FAILURE                                                */
/*  3 FUNC_FAILURE                                               */
/* Complexity: O(n)                                              */
run_status_ty SchedulerRun(scheduler_ty *scheduler);

/* Signals the scheduler to stop */
/* Complexity: O(1)              */
void SchedulerStop(scheduler_ty *scheduler); 

/* Returns number of tasks in the scheduler */
/* Complexity: O(n)                         */
size_t SchedulerSize(const scheduler_ty *scheduler);

/* Returns TRUE if scheduler is empty and FALSE otherwise */
/* Complexity: O(1)                                       */
boolean_ty SchedulerIsEmpty(const scheduler_ty *scheduler);

/* Clears all tasks from the scheduler */
/* Complexity: O(n)                    */
void SchedulerClear(scheduler_ty *scheduler);

#endif	/* __SCHEDULER_H__ */
