/*
 * File:		darray.c
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

/*** BEGIN DARRAY MANAGEMENT SUBROUTINES *********************/
#include <stdio.h>
#include <stdlib.h>
#include "darray.h"
#include "darray_p.h"


/*BEGIN PROTOTYPES *******/

int find_item(darray_t *a, void *data, int (*compar)());
void free_darray(darray_t *a);
darray_t *crt_darray(unsigned int len,unsigned int mod);
int delete_item(darray_t *a,int row);
int put_item(darray_t *a,void *data,int row);
int get_item(darray_t *a,void *data,int row);
int pop_first(darray_t * a,void * data);
int get_first(darray_t * a,void * data);
int pop_here(darray_t *a,void * data,int row);
int pop_last(darray_t * a,void * data);
int get_last(darray_t * a,void * data);
int push_here(darray_t *a,void *data,int row);
int push_inorder(darray_t *a, void *data, int (*compar)());
int push_first(darray_t *a, void * data);
int push_last(darray_t *a, void  *data);

/*END PROTOTYPES ***************/



int find_item(darray_t *a, void *data, int (*compar)())
{
    static int low, high, mid, x;
  
    if(!ENTRIES)
	return -1;

   
    low = SPOS + 1; high = EPOS - 1;
    while(low<=high) {
	mid = (low+high)>>1;
	x = (*compar)(data, (char*)(DATA) + mid*ISIZE);
	if(x < 0)
	    high = mid - 1;
	else if(x > 0)		
	    low = mid + 1;
	else
		return (mid - SPOS - 1);
    }
    return -1;
}

void free_darray(darray_t *a)
{
    if (DATA)
	free((void *)DATA);
    free((void*)a);
} 


darray_t *crt_darray(unsigned int len,unsigned int mod)
{

    darray_t *a;
    a = (darray_t*)calloc(1,sizeof(darray_t));
    if(a != NULL) { 
        ISIZE = len;
        MFACT = mod;
	SPOS  = -1;
	INC  = MFACT * ISIZE;
	DATA = (void *)calloc(1,1); /* initiate data area */
    }
    return(a);
}

int delete_item(darray_t *a,int row)
{
    static unsigned size;
    static void * ptr;
     
    if((row >= (int)(ENTRIES)) || (row < 0)) {
        DA_ERROR = 2;
        return 0;
    }
    size = (ENTRIES - row - 1) * ISIZE;
    ptr = ((char *)DATA+ISIZE*(row + SPOS + 1));

    setINTERNALdata(ptr,(char*)ptr+ISIZE,size);
/*    memcpy(ptr,(char*)ptr+ISIZE,size);*/
    ENTRIES--;
    EPOS--;
    if(!(EPOS % MFACT)){
	hold = DATA;
	TSIZE = (TSIZE <= INC) ? 1: TSIZE - INC;
	DATA = (void *) realloc(DATA,TSIZE);
	if (!DATA) {
	    DA_ERROR = 1;
	    DATA = hold;
	    return 0;
	}
    }
    return 1;
}


int put_item(darray_t *a,void *data,int row)
{
    static void *ptr;
  
    if((row >= (int)(ENTRIES)) || (row < 0)) {
	DA_ERROR = 2;
	return 0;
    }
    ptr = ((char *)DATA+ISIZE*(row + SPOS + 1));
    setINTERNALdata(ptr,data,ISIZE);
    return 1;
}

int get_item(darray_t *a,void *data,int row)
{
    static void *ptr;
    
    if(row >= (int) (ENTRIES) || row < 0)
        return 0;

    ptr = ((unsigned char *)DATA + 
	  (unsigned)ISIZE  * (unsigned) (row + SPOS + 1));

    setINTERNALdata(data,ptr,ISIZE);
    return 1;
}




int pop_here(darray_t *a,void * data,int row)
{
    static unsigned size;
    static void * ptr;
     
    if((row >= (int) (ENTRIES)) || (row < 0)) {
        DA_ERROR = 2;
        return 0;
    }
    size = (ENTRIES - row - 1) * ISIZE;
    ptr = ((char *)DATA+ISIZE*(row + SPOS + 1));

    setINTERNALdata(data,ptr,ISIZE);
    memcpy(ptr,(char*)ptr+ISIZE,size);
    ENTRIES--;
    EPOS--;
    if(!(EPOS % MFACT)){
	hold = DATA;
	TSIZE = (TSIZE <= INC) ? 1: TSIZE - INC;
        DATA = (void *) realloc(DATA,TSIZE);
	if (DATA == NULL) {
	    DA_ERROR = 1;
	    DATA = hold;
	    return 0;
	}
    }
    return 1;
}

 

