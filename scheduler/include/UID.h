/************************************************
* File: UID.h						         	*
* Date: 08/04/2021							    *
* Version: 1.0								    *
* Description: UID API  		                * 
************************************************/

#ifndef __UID_H__
#define __UID_H__

#include <stddef.h>		/* size_t		*/
#include <time.h>		/* time_t		*/

#include <sys/types.h>	/* pid_t		*/

#include "utils.h"		/* boolean_ty	*/

typedef struct ilrd_uid
{
    pid_t PID;
    time_t timestamp;
    size_t counter;
} ilrd_uid_ty;

/* Returns a new UID */
/* On failure returns Bad_UID */
/* Complexity: O(1) */
ilrd_uid_ty UIDCreate(void);

/* Returns TRUE if UID 'uid1' is equal to UID 'uid2' */
/* Returns FALSE otherwise */
/* Complexity O(1) */
boolean_ty UIDIsEqual(ilrd_uid_ty uid1, ilrd_uid_ty uid2);

/* Returns the Bad_UID */
/* Complexity O(1) */
ilrd_uid_ty UIDGetBadUID(void);

#endif /* __UID_H__ */
