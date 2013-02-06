/*
 * File:		darray_p.h
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

/* darray private header */
#ifndef DARRAY_PH
#define DARRAY_PH
static void    *hold;     /** ERROR HANDLING */
char   DA_ERROR = 0;      /** ERROR VARIABLE */
 
/*BEGIN DEFINITIONS ***************/

#define ISIZE     a->elmsize
#define ENTRIES   a->N
#define MFACT     a->mod
#define NINC      a->ninc
#define DATA      a->data
#define SPOS      a->spos
#define EPOS      a->epos
#define TSIZE     a->tsize
#define INC       a->inc

#define setINTERNALdata(x,y,size)    memcpy((void*)(x),(void*)(y),(size))

#ifdef Sun
/* prototype for bcopy */
void bcopy(char *b1, char *b2, int length);
#define memmove(x,y,z)     bcopy((char*)(y),(char*)(x),(int)(z))
#else
/* prototype for memmove */
/* void *memmove(void *s1, const void *s2, size_t n); */
#include <string.h>
#endif

#endif

