/****************************** File Header ***********************************\
* File Name: utils.h
* Author: Daniel Brodsky
* Description: Utilities for different programs.
\******************************************************************************/

#ifndef	__UTILS_H__
#define	__UTILS_H__

#define UNUSED(x) ((void)(x))
/* escape keys for printf to color output for stodout */
#define NORMAL	"\x1B[0m"
#define BLACK   "\x1B[30m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define PURPLE  "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

/* print colored output of SUCCESS and FAILURE */
#define PRINT_SUCCESS printf (GREEN "SUCCESS\n" NORMAL)
#define PRINT_FAILURE printf (RED "FAILURE\n" NORMAL)

/*  exits the program with the given 'ret status' if 'is_bad' is true     */
void ExitIfError(int is_bad, char *msg_to_print, int ret_stat)
{
	if (is_bad)
	{
		puts(msg_to_print);
		exit(ret_stat);
	}
}

#endif	/* __UTILS_H__ */
