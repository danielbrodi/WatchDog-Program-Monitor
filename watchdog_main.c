/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:						   								
* Version:				1.0		
* Description:			Main function of the Watch Dog program.
\******************************************************************************/

/******************************** Header Files ********************************/

#define _XOPEN_SOURCE

#include <assert.h>		/*	assert	*/	
#include <stddef.h>		/*	size_t, NULL	*/
#include <stdio.h>		/*	sprintf			*/
#include <stdlib.h>		/*	setenv, getenv	*/
#include <string.h>

#include <pthread.h>	/*	pthread_create, pthread_t	*/
#include <signal.h>
#include <sys/types.h>	/*	pid_t			*/

#include "utils.h"
#include "wd_internal.h"

/***************************** Global Definitions *****************************/

/**************************** Forward Declarations ****************************/

/************************* Functions  Implementations *************************/

/*	start main function */
int main(int argc, char *argv[])
{
	size_t signal_intervals = 0;
	size_t num_allowed_misses = 0;
	
	info_ty wd_info = {0};
	info_ty *info = &wd_info;
	char **argv_to_run = NULL;
	
	/*	add itself to env variable to indicate there is a running watch dog */
	/*	handle errors	*/
	putenv("WD_IS_ON=1");
	printf(BLUE "[wd] WD IS RUNNING!\n" NORMAL);
	/* copy argv and attach wd_app_name to the beginning */
	argv_to_run = (char **)(calloc(argc, sizeof(char *)));
	ReturnIfError(NULL == argv_to_run, "Failed to create argv array\n", -1);
	
	memcpy(argv_to_run + 1, argv, argc - 1);
	
	signal_intervals = atol(getenv("SIGNAL_INTERVAL"));
	num_allowed_misses = atol(getenv("NUM_ALLOWED_FAILURES"));
	
	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info.argv_for_wd = argv + 1;
	wd_info.num_allowed_misses = num_allowed_misses;
	wd_info.signal_intervals = signal_intervals;
	wd_info.i_am_wd = 1;
	
	/*	set signal handlers to deal SIGUSR1 &  SIGUSR2 */
	SetSignalHandler(SIGUSR1, handler_siguser1);
	SetSignalHandler(SIGUSR2, handler_siguser2);
	
	/* WDManageScheduler for parent id */
	WDManageSchedulerIMP(info);
	
	/*	return */
	return (0);
}
/******************************************************************************/
