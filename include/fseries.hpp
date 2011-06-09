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
enum {computebresen=true, computeperiod=false};

#include "rect3dmm.hpp"
#include "glinemm3d.hpp"
#include "genfmin.hpp"
#include "genfmax.hpp"
#include "lfopen3d.hpp"
#include "lferode3d.hpp"
#include "lfdilate3d.hpp"
#include "lfclose3d.hpp"


template <typename Type>
void rect3dminmax(Type *in, int nx, int ny, int nz, int w, int b,
		int d, bool usemin );

template <typename Type>
int glineminmax3d(Type *f, int nx, int ny, int nz, int k, int dx, int dy, int dz,bool usemin, bool lineop );

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


/* rect morphological operation */
template <typename Type>
int lfopen3d_rect(Type *inbuf, Type *outbuf, int ncol, int nrow,int nslice, int dimx, int dimy, int dimz);

template <typename Type>
int lferode3d_rect(Type *inbuf, Type *outbuf, int ncol, int nrow, int nslice, int dimx, int dimy, int dimz);

template <typename Type>
int lfdilate3d_rect_char(Type *inbuf, Type *outbuf, int ncol, int nrow,int nslice, int dimx, int dimy, int dimz);

template <typename Type>
int lfclose3d_rect_char(Type *inbuf, Type *outbuf, int ncol, int nrow,int nslice, int dimx, int dimy, int dimz);


/* line morphological operation */
template <typename Type>
int lfopen3d_line(Type *inbuf, Type *outbuf,int ncol, int nrow, int nslice, int length,int dx, int dy, int dz, int type);

template <typename Type>
int lferode3d_line(Type *inbuf, Type *outbuf, int ncol, int nrow,int nslice, int length, int dx, int dy, int dz, int type);

template <typename Type>
int lfdilate3d_line(Type *inbuf, Type *outbuf, int ncol, int nrow,int nslice, int length, int dx, int dy, int dz, int type);


#endif // LIAR_FSERIES_HPP


