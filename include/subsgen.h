/*
 * File:		subs.h
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

#ifndef _SUBSGEN_H
#define _SUBSEGN_H

#include "splayqgen.h"

genspsubtree *genspsubinit();
int genspsubenq(gensptree *tree, genspsubtree *q, gensubkey_t subkey, genvalue_t value, gendata_t data);
int genspsubdeq(gensptree *tree, genspsubtree *q, gensubkey_t *subkey, genvalue_t *value, gendata_t *data);
int genspsubfind1(genspsubtree *q, gensubkey_t *subkey, genvalue_t *val, gendata_t *data);
int genspsubfind(genspsubtree *q, gensubkey_t subkey, genvalue_t *val, gendata_t *data);
int genspsubdel(gensptree *tree, genspsubtree *q, gensubkey_t subkey, genvalue_t *val, gendata_t *data);

#endif
