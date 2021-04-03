/**********************************FILE-HEADER*********************************\
* File: dlist.c						 		  								
* Author: Daniel Brodsky					  								
* Date: 01/04/2021							   								
* Version: Beta				   								
* Reviewer: Danel						   								
* Description: Doubly Linked List Functions Implementations.			 
\******************************************************************************/

/************************** Macros Definitions ********************************/

#define ITER_TO_NODE_PTR(iter) (dlist_node_ty *)iter
#define NODE_PTR_TO_ITER(iter) (dlist_iter_ty)(iter)
#define IS_END(iter) (ITER_TO_NODE_PTR(iter)->next == NULL)

/**************************** Inclusions **************************************/
#include <assert.h>		/* assert */
#include <stddef.h>		/* size_t */
#include <stdio.h> 		/* fprintf, NULL */
#include <stdlib.h>		/* malloc, free */

#include "../include/dlist.h"
#include "../include/utils.h"		/* status_ty, bolean_ty*/

/************************ Global Definitions **********************************/
struct dlist
{
	dlist_iter_ty head;			/* represents the beginning of the list */
	dlist_iter_ty tail;			/* represents the end of the list */
};

struct dlist_node
{
	void *data;
	dlist_iter_ty previous;		/* points to the previous node in the list */
	dlist_iter_ty next;			/* points to the next node in the list */
};

