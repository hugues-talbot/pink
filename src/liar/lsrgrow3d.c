/*
 * File:		lsrgrow3d.c
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
#include <string.h>
#include <math.h>
#include "liarp.h"
#include "liarwrap.h"


#define BORDER -1

typedef struct {
    int key;
    int pos;
    int label;
} item_t;

typedef struct {
    PIX_TYPE **image;	    /* control image for PIX_TYPE input */
    INT4_TYPE **intimage;          /* control images(s) for int input */
    int type;                               /* input type */
    SEED_TYPE *seeds;	    /* seed image */
    int comps;	    /* number of images */
    int Nbr;	    /* Number of neighbours */
    long *Nf;	    /* Neighbourhood function */
    long size;	    /* number of pixels in the control image */
    int nseeds;	    /* Number of seeds in the seed image */
    int *N;	                  /* Number of pixels associated with each seed */
    int *label;	    /* array of labels */
    int *idx;	    /* index array */
    long **sum;	    /* sum of all pixels associated with each seed */
    float **ave;	    /* average of pixels associated with each seed */
    int (*delta)();	    /* calculates pixel differences */
    void (*update)();	    /* update statistics function */
    sptree *mque;	    /* positional queues */
} que_t;

static int comp_ints(void * i1, void *i2)
{
    return (*(int*)i1 - *(int*)i2);
}

static void update(que_t *que, int idx, int pos)
{
    int i;

    que->N[idx]++;
    if (que->type == PIX_CODE)
      for(i = 0; i < que->comps; i++) {
	que->sum[i][idx] += (int)que->image[i][pos];
	que->ave[i][idx] = (float)que->sum[i][idx]/(float)que->N[idx];
      }
    else
      for(i = 0; i < que->comps; i++) {
	que->sum[i][idx] += (int)que->intimage[i][pos];
	que->ave[i][idx] = (float)que->sum[i][idx]/(float)que->N[idx];
      }
}

static int gdelta(que_t *que, int label, int pos)
{
    static int i, v;

    if(que->type == PIX_CODE)
      for(v = i = 0; i < que->comps; i++)
	v += abs((int)que->ave[i][label] - (int)que->image[i][pos]);
    else 
      for(v = i = 0; i < que->comps; i++)
	v += abs((int)que->ave[i][label] - (int)que->intimage[i][pos]);

    return (v + 2);
}

static int Euclid_delta(que_t *que, int label, int pos)
{
    static int i, v, diff;

    if(que->type == PIX_CODE)
      for(v = i = 0; i < que->comps; i++) {
	diff = (int)que->ave[i][label] - (int)que->image[i][pos];
	v += diff *diff;
      }
    else
      for(v = i = 0; i < que->comps; i++) {
	diff = (int)que->ave[i][label] - (int)que->intimage[i][pos];
	v += diff *diff;
      }

    return (sqrt(v) + 2);
}

static void collect_gstats(que_t *que)
{
    int i, idx, k;
    SEED_TYPE *seeds;

    seeds = que->seeds;
    if(que->type == PIX_CODE) {
      for(i = 0; i < que->size; i++)
	if(seeds[i] > 0) {
	  idx = que->idx[seeds[i]];
	  que->N[idx]++;
	  for(k = 0; k < que->comps; k++)
	    que->sum[k][idx] += que->image[k][i];
	}
    }
    else {
      for(i = 0; i < que->size; i++)
	if(seeds[i] > 0) {
	  idx = que->idx[seeds[i]];
	  que->N[idx]++;
	  for(k = 0; k < que->comps; k++)
	    que->sum[k][idx] += que->intimage[k][i];
	}
    }
    for(i = 0; i < que->nseeds; i++)
	for(k = 0; k < que->comps; k++)
	    que->ave[k][i] = (float)que->sum[k][i]/(float)que->N[i];
}

