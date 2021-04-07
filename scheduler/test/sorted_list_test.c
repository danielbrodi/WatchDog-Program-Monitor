/*********************************FILE__HEADER*********************************\
* File: sorted_list_test.c						 		  								
* Author: Daniel Brodsky					  								
* Date: 07/04/2021						   								
* Version: Beta				   								
* Reviewer: Ariel						   								
* Description: Sorted Linked List API's Functions Testings.		 
\******************************************************************************/

/********************************* Inclusions *********************************/
#include <assert.h>	/*	assert	*/
#include <stddef.h>	/*	size_t	*/
#include <stdio.h>	/* printf, NULL */
#include <stdlib.h> /* rand, srand */
#include <string.h> /* strcmp */
#include <time.h>	/* time */

#include "utils.h" /* status_ty, boolean_ty, ANSI_COLOR_*/
#include "sorted_list.h"

/***************************** Macros Definitions *****************************/
/* print colored output */
#define PRINT_SUCCESS printf (ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET)
#define PRINT_FAILURE printf (ANSI_COLOR_RED "FAILURE\n" ANSI_COLOR_RESET)

/* generates random number from 1 to 100 */
#define RANDOM_NUM ((rand() % 100) + 1)

#define UNUSED(x) (void)(x)
/**************************** Forward Declarations ****************************/
int CompareElements(const void *data1, const void *data2);
static void SortedListCreateTest(sorted_list_ty *sorted_list);
static void SortedListDestroyTest(sorted_list_ty *list1, sorted_list_ty *list2);
static void SortedListIteratorBeginTest(sorted_list_ty *sorted_list);
static void SortedListInsertTest(sorted_list_ty *sorted_list);
static void SortedListRemoveTest(sorted_list_ty *sorted_list);
static void SortedListIsEmptyTest(sorted_list_ty *sorted_list);
static void SortedListPopFrontTest(sorted_list_ty *sorted_list);
static void SortedListPopBackTest(sorted_list_ty *sorted_list);
static void SortedListSetDataTest(sorted_list_ty *sorted_list);
/*static status_ty PrintList(void *data, void *param);*/
static boolean_ty IsMatch(const void *data, void *param);
/*static void InsertIntToList(sorted_list_ty *sorted_list);*/
/*static void SortedListForEachTest(sorted_list_ty *sorted_list);*/
/*static void SortedListFindTest(sorted_list_ty *sorted_list);*/
/*static void SortedListMultiFindTest(sorted_list_ty *sorted_list, sorted_list_ty *src_list);*/
/*static void SortedListSpliceTest(sorted_list_ty *sorted_list_dest, sorted_list_ty *sorted_list_src);*/
/******************************************************************************/
/******************************* Main__Function *******************************/
int main()	
{
	/* Intializes two empty new sorted lists */
	sorted_list_ty *dest_list = SortedListCreate(CompareElements);
	sorted_list_ty *src_list = SortedListCreate(CompareElements);
	
	/* Intializes random number generator */
	srand(time(0));
	
	SortedListCreateTest(dest_list);
	SortedListIteratorBeginTest(dest_list);
	SortedListInsertTest(dest_list);
	SortedListRemoveTest(dest_list);
	SortedListIsEmptyTest(dest_list);
	SortedListPopFrontTest(dest_list);
	SortedListPopBackTest(dest_list);
/*	SortedListSetDataTest(dest_list);*/
/*	InsertIntToList(dest_list);*/
/*	SortedListForEachTest(dest_list);*/
/*	SortedListFindTest(dest_list);*/
/*	SortedListMultiFindTest(dest_list, src_list);*/
/*	SortedListSpliceTest(dest_list, src_list);*/
	SortedListDestroyTest(dest_list, src_list);
	
	return (0);
}
/******************************************************************************/
/************************Test Functions Implementations************************/
/******************************************************************************/
static void SortedListCreateTest(sorted_list_ty *sorted_list)
{
	printf("\nSorted List Creation Test: ");
	NULL == sorted_list ? PRINT_FAILURE : PRINT_SUCCESS;
}
/******************************************************************************/
static void SortedListDestroyTest(sorted_list_ty *list1, sorted_list_ty *list2)
{
	SortedListDestroy(list1);
	SortedListDestroy(list2);
	printf (ANSI_COLOR_CYAN "\nThe Lists have been deleted\n\n" 
															ANSI_COLOR_RESET);
}
/******************************************************************************/
static void SortedListIteratorBeginTest(sorted_list_ty *sorted_list)
{
	printf("Iter To Begin/End/IsEqual Test: ");
	SortedListIteratorIsEqual(SortedListIteratorBegin(sorted_list),
	 		SortedListIteratorEnd(sorted_list)) ? PRINT_SUCCESS : PRINT_FAILURE;
	 											  
}
/******************************************************************************/
static void SortedListInsertTest(sorted_list_ty *sorted_list)
{	
	boolean_ty is_working = TRUE;
	sorted_list_iter_ty new_node = NULL;
	sorted_list_iter_ty tail = SortedListIteratorEnd(sorted_list);
	
	size_t num_of_inserts = 10, i = 0;
	
	printf("Sorted List Insert+Next+GetData Test: ");
	
	for(i = 0; i < num_of_inserts; ++i)
	{
		new_node = SortedListInsert(sorted_list, (void *)(long)RANDOM_NUM);
		/*
		if the tail node is the return value of the insert function,
		it indicates that the insertation has been failed.
		*/
		is_working *= !SortedListIteratorIsEqual(tail, new_node);
	}
	
	new_node = SortedListIteratorBegin(sorted_list);
	
	/* check if the list is sorted as needed by the criteria */
	while (num_of_inserts - 1 > 0 && CompareElements(
					SortedListGetData(new_node),
					SortedListGetData(SortedListIteratorNext(new_node))) >= 0)
	{
		new_node = SortedListIteratorNext(new_node);
		--num_of_inserts;
	}
	
	is_working *= SortedListIteratorIsEqual(
										tail, SortedListIteratorNext(new_node));
	
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
	 											
}
/******************************************************************************/
static void SortedListRemoveTest(sorted_list_ty *sorted_list)
{
	boolean_ty is_working = TRUE;
	size_t original_size = SortedListSize(sorted_list);
	printf("%lu", original_size);
	printf("Sorted List Remove Test: ");
	
	while(original_size > 0)
	{
		SortedListRemove(SortedListIteratorBegin(sorted_list));
		is_working *= (original_size - 1) == SortedListSize(sorted_list);
		--original_size;
	}
	
	is_working ? PRINT_SUCCESS : PRINT_FAILURE;
	
}
/******************************************************************************/
static void SortedListIsEmptyTest(sorted_list_ty *sorted_list)
{
	printf("Sorted List IsEmpty Test: ");
	TRUE == SortedListIsEmpty(sorted_list) ? PRINT_SUCCESS : PRINT_FAILURE;
}
/******************************************************************************/
static void SortedListPopFrontTest(sorted_list_ty *sorted_list)
{	
}
/******************************************************************************/
static void SortedListPopBackTest(sorted_list_ty *sorted_list)
{
	
}
/******************************************************************************/
static void SortedListSetDataTest(sorted_list_ty *sorted_list)
{

}
/******************************************************************************/
int CompareElements(const void *data1, const void *data2)
{
	assert(data1);
	assert(data2);
	
	return ((int)(long)(data2) - (int)(long)(data1));
}
/******************************************************************************/
static boolean_ty IsMatch(const void *data, void *param)
{	
	return (((int)(long)data == (int)(long)param));
}
/******************************************************************************/
