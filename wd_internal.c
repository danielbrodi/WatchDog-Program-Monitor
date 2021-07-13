/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:						   								
* Version:				1.0		
* Description:			Watchdog's program internal functions.
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
			/*---------------------------------*/
	
/*	KeepMeAliveIMP function - end	*/
}
/******************************************************************************/
/*	TODO add headline */
void *WDManageScheduler(void *process_to_watch)
{
	/*	create scheduler	*/
	
	/*	create a scheduler task SendSignal */
	
	/*	create a scheduler task CheckIfSignalReceived */

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
operation_func_ty SendSignal
{
	/*	send SIGUSR1 to process_to_watch every X seconds */
}
/******************************************************************************/
operation_func_ty CheckIfSignalReceived
{
	/*	create a counter of num of missed signals */

	/*	check if the "received signal" flag is toggled */
		/*	if yes : decrement it and do nothing, continue.
			if not:  increment num_missed_signals counter 	*/
			
	/*	if num_missed_signals equals num_allowed_fails : */

		/*	terminate process_to_watch process */

		/*	restart process_to_watch using argc argv parameters  */

		/*	reset number_missed_signals counter */
	
	/*	end if reached num_allowed_fails */
}
/******************************************************************************/
void KillnRestartProcess(pid_t process_to_kill, char *argv[])
{
	/*	terminate process_to_kill	*/
	
	/*	verify its terminated	*/
	
	/*	fork: 	*/
		
	/*---------------------------------*/
	/*	if child: */
		
		/*	change niceness + 1 */
		
		/*	execv WATCHDOG PROGRAM with argv	-*/
		
		/*	return (-1) if any errors */
		
	/*	end child 		*/
	/*---------------------------------*/
	
	/*	if parent:	*/
	
		/*	return */
		
	/*	end parent */
	/*---------------------------------*/
}
/******************************************************************************/