int push_here(darray_t *a,void *data,int row)
{
    static int size, rrow;
    static void *ptr;

    if((row > (int) (ENTRIES)) || (row < 0)) {
            DA_ERROR = 2;
            return 0;
    }
    rrow = row + SPOS + 1;

    if(!(EPOS % MFACT)) {
        hold = DATA; 
	if(MFACT <= (ENTRIES >> 2)) {
	    MFACT += MFACT;
	    INC =  MFACT * ISIZE;
	}
	TSIZE += INC;
	DATA = (void *) 
            realloc(DATA,TSIZE);
        if(DATA == NULL){
            DA_ERROR = 1;
	    DATA = hold;
            return 0;
        }
    }

    size = (EPOS - rrow) * ISIZE;
    ptr = (char*)DATA + rrow * ISIZE;

    /** HANDLE OVERLAP **/
    memmove((void*)((char*)ptr+ISIZE),ptr,size);

    setINTERNALdata(ptr,data,ISIZE);
    ENTRIES++;
    EPOS++;
    return 1;
}


int push_inorder(darray_t *a, void *data, int (*compar)())
{
    static int low, high, mid, x;
  
    if(!ENTRIES)
	return(push_last(a,data));

    low = SPOS + 1; high = EPOS - 1;
    while(low<=high) {
	mid = (low+high)>>1;
	x = (*compar)(data, (char*)(DATA) + mid*ISIZE); 
	if(x < 0)
	    high = mid - 1;
	else if(x > 0)		
	    low = mid + 1;
	else
		break;
    }

    if(low == (int)(EPOS))
	return(push_last(a,data));

    if(low > mid)
	mid++;
    return(push_here(a,data,mid - (SPOS+1)));
}

int pop_first(darray_t * a,void * data)
{
    static int size;
    static void * ptr;

    if(!ENTRIES)
	return 0;
    ENTRIES--;
    SPOS++;

    ptr = ((char*)DATA + SPOS * ISIZE);
    setINTERNALdata(data,ptr,ISIZE);

    if(SPOS+1  == (int)(MFACT)) {
	size = (MFACT)*ISIZE;
	memcpy(DATA,(char*)DATA+size,
	       (ENTRIES*ISIZE));
	EPOS -= (MFACT);
	TSIZE = (TSIZE <= INC) ? 1: TSIZE - INC;
	SPOS = -1;
	DATA = (void *)realloc(DATA,TSIZE);
    }

    return 1;
}


/* get first item but do not pop off the queue */
int get_first(darray_t * a,void * data)
{
    static void * ptr;

    if(!ENTRIES)
	return 0;

    ptr = ((char*)DATA + (SPOS+1) * ISIZE);
    setINTERNALdata(data,ptr,ISIZE);
    return 1;
}


int push_first(darray_t *a, void * data)
{
    static unsigned int size;
    static void *ptr;
  
    if(SPOS < 0) {
	hold = DATA; 
	if(MFACT <= (ENTRIES >>2)) {
	    MFACT += MFACT;
	    INC =  MFACT * ISIZE;
	}
	TSIZE += INC;
	DATA = (void *) 
            realloc(DATA,TSIZE);

        if(DATA == NULL){
            DA_ERROR = 1;
	    DATA = hold;
            return 0;
        }
	size = (ENTRIES) * ISIZE;
	ptr = (void*)((char*)DATA+(MFACT*ISIZE));
	
	if(size) /* HANDLE OVERLAP */
	    memmove(ptr,DATA,size);

	EPOS += MFACT;
	SPOS = MFACT-1;
    }
    ptr = ((char*)DATA + SPOS * ISIZE);
    setINTERNALdata(ptr,data,ISIZE);
    SPOS--;
    ENTRIES++;
    return 1;
}


int pop_last(darray_t * a,void * data)
{

    static void * ptr;

    if(!ENTRIES)
	return 0;
    ENTRIES--;
    EPOS--;
    
    ptr = ((char*)DATA + EPOS * ISIZE);

    setINTERNALdata(data,ptr,ISIZE);
    
    if(!(EPOS%MFACT)) {	
	TSIZE = (TSIZE <= INC) ? 1: TSIZE - INC;
	DATA = (void *)realloc(DATA,TSIZE);
    }
    return 1;
}

/* doesn't pop the item off the queue */
int get_last(darray_t * a,void * data)
{
    static void * ptr;
    if(!ENTRIES)
	return 0;

    ptr = ((char*)DATA + (EPOS-1) * ISIZE);
    setINTERNALdata(data,ptr,ISIZE);

    return 1;
}


int push_last(darray_t *a, void  *data)
{
    static void * ptr;

    if(!(EPOS % MFACT)) {
        hold = DATA;
	if(MFACT <= (ENTRIES >> 2)) {
	    MFACT += MFACT;
	    INC =  MFACT * ISIZE;
	}
        TSIZE += INC;
        DATA = (void *) realloc(DATA,TSIZE );
        if(DATA == NULL) {
            DA_ERROR = 1;
            DATA =  hold;
	    return 0;
        }
        DA_ERROR = 0;
	NINC++;
    }
    
    ptr = (void*)((char*)DATA + EPOS * ISIZE);
    setINTERNALdata(ptr,data,ISIZE);
    ENTRIES++;
    EPOS++;
    return 1;
}











