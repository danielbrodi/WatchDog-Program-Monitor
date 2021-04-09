/*********************************FILE__HEADER*********************************\
* File: UID.c					 		  								
* Author: Daniel Brodsky				  								
* Date: 08/04/2021							   								
* Version: 1.0 (Before Review)			   								
* Reviewer: Omer						   								
* Description: Unique identifier Functions Implementations.			 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <stddef.h>		/* size_t		*/
#include <unistd.h>		/* pid_t		*/
#include <time.h>		/* time_t		*/

#include "utils.h"		/* boolean_ty	*/
#include "UID.h"

/***************************** Static Definitions *****************************/
ilrd_uid_ty UIDGetBadUID(void);
size_t shared_counter = 0;	/* a shared counter for all new created uid's	*/

/******************************************************************************/
/************************* Functions__Implementations *************************/
/******************************************************************************/
ilrd_uid_ty UIDCreate(void)
{
	ilrd_uid_ty new_uid;
	time_t curr_time = time(0);
	
	if (-1 == curr_time)
	{
		return (UIDGetBadUID());
	}
	
	++shared_counter;
	
	new_uid.PID = getpid();
	new_uid.timestamp = curr_time;
	new_uid.counter = shared_counter;
	
	return(new_uid);
}
/******************************************************************************/
boolean_ty UIDIsEqual(ilrd_uid_ty uid1, ilrd_uid_ty uid2)
{
	boolean_ty is_match = TRUE;
	
	is_match *= (uid1.PID == uid2.PID);
	is_match *= (uid1.timestamp == uid2.timestamp);
	is_match *= (uid1.counter == uid2.counter);
	
	return (is_match);
}
/******************************************************************************/
ilrd_uid_ty UIDGetBadUID(void)
{
	ilrd_uid_ty bad_uid;
	
	bad_uid.PID = 0;
	bad_uid.timestamp = 0; 
	bad_uid.counter = 0;
	
	return (bad_uid);
}
/******************************************************************************/
