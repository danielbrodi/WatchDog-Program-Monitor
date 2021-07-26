/*********************************FILE__HEADER*********************************\
* File:					watchdog.c
* Author:				Daniel Brodsky				 		  												  								
* Date:					12-July-2021
* Pseudocode Reviewer:	Eran Barnoy
* Code Reviewer:		Avital Moses						   								
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
	size_t num_allowed_fails = 5;
	
	size_t num_seconds_between_checks = 2;
	
	time_t start_time = time(0);
	time_t time_to_wait = 10;  /*	in seconds */
	time_t end_time = start_time + time_to_wait;
	
	printf(GREEN "[app %d] USER APP STARTED RUNNING|\n", getpid());
	
	KeepMeAlive(argc, argv, num_seconds_between_checks, num_allowed_fails);
	
	while(time(0) < end_time)
	{
		printf("\n%60s|Program[pid:%d] is running . . .|\n"
				"%60s|Waiting for a signal . . .%9s|\n", "",getpid(), "", "");
		sleep(1);
	}
	
	printf(WHITE "\n%60s|*USER APP IS DONE*: EXITING . . .|\n" NORMAL, "");
	
	/* dnr */
	DNR();

	return (0);
}
