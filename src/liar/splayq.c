/*
 * File:		splayq.c
 *
 * Written by:		Image Analysis Group staff,
 * 			CSIRO Mathematical and Information Sciences.
 *
 * Date:		March 2001
 *
 *
 * Splay queues, a type of priority queues. Iniated by Ed Breen, 1993.
 *
 * 
 *
*/

#include <stdio.h>
#include <stdlib.h>

#include "liarp.h"
#include "splayq.h"
#include "subs.h"


sptree *spinit()
{
	sptree *tmp;
	tmp = (sptree*)malloc(sizeof(sptree));
	tmp->root = NULL;
	tmp->garbage_status = 1;	/* Collect garbage by default */
	tmp->garbage = NULL;
	tmp->subgarbage = NULL;
	return tmp;
}

int spenq(sptree *tree, prikey_t key, subkey_t subkey, value_t value)
{
	i_spnode_t *parent=NULL, *tmp;
	int found = 0, finish = 0;

	tmp = tree->root;
	if (!tmp)
	{
		finish = 1;
		found = 0;
		parent = NULL;
	}

	while (!finish)
	{
		if(tmp->key == key)
		{
			found = 1;
			finish = 1;
			continue;
		}
		if(key < tmp->key)
		{
			if(tmp->left)
			{
				tmp = tmp->left;
				continue;
			}
			else
			{
				finish = 1;
				found = 0;
				parent = tmp;
				continue;
			}
		}
		if(key > tmp->key)
		{
			if(tmp->right)
			{
				tmp = tmp->right;
				continue;
			}
			else
			{
				finish = 1;
				found = 0;
				parent = tmp;
				continue;
			}
		}
	}

	if(!found)
	{
		i_spnode_t *new;

		/* There wasn't one in the tree, prev node is in parent */
		if(tree->garbage_status && tree->garbage)
		{
			new = tree->garbage;
			tree->garbage = tree->garbage->up;
		}
		else
			new = (i_spnode_t*)malloc(sizeof(i_spnode_t));
		new->up = parent;
		new->left = NULL;
		new->right = NULL;
		new->key = key;
		new->sub = spsubinit();
		if(!parent)	/* This is the first item in the tree */
			tree->root = new;
		else if (key < parent->key)
			parent->left = new;
		else
			parent->right = new;
		tmp = new;
	}
		
	/* tmp points to the found node, or the node we have just created */
	spsubenq(tree, tmp->sub, subkey, value);

	splaypntr(tree, tmp);

	return 1;
}

int spdeq(sptree *tree, prikey_t *key, subkey_t *subkey, value_t *value)
{
	/* Procudure - find the leftmost node in the tree, deq that one */

	i_spnode_t *tmp1;

	if (!tree->root)
		return 0;	/* Nothing to dequeue */

	for(tmp1 = tree->root;tmp1->left;tmp1 = tmp1->left);	/* Go left */

	if(!spsubdeq(tree, tmp1->sub, subkey, value))
	{
	        LIARerror("Fell off (old) queue\n");
		return 0;	/* Bad news - an empty queue */
	}

	*key = tmp1->key;

	/* Ok, now if the queue is empty, we must remove this node.
	** Note that the left subtree must be empty, so we can just replace it
	** with the right subtree, and splay at the parent (sleator-tarjan85)
	** We also know we are a left child. It is possible that we are the
	** root of the tree, in which case modify tree->root.
	*/

	if(!tmp1->sub->first)
	{
/*		LIARdebug("Scrapping old queue\n");*/
		if(tmp1->up)	/* Not root */
		{
			tmp1->up->left = tmp1->right;	/* This cuts us out */
			if(tmp1->right)
				tmp1->right->up = tmp1->up;
			/* Splay tmp1->up here... */
			splaypntr(tree, tmp1->up);
		}
		else	/* root */
		{
			tree->root = tmp1->right;	/* This cuts us out */
			if(tmp1->right)
				tree->root->up = NULL;
		}
		free(tmp1->sub);		/* junk the empty queue */
		if (tree->garbage_status)
		{
			tmp1->up = tree->garbage;	/* Garbage collect us */
			tree->garbage = tmp1;
		}
		else
			free(tmp1);
	}
	else
		splaypntr(tree, tmp1);
	

	return 1;
}

int spfind(sptree *tree, prikey_t key, subkey_t *subkey, value_t *value)
{
	i_spnode_t *ptr;

	/* First find the appropriate splay tree node */
	for(ptr = tree->root; ptr && (ptr->key != key); ptr = (key < ptr->key) ? ptr->left : ptr->right)
		;
	/* now at node, or fallen off */
	if (!ptr)
		return 0;

	return spsubfind1(ptr->sub, subkey, value);
}

