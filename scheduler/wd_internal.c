/*********************************FILE__HEADER*********************************\
* File:				watchdog.c
* Author:			Daniel Brodsky				 		  												  								
* Date:				12-July-2021
* Code Reviewer:						   								
* Version:			1.0		
* Description:		Watchdog's program internal functions.
\******************************************************************************/

/******************************************************************************/
/*	KeepMeAliveIMP	function - start */
void *KeepMeAliveIMP(void *info)
{
	/*	asserts */
	
	/*	while program is running */
	
		/*	fork: 	*/
			/*---------------------------------*/
			/*	if child: */
			
				/*	execv WATCHDOG PROGRAM	-*/
				
				/*	handle error if execv fails.. maybe exit? */
				
			/*	end child 		*/
			/*---------------------------------*/
			/*	if parent:	*/
			
				/*	while parent runs */
				
					/*	run the exact code of `WATCHDOG - EXTERNAL PROGRAM`
					 *	just with a different signal handler and instead 
					 *	of sending signals to PPID , send to PID, and instead
					 *	of killing PID process, kill PPID and restart it if
					 *	needed */
					 
				/*	end while of parent runs */
				
			/*	end parent */
			/*---------------------------------*/
	/*	end of while program is runnig  - maybe not needed? its for restart */
	
/*	KeepMeAliveIMP function - end	*/
}
/******************************************************************************/
/*	creates a watch dog program process to watch a given process */
void WatchDogCreate(pid_t process_to_watch)
{
	/*	create scheduler	*/
	
	/*	create a counter of num of missed signals */
	
	/*	create a static global flag that determines if the right signal was received	*/

	/*	register signal handler that increment the flag */
	
	/*	create a scheduler task that will signal a given process every X seconds - TASK1 */
	
	/*	create a scheduler task that will receive the address of the flag, and
	 *	check whether its toggled or not - TASK2 */

	/*	while program runs */

		/*	while num_missed_signals < num_allowed_fails */

			/*	send signal to process_to_watch using TASK 1	*/
			/*	using TASK 2 check if there is any change to the flag */
				/*	if yes : decrement it and do nothing, continue.
					if not:  increment num_missed_signals counter 	*/
					
		/*	end of while number_missed_signals - which means reached maximum fails that allowed */
		
		/*	terminate user's process */
		
		/*	restart user's program using argc argv parameters that were received through execv */
		
		/*	restart number_missed_signals counter */
	
	/*	end while program runs */
}
/******************************************************************************/
/*	signal handler function - start 	*/
void SigHandlerIMP(int sig_id)
{
	/*	increment global flag of received or not signal */
	
/*	signal handler function - end	*/	
}
/******************************************************************************/
