/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					11-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:						   								
* Version:				1.0		
* Description:			Watchdog's program design pseudo-code. 
						Watchdog should protect a client's program from being
						terminated and restart it if needed. 
						The client's program should also protect the watchdog
						program from being terminated and also
						restart it if needed.
\******************************************************************************/

/******************************** Header Files ********************************/

#define _XOPEN_SOURCE

#include <assert.h>		/*	assert	*/	
#include <stddef.h>		/*	size_t, NULL	*/
#include <stdio.h>
#include <stdlib.h>		/*	setenv, getenv	*/
#include <string.h>

#include <pthread.h>	/*	pthread_create, pthread_t	*/
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>	/*	pid_t			*/

#include "utils.h"
#include "wd_internal.h"
#include "wd_internal.c"
#include "watchdog.h"

/***************************** Global Definitions *****************************/

static pthread_t g_wd_thread = 0;

/************************* Functions  Implementations *************************/	

void KeepMeAlive(int argc, char *argv[], size_t signal_intervals,
													size_t num_allowed_misses)
{
	info_ty wd_info = {0};
	
	char **argv_for_wd = NULL;
	
	/*	stores values of num_allowed_misses and signal_intervals as env vars */
	char env_num_allowed_misses[120] = {'\0'};
	char env_signal_intervals[120] = {'\0'};
		
	info_ty *info = &wd_info;
	
	/*	asserts */
	assert(signal_intervals);
	assert(num_allowed_misses);
	
	/*	register SIGUSR1 signal handler to manage received signals status */
	SetSignalHandler(SIGUSR1, handler_siguser1);
	
	/*	set ENV variables of num_allowed_misses and signal_intervals */
	sprintf(env_signal_intervals, "SIGNAL_INTERVAL=%ld", signal_intervals);
	putenv(env_signal_intervals);
	
	sprintf(env_num_allowed_misses, "NUM_ALLOWED_FAILURES=%ld", 
															num_allowed_misses);
	putenv(env_num_allowed_misses);
	
	/* copy argv and attach wd_app_name to the beginning */
	argv_for_wd = (char **)(calloc(argc + 2, sizeof(char *)));
	ReturnIfError(NULL == argv_for_wd, "Failed to create argv array\n", -1);
	
	argv_for_wd[0] = "./watchdog";
	
	memcpy(argv_for_wd + 1, argv, argc);
	
	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info.argv_for_wd = argv_for_wd;
	wd_info.num_allowed_misses = num_allowed_misses;
	wd_info.signal_intervals = signal_intervals;
	wd_info.i_am_wd = 0;
	
	/*	check if there is already a watch dog (by an env variable): */
		/*	if yes - check its pid */
		/*	if no - create a new process and run WD and get its pid */
	if (getenv("WD_IS_ON"))
	{
		g_process_to_signal = getppid();
	}
	else
	{
		printf(GREEN "[app] WD DOES NOT EXIST - LAUNCHING\n" NORMAL );
		StartWDProcess(info);
	}
	
	printf(CYAN "[app] P TO SIGNAL: %d\n" NORMAL, g_process_to_signal);
	ReturnIfError(g_process_to_signal <= 0, 
								"[app] Failed to create watch dog process!\n", -1);
	
	/*	create a thread that will use a scheduler
	 *	to communicate with the Watch Dog process */
	 /*	handle errors*/
	ReturnIfError(pthread_create(&g_wd_thread, NULL, WDThreadSchedulerIMP,
								info),"[app] Failed to create a WD thread\n", -1);
										
	/*	return success */
	return;
}
/******************************************************************************/
/*	DNR function - start */
int DNR(void)
{
	/*	set DNR flag as 1 */
	__sync_fetch_and_add(&g_scheduler_should_stop, 1);
	
	/* verify the watch dog is indeed terminated	*/
	if (1 == IsProcessAliveIMP(g_process_to_signal))
	{
		fprintf(stderr, "Failed to destroy the WD\n");
		return (FAILURE);
	}

	if (pthread_join(g_wd_thread, NULL))
	{
		fprintf(stderr, "Failed to finish WD thread\n");
		return (FAILURE);
	}
	
	return (SUCCESS);
	
/*	DNR function - end */
}
/******************************************************************************/