int spfind2(sptree *tree, prikey_t key, subkey_t subkey, value_t *value)
{
	i_spnode_t *ptr;

	/* First find the appropriate splay tree node */
	for(ptr = tree->root; ptr && (ptr->key != key); ptr = (key < ptr->key) ? ptr->left : ptr->right)
		;
	/* now at node, or fallen off */
	if (!ptr)
		return 0;

	return spsubfind(ptr->sub, subkey, value);
}

int spdelete2(sptree *tree, prikey_t key, subkey_t subkey, value_t *value)
{
	int retval;
	i_spnode_t *ptr;

	/* First find the appropriate splay tree node */
	for(ptr = tree->root; ptr && (ptr->key != key); 
	                       ptr = (key < ptr->key) ? ptr->left : ptr->right)
	    ;
	/* now at node, or fallen off */
	if (!ptr)
	{
		LIARdebug("Warning  - fell out of tree in spdelete2\n");
		return 0;
	}


	retval = spsubdel(tree, ptr->sub, subkey, value);
	if(!retval)
		LIARdebug("Warning - spsubdel failed!\n");


	if(!ptr->sub->first) {
	    sptree *tmptree;
	    i_spnode_t *rtmp;
	    
	    /* Need to delete this node. Should be easy, splay the
	     ** rightmost node of the left subtree to the root of
	     ** the left subtree, attach this at the current point with
	     ** the right subtree as its right child.
	     */
	    
	    /* First deal with trivial cases */
	    if(!ptr->left) {
		if(!ptr->right) {
		    /* We are a leaf node! */
		    if(ptr->up) {
			if(ptr->up->left == ptr)
			    ptr->up->left = NULL;
			else
			    ptr->up->right = NULL;
			splaypntr(tree, ptr->up);
		    }else
			tree->root = NULL;
		}else {
		    /* No left, so replace us with right */
		    if(ptr->up){
			if(ptr->up->left == ptr)
			    ptr->up->left = ptr->right;
			else
			    ptr->up->right = ptr->right;
		    } else 
			tree->root = ptr->right;
		    ptr->right->up = ptr->up;
		    splaypntr(tree, ptr->right);
		}
		free(ptr->sub);		 /* Kill empty subqueue */
		if(tree->garbage_status) {
		    ptr->up = tree->garbage; /* Do garbage collection */
		    tree->garbage = ptr;
		} else
		    free(ptr);
		return retval;
	    }
	    if(!ptr->right) {
		/* Can't be a leaf node  - replace with left */
		if(ptr->up) {
		    if(ptr->up->left == ptr)
			ptr->up->left = ptr->left;
		    else
			ptr->up->right = ptr->left;
		} else
		    tree->root = ptr->left;
		ptr->left->up = ptr->up;
		free(ptr->sub);		 /* Kill empty subqueue */
		if(tree->garbage_status) {
		    ptr->up = tree->garbage; /* Do garbage collection */
		    tree->garbage = ptr;
		} else
		    free(ptr);
		splaypntr(tree, ptr->left);
		return retval;
	    }
	    
	    /* First create a tree for splay to play with */
	    tmptree = (sptree*)malloc(sizeof(sptree));
	    tmptree->root = ptr->left;
	    /* Don't have to worry about tmptree->root being null */
	    tmptree->root->up = NULL; /* So the splay terminates */
	    for(rtmp = tmptree->root;rtmp->right;rtmp = rtmp->right);
	    /* Do the splay */
	    splaypntr(tmptree, rtmp);
	    /* Now, fix it up */
#ifdef DEBUG
	    if(tmptree->root->right)
	    {
		LIARdebug("Panic - splay did something stupid!\n");
		LIARdebug("Node key is %d, root is%d \n", ptr->key, tree->root->key);
		LIARdebug("Dump of tree:\n");
		dumptree(tree);
	    }
#endif
	    tmptree->root->right = ptr->right; /* root->right was empty */
	    if(tmptree->root->right)
		tmptree->root->right->up = tmptree->root;
	    tmptree->root->up = ptr->up;
	    if(ptr->up) {
		if(ptr->up->left == ptr)
		    ptr->up->left = tmptree->root;
		else
		    ptr->up->right = tmptree->root;
	    } else 
		tree->root = tmptree->root;
	    splaypntr(tree, tmptree->root);
	    free(tmptree);
	    free(ptr->sub);			/* Kill empty subqueue */
	    if(tree->garbage_status) {
		ptr->up = tree->garbage;	/* Do garbage collection */
		tree->garbage = ptr;
	    } else
		free(ptr);
	}else
	    splaypntr(tree, ptr);
	return retval;
}

