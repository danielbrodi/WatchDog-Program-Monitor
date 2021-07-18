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

#define _POSIX_C_SOURCE 199506L	/*	sigaction	*/
#define _DEFAULT_SOURCE	/*	putenv	*/

#include <assert.h>		/*	assert	*/	
#include <stddef.h>		/*	size_t, NULL	*/
#include <stdio.h>		/*	sprintf	*/
#include <stdlib.h>		/*	setenv, getenv	*/
#include <string.h>		/*	memcpy		*/

#include <pthread.h>	/*	pthread_create, pthread_t	*/
#include <signal.h>		/*	signals functions */
#include <unistd.h>
#include <sys/types.h>	/*	pid_t			*/

#include "utils.h"		/*	ReturnIfError	*/
#include "wd_internal.h"
#include "watchdog.h"

/***************************** Global Definitions *****************************/

/*	A thread is used as a communicator between the user's app and the
 *	Watch Dog program.	*/
static pthread_t g_wd_thread = 0;

/************************* Functions  Implementations *************************/	

void KeepMeAlive(int argc, char *argv[], size_t signal_intervals,
													size_t num_allowed_misses)
{
	info_ty *wd_info = (info_ty *)malloc(sizeof(info_ty));
	
	char **argv_for_wd = NULL;
	
	/*	stores values of num_allowed_misses and signal_intervals as env vars */
	char env_num_allowed_misses[120] = {'\0'};
	char env_signal_intervals[120] = {'\0'};
	
	/*	asserts */
	assert(signal_intervals);
	assert(num_allowed_misses);
	
	/*	register SIGUSR1 signal handler to manage received signals status */
	SetSignalHandler(SIGUSR1, handler_ResetErrorsCounter);
	
	/*	set ENV variables of num_allowed_misses and signal_intervals */
	sprintf(env_signal_intervals, "SIGNAL_INTERVAL=%ld", signal_intervals);
	putenv(env_signal_intervals);
	
	sprintf(env_num_allowed_misses, "NUM_ALLOWED_FAILURES=%ld", 
															num_allowed_misses);
	putenv(env_num_allowed_misses);
	
	/*	allocate memory for new arguments to run with the Watch Dog program */
	argv_for_wd = (char **)(calloc(argc + 2, sizeof(char *)));
	ReturnIfError(NULL == argv_for_wd, "[app] Failed to create argv array\n", -1);
	
	/* copy argv and attach wd_app_name to the beginning */
	argv_for_wd[0] = "./watchdog";
	
	memcpy(argv_for_wd + 1, argv, argc * sizeof(char *));

	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info->argv_for_wd = argv_for_wd;
	wd_info->num_allowed_misses = num_allowed_misses;
	wd_info->signal_intervals = signal_intervals;
	wd_info->i_am_wd = 0;
	
	/*	check if there is already a watch dog (by an env variable): */
		/*	if yes - check its pid */
		/*	if no - create a new process and run WD and get its pid */
	if (getenv("WD_IS_ON"))
	{
		printf(RED "[app %d] WD already exists, no need to create a new one!\n" NORMAL, getpid());
		SetProcessToSignalIMP(getppid());
	}
	else
	{
		printf(RED "\n[app %d] WD DOES NOT EXIST - CREATING WD . . .\n" NORMAL, getpid() );
		ReturnIfError(FAILURE == StartWDProcess(wd_info), 
									"[app] Failed to create WD process!\n", -1);
	}
	
	printf(YELLOW "[app %d] WatchDog PID: %d\n" NORMAL, getpid(), GetProcessToSignalIMP());
	ReturnIfError(GetProcessToSignalIMP() <= 0, 
					"[app] PID of new created WD process is invalid!\n", -1);
	
	/*	create a thread that will use a scheduler
	 *	to communicate with the Watch Dog process */
	 /*	handle errors*/
	ReturnIfError(pthread_create(&g_wd_thread, NULL, WDThreadSchedulerIMP,
							wd_info),"[app] Failed to create a WD thread\n", -1);
		
	/*	return success */
	return;
}
/******************************************************************************/
/*	DNR function - start */
int DNR(void)
{
	/*	set DNR flag as 1 */
	handler_SetOnDNR(0);
	
	/* verify the watch dog is indeed terminated	*/
	if (1 == IsProcessAliveIMP(GetProcessToSignalIMP()))
	{
		fprintf(stderr, "[app] Failed to destroy the WD\n");
		return (FAILURE);
	}

	if (pthread_join(g_wd_thread, NULL))
	{
		fprintf(stderr, "[app] Failed to finish WD thread\n");
		return (FAILURE);
	}
	
	return (SUCCESS);
	
/*	DNR function - end */
}
/******************************************************************************/
