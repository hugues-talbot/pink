/*
 * File:		splayq.h
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

#ifndef _SPLAYS_H
#define _SPLAYS_H

typedef int prikey_t;
typedef int subkey_t;
typedef int value_t;

typedef struct spnode_t
{
	struct spnode_t *next;

	subkey_t subkey;
	value_t val;
} spnode_t;

typedef struct spsubtree
{
	struct spnode_t *first;
	struct spnode_t *last;
} spsubtree;

typedef struct i_spnode_t
{
	struct i_spnode_t *up;
	struct i_spnode_t *left;
	struct i_spnode_t *right;

	prikey_t	key;
	spsubtree *sub;
} i_spnode_t;

typedef struct sptree
{
	struct i_spnode_t *root;
	int garbage_status;
	struct i_spnode_t *garbage;
	struct spnode_t *subgarbage;
} sptree;


#define spgaron(tree)	(tree)->garbage_status = 1

sptree *spinit();
void spclose(sptree *tree);
/*void spgaron(sptree *tree);*/
void spgaroff(sptree *tree);
int spenq(sptree *tree, prikey_t key, subkey_t subkey, value_t value);
int spdeq(sptree *tree, prikey_t *key, subkey_t *subkey, value_t *value);
int spfind(sptree *tree, prikey_t key, subkey_t *subkey, value_t *value);
int spfind2(sptree *tree, prikey_t key, subkey_t subkey, value_t *value);
int spdelete(sptree *tree, prikey_t key, subkey_t *subkey, value_t *value);
int spdelete2(sptree *tree, prikey_t key, subkey_t subkey, value_t *value);

void splaypntr(sptree *tree, i_spnode_t *node);

/* Debugging functions */
void dumptree(sptree *tree);
void dumpnode(i_spnode_t *node, int lev);
void dumpq(spsubtree *q);
#endif
