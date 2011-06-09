/*
 * File:		fseries.h
 *

 *
*/

/**********************************************************
 fseries.h
 -----------

**********************************************************/

#ifndef LIAR_FSERIES_HPP
#define LIAR_FSERIES_HPP

/* f-series directory */
#define PERIODIC	0
#define BRESENHAM	1

enum {computemin=true, computemax=false};

#include "rect3dmm.hpp"
#include "genfmin.hpp"
#include "genfmax.hpp"
#include "lfopen3d.hpp"




template <typename Type>
void rect3dminmax(Type *in, int nx, int ny, int nz, int w, int b,
		int d, bool usemin );

/* instantiation explicite */
//template void rect3dminmax<PIX_TYPE>(PIX_TYPE *in, int nx, int ny, int nz, int w, int b, int d, bool usemin );


template <typename Type>
void genfmin(Type *f,
            Type *g,
            Type *h,
            long *p,
            unsigned int nx, unsigned int K);

template <typename Type>
void genfmax(Type *f,
            Type *g,
            Type *h,
            long *p,
            unsigned int nx, unsigned int K);

template <typename Type>
int lfopen3d_rect(Type *inbuf, Type *outbuf, int ncol, int nrow,int nslice, int dimx, int dimy, int dimz);

///* prototypes from genfmin.c, function in lfdilate.c etc */
//extern void genfmin_char(PIX_TYPE *f,PIX_TYPE *g,PIX_TYPE *h,INT4_TYPE *p,
//		    unsigned int nx, unsigned int K);
//
//extern void genfmin_int4(INT4_TYPE *f,INT4_TYPE *g,INT4_TYPE *h,INT4_TYPE *p,
//			 unsigned int nx,unsigned int K);
//
//extern void genfmin_dbl(DBL_TYPE *f,DBL_TYPE *g,DBL_TYPE *h,INT4_TYPE *p,
//			unsigned int nx,unsigned int K);
//
//
///* prototypes from genfmax.c, function in lfdilate.c etc */
//extern void genfmax_char(PIX_TYPE *f,PIX_TYPE *g,PIX_TYPE *h,INT4_TYPE *p,
//		    unsigned int nx, unsigned int K);
//
//extern void genfmax_int4(INT4_TYPE *f,INT4_TYPE *g,INT4_TYPE *h,INT4_TYPE *p,
//			 unsigned int nx, unsigned int K);
//
//extern void genfmax_dbl(DBL_TYPE *f,DBL_TYPE *g,DBL_TYPE *h,INT4_TYPE *p,
//			unsigned int nx,unsigned int K);

#endif // LIAR_FSERIES_HPP