int spdelete(sptree *tree, prikey_t key, subkey_t *subkey, value_t *value)
{
	int retval;
	i_spnode_t *ptr;

	/* First find the appropriate splay tree node */
	for(ptr = tree->root; ptr && (ptr->key != key); ptr = (key < ptr->key) ? ptr->left : ptr->right)
		;
	/* now at node, or fallen off */
	if (!ptr)
	{
		LIARdebug("Warning  - fell out of tree in spdelete2\n");
		return 0;
	}


	retval = spsubdeq(tree, ptr->sub, subkey, value);
	if(!retval)
		LIARdebug("Warning - spsubdeq failed!\n");

	if(!ptr->sub->first)
	{
		sptree *tmptree;
		i_spnode_t *rtmp;

		/* Need to delete this node. Should be easy, splay the
		** rightmost node of the left subtree to the root of
		** the left subtree, attatch this at the current point with
		** the right subtree as its right child.
		*/

		/* First deal with trivial cases */
		if(!ptr->left)
		{
			if(!ptr->right)
			{
				/* We are a leaf node! */
				if(ptr->up)
				{
					if(ptr->up->left == ptr)
						ptr->up->left = NULL;
					else
						ptr->up->right = NULL;
					splaypntr(tree, ptr->up);
				}
				else
					tree->root = NULL;
			}
			else
			{
				/* No left, so replace us with right */
				if(ptr->up)
				{
					if(ptr->up->left == ptr)
						ptr->up->left = ptr->right;
					else
						ptr->up->right = ptr->right;
				}
				else tree->root = ptr->right;
				ptr->right->up = ptr->up;
				splaypntr(tree, ptr->right);
			}
			free(ptr->sub);		 /* Kill empty subqueue */
			if(tree->garbage_status)
			{
				ptr->up = tree->garbage; /* Do garbage collection */
				tree->garbage = ptr;
			}
			else
				free(ptr);
			return retval;
		}
		if(!ptr->right)
		{
			/* Can't be a leaf node  - replace with left */
			if(ptr->up)
			{
				if(ptr->up->left == ptr)
					ptr->up->left = ptr->left;
				else
					ptr->up->right = ptr->left;
			}
			else
				tree->root = ptr->left;
			ptr->left->up = ptr->up;
			free(ptr->sub);		 /* Kill empty subqueue */
			if(tree->garbage_status)
			{
				ptr->up = tree->garbage; /* Do garbage collection */
				tree->garbage = ptr;
			}
			else
				free(ptr);
			splaypntr(tree, ptr->left);
			return retval;
		}
	
		/* First create a tree for splay to play with */
		tmptree = (sptree*)malloc(sizeof(sptree));
		tmptree->root = ptr->left;
		/* Don't have to worry about tmptree->root being null */
		tmptree->root->up = NULL; /* So the splay terminates */
		for(rtmp = tmptree->root;rtmp->right;rtmp = rtmp->right);
		/* Do the splay */
		splaypntr(tmptree, rtmp);
		/* Now, fix it up */
#ifdef DEBUG
		if(tmptree->root->right)
		{
			LIARdebug("Panic - splay did something stupid!\n");
			LIARdebug("Node key is %d, root is%d \n", ptr->key, tree->root->key);
			LIARdebug("Dump of tree:\n");
			dumptree(tree);
		}
#endif
		tmptree->root->right = ptr->right; /* root->right was empty */
		if(tmptree->root->right)
			tmptree->root->right->up = tmptree->root;
		tmptree->root->up = ptr->up;
		if(ptr->up)
		{
			if(ptr->up->left == ptr)
				ptr->up->left = tmptree->root;
			else
				ptr->up->right = tmptree->root;
		}
		else 
			tree->root = tmptree->root;
		splaypntr(tree, tmptree->root);
		free(tmptree);
		free(ptr->sub);			/* Kill empty subqueue */
		if(tree->garbage_status)
		{
			ptr->up = tree->garbage;	/* Do garbage collection */
			tree->garbage = ptr;
		}
		else
			free(ptr);
	}
	else
		splaypntr(tree, ptr);
	return retval;
}

