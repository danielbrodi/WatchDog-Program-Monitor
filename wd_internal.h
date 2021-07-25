/*********************************FILE__HEADER*********************************\
* File:				watchdog.h						 		  								
* Author:			Daniel Brodsky					  								
* Date:				14-July-2021
* Version:			1.0					   												   								
* Description:		Watchdog's program internal functions API.		 
\******************************************************************************/

#ifndef __WD_INTERNAL_H__
#define __WD_INTERNAL_H__

#include "scheduler.h" 	/*	oper_ret_ty	*/

/*	a struct that contains all relevant and needed data to start a program */
typedef struct info
{
	char **argv_for_wd;
	size_t num_allowed_misses;
	size_t signal_intervals;	
	int i_am_wd; /*	0 if the caller is the user app, 1 if its the WD program */
	sem_t *is_wd_ready;
}info_ty;

/*	Creates a new process and runs through it the Watch Dog program or the
 *	user's main program.
 *	The decesion is made by which of the two got terminated and needs to restart.
*/
int StartWDProcess(info_ty *info);

/*	Manages the Watch Dog operation of scheduling sending signals and checking
 *	whether the other side is responding or not.
 *	Uses a scheduler's program to synchornize the tasks.	*/
void *WDThreadSchedulerIMP(void *info);
int WDManageSchedulerIMP(info_ty *info);

/*	Scheduler's task which sends SIGUSR1 to the needed process each preiod of
 *	time */
oper_ret_ty OnIntervalSendSignalIMP(void *unused);

/*	Scheduler's task which checks if the maximum number of missed signals has
 *	been reached. If too many signals are missed - restarts the other program */
oper_ret_ty OnIntervalCheckIfMissIMP(void *info);

/*	Scheduler's task which regulary checks if a DNR request has been made and
 *	stops the WatchDog if needed */
oper_ret_ty OnIntervalCheckIfDNR_IMP(void *scheduler_to_stop);

/*	Sets a user defined function as a signal handler of a given signal	*/
void SetSignalHandler(int signal, void(*handler_func)(int));

/*	Signal handler for SIGUSR1 - resets the missed signals counter */
void handler_ResetErrorsCounter(int sig_id);

/*	Signal handler for SIGUSR2 - raises the DNR flag	*/
void handler_SetOnDNR(int sig_id);

/*	Terminates a process and returns 0 if successfully terminated.
 *	A failure to terminate the process returns 1	*/ 
int TerminateProcessIMP(pid_t process_to_kill);

/*	Sets a process to be signaled */
void SetProcessToSignalIMP(pid_t pid);

pid_t GetProcessToSignalIMP();

#endif	/* __WD_INTERNAL_H__	*/
