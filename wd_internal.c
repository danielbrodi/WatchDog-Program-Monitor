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

#define _POSIX_C_SOURCE	/*	sigaction struct 				*/

#include <signal.h>		/*	signals functions				*/

#include <unistd.h>		/*	nice */

#include <sys/time.h>		/*	getpriority	*/
#include <sys/resource.h>	/*	getpriority	*/

#include "scheduler.h"

/***************************** Global Definitions *****************************/

/*	determines if the scheduler should stop, which means the WD should stop */
static int g_scheduler_should_stop = 0;

/*	indicates if right signal from the watched app was received by the WD */
static int g_is_signal_received = 0;

/*	counts amount of times that the WD did not receive a signal from the app */
static int g_counter_missed_signals = 0;

/************************* Functions  Implementations *************************/
/*	WDPCreate	function - start */
/*	returns PID or (-1) if failure */
pid_t WDPCreate(int argc, char *argv[])
{
	pid_t pid = 0;
	
	int curr_priority = 0;
	
	/*	asserts */
	s
	/*	fork: 	*/
	pid = fork();
	
	/*	check if process was successfully created and return error otherwise */
	if (pid < 0)
	{
		return (-1);
	}
			/*---------------------------------*/
			/*	if child: */
			if (0 == pid)
			{
				/*	upgrade niceness by on 1	*/
				curr_priority = getpriority(PRIO_PROCESS,getpid());
				
				nice(curr_priority + 1);
				
				/*	execv WATCHDOG PROGRAM with CLI parameters	-*/
				execvp("./watchdog", argv + 1);
				
				/*	return (-1) if any errors */
				return (-1);
			}
			/*	end child 		*/
			/*---------------------------------*/
			
			/*---------------------------------*/
			/*	if parent:	*/
			else
			{
				/*	return child's pid */
				return (pid);
				
			/*	end parent */
			}
			/*---------------------------------*/
			
	return (-1);
}
/******************************************************************************/
/*	manages WD scheduler - sends and checks for signals */
/* TODO handle errors for each function in this part */
void *WDManageScheduler(void *info)
{	
	scheduler_ty *wd_scheduler = NULL;

	/*	asserts	*/
	assert(info);
	
	/*	create scheduler	*/
	wd_scheduler = SchedulerCreate();
	ExitIfError(!wd_scheduler, "Failed to create a WatchDog Scheduler!\n", -1);
	
	/*	create a scheduler task SendSignal */
	SchedulerAdd(wd_scheduler, SendSignal, info.signal_intervals, 
														info.process_to_watch);
	
	/*	create a scheduler task CheckIfSignalReceived */
	SchedulerAdd(wd_scheduler, CheckIfSignalReceived, info.signal_intervals + 5, 
																		info);
	/*	scheduler run */
	SchedulerRun(wd_scheduler);
	
	/*	scheduler destroy */
	SchedulerDestroy(wd_scheduler);
	
	/*	return */
	return (NULL);
}
/******************************************************************************/
void SigHandlerIMP(int sig_id)
{
	/*	increment global flag of received or not signal */
	++g_is_signal_received;
	
	return;
}
/******************************************************************************/
oper_ret_ty SendSignalIMP(void *process_to_signal)
{
	pid_t pid = 0;
	
	assert(process_to_signal);
	
	pid = *(pid_t *)process_to_signal;
	
	/*	send SIGUSR1 to process_to_signal and handle errors if any */
	if (kill(pid, SIGUSR1))
	{
		return (OPER_FAILURE);
	}
	
	return (DONE);
}
/******************************************************************************/
oper_ret_ty CheckIfSignalReceived(void *info)
{
	/*	check if the "received signal" flag is toggled */
	if (g_is_signal_received)
	{
		/*	if yes : decrement it and do nothing, continue. */
			--g_is_signal_received;
	}
	/*	if signal was not received:  increment num_missed_signals counter 	*/
	else
	{
		++num_missed_signals;
	}
	
	/*	if num_missed_signals equals num_allowed_fails : */
	if (num_missed_signals == info.num_allowed_fails)
	{
		/*	terminate process_to_watch process */
		/*	restart process_to_watch using argc argv parameters  */
		KillnRestartProcess(info.process_to_watch, /*TODO*/);
			
		/*	reset number_missed_signals counter */
		num_missed_signals = 0;
	}
		
	/*	end if reached num_allowed_fails */
	return (DONE);
}
/******************************************************************************/
void KillnRestartProcess(pid_t process_to_kill, char *argv[])
{
	pid_t pid = 0;
	
	assert(process_to_kill);
	
	/*	terminate process_to_kill	*/
	kill(process_to_kill, SIGTERM);
	
	/*	TODO verify its terminated	*/
	
	/*	fork: 	*/
	pid = fork();
	
	/*	handle fork errors */
	ExitIfError(pid < 0, "Failed to restart application!\n", -1);
	
	/*---------------------------------*/
	/*	if child: */
	if (0 == pid)
	{
		/*	execv WATCHDOG PROGRAM with argv	-*/
		execvp("./watchdog", argv + 1);
			
		/*	return (-1) if any errors */
		return (-1);
		
	/*	end child 		*/
	}
	/*---------------------------------*/
	
	/*	if parent:	*/
	else
	{
		/*	update process_to_watch TODO make it a ptr? maybe a struct including process & argv? */
		process_to_watch = pid;
		
		return;
		
	/*	end parent */
	}
	/*---------------------------------*/
}
/******************************************************************************/
void SetSignalHandler(int signal, void(*handler_func)(int))
{
	struct sigaction signal_action = {0};
	
	/* 	clean the set of signals that aren’t permitted to
	*	interrupt execution of this handler.	*/
	ExitIfBad(-1 == sigemptyset(&signal_action.sa_mask), 
								"Error: Could not empty set of signals\n", 1);

	signal_action.sa_flags = 0;

	/*	set the signal handler as the parameter given fuction	*/
	signal_action.sa_handler = handler_func;

	/*	register the user defined signal handler to handle the signal	*/
	/*	handle errors if any	*/
	ExitIfBad(sigaction(signal, &signal_action, NULL) < 0, 
				"Error: Could not set register a sigaction handler\n", 1);
}
/******************************************************************************/
