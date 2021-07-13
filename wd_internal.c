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

#include <sys/time.h>	/*	getpriority	*/
#include <sys/resource.h>	/*	getpriority	*/

#include "scheduler.h"


/***************************** Global Definitions *****************************/



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
				/*	do nothing */
				return;
				
			/*	end parent */
			}
			/*---------------------------------*/
	
/*	KeepMeAliveIMP function - end	*/
}
/******************************************************************************/
/*	manages WD scheduler - sends and checks for signals */
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
/*	signal handler function - start 	*/
void SigHandlerIMP(int sig_id)
{
	/*	increment global flag of received or not signal */
	
/*	signal handler function - end	*/	
}
/******************************************************************************/
oper_ret_ty SendSignal(void *process_to_signal)
{
	/*	send SIGUSR1 to process_to_watch every X seconds */
}
/******************************************************************************/
oper_ret_ty CheckIfSignalReceived(void *info)
{
	/*	create a counter of num of missed signals */

	/*	check if the "received signal" flag is toggled */
		/*	if yes : decrement it and do nothing, continue.
			if not:  increment num_missed_signals counter 	*/
			
	/*	if num_missed_signals equals num_allowed_fails : */

		/*	terminate process_to_watch process */

		/*	restart process_to_watch using argc argv parameters  */

		/*	reset number_missed_signals counter */
	
	/*	end if reached num_allowed_fails */
}
/******************************************************************************/
void KillnRestartProcess(pid_t process_to_kill, char *argv[])
{
	/*	terminate process_to_kill	*/
	
	/*	verify its terminated	*/
	
	/*	fork: 	*/
		
	/*---------------------------------*/
	/*	if child: */
		
		/*	execv WATCHDOG PROGRAM with argv	-*/
		
		/*	return (-1) if any errors */
		
	/*	end child 		*/
	/*---------------------------------*/
	
	/*	if parent:	*/
	
		/*	update process_to_watch */
		
	/*	end parent */
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
