/*
 * File:		lopenbun_generic.hpp
 *

 *
*/

#ifndef LOPENBUN_HPP
#define LOPENBUN_HPP

#include "liarp.h"

/** \brief opening by union of lines
 * 
 * This computes an opening by a number of line segments so as to cover most discrete angles
 * This is a classical filter in mathematical morphology for preserving thin, locally straight objects.
 * The segments all have a length 2*radius+1, this means this filters out more lines near 45 degree
 * than either horizontal or vertical lines.
 * 
 * 
 * \return 0 if no error in execution
 */
template <typename Type>
int compute_openbun(const Type *in, /**< [in] input buffer */
		    int nx,         /**< [in] number of columns (x dimension) of the input image */
		    int ny,         /**< [in] number of lines (y dimensions) of the input image */
		    int radius,     /**< [in] length of all segments */ 
		    int n,          /**< [in] number of segments to consider */
		    Type *out       /**< [out] output buffer */ )
{
    int j,k,size;
    Type *wk;
    float degrees, th;

    size = nx*ny;
    /* calloc() args swapped */
    wk = (Type*)calloc(size, sizeof(Type));
    if(!wk) {
        pink_error("lopenbun: Not enough memory\n");
        return 1;
    }

    k = 2 * radius;
    degrees = 180.0/(float)n;
    memset(out,0,size*sizeof(Type));
    for(th = -90.0;th < 90.0; th += degrees) {
      memcpy(wk,in,size*sizeof(Type));
      glineminmax(wk,nx,ny,k,(int)th,genfmin_char,bresenham);
      glineminmax(wk,nx,ny,k,(int)th,genfmax,bresenham);
      for(j=0;j<size;j++,out++,wk++)
	*out = *out > *wk ? *out : *wk;
      out -= size;
      wk -= size;
    }
    free(wk);
    return 0;
}

/** \brief opening by union of lines with a range of angles
 * 
 * This computes an opening by a number of line segments so as to cover some discrete angles
 * from a starting angle to an ending one. Since centered segments are considered, only a 
 * double-ended cone angle range can be given. To cover all angles, a starting value of zero
 * and a ending angle of 180 are necessary.
 * 
 * This is a classical filter in mathematical morphology for preserving thin, locally straight objects.
 * The segments all have a length 2*radius+1, this means this filters out more lines near 45 degree
 * than either horizontal or vertical lines.
 * 
 * 
 * \return 0 if no error in execution
 */
template <typename Type>
int compute_openbun_limits(Type *in,       /**< [in] input buffer */
			   int nx,	   /**< [in] number of columns (x dimension) of the input image */
			   int ny,	   /**< [in] number of lines (y dimensions) of the input image */
			   int radius,	   /**< [in] length of all segments */ 
			   int n,	   /**< [in] number of segments to consider */
			   double angle,   /**< [in] starting angle */	 
			   double range,   /**< [in] angle range */
			   Type *out       /**< [out] output buffer (can be same as input) */)
{
    int i,j,k,size;
    Type *wk;
    double degrees,th,limit1,limit2;

    size = nx*ny;
    /* calloc() args swapped */
    wk = (Type *)calloc(size, sizeof(Type));
    if(!wk) {
        LIARerror("lopenbun: Not enough memory\n");
        return 1;
    }

    k = 2 * radius;

    limit1=angle-range/2.0;
    if (limit1<-90)
	limit1+=180;
    limit2=angle+range/2.0;
    if (limit2>90)
	limit2-=180;
    if (limit1<limit2)
	degrees = (limit2-limit1)/(double)n;
    else
	degrees = (180 + limit2 - limit1)/(double)n;

    memset(out,0,size*sizeof(Type));
    for(th = limit1,i=0;i<n;++i) {
	memcpy(wk,in,size*sizeof(Type));
	glineminmax(wk,nx,ny,k,(int)th,genfmin_char,bresenham);
	glineminmax(wk,nx,ny,k,(int)th,genfmax,bresenham);
	for(j=0;j<size;j++,out++,wk++)
	    *out = *out > *wk ? *out : *wk;
	out -= size;
	wk -= size;
	th += degrees;
	if (th>90.0)
	    th-=180.0;
    }
    free(wk);
    return 0;
}


