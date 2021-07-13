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
	/*	set signal handlers to deal SIGUSR1 &  SIGUSR2 */
	SetSignalHandler(SIGUSR1, handle_siguser1);
	SetSignalHandler(SIGUSR2, handle_siguser2);
	
	/*	add itself to env variable to indicate there is a running watch dog */
	
	/* WDManageScheduler for parent id */
	/*	TODO create info struct that contains argc, argv and ppid to watch */
	WDManageScheduler(info);
	
	/*	return */
	return (0);
	
}
/******************************************************************************/
