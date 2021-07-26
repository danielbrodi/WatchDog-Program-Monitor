/*********************************FILE__HEADER*********************************\
* File:					watchdog_main.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:		Avital Moses   								
* Version:				1.5		
* Description:			Main function of the Watch Dog program.
\******************************************************************************/

/******************************** Header Files ********************************/

#define _POSIX_C_SOURCE 199506L	/*	setenv, getenv	*/
#define _DEFAULT_SOURCE			/*	setenv, getenv	*/

#include <assert.h>			/*	assert	*/	
#include <stddef.h>			/*	size_t, NULL	*/
#include <stdio.h>			/*	sprintf			*/
#include <stdlib.h>			/*	putenv, getenv	*/
#include <string.h>			/*	atol			*/

#include <fcntl.h>			/*	O_CREAT	*/
#include <pthread.h>		/*	pthread_create, pthread_t	*/
#include <semaphore.h>		/*	sem_open, sem_post	*/
#include <signal.h>			/*	signals functions */
#include <sys/types.h>		/*	pid_t			*/
#include <unistd.h>			/*	getppid			*/

#include "utils.h"			/*	ReturnIfError, print colors */
#include "wd_internal.h"	/*	WDManageSchedulerIMP, SetSignalHandler,info_ty*/

/************************* Functions  Implementations *************************/

/*	start main function */
int main(int argc, char *argv[])
{	
	size_t signal_intervals = 0;
	size_t num_allowed_misses = 0;
	
	info_ty *wd_info = (info_ty *)malloc(sizeof(info_ty));
	ExitIfError(NULL == wd_info, "Failed to allocate memory for info struct!\n"
																		,-1);
	assert(argv);
	
	/*	set signal handlers to deal SIGUSR1 &  SIGUSR2 */
	SetSignalHandler(SIGUSR1, handler_ResetErrorsCounter);
	SetSignalHandler(SIGUSR2, handler_SetOnDNR);
	
	UNUSED(argc);
	
	/*	add itself to env variable to indicate there is a running watch dog */
	/*	handle errors	*/
	setenv("WD_IS_ON", "1", 1);
	DEBUG printf(GREEN "%100s[wd %d] WD STARTED RUNNING\n", "", getpid());
	
	signal_intervals = atol(getenv("SIGNAL_INTERVAL"));
	num_allowed_misses = atol(getenv("NUM_ALLOWED_MISSES"));
	
	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info->argv_for_wd = argv + 1;
	wd_info->num_allowed_misses = num_allowed_misses;
	wd_info->signal_intervals = signal_intervals;
	wd_info->i_am_wd = 1;
	wd_info->is_process_ready = sem_open("PROCESS_IS_READY", O_CREAT, 0666, 0);
	
	SetProcessToSignalIMP(getppid());
	
	/*	set the process as ready and setted up to receive signals */
	sem_post(wd_info->is_process_ready);
	
	/* WDManageScheduler and start watching the app */
	WDManageSchedulerIMP(wd_info);
	
	/*	free struct's memory */
	memset(wd_info, 0, sizeof(info_ty));
	free(wd_info);
	wd_info = NULL;
	
	/*	return */
	return (0);
}
/******************************************************************************/
