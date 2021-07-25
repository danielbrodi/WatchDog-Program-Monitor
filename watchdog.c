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
#define _DEFAULT_SOURCE			/*	setenv	*/

#include <assert.h>		/*	assert	*/	
#include <stddef.h>		/*	size_t, NULL	*/
#include <stdio.h>		/*	sprintf	*/
#include <stdlib.h>		/*	setenv, getenv	*/
#include <string.h>		/*	memcpy		*/

#include <fcntl.h>			/*	O_CREAT	*/
#include <semaphore.h>		/*	sem_open	*/
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
	sigset_t signal_set = {0};
	
	/*	stores values of num_allowed_misses and signal_intervals as env vars */
	char env_num_allowed_misses[120] = {'\0'};
	char env_signal_intervals[120] = {'\0'};
	
	info_ty *wd_info = (info_ty *)malloc(sizeof(info_ty));
	ExitIfError(NULL == wd_info, "Failed to allocate memory for info struct!\n"
																		,-1);	
	/*	asserts */
	assert(signal_intervals);
	assert(num_allowed_misses);
	
	/*	register SIGUSR1 signal handler to manage received signals status */
	SetSignalHandler(SIGUSR1, handler_ResetErrorsCounter);
	
	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGUSR1);
	
	/*	set ENV variables of num_allowed_misses and signal_intervals */
	sprintf(env_signal_intervals, "%ld", signal_intervals);
	setenv("SIGNAL_INTERVAL", env_signal_intervals, 1);
	
	sprintf(env_num_allowed_misses, "%ld", num_allowed_misses);
	setenv("NUM_ALLOWED_MISSES", env_num_allowed_misses, 1);

	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	 
	/*	allocate memory for new arguments to run with the Watch Dog program	*/ 
	/*	extra 2 spaces for NULL and for the WD program name */
	wd_info->argv_for_wd = (char **)(calloc(argc + 2, sizeof(char *)));
	
	/*	copy argv and attach wd_app_name to the beginning 	*/
	wd_info->argv_for_wd[0] = "./watchdog";
	memcpy(wd_info->argv_for_wd+1, argv, argc * sizeof(char *));
	
	wd_info->num_allowed_misses = num_allowed_misses;
	wd_info->signal_intervals = signal_intervals;
	wd_info->i_am_wd = 0;
	wd_info->is_process_ready = sem_open("PROCESS_IS_READY", O_CREAT, 0666, 0);
	
	/*	check if there is already a watch dog (by an env variable): */
		/*	if yes - get its pid and mark it as the WD */
		/*	if no - create a new process and run WD and get its pid */
	if (getenv("WD_IS_ON"))
	{
		printf(RED "[app %d] WD already exists, no need to create a new one!\n" 
															NORMAL, getpid());
		SetProcessToSignalIMP(getppid());
	}
	else
	{
		printf(RED "\n[app %d] WD DOES NOT EXIST - CREATING WD . . .\n" NORMAL,
																	getpid() );
		ReturnIfError(FAILURE == StartWDProcess(wd_info), 
									"[app] Failed to create WD process!\n", -1);
	}
	
	printf(YELLOW "[app %d] WatchDog PID: %d\n" NORMAL, getpid(), 
													GetProcessToSignalIMP());
	ReturnIfError(GetProcessToSignalIMP() <= 0, 
					"[app] PID of new created WD process is invalid!\n", -1);
	
	sem_post(wd_info->is_process_ready);
	
	/*	block SIGUSR1 in the current main user's app thread so only the
	 *	communicator thread will be able to receive SIGUSR1 from the Watchdog */
	pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
	
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
	printf(YELLOW "\n\n%60sSENDING SIGUSR2 DNR\n\n" NORMAL, "");
	kill(GetProcessToSignalIMP(), SIGUSR2);
	
	handler_SetOnDNR(0);
	
	/* verify the watch dog is terminated	*/
	TerminateProcessIMP(GetProcessToSignalIMP());

	if (pthread_join(g_wd_thread, NULL))
	{
		fprintf(stderr, "[app] Failed to finish WD thread\n");
		return (FAILURE);
	}
	
	return (SUCCESS);
	
/*	DNR function - end */
}
/******************************************************************************/
