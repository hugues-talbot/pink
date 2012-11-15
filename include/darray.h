/*
 * File:		darray.h
 *
 * Flexible, C-only vector structure
 *
 * By Ed Breen, CSIRO 1993
 *
 *
 */

/** File darray.h:  Begin darray header file **/
#ifndef _DARRAYH
#define _DARRAYH



#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>

typedef struct {      /**  THE DARRAY DATA STRUCTURE ***/
    int  spos;        /** start position **/
    size_t epos;      /** end position   **/
    size_t N;         /** Number of entries **/
    size_t mod;       /** increasing modulation factor **/
    size_t ninc;      /** number of increments **/
    size_t elmsize;   /** size of data item **/
    size_t tsize;     /** total size of space allocated **/
    size_t inc;       /** Increment for increase or decrease of tsize */
    void *data;       /** data storage starts here **/
    int type;         /** type tag for user specified purposes */
}darray_t;


#define getfirst_address(x)  (void*)((char*)(x)->data+((x)->spos+1)*(x)->elmsize)
#define getlast_address(x)   (void*)((char*)(x)->data+((x)->spos+(x)->N)*(x)->elmsize)
#define getnth_address(x,n)  (void*)((char*)(x)->data+((x)->spos+1+(n))*(x)->elmsize)



#define getnumitems(p)  ((p)->N)
#define getdata(p)     getfirst_address(p)
#define getmod(p)      ((p)->mod)

/*BEGIN PROTOTYPES *******/

int find_item(darray_t *a, void *data, int (*compar)());
int push_inorder(darray_t *a, void *data, int (*compar)());

darray_t *crt_darray(unsigned int len,unsigned int mod);
int delete_item(darray_t *a,int row);
int pop_first(darray_t * a,void * data);
int get_first(darray_t * a,void * data);
int pop_last(darray_t * a,void * data);
int get_last(darray_t * a,void * data);
int push_first(darray_t *a, void * data);
int push_last(darray_t *a, void  * data);
int put_item(darray_t *a,void *data,int row);
int get_item(darray_t *a,void *data,int row);
int pop_here(darray_t *a,void * data,int row);
int push_here(darray_t *a,void *data,int row);
void free_darray(darray_t *a);

/*END PROTOTYPES ***************/


extern char DA_ERROR;       /** ERROR VARIABLE **/
/* 0  NO ERROR */
/* 1  MEMORY ALLOCATION ERROR */
/* 2  DOMAIN ERROR */


#ifdef __cplusplus
}
#endif


#endif
/*** END DARRAY HEADER FILE *********************/






