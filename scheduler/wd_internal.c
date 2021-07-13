/*********************************FILE__HEADER*********************************\
* File:				watchdog.c
* Author:			Daniel Brodsky				 		  												  								
* Date:				12-July-2021
* Code Reviewer:						   								
* Version:			1.0		
* Description:		Watchdog's program internal functions.
\******************************************************************************/

/******************************************************************************/
/*	WDPCreate	function - start */
/*	returns PID or (-1) if failure */
int WDPCreate(int argc, char *argv[])
{
	/*	asserts */
	
		/*	fork: 	*/
		
			/*---------------------------------*/
			/*	if child: */
				
				/*	change niceness + 1 */
				
				/*	execv WATCHDOG PROGRAM with CLI parameters	-*/
				
				/*	return (-1) if any errors */
				
			/*	end child 		*/
			/*---------------------------------*/
			
			/*	if parent:	*/
			
				/*	return */
				
			/*	end parent */
			/*---------------------------------*/s
	
/*	KeepMeAliveIMP function - end	*/
}
/******************************************************************************/
/*	TODO add headline */
void *WDManageScheduler(void *process_to_watch)
{
	/*	create scheduler	*/
	
	/*	create a scheduler task that will signal a given process every X seconds - TASK1 */
	
	/*	create a scheduler task that will receive the address of the flag, and
	 *	check whether its toggled or not - TASK2 */

	/*	scheduler run */
	
	/*	scheduler destroy */
	
	/*	return */
}
/******************************************************************************/
/*	signal handler function - start 	*/
void SigHandlerIMP(int sig_id)
{
	/*	increment global flag of received or not signal */
	
/*	signal handler function - end	*/	
}
/******************************************************************************/
					/*	TASK 1 */

	/*	send SIGUSR1 to process_to_watch */

/******************************************************************************/
					/*	TASK 2 */

	/*	create a counter of num of missed signals */

	/*	check if the "received signal" flag is toggled */
		/*	if yes : decrement it and do nothing, continue.
			if not:  increment num_missed_signals counter 	*/
			
	/*	if num_missed_signals equals num_allowed_fails : */

		/*	terminate process_to_watch process */

		/*	restart process_to_watch using argc argv parameters  */

		/*	reset number_missed_signals counter */
	
	/*	end if reached num_allowed_fails */
/******************************************************************************/
