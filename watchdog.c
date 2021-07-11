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
	/*	joins the thread */
/*	KeepMeAlive - function - end */

/*	KeepMeAliveIMP	function - start */
	/*	asserts */
	/*	set signal handlers */
	/*	mask signals	*/
	/*	save the parameters info in local variables */
	
	/*	fork: 	*/
		
		/*	if child: */
			/*	execv	*/
		/*	end if 		*/
			
		/*	if parent:	*/
			/*	while time from last check is fine: */
				/*	while watchdog doesn't respond && not pass num_allowed_fails : 
							/*	try again */
							/*	sleep for num_seconds_between_checks	*/
				/*	end while doesn't respond */
				
				/*	wait for signal */
				
			/*	end while program runs */
		/*	end if */
	/*	
	
