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

#include <assert.h>		/*	assert	*/
#include <pthread.h>	/*	pthread_create, pthread_t	*/
#include <stddef.h>		/*	size_t, NULL	*/
#include <sys/types.h>	/*	pid_t			*/

#include "utils.h"		/*	ExitIfError, UNUSED	*/

/***************************** Global Definitions *****************************/

/*	a struct that will be transferred from KeepMeAlive to KeepMeAliveIMP with
 *	all CLI user parameters 	*/
typedef struct info
{
	int argc;
	char *argv[];
	size_t num_allowed_misses;
	time_t signal_intervals;	
}info_ty;

/************************* Functions  Implementations *************************/	

int KeepMeAlive(int argc, char *argv[], time_t signal_intervals,
													size_t num_allowed_misses)
{
	info_ty wd_info = {0};
	
	pid_t wd_pid = 0;
	
	pthread_t wd_thread = 0;
	
	/*	asserts */
	assert(signal_intervals);
	assert(num_allowed_misses);
	
	/*	register SIGUSR1 signal handler to manage received signals status */
	SetSignalHandler(SIGUSR1, handle_siguser1);
	/*	register SIGUSR2 signal handler request of DNR */	
	SetSignalHandler(SIGUSR2, handle_siguser2);
	
	/*	check if there is already a watch dog (by an env variable): */
		/*	if yes - check its pid */
		/*	if no - create a new process and run WD and get its pid */
	wd_pid = getenv("WD_STATUS") ? getppid() : WDPCreate(argv);
	
	ExitIfError(wd_pid < 0, "Failed to create watch dog process!\n", -1);
	
	/*	TODO set ENV variables to include num_allowed_misses and 
	 *	signal_intervals */
	
	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info.argc = argc;
	wd_info.argv = argv;
	wd_info.num_allowed_misses = num_allowed_misses;
	wd_info.signal_intervals = signal_intervals;
	
	/*	create a thread (with niceness + 1) that will be using a scheduler
	 *	to communicate with the Watch Dog process */
	 /*	handle errors*/
	ExitIfBad(pthread_create(&wd_thread, NULL, WDManageScheduler, info),
										"Failed to create a WD thread\n", -1;);
	/*	return success */
	return (0);
}
/******************************************************************************/
/*	DNR function - start */
void DNR(void)
{
	/*	set DNR flag as 1 */
	g_scheduler_should_stop = 1;
	
	/*	busy wait and verify the watch dog is indeed terminated	*/
	waitpid()....
	
/*	DNR function - end */
}
/******************************************************************************/
