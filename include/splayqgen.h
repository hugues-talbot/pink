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

#ifndef _SPLAYQGEN_H
#define _SPLAYQGEN_H

typedef int genprikey_t;
typedef int gensubkey_t;
typedef int genvalue_t;
typedef void *gendata_t;

typedef struct genspnode_t
{
	struct genspnode_t *next;

	gensubkey_t subkey;
	genvalue_t val;
        gendata_t data;  
} genspnode_t;

typedef struct genspsubtree
{
	struct genspnode_t *first;
	struct genspnode_t *last;
} genspsubtree;

typedef struct gen_i_spnode_t
{
	struct gen_i_spnode_t *up;
	struct gen_i_spnode_t *left;
	struct gen_i_spnode_t *right;

	genprikey_t	key;
	genspsubtree *sub;
} gen_i_spnode_t;

typedef struct gensptree
{
	struct gen_i_spnode_t *root;
	int garbage_status;
	struct gen_i_spnode_t *garbage;
	struct genspnode_t *subgarbage;
} gensptree;


#define spgaron(tree)	(tree)->garbage_status = 1

gensptree *genspinit();
void genspclose(gensptree *tree);
/*void spgaron(sptree *tree);*/
void genspgaroff(gensptree *tree);
int genspenq(gensptree *tree, genprikey_t key, gensubkey_t subkey, genvalue_t value, gendata_t data);
int genspdeq(gensptree *tree, genprikey_t *key, gensubkey_t *subkey, genvalue_t *value, gendata_t *data);
int genspfind(gensptree *tree, genprikey_t key, gensubkey_t *subkey, genvalue_t *value, gendata_t *data);
int genspfind2(gensptree *tree, genprikey_t key, gensubkey_t subkey, genvalue_t *value, gendata_t *data);
int genspdelete(gensptree *tree, genprikey_t key, gensubkey_t *subkey, genvalue_t *value, gendata_t *data);
int genspdelete2(gensptree *tree, genprikey_t key, gensubkey_t subkey, genvalue_t *value, gendata_t *data);

void gensplaypntr(gensptree *tree, gen_i_spnode_t *node);

/* Debugging functions */
void dgenumptree(gensptree *tree);
void gendumpnode(gen_i_spnode_t *node, int lev);
void gendumpq(genspsubtree *q);
#endif
