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

/********************************* Pseudo Code ********************************/

/******************************************************************************/	
/*	KeepMeAlive function - start */
void KeepMeAlive(int argc, char *argv[], size_t num_seconds_between_checks,
													size_t num_allowed_fails)
{
	/*	asserts */
	
	/*	upgrade niceness by one	*/
	
	/*	check if there is already a watch dog , and if yes - connect it */
	
	/*	create a new thread that run WatchDogCreate */
	
/*	KeepMeAlive - function - end */
}
/******************************************************************************/
/*	DNR function - start */
	/*	set DNR flag as 1 */
/*	DNR function - end */
/******************************************************************************/