/** \brief rank-max opening by union of lines with a range of angles
 * 
 * This computes a rank-max opening by a number of line segments so as to cover some discrete angles
 * from a starting angle to an ending one. Since centered segments are considered, only a 
 * double-ended cone angle range can be given. To cover all angles, a starting value of zero
 * and a ending angle of 180 are necessary.
 * 
 * Rank max opening were proposed by Christian Ronse as a way to improve the noise characteristics
 * of a filter. Normal openings require the whole structuring elements to be included in the feature
 * of interest. This means a single dark noise pixel may prevent an image structure to be preserved by 
 * the opening. With the rank max parameter, a proportion of pixels are ignored. If this parameter
 * is set to 0.1, this means we allow in 10% of noise. It doesn't matter where these noise pixels
 * are located. This is implemented as a max with a rank filter, and is very efficient.
 * Henk Heijmans showed that alternating filters made of these had the best characteristics among morphological
 * filters in ISMM 2002. Soille and Talbot proposed these for unions of openings by segments along
 * with other improvements. Tankyevych showed these filters were best in very noisy conditions to 
 * segment thin objects.
 * 
 * The segments all have a length 2*radius+1, this means this filters out more lines near 45 degree
 * than either horizontal or vertical lines.
 * 
 * Due to the way the rank filter is implemented, this is only efficient for 8-bit images.
 * 
 * \return 0 if no error in execution
 */

template <typename Type>
int compute_openbun_rankmax(Type *in,	     /**< [in] buffer */
			    int nx,	     /**< [in] number of columns */
			    int ny,	     /**< [in] number of rows */
			    int radius,	     /**< [in] radius of search circle */
			    int n,	     /**< [in] number of lines considered  */
			    double angle,    /**< [in] center angle */
			    double range,    /**< [in] angle range */
			    double rank,     /**< [in] 0 <= rank <= 1 */
			    Type *out        /**< [out] Output buffer */)			    
{
    int i,j,k,size;
    Type *wk, *wk2;
    double degrees,th,limit1,limit2;

    size = nx*ny;
    /* calloc() args swapped */
    wk = (Type *)calloc(size, sizeof(Type));
    if(!wk) {
        LIARerror("lopenbun: Not enough memory\n");
        return 1;
    }
    /* calloc() args swapped */
    wk2 = (Type *)calloc(size, sizeof(Type));
    if(!wk2) {
        LIARerror("lopenbun: Not enough memory\n");
        return 1;
    }

    k = 2 * radius; /* k HAS TO be even, otherwise at least gsrank will fail */

    limit1=angle-range/2.0;
    if (limit1<-90)
	limit1+=180;
    limit2=angle+range/2.0;
    if (limit2>90)
	limit2-=180;
    if (limit1<limit2)
	degrees = (limit2-limit1)/(double)n;
    else
	degrees = (180 + limit2 - limit1)/(double)n;

    memset(out,0,size*sizeof(Type));
    for(th = limit1,i=0;i<n;++i) {

	gsrank(in, wk2, nx, ny, k, (double)((int)th), rank);
	glineminmax(wk2,nx,ny,k,(int)th,genfmax,bresenham);
	li_min(in, wk2, wk, nx*ny);

	for(j=0;j<size;j++,out++,wk++)
	    *out = *out > *wk ? *out : *wk;
	out -= size;
	wk -= size;
	th += degrees;
	if (th>90.0)
	    th-=180.0;
    }
    free(wk);
    free(wk2);
    return 0;
}

#endif // LOPENBUN_HPP
