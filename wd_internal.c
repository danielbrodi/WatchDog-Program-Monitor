/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:						   								
* Version:				1.0		
* Description:			Watchdog's program internal functions.
\***********************************s*******************************************/

/******************************** Header Files ********************************/

#define _POSIX_SOURCE	/*	sigaction struct 				*/

#include <assert.h>
#include <errno.h>		/*	errno							*/
#include <signal.h>		/*	signals functions				*/
#include <stdio.h>      /*  puts							*/
#include <stdlib.h>     /*  exit                            */
#include <time.h>

#include <unistd.h>     /*  fork, getppid, sleep            */
#include <sys/types.h>	/*	pid_t							*/

#include "utils.h"		/*	prints colors, UNUSED			*/
#include "wd_internal.h"
#include "scheduler.h"

/***************************** Global Definitions *****************************/

/*	determines if the scheduler should stop, which means the WD should stop */
static volatile sig_atomic_t g_scheduler_should_stop = 0;

/*	counts amount of times that the WD did not receive a life signal	*/
static volatile sig_atomic_t g_counter_missed_signals = 0;

/* ID of the process which should to be signaled	*/
static volatile pid_t g_process_to_signal = 0;

/**************************** Forward Declarations ****************************/

int StartWDProcess(info_ty *info);

void *WDThreadSchedulerIMP(void *info);

int WDManageSchedulerIMP(info_ty *info);

oper_ret_ty OnIntervalSendSignalIMP(void *unused);

oper_ret_ty OnIntervalCheckIfMissIMP(void *info);

oper_ret_ty OnIntervalCheckIfDNR_IMP(void *scheduler_to_stop);

void handler_siguser2(int sig_id);

void handler_siguser1(int sig_id);

void SetSignalHandler(int signal, void(*handler_func)(int));

int IsProcessAliveIMP(pid_t process_to_check);

int TerminateProcessIMP(pid_t process_to_kill);

/************************* Functions  Implementations *************************/

enum {CHILD = 0};

