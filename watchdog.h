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

void KeepMeAlive(int argc, char *argv[], size_t signal_intervals,
													size_t num_allowed_misses);
int DNR(void);

#endif	/* __WATCHDOG_H__	*/
