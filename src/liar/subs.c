/*
 * File:		subs.c
 *
 * Written by:		Image Analysis Group staff,
 * 			CSIRO Mathematical and Information Sciences.
 *
 * Date:		March 2001
 *
 *
 * CSIRO Mathematical and Information Sciences is the owner of all
 * copyright subsisting in the software contained in this file. It may
 * not be disclosed to, or used by, anyone without the prior approval 
 * of the Chief, CSIRO Mathematical and Information Sciences.
 *
*/

#include <stdio.h>
#include <stdlib.h>

#include "liarp.h"
#include "splayq.h"

spsubtree *spsubinit()
{
	/* returns a pointer to a new hqueue */
	spsubtree *new;

	new = (spsubtree*)malloc(sizeof(spsubtree));
	new->first = NULL;
	new->last = NULL;
	return new;
}

int spsubenq(sptree *tree, spsubtree *q, subkey_t subkey, value_t value)
{
	spnode_t *new;

	if (tree->garbage_status && tree->subgarbage)
	{
		new = tree->subgarbage;
		tree->subgarbage = tree->subgarbage->next;
	}
	else
		new = (spnode_t*)malloc(sizeof(spnode_t));

	new->subkey = subkey;
	new->val = value;
	new->next  = NULL;
	if (!q->first)
	{
		q->first = new;
		q->last = new;
	}
	else
		q->last->next = new;
	q->last = new;
	return 1;
}

int spsubdeq(sptree *tree, spsubtree *q, subkey_t *subkey, value_t *value)
{
	spnode_t *ptr1;

	/* Rip off top of list */
	if(!q->first)
		return 0;
	ptr1 = q->first;
	q->first = ptr1->next;
	if(!q->first)
		q->last = NULL;
	/* Fill in the bits */
	*subkey = ptr1->subkey;
	*value = ptr1->val;
	if (tree->garbage_status)
	{
		/* Stick the old thing on the garbage */
		ptr1->next = tree->subgarbage;
		tree->subgarbage = ptr1;
	}
	else
		free(ptr1);
	return 1;
}

int spsubfind1(spsubtree *q, int *subkey, int *val)
{

	if (!q->first)
		return 0;

	*val = q->first->val;	/* Got it, so fill in the field */
	*subkey = q->first->subkey;
	return 1;
}

int spsubfind(spsubtree *q, int subkey, int *val)
{

	spnode_t *ptr;

	if (!q->first)
		return 0;

	for (ptr = q->first; ptr && (ptr->subkey != subkey); ptr = ptr->next)
		;
	if (!ptr)
		return 0;	/* Fell off list, isn't there */
	*val = ptr->val;	/* Got it, so fill in the field */
	return 1;
}

int spsubdel(sptree *tree, spsubtree *q, int subkey, int *val)
{

	spnode_t *p = NULL, *pc;

	if (!q->first)
		return 0;
	pc = q->first;
	do {
	    if(pc->subkey == subkey)
		break;
	    p = pc;
	    pc = pc->next;
	}while(pc);

	if(!pc) /* item not in list */
	    return 0;
	if(!p) /* got first item */
	    q->first = pc->next;
	else
	    p->next = pc->next;
	if(!pc->next) /* got last item */
	    q->last = p;

	*val = pc->val;
	if(tree->garbage_status)
	{
		pc->next = tree->subgarbage;
		tree->subgarbage = pc;
	}
	else
		free(pc);
	return 1;
}
/****************************** old code by david
int spsubdel(sptree *tree, spsubtree *q, int subkey, int *val)
{

	spnode_t *ptr, *ptr2;

	if (!q->first)
		return 0;

	if (q->first->subkey == subkey) {
		*val = q->first->val;
		q->first = q->first->next;
		if (!q->first)
			q->last = NULL;

		return 1;
	} 
	
	
	for (ptr = q->first; ptr->next && (ptr->next->subkey != subkey); ptr = ptr->next)
		;
	if (!ptr->next) {
	    LIARerror(" fell off end of list \n");
		return 0;	/ Fell off end of list /
	}

	*val = ptr->next->val;
	ptr2 = ptr->next;	/ We delete this one /
	ptr->next = ptr->next->next;	/ Cut it out /
	if(tree->garbage_status)
	{
		ptr2->next = tree->subgarbage;
		tree->subgarbage = ptr2;
	}
	else
		free(ptr2);
	return 1;
}
***********************************/

void dumptree(sptree *tree)
{
	dumpnode(tree->root, 0);
}

void dumpnode(i_spnode_t *node, int lev)
{
    int i;
    char LIARstrbuf[1024];
    
    if (!node) return;
    dumpnode(node->right, lev+1);
    for(i=0;i<lev;i++)
	LIARdebug("\t");
    sprintf(LIARstrbuf,"%d:<-%d:", node->key, (node->up) ?
	    node->up->key : -1);
    LIARdebug(LIARstrbuf);
    dumpq(node->sub);
    dumpnode(node->left, lev+1);
}

void dumpq(spsubtree *q)
{
    spnode_t *tmp;
    char LIARstrbuf[1024];
    
    for(tmp = q->first;tmp;tmp = tmp->next)
    {
	sprintf(LIARstrbuf, "(%d,%d)", tmp->subkey, tmp->val);
	LIARdebug(LIARstrbuf);
    }
    LIARdebug("|\n");
}














