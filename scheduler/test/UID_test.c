/*********************************FILE__HEADER*********************************\
* File: UID_test.c					 		  								
* Author: Daniel Brodsky				  								
* Date: 08/04/2021							   								
* Version: 1.0 (Before Review)			   								
* Reviewer: Omer						   								
* Description: Unique identifier Functions Implementations Testings.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <stddef.h>		/*	size_t			*/
#include <sys/types.h>	/*	pid_t			*/
#include <stdio.h>		/*	printf, NULL	*/
#include <stdlib.h>		/*	rand, srand		*/
#include <time.h>		/*	time			*/

#include "utils.h"		/*	status_ty, boolean_ty, ANSI_COLOR_	*/

#include "utils.h"		/* 	boolean_ty		*/
#include "UID.h"

/***************************** Macros Definitions *****************************/
/* print colored output */
#define PRINT_FAILURE printf (ANSI_COLOR_RED "FAILURE\n" ANSI_COLOR_RESET)
/**************************** Forward Declarations ****************************/

/******************************************************************************/
/******************************* Main__Function *******************************/
int main()	
{
	ilrd_uid_ty new_uid = UIDCreate();
	
	if(!(UIDIsEqual(new_uid, UIDGetBadUID())))
	{
		printf("PID: %d\n", new_uid.PID);
		printf("Timestamp: %ld\n", new_uid.timestamp);
		printf("Counter: %lu\n", new_uid.counter);
		
		printf (ANSI_COLOR_GREEN "UID has been successfully created!\n"
		 													ANSI_COLOR_RESET);
		return (0);
	}
	
	PRINT_FAILURE;
	return(1);
}
