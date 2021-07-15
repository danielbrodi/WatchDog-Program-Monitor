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

#include "scheduler.h";

/***************************** Global Definitions *****************************/

/*	determines if the scheduler should stop, which means the WD should stop */
static volatile sig_atomic_t g_scheduler_should_stop = 0;

/*	counts amount of times that the WD did not receive a life signal	*/
static volatile sig_atomic_t g_counter_missed_signals = 0;

/* ID of the process which should to be signaled	*/
static volatile pid_t g_process_to_signal = 0;

/************************* Functions  Implementations *************************/

enum {CHILD = 0};

/*	WDPCreate	function - start */
pid_t WDPCreateIMP(int argc, char *argv[])
{
	char *argv_for_wd = NULL;
	
	pid_t pid = 0;
	
	/*	asserts */
	assert(argc);
	assert(argv);
	
	/* copy argv and attach wd_app_name to the beginning */
	/*	TODO 	SWAP BETWEEN THE ARGS */
	argv_for_wd = (char **)malloc(sizeof(argc) * sizeof(char *));
	ReturnIfError(NULL == argv_for_wd, "Failed to create argv array\n", -1);
	
	/*	fork: 	*/
	pid = fork();
	
	/*	check if process was successfully created and return error otherwise */
	ReturnIfError(pid < 0, "Failed to fork a WD process\n", -1);
	
	/*---------------------------------*/
	/*	if child: */
	if (CHILD == pid)
	{	
		/*	execv WATCHDOG PROGRAM with CLI parameters	-*/
		execvp("/.watchdog", argv_for_wd);
		
		/*	return (-1) if any errors */
		ReturnIfError(1, "Failed to execute the WatchDog program\n", -1);
	}
	/*	end child 		*/
	/*---------------------------------*/
	
	/*---------------------------------*/
	/*	if parent:	*/
	else
	{
		sleep(1);
		
		free(argv);		
			
		/*	return child's pid */
		return (pid);
				
	/*	end parent */
	}
	/*---------------------------------*/
}
/******************************************************************************/

enum {SUCCESS = 0, FAILURE = 1};

/*	manages WD scheduler - sends and checks for signals */
/* TODO handle errors for each function in this part */
void *WDThreadSchedulerIMP(void *info)
{
	assert(info);
	
	return (SUCCESS == WDManageScheduler(info->signal_intervals, 
								info->num_allowed_misses) : ("SUCCESS") : NULL);
}

int WDManageSchedulerIMP(size_t signal_intervals, size_t num_allowed_misses)
{
	int ret_status = 0;
	
	scheduler_ty *wd_scheduler = NULL;
	
	typedef struct check_info
	{
		scheduler_ty *scheduler;
		size_t num_allowed_misses;
	}check_info_ty;
	
	check_info_ty info = {0};
	
	assert(num_allowed_misses);
	
	/*	create scheduler	*/
	wd_scheduler = SchedulerCreate();
	ReturnIfError(!wd_scheduler, "Failed to create a WatchDog Scheduler!\n", 
																	FAILURE);
																	
	info.scheduler = wd_scheduler;
	info.num_allowed_misses = num_allowed_misses;
	
	/*	create a scheduler task SendSignal */
	SchedulerAdd(wd_scheduler, OnIntervalSendSignalIMP, signal_intervals,
																	scheduler);
	
	/*	create a scheduler task CheckIfSignalReceived */
	SchedulerAdd(wd_scheduler, OnIntervalCheckIfMissIMP, signal_intervals, info);
	/*	scheduler run */
	ret_status = SchedulerRun(wd_scheduler);
	
	/*	scheduler destroy */
	SchedulerDestroy(wd_scheduler);
	
	/*	return */
	return (SUCCESS == ret_status ? SUCCESS : FAILURE);
}
/******************************************************************************/
oper_ret_ty OnIntervalSendSignalIMP(void *scheduler)
{	
	/*	send SIGUSR1 to process_to_signal and handle errors if any */
	if (kill(g_process_to_signal, SIGUSR1))
	{
		return (OPER_FAILURE);
	}
	
	/*	if DNR flag is on - stop the scheduler	*/
	if (g_scheduler_should_stop)
	{
		SchedulerStop(scheduler);
	}
	
	return (NOT_DONE);
}
/******************************************************************************/
oper_ret_ty OnIntervalCheckIfMissIMP(void *info)
{
	assert(info);
	
	/*	increment missed signals counter	*/
	__sync_fetch_and_add(&g_counter_missed_signals, 1);

	/*	if num_missed_signals equals num_allowed_fails : */
	if (g_counter_missed_signals == (info->num_allowed_misses)
	{
		/*	terminate process_to_watch process */
		/*	restart process_to_watch using argc argv parameters  */
		RestartProcessIMP();
	}
	/*	end if reached num_allowed_fails */
	
	/*	if DNR flag is on - stop the scheduler	*/
	if (g_scheduler_should_stop)
	{
		SchedulerStop(info->scheduler);
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
	/*	reset counter of missed signals by XOR counter with itself */
	__sync_fetch_and_xor(&g_counter_missed_signals, g_counter_missed_signals);
	
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