/************************Functions Implementations*****************************/
/******************************************************************************/
dlist_ty *DlistCreate(void)
{
	dlist_ty *new_list = (dlist_ty *)malloc(sizeof(dlist_ty));
	if (NULL == new_list)
	{
		fprintf(stderr, "Failed to allocate memory\n");
		return(NULL);
	}
	
	/* create a dummy node that will represent the end of the list */
	new_list->tail = (dlist_node_ty *)malloc(sizeof(dlist_node_ty));
	if (NULL == new_list->tail)
	{
		fprintf(stderr, "Failed to allocate memory\n");
		free(new_list);
		return(NULL);
	}
	
	/* tail node points to null because it's the last node in the list */
	new_list->tail->data = new_list;
	new_list->tail->previous = NULL;
	new_list->tail->next = NULL;
	
	/* the first (and only) node of an empty list is the end dummy node */
	new_list->head = new_list->tail;
	
	return(new_list);
}
/******************************************************************************/
void DlistDestroy(dlist_ty *dlist)
{
	dlist_node_ty *runner = NULL;
	
	/* If the received dlist is NULL, no operation is performed */
	if (NULL == dlist)
	{
		return;
	}
	
	while(!DlistIsEmpty(dlist))
	{
		runner = dlist->head;
		dlist->head = dlist->head->next;
		free(runner);
	}
	
	free(dlist->tail);
	dlist->tail = NULL;
	
	free(dlist);
	dlist = NULL;
}
/******************************************************************************/
dlist_iter_ty DlistIteratorBegin(const dlist_ty *dlist)
{
	assert(dlist);
	
	return(dlist->head);
}
/******************************************************************************/
dlist_iter_ty DlistIteratorEnd(const dlist_ty *dlist)
{
	assert(dlist);
	
	return(dlist->tail);
}
/******************************************************************************/
dlist_iter_ty DlistIteratorNext(const dlist_iter_ty iter)
{
	assert(ITER_TO_NODE_PTR(iter));
	assert(ITER_TO_NODE_PTR(iter)->next);
	
	return(ITER_TO_NODE_PTR(iter)->next);
}
/******************************************************************************/
dlist_iter_ty DlistIteratorPrevious(const dlist_iter_ty iter)
{
	assert(ITER_TO_NODE_PTR(iter));
	assert(ITER_TO_NODE_PTR(iter)->previous);
	
	return(ITER_TO_NODE_PTR(iter)->previous);
}
/******************************************************************************/
void *DlistGetData(const dlist_iter_ty iter)
{
	assert(iter);
	assert(ITER_TO_NODE_PTR(iter)->next);
	
	return(ITER_TO_NODE_PTR(iter)->data);
}
/******************************************************************************/
void DlistSetData(dlist_iter_ty iter, void *data)
{
	assert(iter);
	assert(ITER_TO_NODE_PTR(iter)->next);
	assert(data);
	
	(ITER_TO_NODE_PTR(iter))->data = data;
}
/******************************************************************************/
boolean_ty DlistIteratorIsEqual(const dlist_iter_ty iter_a, 
											const dlist_iter_ty iter_b)
{
	assert(iter_a);
	assert(iter_b);
	
	return(NODE_PTR_TO_ITER(iter_a) == NODE_PTR_TO_ITER(iter_b));
}
/******************************************************************************/
dlist_iter_ty DlistInsertBefore(dlist_iter_ty iter, void *data)
{
	dlist_iter_ty new_node = NULL;
	
	assert(iter);
	assert(ITER_TO_NODE_PTR(iter)->previous);
	assert(data);
	
	new_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));
	if (NULL == new_node)
	{
		return(iter);
	}
	
	new_node->data = data;
	new_node->previous = ITER_TO_NODE_PTR(iter)->previous;
	new_node->next = NODE_PTR_TO_ITER(iter);
	
	new_node->previous->next = new_node;
	
	(ITER_TO_NODE_PTR(iter))->previous = new_node;
	
	return(new_node);
}
/******************************************************************************/
dlist_iter_ty DlistRemove(dlist_iter_ty iter)
{
	dlist_iter_ty new_node = NULL;
	
	assert(iter);
	assert(ITER_TO_NODE_PTR(iter)->next);
	assert(ITER_TO_NODE_PTR(iter)->previous);
	
	(ITER_TO_NODE_PTR(iter))->data = (ITER_TO_NODE_PTR(iter))->next->data;
	
	new_node = (ITER_TO_NODE_PTR(iter))->next->next;
	
	if(NULL == new_node)
	{
		((dlist_ty *)(ITER_TO_NODE_PTR(iter))->data)->tail = iter;
	}
	
	free((ITER_TO_NODE_PTR(iter))->next);
	
	(ITER_TO_NODE_PTR(iter))->next = new_node;
	
	(ITER_TO_NODE_PTR(new_node))->previous = iter;
	
	return(iter);
}
/******************************************************************************/
/* Inserts a new element to the beginning of the list */
/* returns iterator to the new element on success or dlist_END on failure */
/* Complexity: O(1) */
dlist_iter_ty DlistPushFront(dlist_ty *dlist, void *data)
{
	dlist_iter_ty new_node = NULL;
	
	assert (dlist);
	assert(data);
	
	new_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));
	if (NULL == new_node)
	{
		return(dlist->tail);
	}
	
	new_node->data = data;
	new_node->previous = NULL;
	new_node->next = dlist->head;
	
	dlist->head->previous = new_node;
	
	dlist->head = new_node;
	
	return(new_node);
	
}
/******************************************************************************/
/* Inserts a new element to the end of the list */
/* returns iterator to the new node on success or dlist_END on failure */
/* Complexity: O(1) */
dlist_iter_ty DlistPushBack(dlist_ty *dlist, void *data)
{
	assert(dlist);
	assert(data);

	return (DlistInsertBefore(DlistIteratorEnd(dlist), data));
}
/******************************************************************************/
/* Removes an element from the beginning of the list */
/* Returns the popped element */
/* Undefined if dlist is empty */
/* Complexity: O(1) */
void *DlistPopFront(dlist_ty *dlist)
{
	dlist_iter_ty temp = NULL;
	void *ret = NULL;
	
	assert(!DlistIsEmpty(dlist));
	
	ret = dlist->head->data;
	
	dlist->head->data = dlist->head->next->data;
	
	temp = dlist->head->next->next;
	
	free(dlist->head->next);
	
	dlist->head->next = temp;
	
	temp->previous = dlist->head;
	
	return (ret);
}
/******************************************************************************/
/* Removes an element from the end of the list */
/* Returns the popped element */
/* Undefined if dlist is empty */
/* Complexity: O(1) */
void *DlistPopBack(dlist_ty *dlist)
{
	dlist_iter_ty new_node = NULL;
	void *ret = NULL;
	
	assert(dlist);
	assert(!DlistIsEmpty(dlist));
	
	ret = dlist->tail->previous->data;
	
	dlist->tail->previous->data = dlist;
	
	dlist->tail = dlist->tail->previous;
	
	free(dlist->tail->previous->next);
	
	dlist->tail->previous->next = NULL;
	
	(ITER_TO_NODE_PTR(new_node))->previous = iter;
	
	
	return (ret);
}
/******************************************************************************/
boolean_ty DlistIsEmpty(const dlist_ty *dlist)
{
	assert(dlist);
	
	if (dlist->tail == dlist->head)
	{
		return (TRUE);
	}
	
	return (FALSE);
}
/******************************************************************************/
size_t DlistSize(const dlist_ty *dlist)
{
	size_t counter = 0;
	
	(dlist_node_ty *)nodes_runner = dlist->head;
	
	assert (dlist);
	
	while (dlist->tail != nodes_runner)
	{
		nodes_runner = nodes_runner->next;
		++counter;
	}
	
	return(counter);
}
/******************************************************************************/
dlist_iter_ty DlistFind(const dlist_iter_ty from_iter, 
	const dlist_iter_ty to_iter, IsMatch_Func is_match_func,
	void *param)
{
	dlist_iter_ty runner = NULL;
	
	assert(from_iter);
	assert(to_iter);
	assert(param);

	runner = from_iter;
	
	while(runner != to_iter)
	{
		if(TRUE == is_match_func(runner->data, param))
		{
			return(runner);
		}
		runner = runner->next;
	}

	return(to_iter);
}
/******************************************************************************/
/* Stores each matching data in an element in dlist_output, */
/* in range of [from_iter, to_iter) */
/* Returns the number of matches */
/* returns 0 if data not found */
/* Complexity: O(n) */
size_t DlistMultiFind(const dlist_iter_ty from_iter, 
								const dlist_iter_ty to_iter, 
									IsMatch_Func_ty match_func, void *param,
														dlist_ty *dlist_output)
{	
	dlist_iter_ty runner = NULL;
	size_t matches_counter = 0;
	
	assert(from_iter);
	assert(to_iter);
	assert(dlist_output);
	assert(param);
	
	runner = from_iter;
	
	while(runner != to_iter)
	{
		if(TRUE == is_match_func(runner->data, param))
		{
			DlistPushFront(dlist_output, runner->data);
			++matches_counter;
		}
		runner = runner->next;
	}

	return(matches_counter);
	
}
/******************************************************************************/
status_ty DlistForEach(dlist_iter_ty from_iter,
const dlist_iter_ty to_iter, Action_Func action_func, void *param)
{
	dlist_iter_ty iterator = NULL;
		
	assert(from_iter);
	assert(to_iter);
	
	iterator = from_iter;
	
	while(iterator != to_iter)
	{
		if(SUCCESS != action_func(iterator->data, param))
		{
			return(FAILURE);
		}
		
	iterator = iterator->next;
	}
	
	return(SUCCESS);
}
/******************************************************************************/
dlist_iter_ty Splice(dlist_iter_ty dest_iter, 
								dlist_iter_ty src_from, dlist_iter_ty src_to)
{
	assert(dest_iter);
	assert(src_from);
	assert(src_to);
	
	if (DlistIsEmpty(src_Dlist))
	{
		return;
	}
	
	/* switch tail dummy of dest with first node in src */
	dest_dlist->tail->next = src_dlist->head->next->next;
	dest_dlist->tail->data = src_dlist->head->next->data;
	/* switch tail dummy of src with tail dummy of dest */
	src_dlist->tail->data = dest_Dlist;
	dest_dlist->tail = src_dlist->tail;
	/* make sure src list is empty */
	src_dlist->head->next->next = NULL;
	src_dlist->head->next->data = src_Dlist;
	src_dlist->tail = src_dlist->head->next;
	
}
/******************************************************************************/
