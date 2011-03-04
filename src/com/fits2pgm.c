/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file fits2pgm.c

\brief converts from fits format into pgm format

<B>Usage:</B> fits2pgm in.fits out.pgm

<B>Description:</B> Converts from fits format into pgm format. 

Parameters:
\li \b in.fits : source file in fits format
  
<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2000 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include "fitsio.h"




#define BUFFSIZE 1024    /* buffer size when reading from fits file */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */

#ifdef HAVE_FITS_LIB
{

    struct xvimage *image;
    fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
    int status, anynul, i, A;
    char *filename;
    //char comment[FLEN_COMMENT];
    long nelements, firstelem, naxis1, naxis2, naxes[2], nbuffer;
    short *rawdata;
    const ShortPixel;
    
    int   kwvBITPIX, numAxis;


    if (argc != 3) {
        fprintf(stderr, "usage: %s in.fits out.pgm \n", argv[0]);
        exit(1);
    }

    filename=argv[1];
    status=0;  /* initialize status before calling fitsio routines */
    firstelem=1;
    anynul=0;
    


 

    fits_open_file (&fptr, filename, READONLY, &status);
    fits_report_error(stderr,status);

  	/* Display the data type in the image. */
  	fits_get_img_type(fptr, &kwvBITPIX, &status);
  	fits_report_error(stderr,status);
  	switch(kwvBITPIX) {
    	case BYTE_IMG   : fprintf(stderr, "Image Type:  8-bit byte pixels, 0 - 255\n");  break;
    	case SHORT_IMG  : fprintf(stderr, "Image Type: 16 bit integer pixels\n");        break;
    	case LONG_IMG   : fprintf(stderr, "Image Type: 32-bit integer pixels\n");        break;
    	case FLOAT_IMG  : fprintf(stderr, "Image Type: 32-bit floating point pixels\n"); break;
    	case DOUBLE_IMG : fprintf(stderr, "Image Type: 64-bit floating point pixels\n"); break;
    	default         : fprintf(stderr, "Image Type: UNKNOWN\n");                      break;
  	}
    
    fits_get_img_dim(fptr, &numAxis,  &status);
	fprintf(stderr, "Number of axis: %d\n", numAxis);

	/* Find the x/y-axis dimensions and the color dimension if it exists. */
  	if(numAxis < 2) {
    	fprintf(stderr, "Too few axes to be a real image!\n");
    	exit(1);
  	} else if(numAxis > 3) {
    	fprintf(stderr, "Too many axes to be a real image!\n");
    	exit(2);
  	}

    
    fits_get_img_size(fptr, 3, naxes, &status);

 //fits_read_key_lng(fptr,"NAXIS1",&naxis1,&comment,&status);
 //fits_read_key_lng(fptr,"NAXIS2",&naxis2,&comment,&status);

 //naxes[0]=naxis1;
 //naxes[1]=naxis2;

 
    nelements=naxes[0]*naxes[1]; /* number of pixels to read */

    fprintf(stderr, "naxes[0]=%ld, naxes[1]=%ld\n",naxes[0],naxes[1]);
 

    image = allocimage(NULL, naxes[0], naxes[1], 1, VFF_TYP_2_BYTE);
 
    //rawdata = (short *)malloc(nelements*sizeof(short));


    //while (nelements>0) {
        //nbuffer = nelements;
        //if (nbuffer>BUFFSIZE) nbuffer = BUFFSIZE;

      /* read in a buffer full of data from the image array */
      
        fits_read_img(fptr, TDOUBLE,  firstelem,  nelements, 0, image->image_data, &anynul, &status);

      /*for (i=0; i<nelements; ++i)


      {
          A=floor(i/naxes[1]);
          image[A][i-A*naxes[1]-1]=rawdata[i];
      } */	
             
        //nelements -= nbuffer;
        //firstelem += nbuffer;
    //}

   
    

    if (status == 0) {
        //image->xdim = naxes[0];
        //image->ydim = naxes[1];
        //image->zdim = 1;
        //image->image_data=rawdata;
        //SSHORTDATA(image) = rawdata;
        writeimage(image, argv[argc - 1]);
        freeimage(image);
    } else {
        fits_report_error(stderr, status); /* print out any error messages */
        exit(1);
    }
 
 	fits_close_file(fptr, &status);    /* close the file */
  
    return 0;
} // main

#else
{
    fprintf(stderr,"FITS library not found at compile time\n");
    return 1;
}
#endif // HAVE_FITS_LIB



