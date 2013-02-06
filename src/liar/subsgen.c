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
#include "splayqgen.h"
#include "subsgen.h"

genspsubtree *genspsubinit()
{
	/* returns a pointer to a new hqueue */
	genspsubtree *new;

	new = (genspsubtree*)malloc(sizeof(genspsubtree));
	new->first = NULL;
	new->last = NULL;
	return new;
}

int genspsubenq(gensptree *tree, genspsubtree *q, gensubkey_t subkey, genvalue_t value, gendata_t data)
{
	genspnode_t *new;

	if (tree->garbage_status && tree->subgarbage)
	{
		new = tree->subgarbage;
		tree->subgarbage = tree->subgarbage->next;
	}
	else
		new = (genspnode_t*)malloc(sizeof(genspnode_t));

	new->subkey = subkey;
	new->val = value;
	new->data = data;
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

int genspsubdeq(gensptree *tree, genspsubtree *q, gensubkey_t *subkey, genvalue_t *value, gendata_t *data)
{
	genspnode_t *ptr1;

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
	*data = ptr1->data;
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

int genspsubfind1(genspsubtree *q, int *subkey, int *val, gendata_t *data)
{

	if (!q->first)
		return 0;

	*val = q->first->val;	/* Got it, so fill in the field */
	*data = q->first->data;
	*subkey = q->first->subkey;
	return 1;
}

int genspsubfind(genspsubtree *q, int subkey, int *val, gendata_t *data)
{

	genspnode_t *ptr;

	if (!q->first)
		return 0;

	for (ptr = q->first; ptr && (ptr->subkey != subkey); ptr = ptr->next)
		;
	if (!ptr)
		return 0;	/* Fell off list, isn't there */
	*val = ptr->val;	/* Got it, so fill in the field */
	*data = ptr->data;
	return 1;
}

int genspsubdel(gensptree *tree, genspsubtree *q, int subkey, int *val, gendata_t *data)
{

	genspnode_t *p = NULL, *pc;

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
	*data = pc->data;
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
int spsubdel(gensptree *tree, spsubtree *q, int subkey, int *val)
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

void gendumptree(gensptree *tree)
{
	gendumpnode(tree->root, 0);
}

void gendumpnode(gen_i_spnode_t *node, int lev)
{
    int i;
    char LIARstrbuf[1024];
    
    if (!node) return;
    gendumpnode(node->right, lev+1);
    for(i=0;i<lev;i++)
	LIARdebug("\t");
    sprintf(LIARstrbuf,"%d:<-%d:", node->key, (node->up) ?
	    node->up->key : -1);
    LIARdebug(LIARstrbuf);
    gendumpq(node->sub);
    gendumpnode(node->left, lev+1);
}

void gendumpq(genspsubtree *q)
{
    genspnode_t *tmp;
    char LIARstrbuf[1024];
    
    for(tmp = q->first;tmp;tmp = tmp->next)
    {
	sprintf(LIARstrbuf, "(%d,%d)", tmp->subkey, tmp->val);
	LIARdebug(LIARstrbuf);
    }
    LIARdebug("|\n");
}














