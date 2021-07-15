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

#include "scheduler.h"

/***************************** Global Definitions *****************************/

/*	determines if the scheduler should stop, which means the WD should stop */
static volatile int g_scheduler_should_stop = 0;

/*	counts amount of times that the WD did not receive a life signal	*/
static volatile int g_counter_missed_signals = 0;

/* ID of the process which should to be signaled	*/
static volatile pid_t g_process_to_signal = 0;

/************************* Functions  Implementations *************************/
/*	WDPCreate	function - start */
pid_t WDPCreate(char *argv[])
{
	pid_t pid = 0;
	
	/*	asserts */
	assert(argv);
	
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
				/*	execv WATCHDOG PROGRAM with CLI parameters	-*/
				execvp(argv[1], argv);
				
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
void *WDManageScheduler()
{	
	scheduler_ty *wd_scheduler = NULL;

	/*	asserts	*/
	assert(info);
	
	/*	create scheduler	*/
	wd_scheduler = SchedulerCreate();
	ExitIfError(!wd_scheduler, "Failed to create a WatchDog Scheduler!\n", -1);
	
	/*	create a scheduler task SendSignal */
	SchedulerAdd(wd_scheduler, SendSignalIMP, atoi(getenv(env_signal_intervals)), 
														NULL);
	
	/*	create a scheduler task CheckIfSignalReceived */
	SchedulerAdd(wd_scheduler, CheckIfSignalReceived, 
								atoi(getenv(env_signal_intervals)), info);
	/*	scheduler run */
	SchedulerRun(wd_scheduler);
	
	/*	scheduler destroy */
	SchedulerDestroy(wd_scheduler);
	
	/*	return */
	return (NULL);
}
/******************************************************************************/
oper_ret_ty SendSignalIMP(void *unused)
{	
	/*	send SIGUSR1 to process_to_signal and handle errors if any */
	if (kill(g_process_to_signal, SIGUSR1))
	{
		return (OPER_FAILURE);
	}
	
	/*	if DNR flag is on - finish the task	*/
	if (g_scheduler_should_stop)
	{
		return (DONE);
	}
	
	return (NOT_DONE);
}
/******************************************************************************/
oper_ret_ty CheckIfSignalReceived(void *info)
{	
	/*	if num_missed_signals equals num_allowed_fails : */
	if (g_counter_missed_signals == atoi(getenv(env_num_allowed_misses)));
	{
		/*	terminate process_to_watch process */
		/*	restart process_to_watch using argc argv parameters  */
		KillnRestartProcess(g_process_to_signal);
			
		/*	reset number_missed_signals counter */
		num_missed_signals = 0;
	}
	/*	end if reached num_allowed_fails */
	
	/*	if DNR flag is on - finish the task	*/
	if (g_scheduler_should_stop)
	{
		return (DONE);
	}
	
	return (NOT_DONE);
}
/******************************************************************************/
void KillnRestartProcess(pid_t process_to_restart, char *argv[])
{
	process_to_kill = process_to_restart;
		
	/*	terminate process_to_kill	*/
	kill(process_to_kill, SIGTERM);
	
	/*	verify its terminated	*/
	while (0 != kill(process_to_kill, 0)){};
	
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
		g_process_to_signal = pid;
		
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
void handler_siguser1(int sig_id)
{
	/*	increment global flag of received or not signal */
	__sync_fetch_and_add(&g_counter_missed_signals, 1);
	
	return;
}
/******************************************************************************/
void handler_siguser2(int sig_id)
{
	/*	set DNR flag as 1 */
	__sync_fetch_and_add(&g_scheduler_should_stop, 1);
	
	return;
}
/******************************************************************************/
