/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:						   								
* Version:				1.0		
* Description:			Main function of the Watch Dog program.
\******************************************************************************/

/******************************** Header Files ********************************/

#include <sys/types.h>	/*	pid_t							*/
#include <stdio.h>	/*	printf */
/* ....... */
/* ....... */
/* ....... */

/***************************** Global Definitions *****************************/

/**************************** Forward Declarations ****************************/

/************************* Functions  Implementations *************************/

/*	start main function */
int main(int argc, char *arv[])
{
	char *env_var_name = "WD_STATUS";
	char *env_var_val = "ON";
	
	size_t signal_intervals = 0;
	size_t num_allowed_fails = 0;
	
	atoi((strlen(getenv(env_signal_intervals)) + 1);
	
	/*	set signal handlers to deal SIGUSR1 &  SIGUSR2 */
	SetSignalHandler(SIGUSR1, handle_siguser1);
	SetSignalHandler(SIGUSR2, handle_siguser2);
	
	/*	add itself to env variable to indicate there is a running watch dog */
	/*	handle errors	*/
	ExitIfError(setenv(env_var_name, env_var_val), "Failed to create ENV\n",-1);
	
	/* WDManageScheduler for parent id */
	WDManageScheduler();
	
	/*	return */
	return (0);
}
/******************************************************************************/
