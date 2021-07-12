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

/********************************* Pseudo Code ********************************/

/*----------------------------------------------------------------------------*/

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
	
/*----------------------------------------------------------------------------*/
	
/*	KeepMeAlive function - start */
	/*	asserts */
	/*	create a new thread and send it KeepMeAliveIMP function with
	 *	struct info with all the needed info */
	/*	upgrade niceness	*/
/*	KeepMeAlive - function - end */

/*----------------------------------------------------------------------------*/

/*	KeepMeAliveIMP	function - start */
	/*	asserts */
	/*	set signal handlers */
	/*	mask signals	*/
	/*	save the parameters info in local variables */
	
	/*	while program is running */
		/*	fork: 	*/
			
			/*	if child: */
				/*	execv	*/
				/*	pass cli parmeters */
				/*	exit if execv fails */
			/*	end child 		*/
				
			/*	if parent:	*/
				/*	register signal handler that will reset the time */
				
				/*	while maximum time has not passed :*/
					/*	send signal every num_seconds_between_checks using scheduler	*/
				/*	end of while */
				
				/*	terminate child process (watchdog) */
			/*	end parent */
	/*	end of while program is runnig */
/*	KeepMeAliveIMP function - end	*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*-----	WATCHDOG - EXTERNAL PROGRAM -----*/

/*	create a variable that tracks the number of missed signals */

/*	register signal handler that will reset the time that has been passed since the last signal that was send */

/*	while program runs */

	/*	while number_missed_signals < num_allowed_fails */

		/*	send signal each num_seconds_between_checks using a scheduler */
		
		/*	while sleep(num_seconds_between_checks + 30 seconds) */
				
				/*	if woke up : */
					/*	right signal was received = break sleep loop */
					/*	wrong signal was received = continue sleep */
		
		/*	end of while sleep */
		
		/*	if no more time left to sleep: increment number_missed_signals counter */
		
	/*	end of while number_missed_signals - which means reached maximum fails that allowed */
	
	/*	restart user's program using argc argv parameters that were received through execv */
	
	/*	restart number_missed_signals counter */
	
/*	end while program runs */

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
