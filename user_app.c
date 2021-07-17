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

#include <unistd.h>

#include "watchdog.h"

int main(int argc, char *argv[])
{
	int num = 0;
	
	size_t num_allowed_fails = 3;
	
	size_t num_seconds_between_checks = 10;
	
	KeepMeAlive(argc, argv, num_seconds_between_checks, num_allowed_fails);
	
	while (num < 1000)
	{
		printf("Your lucky number is: %d\n", num);
		sleep(1);
		++num;
	}
	
	DNR();
	
	return (0);
}