static que_t *init_que(void **image,
		int type,
		int comps,
		SEED_TYPE *seeds,
		long size,
		long *Nf,
		int Nbr,
		int metric)
  /*
que_t *init_que(
		PIX_TYPE **image,
		SEED_TYPE *seeds,
		long size,
		int comps,
		long *Nf,
		int Nbr,
		int metric)
  */
{
    long i;
    darray_t *count;
    que_t *que;

    /* 
     * Allow for seeds not being numbered sequentially.
     * Thus, must fist sort seed values. 
     */
    count = crt_darray(sizeof(int), 100);
    for(i = 0; i < size; ++i, ++seeds)
	if(*seeds > 0){		/* seed point */
	    if(find_item(count, seeds, comp_ints) == -1)
		push_inorder(count,seeds,comp_ints);
	}

    seeds -= size;
    /* calloc() args swapped */
    que = (que_t *)calloc(1, sizeof(que_t));
    que->type = type;
     if (type == PIX_CODE)
	que->image = (PIX_TYPE**)image;
    else
	que->intimage = (INT4_TYPE**)image;

    que->size = size;
    que->comps = comps;
    que->seeds = seeds;
    que->Nf = Nf;
    que->Nbr = Nbr;
    que->update = update;
    que->nseeds = count->N;
    que->N = (int *)calloc(count->N, sizeof(int));
    que->label = (int *)calloc(count->N, sizeof(int));
    que->mque = spinit();
    que->ave = (float **)calloc(comps, sizeof(float *)); 
    que->sum = (long **)calloc(comps, sizeof(long *));
    for(i = 0; i < comps; i++) {
	que->sum[i] = (long *)calloc(count->N, sizeof(long));
	que->ave[i] = (float *)calloc(count->N, sizeof(float));
    }

    if(metric)
	que->delta = Euclid_delta;
    else
	que->delta = gdelta;

    /*
     * Move (pop) seed values to the label array.
     */
    for(i = 0; pop_first(count,&que->label[i]); i++);
    free_darray(count);

    /*
     * Create index array. It's length is determined from  the
     * the largest seed value.
     */
    que->idx = (int *)calloc(que->label[que->nseeds-1] + 1, sizeof(int));

    /*
     * Assign a sequential value for each label value
     * for indexing purposes.
     */
    for(i = 0; i < que->nseeds; i++)
	que->idx[que->label[i]] = i;

    return que;
}

static void free_que(que_t * que)
{
    int i;

    free(que->N);
    free(que->label);
    free(que->idx);
    spclose(que->mque);
    for(i = 0; i < que->comps; i++) {
	free(que->sum[i]);
	free(que->ave[i]);
    }
    free(que->ave);
    free(que->sum);
    free(que);
}

static void init_mque(que_t *que)
{
    int i, k;
    int idx, delta;
    SEED_TYPE *seeds;
    item_t item;

    seeds = que->seeds;
    for(i = 0; i < que->size; i++, seeds++)
	if(*seeds > 0) 
	    for(k = 0; k < que->Nbr; k++)
	      if(*(seeds + que->Nf[k]) == 0 ||
		 ((*(seeds + que->Nf[k]) < BORDER) &&
		  (*(seeds + que->Nf[k]) != BORDER - *seeds))
		 ) {
		idx = que->idx[*seeds];
		delta = (*que->delta)(que, idx, i + que->Nf[k]);
		item.key = delta;
		item.label = (*seeds);
		item.pos = i + que->Nf[k];
		*(seeds + que->Nf[k]) = BORDER - *seeds;
		spenq(que->mque, item.key, item.pos, item.label);
	      }
}/* init_mque() */

static void grow_region(que_t *que, int borders)
{
    static unsigned int k;
    int idx;
    static item_t item;
    static int delta, pos;
    SEED_TYPE *ptr, *seeds;

    seeds = que->seeds;
    while(spdeq(que->mque, &item.key, &item.pos, &item.label)){
	ptr = seeds + item.pos;
	idx = que->idx[item.label];

	if(*ptr > 0)
	  continue;

	if(borders) {
	    if(testINTneighs(ptr, que->Nf, que->Nbr) == 1)
		*ptr = (SEED_TYPE)item.label;
	    else {
		*ptr = BORDER;
		continue;
	    }
	} else
	    *ptr = (SEED_TYPE)item.label;

	(*que->update)(que, idx, item.pos);
	for(k = 0; k < (unsigned int)(que->Nbr); k++)
	    if(*(ptr + que->Nf[k]) == 0 ||
	       ((*(ptr + que->Nf[k]) < BORDER) &&
		(*(ptr + que->Nf[k]) != BORDER - item.label))
	       ) {
		pos = item.pos + que->Nf[k];
		delta = (*que->delta)(que, idx, pos);
		*(seeds + pos) = BORDER - item.label;
		spenq(que->mque, delta, pos, item.label);
	    }
    }
}/* grow_region() */

