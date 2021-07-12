/*********************************FILE__HEADER*********************************\
* File:				watchdog.c
* Author:			Daniel Brodsky				 		  												  								
* Date:				11-July-2021
* Code Reviewer:						   								
* Version:			1.0		
* Description:		Watchdog's program design pseudo-code. 
					Watchdog should protect a client's program from being
					terminated and restart it if needed. 
					The client's program should also protect the watchdog
					program from being terminated and also restart it if needed.
\******************************************************************************/

/******************************** Header Files ********************************/

#include <sys/types.h>	/*	pid_t							*/
/* ....... */
/* ....... */
/* ....... */

/***************************** Global Definitions *****************************/

/*	a struct that will be transferred from KeepMeAlive to KeepMeAliveIMP with
 *	all CLI user parameters 	*/
	 
	/*	struct info
		{
			int argc;
			char *argv[];
			size_t num_allowed_fails;
			size_t num_seconds_between_checks;	
		}
	*/

/**************************** Forward Declarations ****************************/

void *KeepMeAliveIMP(void *info);

/*	returns PID of the created Watch Dog */
pid_t WatchDogCreate();

/********************************* Pseudo Code ********************************/

/******************************************************************************/	
/*	KeepMeAlive function - start */
void KeepMeAlive(int argc, char *argv[], size_t num_seconds_between_checks,
													size_t num_allowed_fails)
{
	/*	asserts */
	
	/*	upgrade niceness by one	*/
	
	/*	create a new thread and send it KeepMeAliveIMP function with
	 *	struct info with all the needed info */
	
/*	KeepMeAlive - function - end */
}
/******************************************************************************/
/*	KeepMeAliveIMP	function - start */
void *KeepMeAliveIMP(void *info)
{
	/*	asserts */
	
	/*	set signal handlers */
	
	/*	mask signals	*/
	
	/*	save the parameters info in local variables */
	
	/*	while program is running */
	
		/*	fork: 	*/
			/*---------------------------------*/
			/*	if child: */
			
				/*	execv WATCHDOG - EXTERNAL PROGRAM	-*/
				
				/*	handle error if execv fails.. maybe exit? */
				
			/*	end child 		*/
			/*---------------------------------*/
			/*	if parent:	*/
			
				/*	while program runs */
					/*	run the exact code of `WATCHDOG - EXTERNAL PROGRAM`
					 *	just with a different signal handler and instead 
					 *	of sending signals to PPID , send to PID, and instead
					 *	of killing PID process, kill PPID and restart it if
					 *	needed */
				/*	end while of program runs */
				
			/*	end parent */
			/*---------------------------------*/
	/*	end of while program is runnig  - maybe not needed? its for restart */
	
/*	KeepMeAliveIMP function - end	*/
}
/******************************************************************************/
/*	signal handler function - start 	*/
void SigHandler(int sig_id)
{
	/*	set number_missed_signals as 0 */
	
/*	signal handler function - end	*/	
}
/******************************************************************************/
/*	DNR function - start */
	/*	set DNR flag as 1 */
/*	DNR function - end */
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
				/*-----	WATCHDOG - EXTERNAL PROGRAM : START -----*/
				
pid_t WatchDogCreate()
{
	/*	create a variable that tracks the number of missed signals */

	/*	register signal handler that will reset the number of missed signals counter */

	/*	while program runs */

		/*	while number_missed_signals < num_allowed_fails */

			/*	send signal to parent PID each num_seconds_between_checks using a scheduler */
			
			/*	while sleep(num_seconds_between_checks + 30 seconds) */
					
					/*	if woke up : */
						/*	right signal was received = break sleep loop */
						/*	wrong signal was received = continue sleep */
			
			/*	end of while sleep */
			
			/*	if no more time left to sleep: increment number_missed_signals counter */
			
		/*	end of while number_missed_signals - which means reached maximum fails that allowed */
		
		/*	terminate user's process */
		
		/*	restart user's program using argc argv parameters that were received through execv */
		
		/*	restart number_missed_signals counter */
	
	/*	end while program runs */
}
				/*-----	WATCHDOG - EXTERNAL PROGRAM : END -----*/
/******************************************************************************/
/******************************************************************************/
