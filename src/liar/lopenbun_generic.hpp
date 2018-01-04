/*
 * File:		lopenbun_generic.hpp
 *

 *
*/

#ifndef LOPENBUN_HPP
#define LOPENBUN_HPP

#include <cmath>
#include <limits>  // for type min(), max()
#include "liarp.h"
#include "pink_messages.hpp"

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
      glineminmax(wk,nx,ny,k,(int)th,computemin,computebresen);
      glineminmax(wk,nx,ny,k,(int)th,computemax,computebresen);
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
 * The segments all have a length in pixels that depends on the orientation, so that they all have the
 * same Euclidean length.
 *
 * If the input image is 3D, the operator is repeated on every plane
 * 
 * \return 0 if no error in execution
 */
template <typename Type>
int compute_openbun_limits(Type *in,       /**< [in] input buffer */
			   int nx,	   /**< [in] number of columns (x dimension) of the input image */
			   int ny,	   /**< [in] number of lines (y dimensions) of the input image */
                           int nz,         /**< [in] number of planes (z dimensions) of the input image */
			   int radius,	   /**< [in] length of all segments */ 
			   int n,	   /**< [in] number of segments to consider */
			   double angle,   /**< [in] starting angle */	 
			   double range,   /**< [in] angle range */
			   Type *out       /**< [out] output buffer (can be same as input) */)
{
    int i,j,k,anglek;
    Type *wk;
    double degrees,th,limit1,limit2;
    long planesize = nx*ny, totalsize = nx*ny*nz;

    /* we allocate the whole cube of data, but we do not initialize it */
    wk = (Type *)malloc(totalsize * sizeof(Type));
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

    /* initially the output image is -\infty */
    memset(out,std::numeric_limits<Type>::min(),totalsize*sizeof(Type));

    for(th = limit1,i=0;i<n;++i) {
        double theta = th*M_PI/180.0;
        double isofactor = liarmax(std::fabs(k*std::cos(theta)), std::fabs(k*std::sin(theta)));
        anglek = (int)std::ceil(isofactor);
        
        /* plane by plane */
        /* a single copy of the whole data */        
        memcpy(wk,in,totalsize*sizeof(Type));
        for (int z = 0 ; z < nz ; ++z) {
            int deltaz = z * planesize;
            /* in-place computation */
            glineminmax(wk+deltaz,nx,ny,anglek,(int)th,computemin,computebresen);
            glineminmax(wk+deltaz,nx,ny,anglek,(int)th,computemax,computebresen);
            /* take pointwise max */
            Type *pout = out+deltaz;
            Type *wkout = wk + deltaz;
            for(j=0; j < planesize ; j++,pout++,wkout++)
                *pout = (*pout > *wkout) ? *pout : *wkout;
        }
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
 * The segments have all approximately the same Euclidean length so as not to create a directional bias.
 * 
 * Due to the way the rank filter is implemented, this is only efficient for 8-bit images.
 * 
 * \return 0 if no error in execution
 */

template <typename Type>
int compute_openbun_rankmax(Type *in,	     /**< [in] buffer */
			    int nx,	     /**< [in] number of columns */
			    int ny,	     /**< [in] number of rows */
                            int nz,          /**< [in] number of planes */
			    int radius,	     /**< [in] radius of search circle */
			    int n,	     /**< [in] number of lines considered  */
			    double angle,    /**< [in] center angle */
			    double range,    /**< [in] angle range */
			    double rank,     /**< [in] 0 <= rank <= 1 */
			    Type *out        /**< [out] Output buffer */)			    
{
    int i,j,k,anglek,planesize;
    Type *wk, *wk2;
    double degrees,th,limit1,limit2;
    long totalsize = nx*ny*nz;
    
    planesize = nx*ny;

    /* allocate all 3D block */
    wk = (Type *)malloc(totalsize * sizeof(Type));
    if(!wk) {
        LIARerror("lopenbun: Not enough memory\n");
        return 1;
    }
    wk2 = (Type *)malloc(totalsize * sizeof(Type));
    if(!wk2) {
        free(wk); // useless now.
        LIARerror("lopenbun: Not enough memory\n");
        return 1;
    }

    k = 2 * radius; /* k HAS TO be even, otherwise at least gsrank will fail */

    //fprintf(stderr, "angle=%g, range=%g\n", angle, range);

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

    /* set the initial value to -\infty */
    memset(out,std::numeric_limits<Type>::min(),totalsize*sizeof(Type));
    for(th = limit1,i=0;i<n;++i) {
      double theta = th*M_PI/180.0;
      double isofactor = liarmax(std::fabs(k*std::cos(theta)), std::fabs(k*std::sin(theta)));
      anglek = (int)std::ceil(isofactor);
      if ((anglek % 2) != 0)
	--anglek; // so that anglek remains even, see above

      //fprintf(stderr,"Theta = %g (degree), Anglek = %d\n", th, anglek);
      for (int z = 0 ; z < nz ; ++z) {
          int deltaz = z * planesize;
          gsrank(in + deltaz, wk2 + deltaz, nx, ny, anglek, (double)((int)th), rank);
          glineminmax(wk2+deltaz,nx,ny,anglek,(int)th,computemax,computebresen);
          li_min(in+deltaz, wk2+deltaz, wk+deltaz, planesize);
          // take max of all directions
          Type *pout = out + deltaz; 
          Type *wkout = wk + deltaz;
          for(j=0;j<planesize;j++,pout++,wkout++)
              *pout = (*pout > *wkout) ? *pout : *wkout;
      }
          
      th += degrees;
      if (th>90.0)
          th-=180.0;
    }
    free(wk);
    free(wk2);
    return 0;
}

#endif // LOPENBUN_HPP
