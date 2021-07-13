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
	size_t num_allowed_fails;
	size_t num_seconds_between_checks;	
}info_ty;

/************************* Functions  Implementations *************************/	

int KeepMeAlive(int argc, char *argv[], size_t num_seconds_between_checks,
													size_t num_allowed_fails)
{
	pid_t wd_pid = 0;
	
	pthread_t wd_thread = 0;
	
	/*	asserts */
	assert(num_seconds_between_checks);
	assert(num_allowed_fails);
	
	/*	register SIGUSR1 signal handler that increment the flag */
	SetSignalHandler(SIGUSR1, handle_siguser1);
	
	/*	check if there is already a watch dog (by an env variable): */
		/*	if yes - check its pid */
		/*	if no - WDPCreate, get its pid */
	wd_pid = /*TODO*/ if watch dot exits ? get pid : WDPCreate();
	ExitIfError(wd_pid < 0, "Failed to create watch dog process!\n", -1);
		
	/*	create a thread (with niceness + 1) that will be using a scheduler
	 *	to communicate with the Watch Dog process */
	 /*	handle errors*/
	ExitIfBad(pthread_create(&wd_thread, NULL, WDManageScheduler, &wd_pid),
										"Failed to create a WD thread\n", -1;);
	/*	return success */
	return (0);
}
/******************************************************************************/
/*	DNR function - start */
	/*	set DNR flag as 1 */
	/*	busy wait and verify the watch dog is indeed terminated	*/
/*	DNR function - end */
/******************************************************************************/
