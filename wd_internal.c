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

#define _POSIX_C_SOURCE 199506L /*	sigaction struct 		*/
#define _XOPEN_SOURCE

#include <assert.h>		/*	assert							*/
#include <errno.h>		/*	errno							*/
#include <stdio.h>      /*  printf							*/
#include <time.h>		/*	time_t, time()					*/
#include <string.h>

#include <signal.h>		/*	signals functions				*/
#include <unistd.h>     /*  fork, getppid, sleep            */
#include <sys/types.h>	/*	pid_t							*/

#include "utils.h"		/*	print colors, UNUSED, ExitIfError, ReturnIfError */
#include "wd_internal.h"
#include "scheduler.h"	/*	scheduler's API functions */

/***************************** Global Definitions *****************************/

/*	determines if the scheduler should stop, which means the WD should stop */
static volatile sig_atomic_t g_scheduler_should_stop = 0;

/*	counts amount of times that the WD did not receive a life signal	*/
static volatile sig_atomic_t g_counter_missed_signals = 0;

/* ID of the process which should to be signaled	*/
static volatile pid_t g_process_to_signal = 0;

/**************************** Forward Declarations ****************************/

/*	Creates a new process and runs through it the Watch Dog program or the
 *	user's main program.
 *	The decesion is made by which of the two got terminated and needs to restart.
 *	Returns '0' if process was started successfully, otherwise returns '1'.
*/
int StartWDProcess(info_ty *info);

/*	Manages the Watch Dog operation of scheduling sending signals and checking
 *	whether the other side is responding or not.
 *	Uses a scheduler's program to synchornize the tasks.	*/
void *WDThreadSchedulerIMP(void *info);
int WDManageSchedulerIMP(info_ty *info);

/*	Scheduler's task which sends SIGUSR1 to the needed process each preiod of
 *	time */
oper_ret_ty OnIntervalSendSignalIMP(void *unused);

/*	Scheduler's task which checks if the maximum number of missed signals has
 *	been reached. If too many signals are missed - restarts the other program */
oper_ret_ty OnIntervalCheckIfMissIMP(void *info);

/*	Scheduler's task which regulary checks if a DNR request has been made and
 *	stops the WatchDog if needed */
oper_ret_ty OnIntervalCheckIfDNR_IMP(void *scheduler_to_stop);

/*	Sets a user defined function as a signal handler of a given signal	*/
void SetSignalHandler(int signal, void(*handler_func)(int));

/*	Signal handler for SIGUSR1 - resets the missed signals counter */
void handler_ResetErrorsCounter(int sig_id);

/*	Signal handler for SIGUSR2 - raises the DNR flag	*/
void handler_SetOnDNR(int sig_id);

/*	Checks if a process is alive and returns 1(alive) or 0(not alive).	*/
int IsProcessAliveIMP(pid_t process_to_check);

/*	Terminates a process and returns 0 if successfully terminated.
 *	A failure to terminate the process returns 1	*/ 
int TerminateProcessIMP(pid_t process_to_kill);

/*	Sets a process to be signaled */
void SetProcessToSignalIMP(pid_t pid);

/*	Returns PID Of the process to be signaled */
pid_t GetProcessToSignalIMP();

/************************* Functions  Implementations *************************/

/*	for improved readabillity	*/
enum {CHILD = 0};

