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

#include "../../ds/include/scheduler.h"
#include "../../ds/source/scheduler.c"
#include "../../ds/include/operation_func.h"

/******************************** Header Files ********************************/

#define _POSIX_SOURCE

#include <assert.h>		/*	assert	*/
#include <pthread.h>	/*	pthread_create, pthread_t	*/
#include <stddef.h>		/*	size_t, NULL	*/
#include <stdio.h>		/*	sprintf			*/
#include <stdlib.h>		/*	setenv, getenv	*/
#include <sys/types.h>	/*	pid_t			*/

#include "wd_internal.h"
#include "utils.h"		/*	ExitIfError, UNUSED	*/

/***************************** Global Definitions *****************************/

static pthread_t g_wd_thread = 0;

/************************* Functions  Implementations *************************/	

void KeepMeAlive(int argc, char *argv[], size_t signal_intervals,
													size_t num_allowed_misses)
{
	info_ty wd_info = {0};
	
	char **argv_for_wd = NULL;
	
	/*	stores values of num_allowed_misses and signal_intervals as env vars */
	char env_num_allowed_misses[8] = {0};
	char env_signal_intervals[8] = {0};
		
	/*	asserts */
	assert(signal_intervals);
	assert(num_allowed_misses);
	
	/*	register SIGUSR1 signal handler to manage received signals status */
	SetSignalHandler(SIGUSR1, handle_siguser1);
	
	/*	set ENV variables of num_allowed_misses and signal_intervals */
	snprintf(env_signal_intervals, sizeof(size_t), "%ld", signal_intervals);
	setenv("SIGNAL_INTERVAL", env_signal_intervals);
	
	snprintf(env_num_allowed_misses, sizeof(size_t), "%ld", num_allowed_misses);
	setenv("NUM_ALLOWED_FAILURES", env_num_allowed_misses);
	
	/* copy argv and attach wd_app_name to the beginning */
	argv_for_wd = (char **)malloc((argc + 2) * sizeof(char *));
	ReturnIfError(NULL == argv_for_wd, "Failed to create argv array\n", -1);
	
	argv_for_wd[0] = "./watchdog";
	
	memcpy(argv_for_wd + 1, argv, argc);
	
	/*	check if there is already a watch dog (by an env variable): */
		/*	if yes - check its pid */
		/*	if no - create a new process and run WD and get its pid */
	if (getenv("WD_STATUS"))
	{
		g_process_to_signal = getppid();
	}
	else
	{
		WDPCreate(argv_for_wd, 0);
	}
	
	ReturnIfError(g_process_to_signal <= 0, 
								"Failed to create watch dog process!\n", -1);
	
	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info.argv_for_wd = argv_for_wd;
	wd_info.num_allowed_misses = num_allowed_misses;
	wd_info.signal_intervals = signal_intervals;
	
	/*	create a thread that will use a scheduler
	 *	to communicate with the Watch Dog process */
	 /*	handle errors*/
	ReturnIfError(pthread_create(&wd_thread, NULL, WDThreadManageScheduler,
								wd_info),"Failed to create a WD thread\n", -1);
										
	/*	return success */
	return (0);
}
/******************************************************************************/
/*	DNR function - start */
void DNR(void)
{
	/*	set DNR flag as 1 */
	__sync_fetch_and_add(&g_scheduler_should_stop, 1);
	
	/* verify the watch dog is indeed terminated	*/
	if (1 == IsProcessAliveIMP(g_process_to_signal))
	{
		fprintf(stderr, "Failed to destroy the WD\n");
		return;
	}

	if (pthread_join(&g_wd_thread, NULL))
	{
		fprintf(stderr, "Failed to finish WD thread\n");
		return;
	}
	
	return;
	
/*	DNR function - end */
}
/******************************************************************************/