/*	WDPCreate	function - start */
int StartWDProcess(info_ty *info)
{
	pid_t pid = 0;
	
	char *program_to_run = NULL;
	char **argv_to_run = NULL;
	
	/*	asserts */
	assert(info);
	
	if (info->i_am_wd)
	{
		program_to_run = "./userapp";
		argv_to_run = info->argv_for_wd + 1;
	}
	else
	{
		program_to_run = "./watchdog";
		argv_to_run = info->argv_for_wd;
	}
	
	/*	fork: 	*/
	pid = fork();
	
	/*	check if process was successfully created and return error otherwise */
	ReturnIfError(pid < 0, "Failed to fork a WD process\n", FAILURE);
	
	/*---------------------------------*/
	/*	if child: */
	if (CHILD == pid)
	{	
		/*	execv needed program	*/
		execvp(program_to_run, argv_to_run);
		
		/*	return (-1) if any errors */
		ReturnIfError(1, "Failed to execute the WatchDog program\n", FAILURE);
	}
	/*	end child 		*/
	/*---------------------------------*/
	
	/*---------------------------------*/
	/*	if parent:	*/
	else
	{
		sleep(1);
		
		/*	return child's pid */
		g_process_to_signal = pid;
		
		return (SUCCESS);
				
	/*	end parent */
	}
	/*---------------------------------*/
	
	return (FAILURE);
}
/******************************************************************************/
/*	manages WD scheduler - sends and checks for signals */
void *WDThreadSchedulerIMP(void *info)
{
	assert(info);
	
	return (NULL);
}
/*----------------------------------------------------------------------------*/
int WDManageSchedulerIMP(info_ty *info)
{
	int ret_status = 0;
	
	size_t signal_intervals = 0;
	size_t num_allowed_misses = 0;
	
	scheduler_ty *wd_scheduler = NULL;
	
	assert(info);
	
	signal_intervals = info->signal_intervals;
	num_allowed_misses = info->num_allowed_misses;
	
	assert(signal_intervals);
	assert(num_allowed_misses);
	
	/*	create scheduler	*/
	wd_scheduler = SchedulerCreate();
	ReturnIfError(!wd_scheduler, "Failed to create a WatchDog Scheduler!\n", 
																	FAILURE);
																	
	/*	add a scheduler task that sends signals */
	SchedulerAdd(wd_scheduler, OnIntervalSendSignalIMP, signal_intervals, NULL);
	
	/*	add a scheduler task that checks if there are signals from the process */
	SchedulerAdd(wd_scheduler, OnIntervalCheckIfMissIMP, signal_intervals, info);
															
	/*	add a scheduler task that checks the DNR status and stops scheduler 
	 *	if needed */
	SchedulerAdd(wd_scheduler, OnIntervalCheckIfDNR_IMP, 1, wd_scheduler);
	
	/*	scheduler run and check its return status */
	ret_status = SchedulerRun(wd_scheduler);
	
	/*	scheduler destroy */
	SchedulerDestroy(wd_scheduler);
	
	/*	return if scheduler has successfully finished */
	return (SUCCESS == ret_status ? SUCCESS : FAILURE);
}
/******************************************************************************/
oper_ret_ty OnIntervalSendSignalIMP(void *unused)
{	
	UNUSED(unused);
	
	/*	send SIGUSR1 to process_to_signal and handle errors if any */
	if (kill(g_process_to_signal, SIGUSR1))
	{
		return (OPER_FAILURE);
	}
	
	/*	keep signaling */
	return (NOT_DONE);
}
/******************************************************************************/
oper_ret_ty OnIntervalCheckIfMissIMP(void *info)
{
	sig_atomic_t num_allowed_misses = 0;
	
	assert(info);
	
	num_allowed_misses = ((info_ty *)info)->num_allowed_misses;
	
	assert(num_allowed_misses);
	
	/*	increment missed signals counter	*/
	__sync_fetch_and_add(&g_counter_missed_signals, 1);

	/*	if num_missed_signals equals num_allowed_fails : */
	if (num_allowed_misses == g_counter_missed_signals)
	{
		/*	restart process_to_watch using its original argv parameters  */
		if (FAILURE == TerminateProcessIMP(g_process_to_signal))
		{
			return (OPER_FAILURE);
		}
		
		if (-1 == StartWDProcess(info))
		{
			return (OPER_FAILURE);
		}
	}
	/*	end if reached num_allowed_fails */
	
	/*	keep checking */
	return (NOT_DONE);
}
/******************************************************************************/
oper_ret_ty OnIntervalCheckIfDNR_IMP(void *scheduler_to_stop)
{
	assert(scheduler_to_stop);
	
	/*	if DNR flag is on - stop the scheduler	*/
	if (g_scheduler_should_stop)
	{
		SchedulerStop((scheduler_ty *)scheduler_to_stop);
	}
	
	/*	keep checking */
	return (NOT_DONE);
}
/******************************************************************************/
int TerminateProcessIMP(pid_t process_to_kill)
{    
	assert(process_to_kill);
	
	/*---------------------------------------------------------*/
	/*	check if process is already does not exist */
	if (!IsProcessAliveIMP(process_to_kill))
	{
		return (SUCCESS);
	}
	/*---------------------------------------------------------*/
	/*	terminate process_to_kill	*/
	kill(process_to_kill, SIGTERM);
	
	if (!IsProcessAliveIMP(process_to_kill))
	{
		return (SUCCESS);
	}
	
	/*---------------------------------------------------------*/
	/*	if its still alive, SIGKILL it */
	kill(process_to_kill, SIGKILL);
	
	if (!IsProcessAliveIMP(process_to_kill))
	{
		return (SUCCESS);
	}
	/*---------------------------------------------------------*/
	
	return (FAILURE);
}
/******************************************************************************/
int IsProcessAliveIMP(pid_t process_to_check)
{
	time_t start_time = 0;
	time_t end_time = 0;
	time_t time_to_wait = 10; /* in seconds */
	
	assert(process_to_check);
	
	/*	check if its already dead */
	if (0 == kill(process_to_check, 0))
	{
		return (0);
	}
	
	start_time = time(0);
	end_time = start_time + time_to_wait;
	
	/*	verify its terminated	*/
	while (0 != kill(process_to_check, 0) && start_time < end_time)
	{
		sleep(1);
	}
	
	/*	check if it still exists */
	if (0 == kill(process_to_check, 0))
	{
		return (0);
	}
	
	return (1);
}
/******************************************************************************/
void SetSignalHandler(int signal, void(*handler_func)(int))
{
	struct sigaction signal_action = {0};
	
	/* 	clean the set of signals that aren’t permitted to
	*	interrupt execution of this handler.	*/
	ExitIfError(-1 == sigemptyset(&signal_action.sa_mask), 
								"Error: Could not empty set of signals\n", 1);

	signal_action.sa_flags = 0;

	/*	set the signal handler as the parameter given fuction	*/
	signal_action.sa_handler = handler_func;

	/*	register the user defined signal handler to handle the signal	*/
	/*	handle errors if any	*/
	ExitIfError(sigaction(signal, &signal_action, NULL) < 0, 
				"Error: Could not set register a sigaction handler\n", 1);
}
/******************************************************************************/
void handler_siguser1(int sig_id)
{
	UNUSED(sig_id);
	
	/*	reset counter of missed signals by XOR counter with itself */
	__sync_fetch_and_xor(&g_counter_missed_signals, g_counter_missed_signals);
	
	return;
}
/******************************************************************************/
void handler_siguser2(int sig_id)
{
	UNUSED(sig_id);
	
	/*	set DNR flag as 1 */
	__sync_fetch_and_add(&g_scheduler_should_stop, 1);
	
	return;
}
/******************************************************************************/