/*	WDPCreate	function - start */
int StartWDProcess(info_ty *info)
{
	pid_t pid = 0;
	
	char *program_to_run = NULL;
	char **argv_to_run = NULL;
	
	/*	asserts */
	assert(info);
	
	/*	check if the caller of the function is the Watch Dog or the user
	 *	program. */
	if (info->i_am_wd)
	{
		printf(CYAN "%120s[wd %d] Trying to Fork and run User_APP\n" NORMAL, "", getpid());	
		program_to_run = "./user_app";
	}
	else
	{
		printf(CYAN "[app %d] Trying to Fork and Run WD\n" NORMAL, getpid());
		program_to_run = "./watchdog";
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
		argv_to_run = info->argv_for_wd;
		execvp(program_to_run, argv_to_run);
		
		/*	return (1) if any errors */
		ReturnIfError(1, "Failed to execute the WatchDog program\n", FAILURE);
	}
	/*	end child 		*/
	/*---------------------------------*/
	
	/*---------------------------------*/
	/*	if parent:	*/
	else
	{
		SetProcessToSignalIMP(pid);
		
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
	
	WDManageSchedulerIMP(info);
	
	free(info);
	
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
	if (((info_ty *)info)->i_am_wd)
	{
		printf(GREEN "%120s wd %d ", "", getpid());
	}
	else
	{
		printf(GREEN "\n app %d ", getpid());
	}
												
	/*	add a scheduler task that sends signals */
	SchedulerAdd(wd_scheduler, OnIntervalSendSignalIMP, signal_intervals, info);
	
	/*	add a scheduler task that checks if there are signals from the process */
	SchedulerAdd(wd_scheduler, OnIntervalCheckIfMissIMP, signal_intervals, info);
															
	/*	add a scheduler task that checks the DNR status and stops scheduler 
	 *	if needed */
	SchedulerAdd(wd_scheduler, OnIntervalCheckIfDNR_IMP, 1, wd_scheduler);
	
	/*	scheduler run and check its return status */
	ret_status = SchedulerRun(wd_scheduler);
	
	printf("RET STATUS : %d\n", ret_status);
	
	/*	scheduler destroy */
	SchedulerDestroy(wd_scheduler);
	
	/*	return if scheduler has successfully finished */
	return (SUCCESS == ret_status ? SUCCESS : FAILURE);
}
/******************************************************************************/
oper_ret_ty OnIntervalSendSignalIMP(void *info)
{	
	/*	send SIGUSR1 to process_to_signal and handle errors if any */
	if (kill(g_process_to_signal, SIGUSR1))
	{
		return (OPER_FAILURE);
	}
	
	if (((info_ty *)info)->i_am_wd)
	{
		printf(GREEN "%120sWatchDog[pid:%d] ", "", getpid());
	}
	else
	{
		printf(GREEN "\nUserApp[pid:%d] ", getpid());
	}
	printf("Sending signal to [pid:%d]\n" NORMAL, g_process_to_signal);	
		
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
	
	if (((info_ty *)info)->i_am_wd)
	{
		printf(RED "%120s[wd %d] ", "", getpid());
	}
	else
	{
		printf(RED "[app %d] ", getpid());
	}
	printf("[restart check] num of misses: %d\n" NORMAL, g_counter_missed_signals);
	
	/*	if num_missed_signals equals num_allowed_fails : */
	if (num_allowed_misses == g_counter_missed_signals)
	{
		/*	restart process_to_watch using its original argv parameters  */
		if (FAILURE == TerminateProcessIMP(g_process_to_signal))
		{
			return (OPER_FAILURE);
		}
		
		if (FAILURE == StartWDProcess(info))
		{
			return (OPER_FAILURE);
		}
		
		__sync_fetch_and_xor(&g_counter_missed_signals, g_counter_missed_signals);
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
	
	printf(YELLOW "%60sTrying to kill %d\n", "", process_to_kill);
	
	/*---------------------------------------------------------*/
	/*	check if process is already does not exist */
	if (!IsProcessAliveIMP(process_to_kill))
	{
		return (SUCCESS);
	}
	/*---------------------------------------------------------*/
	/*	terminate process_to_kill	*/
	if (kill(process_to_kill, SIGTERM))
	{
		fprintf(stderr, "%s\n", strerror(errno));
	}
	
	if (!IsProcessAliveIMP(process_to_kill))
	{
		return (SUCCESS);
	}
	
	/*---------------------------------------------------------*/
	/*	if its still alive, SIGKILL it */
	if (kill(process_to_kill, SIGKILL))
	{
		fprintf(stderr, "%s\n", strerror(errno));
	}
	
	if (!IsProcessAliveIMP(process_to_kill))
	{
		return (SUCCESS);
	}
	/*---------------------------------------------------------*/
	
	printf(WHITE "%60s%d was not successfully killed!\n", "", process_to_kill);
	return (FAILURE);
}
/******************************************************************************/
int IsProcessAliveIMP(pid_t process_to_check)
{
	time_t start_time = 0;
	time_t end_time = 0;
	time_t time_to_wait = 10; /* in seconds */
	
	assert(process_to_check);
	
	printf(YELLOW "%60sChecking if %d is alive\n", "", process_to_check);
	
	/*	check if its already dead */
	if (0 == kill(process_to_check, 0))
	{
		return (0);
	}
	
	start_time = time(0);
	end_time = start_time + time_to_wait;
	
	/*	give it time to be terminated	*/
	while (0 != kill(process_to_check, 0) && time(0) < end_time)
	{}
	
	/*	check if it still exists */
	if (0 == kill(process_to_check, 0))
	{
		return (0);
	}
	
	printf(WHITE "%60s%d is alive!\n", "", process_to_check);
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
void handler_ResetErrorsCounter(int sig_id)
{
	UNUSED(sig_id);
	printf(BLUE "%60s %d received signal after %d misses\n" NORMAL, "", getpid(), g_counter_missed_signals);
	/*	reset counter of missed signals by XOR counter with itself */
	__sync_fetch_and_xor(&g_counter_missed_signals, g_counter_missed_signals);
	return;
}
/******************************************************************************/
void handler_SetOnDNR(int sig_id)
{
	UNUSED(sig_id);
	printf(YELLOW "DNR IS ON\n");
	/*	set DNR flag as 1 */
	__sync_fetch_and_add(&g_scheduler_should_stop, 1);
	
	return;
}
/******************************************************************************/
void SetProcessToSignalIMP(pid_t pid)
{
	assert(pid);
	
	/*	set global variable of process to signal as a given pid */
	__sync_val_compare_and_swap(&g_process_to_signal, g_process_to_signal, pid);
	
	return;
}
/******************************************************************************/
pid_t GetProcessToSignalIMP()
{
	/*	returns PID Of the process to be signaled */
	return (g_process_to_signal);
}
/******************************************************************************/
