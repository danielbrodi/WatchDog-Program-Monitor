/*********************************FILE__HEADER*********************************\
* File:				watchdog.h						 		  								
* Author:			Daniel Brodsky					  								
* Date:				11-July-2021
* Version:			1.0					   												   								
* Description:		Watchdog program implementation API.		 
\******************************************************************************/

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <stddef.h>		/*	size_t	*/

/*	Creates a Watch Dog program that will watch your program and restart it
 *	if it gets stuck or terminates.
 *	signal_intervals is necessary to determine the time that wait time between
 *	signals sending. num_allowed_misses is the amount of signals that the
 *	Watch Dog will not recevie before deciding that the other side is down,
 *	and will restart it.	*/
void KeepMeAlive(int argc, char *argv[], size_t signal_intervals,
													size_t num_allowed_misses);
													
/*	DNR - Do not resuscitate.
 *	Used to stop the Watch Dog from watching the program.
 *	Returns '0' if DNR request succeeded and WD won't watch the program anymore.
 *	Returns '1' if failed. 	*/
int DNR(void);

#endif	/* __WATCHDOG_H__	*/
