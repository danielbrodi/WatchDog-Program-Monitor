/*********************************FILE__HEADER*********************************\
* File:				watchdog.h						 		  								
* Author:			Daniel Brodsky					  								
* Date:				14-July-2021
* Version:			1.0					   												   								
* Description:		Watchdog's program internal functions API.		 
\******************************************************************************/

#ifndef __WD_INTERNAL_H__
#define __WD_INTERNAL_H__

/*	a struct that contains all relevant and needed data to start a WD program */
typedef struct info
{
	char **argv_for_wd;
	size_t num_allowed_misses;
	time_t signal_intervals;	
	int i_am_wd; /*	0 if the caller is the user app, 1 if its the WD */
}info_ty;

/*	creates and runs a Watch Dog process	*/
/*	returns PID or (-1) if failed to create the process or to run the program */
pid_t WDPCreate(char *argv[]);

void *WDManageScheduler(void *info);

oper_ret_ty SendSignalIMP(void *process_to_signal);

oper_ret_ty CheckIfSignalReceived(void *info);

void KillnRestartProcess(pid_t process_to_kill, char *argv[]);

void SetSignalHandler(int signal, void(*handler_func)(int));

void handler_siguser1(int sig_id);

void handler_siguser2(int sig_id);

#endif	/* __WD_INTERNAL_H__	*/
