/*********************************FILE__HEADER*********************************\
* File:				watchdog.h						 		  								
* Author:			Daniel Brodsky					  								
* Date:				14-July-2021
* Version:			1.0					   												   								
* Description:		Watchdog's program internal functions API.		 
\******************************************************************************/

#ifndef __WD_INTERNAL_H__
#define __WD_INTERNAL_H__

#endif	/* __WD_INTERNAL_H__	*/

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