int lsrgrow3d(void **bufin,	/* input buffer */
	     SEED_TYPE **bufout,	/* output buffer */
	     SEED_TYPE *seeds,	/* seed image */
	     int inimagetype,               /*  pixel type of input image */
	      int inpixsize,                  /* pixel size of input image */
	      int seedpixsize,             /* pixel size of seeds image */
	     int inx,		/* nb of cols in the input */
	     int iny,		/* nb of rows in the input */
	     int inz,		/* nb of slices in the input */
	     int comps,		/* nb of components in the input */
	     int metric,	/* distance measures */
	     int borders,	/* to display region borders only */
	     int connect,	/* connectivity of the image */
	     int showgrey)	/* output greyscale using pixel avge */
{
/** This function segments 3D images using the seeded region growing method

RETURN VALUE:	int

DESCRIPTION:
1/2/3D Segmentation by Seeded Region Growing method, adapted from
Leanne Bishof & Rolf Adam's implementation on 2D function "srgrow"

Among Annick's changes is that previously the only check that an
image was 3D was to check that nz>1. This overlooks cases where 
the image is in the xz or the yz planes. It means that for such images
the connectivity used can be 18 or 26, which is a problem when
the function tries to read pixel data in other planes than that of the
image.
So I've modified the neighbourhood function (now setNeighFunc3d())
in such a way that images in the xz and yz are handled as 2D images
in a sensible way: the allowed connectivities for such images are now
4 and 8, just like for images in the xy plane.

HISTORY:
written by John Lin	               07 Jan 1999
modified by Leanne Bischof    28 April 2000
modified by Annick LeHegarat     May 2000

TESTS:
Works in place.

REFERENCES:

KEYWORDS:
**/
    SEED_TYPE *tmpbuf;
    PIX_TYPE **input=NULL, *dummy;
    INT4_TYPE **intinput=NULL, *intdummy;
    que_t *que;
    int i, outx=inx, outy=iny, outz=inz;
    int imagedims=0, offsetx=0, offsety=0, offsetz=0;
    long Nb[26], size;

    offsetx = (inx > 1) ? 1 : 0;
    offsety = (iny > 1) ? 1 : 0;
    offsetz = (inz > 1) ? 1 : 0;
    outx += 2 * offsetx;
    outy += 2 * offsety;
    outz += 2 * offsetz;
    
    imagedims = offsetx + offsety + offsetz;

    if(imagedims == 3) {
	if (connect != 26 && connect != 18 && connect != 6) {
	    LIARerror("Connectivity of the 3D image must be 26 / 18 or 6");
	    return 1;
	}
    } else if (imagedims == 2) {
	if (connect != 8 && connect != 4) {
	    LIARerror("Connectivity of the 2D image must be 8 or 4");
	    return 1;
	}
    } else 
	connect = 2;

    size = outx * outy * outz;
    tmpbuf = (SEED_TYPE *)calloc(size, seedpixsize);
    if (tmpbuf == NULL) {
	LIARerror("Not enough memory available");
	return 1;
    }
    addborder_3d((void *)seeds, (void **)&tmpbuf, seedpixsize, inx, iny, inz,
		 &outx, &outy, &outz, offsetx, offsety, offsetz);

    /* add a border to input buffer */
    if (inimagetype == PIX_CODE) {
      input = (PIX_TYPE **)calloc(comps, sizeof(void *));
      for (i = 0 ; i < comps ; i++) {
	dummy = (PIX_TYPE *)calloc(size, inpixsize);
	if (dummy == NULL) {
	  LIARerror("Not enough memory available");
	  return 1;
	}
	addborder_3d((void *)bufin[i], (void **)&dummy, inpixsize, inx, iny, inz, 
		     &outx, &outy, &outz, offsetx, offsety, offsetz);
	input[i] = dummy;
      }
    }
    else {
      intinput = (INT4_TYPE **)calloc(comps, sizeof(void *));
      for (i = 0 ; i < comps ; i++) {
	intdummy = (INT4_TYPE *)calloc(size, inpixsize);
	if (intdummy == NULL) {
	  LIARerror("Not enough memory available");
	  return 1;
	}
	addborder_3d((void *)bufin[i], (void **)&intdummy, inpixsize, inx, iny, inz, 
		     &outx, &outy, &outz, offsetx, offsety, offsetz);
	intinput[i] = intdummy;
      }
    }

    setSEEDborder(tmpbuf, outx, outy, outz, BORDER);
    setNeighFunc3d(Nb, connect, outx, outy, outz);
    if (inimagetype == PIX_CODE) 
      que = init_que((void **)input, inimagetype, comps, tmpbuf, size, Nb, connect, metric);
    else
      que = init_que((void **)intinput, inimagetype, comps, tmpbuf, size, Nb, connect, metric);
    collect_gstats(que);
    init_mque(que);
    grow_region(que, borders);

    size = inx * iny * inz;
    getsubset_3d((void *)tmpbuf, (void **)bufout, seedpixsize, outx, outy, outz, &inx, &iny, &inz, offsetx, offsety, offsetz);
    changeSEEDval(*bufout, size, BORDER, 0);

    if (showgrey) {
	for (i = 1; i <= que->nseeds; i++)
	    changeSEEDval(*bufout, size, i, (int)que->ave[0][i-1]);
    }

    free(tmpbuf);
    if (inimagetype == PIX_CODE) {
      for (i = 0 ; i < comps ; i++) 
	free(input[i]);
      free(input);
    }
    else {
      for (i = 0 ; i < comps ; i++) 
	free(intinput[i]);
      free(intinput);
    }
      
    free_que(que);

    return 0;
}
