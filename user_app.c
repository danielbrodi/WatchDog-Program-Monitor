/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:						   								
* Version:				1.0			
* Description:			Sample of a user app that runs a Watch Dog program.
\******************************************************************************/

#include <stdio.h>	/*	printf */
#include <time.h>	/*	time_t, time()	*/
#include <unistd.h>
#include <signal.h>

#include "utils.h"
#include "watchdog.h"

int main(int argc, char *argv[])
{
	sigset_t signal_set;
	
	int num = 0;
	
	size_t num_allowed_fails = 100;
	
	size_t num_seconds_between_checks = 1;
	
	time_t start_time = time(0);
	time_t time_to_wait = 10; /* in seconds */
	time_t end_time = start_time + time_to_wait;
	
/*	sigemptyset(&signal_set);*/
/*	sigaddset(&signal_set, SIGUSR1);*/
/*	*/
/*	sigprocmask(SIG_BLOCK, &signal_set, NULL);*/
	
	printf(GREEN "[app %d] USER APP STARTED RUNNING|\n", getpid());
	
	KeepMeAlive(argc, argv, num_seconds_between_checks, num_allowed_fails);
	
	while (1)
	{
		printf("\n%60s|Program[%d] is running . . .|\n"
								"%60s|Waiting for a signal . . .%5s|\n", "",getpid(), "", "");
		sleep(1);
	}
	
	printf(YELLOW "\n%60s|--DONE USER APP--\n" NORMAL, "");
	
	raise(SIGTERM);
	
	/* dnr */
	/*	sigprocmask(SIG_UNBLOCK, &signal_set, NULL); */

	return (0);
}
