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
int main(int argc, char *argv[])
{
	size_t signal_intervals = 0;
	size_t num_allowed_misses = 0;
	
	info_ty wd_info = {0};
	
	char *env_var_name = "WD_STATUS";
	char *env_var_val = "ON";
	
	/*	add itself to env variable to indicate there is a running watch dog */
	/*	handle errors	*/
	ExitIfError(setenv(env_var_name, env_var_val), "Failed to create ENV\n",-1);
	
	
	/* copy argv and attach wd_app_name to the beginning */
	argv_for_wd = (char **)malloc(argc * sizeof(char *));
	ReturnIfError(NULL == argv_for_wd, "Failed to create argv array\n", -1);
	
	argv_for_wd[0] = "./watchdog";
	
	memcpy(argv_for_wd + 1, argv, argc);
	
	signal_intervals = atol(getenv("SIGNAL_INTERVAL"));
	num_allowed_misses = atol(getenv("NUM_ALLOWED_FAILURES"));
	
	/*	set info struct to be transfered to the scheduler function with all
	 *	the needede information	*/
	wd_info.argv = argv;
	wd_info.num_allowed_misses = num_allowed_misses;
	wd_info.signal_intervals = signal_intervals;
	wd_info.i_am_wd = 1;
	
	/*	set signal handlers to deal SIGUSR1 &  SIGUSR2 */
	SetSignalHandler(SIGUSR1, handle_siguser1);
	SetSignalHandler(SIGUSR2, handle_siguser2);
	
	/* WDManageScheduler for parent id */
	WDManageScheduler(info);
	
	/*	return */
	return (0);
}
/******************************************************************************/