void splaypntr(sptree *tree, i_spnode_t *node)
{
	i_spnode_t *ptr, *yp, *zp;

	ptr = node;
	if(!ptr->up)
		return;	/* Node is already at root */
	while(ptr->up && ptr->up->up)	/* not at terminal step */
	{
		/* Four cases */
		/* Are we a right or left child? */
		if (ptr->up->left == ptr)
		{	/* Left */
			/* straight or zigzag ? */
			if(ptr->up->up->left == ptr->up)
			{	/* Straight - (b) forwards */
				yp = ptr->up;
				zp = ptr->up->up;
				ptr->up = zp->up;
				if(zp->up) {
				    if (zp->up->left == zp)
					zp->up->left = ptr;
				    else
					zp->up->right = ptr;
				}
				
				zp->left = yp->right;
				if (yp->right)
					yp->right->up = zp;
				yp->right = zp;
				zp->up = yp;
				yp->left = ptr->right;
				if(ptr->right)
					ptr->right->up = yp;
				ptr->right = yp;
				yp->up = ptr;
			}
			else
			{	/* Skew - (case 3) forwards */
				yp = ptr->up;
				zp = ptr->up->up;
				ptr->up = zp->up;
				if(zp->up) {
				    if (zp->up->left == zp)
					zp->up->left = ptr;
				    else
					zp->up->right = ptr;
				}
					
				yp->left = ptr->right;
				if(ptr->right)
					ptr->right->up = yp;
				ptr->right = yp;
				yp->up = ptr;
				zp->right = ptr->left;
				if(ptr->left)
					ptr->left->up = zp;
				ptr->left = zp;
				zp->up = ptr;
			}
		}
		else
		{	/* Right */
			/* Are we skew or straight? */
			if (ptr->up->up->left == ptr->up)
			{	/* Skew - symmetric case 3 */
				yp = ptr->up;
				zp = ptr->up->up;
				ptr->up = zp->up;
				if(zp->up) {
				    if (zp->up->left == zp)
					zp->up->left = ptr;
				    else
					zp->up->right = ptr;
				}
					
				yp->right = ptr->left;
				if(ptr->left)
					ptr->left->up = yp;
				ptr->left = yp;
				yp->up = ptr;
				zp->left = ptr->right;
				if(ptr->right)
					ptr->right->up = zp;
				ptr->right = zp;
				zp->up = ptr;
			}
			else
			{	/* Straight - (b) backwards */
				yp = ptr->up;
				zp = ptr->up->up;
				ptr->up = zp->up;
				if(zp->up) {
				    if (zp->up->left == zp)
					zp->up->left = ptr;
				    else
					zp->up->right = ptr;
				}

				zp->right = yp->left;
				if(yp->left)
					yp->left->up = zp;
				yp->left = zp;
				zp->up = yp;
				yp->right = ptr->left;
				if(ptr->left)
					ptr->left->up = yp;
				ptr->left = yp;
				yp->up = ptr;
			}
		}

	}

	if (!ptr->up)
	{
		tree->root = ptr;
		return;	/* Already at root */
	}
	/* Final rotation... */
	if (ptr->up->left == ptr)	/* We were the left child of the root*/
	{
		ptr->up->left = ptr->right;	/* our right cld is ups left */
		if(ptr->right)
			ptr->right->up = ptr->up; /* Fix old right cld up ptr*/
		ptr->right = ptr->up;	/* old up is new right child */
		ptr->right->up = ptr;	/* Fix the old ups up ptr */
		tree->root = ptr;	/* Fix the root */
		ptr->up = NULL;		/* And cut the up ptr */
	}
	else
	{
		ptr->up->right = ptr->left;	/* our left cld is ups right */
		if(ptr->left)
			ptr->left->up = ptr->up; /* Fix old left cld up ptr */
		ptr->left = ptr->up;	/* old up is new left child */
		ptr->left->up = ptr;	/* Fix the old ups up ptr */
		tree->root = ptr;	/* Fix the root */
		ptr->up = NULL;		/* And cut the up ptr */
	}
	return;
}

void spgaroff(sptree *tree)
{
	i_spnode_t *tmp1, *tmp2=NULL;
	spnode_t *stmp1, *stmp2;

	for (tmp1 = tree->garbage; tmp1; tmp1 = tmp2)
	{
		tmp2 = tmp1->up;
		free (tmp1);
	}
	tree->garbage = NULL;

	for (stmp1 = tree->subgarbage; stmp1; stmp1 = stmp2)
	{
		stmp2 = stmp1->next;
		free (stmp1);
	}
	tree->subgarbage = NULL;

	tree->garbage_status = 0;
}

void spclose(sptree *tree)
{
	prikey_t d1;
	subkey_t d2;
	value_t d3;

	spgaroff(tree);	/* Clear up the memory in the garbage collection */
	while(spdeq(tree, &d1, &d2, &d3))
		;
	free (tree);
}
