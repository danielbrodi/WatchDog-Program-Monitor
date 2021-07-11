/*********************************FILE__HEADER*********************************\
* File:				watchdog.c
* Author:			Daniel Brodsky				 		  												  								
* Date:				11-July-2021
* Code Reviewer:						   								
* Version:			1.0		
* Description:		....TODO....
\******************************************************************************/

/********************************* Pseudo Code ********************************/

/*	struct info
	{
		int argc;
		char *argv[];
		size_t num_allowed_fails;
		size_t num_seconds_between_checks;	
	}
*/
	
/*	KeepMeAlive function - start */
	/*	asserts */
	/*	create a new thread and send it KeepMeAliveIMP function with
	 *	struct info with all the needed info */
	/*upgrade niceness*/
/*	KeepMeAlive - function - end */

/*	KeepMeAliveIMP	function - start */
	/*	asserts */
	/*	set signal handlers */
	/*	mask signals	*/
	/*	save the parameters info in local variables */
	
	/*	while program is running */
		/*	RESET: fork: 	*/
			
			/*	if child: */
				/*	execv	*/
				/*	pass cli parmeters */
				/*	exit if execv fails */
			/*	end if 		*/
				
			/*	if parent:	*/
				/*	register signal handler that will reset the time */
				
				/*	while total time has not passed :*/
				
					/*	send signal every num_seconds_between_checks	*/
				/*	end of while */
				
				/*	terminate child process (watchdog) */
	/*	end of while program is runnig */
			
			
		/*	end if */
	/*	
	
