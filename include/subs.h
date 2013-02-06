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

#ifndef _SUBS_H
#define _SUBS_H

#include "splayq.h"

spsubtree *spsubinit();
int spsubenq(sptree *tree, spsubtree *q, subkey_t subkey, value_t value);
int spsubdeq(sptree *tree, spsubtree *q, subkey_t *subkey, value_t *value);
int spsubfind1(spsubtree *q, subkey_t *subkey, value_t *val);
int spsubfind(spsubtree *q, subkey_t subkey, value_t *val);
int spsubdel(sptree *tree, spsubtree *q, subkey_t subkey, value_t *val);

#endif
