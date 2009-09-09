/* $Id: lfft.c,v 1.2 2009-01-06 13:18:15 mcouprie Exp $ */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lfft.h>
#include <math.h>
#include <mcutil.h>

/* ==================================== */
static int32_t powtwo(int32_t n)
/* ==================================== */
{
  while (n > 1) if (n%2) return 0; else n = n / 2;
  return 1;
} // powtwo()

/* ==================================== */
int32_t lfft(struct xvimage *image1, struct xvimage *image2, int32_t dir)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "lfft"
  struct xvimage *r_image, *i_image;

    int32_t i, j, cs, rs;       /* Indexes and sizes of rows and columns */
    int32_t type;               /* FFT type argument for FORTRAN call */
    float *pr, *pi;             /* Pointers to complex pairs array */

    float *fptr,                /* pointer to image1 data */
          *imptr,               /* pointer to image2 (imaginary) data */
          *xr,*xi,*zr,*zi,*tr,*ti;

    int32_t m,m1;                    /* Stuff for Nyquist modulation */

    float         *rptr,        /* pointer to real image data */
                  *iptr;        /* pointer to imaginary image data */

    cs = colsize(image1);            /* Number of rows */
    rs = rowsize(image1);            /* Number of columns */

    /* Check to make sure input image is of correct type */
    if ((datatype(image1) != VFF_TYP_FLOAT) || (datatype(image2) != VFF_TYP_FLOAT))
    {
      (void) fprintf(stderr, "%s: Input images must be of data type FLOAT.\n", F_NAME);
      return(0);
    }

    /* Check to see if optional second input image matches first input image */
    if ((rowsize(image2) != rs) || (colsize(image2) != cs))
    {
      (void) fprintf(stderr, "%s: Input images must be of the same size.\n", F_NAME);
      return(0);
    }

    /* Make sure size is legal for an FFT */
    if ( (powtwo(rs) == 0) || (powtwo(cs) == 0) )
    {
      (void)fprintf(stderr,"%s: Input image is %d rows by %d cols.\n", F_NAME, cs, rs);
      (void)fprintf(stderr,"Image size MUST be a power of 2!\n ");
      return(0);
    }

    /* Get space for the intermediate complex arrays */
    xr = (float *)calloc(1,rs*cs*sizeof(float));
    xi = (float *)calloc(1,rs*cs*sizeof(float));
    zr = (float *)calloc(1,(rs+2)*sizeof(float));
    zi = (float *)calloc(1,rs*sizeof(float));

    if (xr == NULL || xi == NULL || zr == NULL || zi == NULL)
      {
        fprintf(stderr,"%s: Could not allocate enough memory!\n", F_NAME);
        return(0);
      }

    /**** Create the FLOAT output image corresponding to REAL part ****/
    r_image = allocimage(NULL, 
                          rs,                  /* number of rows in image */
                          cs,                  /* number of cols in image */
                          1,                   /* number of planes */
                          VFF_TYP_FLOAT);      /* data storage type */

    if (r_image == NULL)
      {
        (void) fprintf(stderr,"lvfft: Unable to allocate enough memory for output image!\n");
        return(0);
      }
    rptr = FLOATDATA(r_image);
    r_image->xdim = image1->xdim;
    r_image->ydim = image1->ydim;
    r_image->zdim = image1->zdim;

    /**** Create the FLOAT output image corresponding to IMAG part ****/
    i_image = allocimage(NULL, 
                          rs,                  /* number of rows in image */
                          cs,                  /* number of cols in image */
                          1,                   /* number of planes */
                          VFF_TYP_FLOAT);      /* data storage type */

    if (i_image == NULL)
      {
        (void) fprintf(stderr,"lvfft: Unable to allocate enough memory for output image!\n");
        return(0);
      }
    iptr = FLOATDATA(i_image);
    i_image->xdim = image1->xdim;
    i_image->ydim = image1->ydim;
    i_image->zdim = image1->zdim;

             /* assign pointer to input image data */
  if (image2 == NULL)          /* optional second input image not present */
  {
    fptr = FLOATDATA(image1);
  }
  else if (image2 != NULL)
  {
    fptr = FLOATDATA(image1);
    imptr = FLOATDATA(image2);
  }

 if (!dir)   /*<<<<<<<<    Forward FFT, dir =  0 (FALSE)    >>>>>>>>>>>*/
 {
    type = 0;                    /* Set FFT type argument to FORWARD */
    m1 = 1;                      /* Initialize Nyquist modulator */
    for(i=0; i<cs; i++)          /* Do Nyquist modulate and FFT on each row */
      {
        m = m1; m1 = -m1;
        for (j=0; j<rs; j++)
        {
           zr[j] = m*(*fptr++);   /* get real part from first input image */
           zi[j] = m*(*imptr++);  /* get imaginary part from second input image */
           m = -m;
        }
        /* Call routine to do full complex FFT */
        fft842(type,rs,zr,zi);

        pr = xr+i*rs;                 /* Store into (complex) rows) */
        pi = xi+i*rs;
        for(j=0; j<rs; j++)
        {
           *pr++ = zr[j];   /* Store the real part */
           *pi++ = zi[j];   /* Store the imag part */
        }
      }

    free(zr);               /* Give back temp row space */
    free(zi);               /* Give back temp row space */

    /* Get space for temporary complex row */
    tr = (float *)calloc(1,cs*sizeof(float));
    ti = (float *)calloc(1,cs*sizeof(float));
    if (tr == NULL || ti == NULL)
      {
        fprintf(stderr,"lvfft: Couldn't allocate enough memory!\n");
        return(0);
      }

    for(i=0; i<rs; i++)           /* Do FFT for the columns */
      {
        /* Load a column into temporary row */
        pr = xr+i;
        pi = xi+i;
        for (j=0; j<cs; j++)
          {
            tr[j] = *pr; pr += rs;
            ti[j] = *pi; pi += rs;
          } 

        /* Call routine to do FFT for the columns */
        fft842(type,cs,tr,ti);

        /* Put back into the same column */
        pr = xr+i;
        pi = xi+i;
        for (j=0; j<cs; j++)
          {
            *pr = tr[j]; pr += rs;
            *pi = ti[j]; pi += rs;
          }
      }

    /* Load the data in corresponding output images */
    pr = xr;
    pi = xi;

    /* output FLOAT image corresponding to REAL part */
    for (i=0; i<cs; i++)
      for (j=0; j<rs; j++)
        *rptr++ = *pr++;

    /* output FLOAT image corresponding to IMAG part */
    for (i=0; i<cs; i++)
      for (j=0; j<rs; j++)
        *iptr++ = *pi++;

    free(xr);               /* free real and imag data arrays */
    free(xi);
    free(tr);
    free(ti);
 }
 else if (dir)   /*<<<<<<<<<<<<    Inverse FFT, dir = TRUE    >>>>>>>>>>*/
 {
    type = 1;                  /* Set FFT type argument to INVERSE */
    /* Load input data from corresponding input images */
    pr = xr;               /* set pointer to real part of image */
    pi = xi;               /* set pointer to imag part of image */
    for (i=0; i<cs; i++)
    {
      for (j=0; j<rs; j++)
      {
        *pr++ = *fptr++;
        *pi++ = *imptr++;        /* get imag part from second input image */
      }
    }

    /* Get space for the temporaries */
    tr = (float *)calloc(1,cs*sizeof(float));
    ti = (float *)calloc(1,cs*sizeof(float));

    if (tr == NULL || ti == NULL)
      {
        fprintf(stderr,"lvfft: Couldn't allocate enough memory!\n");
        return(0);
      }

    for(i=0; i<rs; i++)    /* Do IFFT for the columns */
    {
        /* Load a column into temporary row */
        pr = xr+i;
        pi = xi+i;
        for (j=0; j<cs; j++)
          {
            tr[j] = *pr; pr += rs;
            ti[j] = *pi; pi += rs;
          } 
        fft842(type,cs,tr,ti);

        /* Put back into the same column */
        pr = xr+i;
        pi = xi+i;
        for(j=0; j<cs; j++)
          {
            *pr = tr[j]; pr += rs;
            *pi = ti[j]; pi += rs;
          }
    }

    free(tr);               /* free temporary arrays */
    free(ti);

    m1 = 1;
    for(i=0; i<cs; i++)          /* Do FFT for each row in image */
    {
        pr = xr+i*rs;             /* Store into (complex) rows) */
        pi = xi+i*rs;
        for (j=0; j<rs; j++)
        {
           zr[j] = *pr++;     /* Get the real part */
           zi[j] = *pi++;   /* Get the imag part */
        }
        fft842(type,rs,zr,zi); /* Do complex IFFT */

        /* Store resulting image */
        m = m1; m1 = -m1;

        /* output FLOAT image for REAL part */
        for (j=0; j<rs; j++)
        {
          *rptr++ = m*zr[j];
          m = -m;
        }

        /* output FLOAT image for IMAG part */
        for (j=0; j<rs; j++)
        {
          *iptr++ = m*zi[j];
          m = -m;
        }
    }
    free(xr);               /* free real and imag data arrays */
    free(xi);
    free(zr);
    free(zi);
 }

    /* Assign data to new image pointers */

   copy2image(image1, r_image);
   copy2image(image2, i_image);
   freeimage (r_image);
   freeimage (i_image);
   return(1);

}  /*** End of main ***/

/*
 *  fft842.c
 *
 *  A C-language implementation of the radix 8-4-2 fast Fourier transform;
 *  see Bergland & Dolan, "Fast Fourier Transform Algorithms", in Programs
 *    for Digital Signal Processing, IEEE Press, 1979.
 *
 *  Responsible:  Rick Bogart			RBogart@solar.Stanford.EDU
 *		  Lyle Bacon			LBacon@solar.Stanford.EDU
 *
 *  Functions:
 *    int32_t fft842 (int32_t in, int32_t n, float *x, float *y);
 *    void newfft842 (int32_t in, int32_t n, double *Wr, double *Wi,
 *	float *x, float *y);
 *    int32_t trig_init (int32_t N, double *Wr, double *Wi);
 *
 *  Description:
 *    fft842() performs a Fourier Transform of its complex argument vector
 *	z = x + iy, replacing the argument with its transform.
 *    newfft842() is the same as fft842, but requires the pre-computed twiddle
 *	factors Wr and Wi.  This is much more efficient when the same size
 *	transform is to be calculated repeatedly, as in a multi-dimensional
 *	transform.
 *    trig_init() computes the twiddle factors for newfft842().
 *
 *  Bugs:
 *    It should not be necessary to have separate functions.  It should also
 *	not be left up to the calling program to have to allocate space
 *	for the twiddle factors and to initialize.
 *    The functions aren't very nice about cases when the input dimension is
 *	not a power of 2.
 *    newfft842() and trig_init() have inappropriate I/O.
 *    newfft842 and trig_init() were added to the library at the last minute
 *	and have not been tested in this environment, although they have been
 *	debugged and extensively tested elsewhere.
 *
 *  Planned updates:
 *    Remove newfft842 and have fft842 reinitialize as necessary; make
 *	trig_init a static (local) function; and recombine r8tx and newr8tx.
 *
 *  Revision history is at end of file.
 */

#define VERSION_NUM	(0.9)

void r2tx (int32_t n, float *cr, float *ci)
/*
 *-----------------------------------------------------------------------
 * r2tx (n, cr, ci)
 *   radix 2 iteration routine
 *-----------------------------------------------------------------------
 */
{
float tmp;
int32_t k, kp1;

for (k=0; k<n; k+=2)
{
  kp1 = k + 1;
  tmp = cr[k] - cr[kp1];
  cr[k] += cr[kp1];
  cr[kp1] = tmp;
  tmp = ci[k] - ci[kp1];
  ci[k] += ci[kp1];
  ci[kp1] = tmp;
}
}

void r4tx (int32_t n, float *cr, float *ci)
/*
 *-----------------------------------------------------------------------
 * r4tx (n, cr, ci)
 *   radix 4 iteration routine
 *-----------------------------------------------------------------------
 */
{
float r1, r2, r3, r4, i1, i2, i3, i4;
int32_t k, kp1, kp2, kp3;

for (k=0; k<n; k+=4)
{
  kp1 = k + 1;
  kp2 = k + 2;
  kp3 = k + 3;

  r1 = cr[k] + cr[kp2];
  r2 = cr[k] - cr[kp2];
  r3 = cr[kp1] + cr[kp3];
  r4 = cr[kp1] - cr[kp3];
  i1 = ci[k] + ci[kp2];
  i2 = ci[k] - ci[kp2];
  i3 = ci[kp1] + ci[kp3];
  i4 = ci[kp1] - ci[kp3];
  cr[k] = r1 + r3;
  ci[k] = i1 + i3;
  cr[kp1] = r1 - r3;
  ci[kp1] = i1 - i3;
  cr[kp2] = r2 - i4;
  ci[kp2] = i2 + r4;
  cr[kp3] = r2 + i4;
  ci[kp3] = i2 - r4;
}
}

void r8tx (int32_t nxtlt, int32_t nthpo, int32_t lengt, float *cr, float *ci)
/*
 *-----------------------------------------------------------------------
 * subroutine:  r8tx
 * radix 8 iteration subroutine
 *-----------------------------------------------------------------------
 */
{
double	c1, c2, c3, c4, c5, c6, c7, s1, s2, s3, s4, s5, s6, s7;
double	ar0, ar1, ar2, ar3, ar4, ar5, ar6, ar7,
	ai0, ai1, ai2, ai3, ai4, ai5, ai6, ai7,
	br0, br1, br2, br3, br4, br5, br6, br7,
	bi0, bi1, bi2, bi3, bi4, bi5, bi6, bi7;
double	arg, scale, tr, ti;
float	*cr0, *cr1, *cr2, *cr3, *cr4, *cr5, *cr6, *cr7,
	*ci0, *ci1, *ci2, *ci3, *ci4, *ci5, *ci6, *ci7;
int32_t	j, k;

cr0 = cr;
ci0 = ci;
cr1 = cr0 + nxtlt;
ci1 = ci0 + nxtlt;
cr2 = cr1 + nxtlt;
ci2 = ci1 + nxtlt;
cr3 = cr2 + nxtlt;
ci3 = ci2 + nxtlt;
cr4 = cr3 + nxtlt;
ci4 = ci3 + nxtlt;
cr5 = cr4 + nxtlt;
ci5 = ci4 + nxtlt;
cr6 = cr5 + nxtlt;
ci6 = ci5 + nxtlt;
cr7 = cr6 + nxtlt;
ci7 = ci6 + nxtlt;
scale = (M_PI + M_PI) / lengt;

for (j=0; j<nxtlt; j++)
{
  arg = j * scale;
  c1 = cos (arg);
  s1 = sin (arg);
  c2 = c1*c1 - s1*s1;
  s2 = c1*s1 + c1*s1;
  c3 = c1*c2 - s1*s2;
  s3 = c2*s1 + s2*c1;
  c4 = c2*c2 - s2*s2;
  s4 = c2*s2 + c2*s2;
  c5 = c2*c3 - s2*s3;
  s5 = c3*s2 + s3*c2;
  c6 = c3*c3 - s3*s3;
  s6 = c3*s3 + c3*s3;
  c7 = c3*c4 - s3*s4;
  s7 = c4*s3 + s4*c3;
  for (k=j; k<nthpo; k+=lengt)
  {
    ar0 = cr0[k] + cr4[k];
    ar1 = cr1[k] + cr5[k];
    ar2 = cr2[k] + cr6[k];
    ar3 = cr3[k] + cr7[k];
    ar4 = cr0[k] - cr4[k];
    ar5 = cr1[k] - cr5[k];
    ar6 = cr2[k] - cr6[k];
    ar7 = cr3[k] - cr7[k];
    ai0 = ci0[k] + ci4[k];
    ai1 = ci1[k] + ci5[k];
    ai2 = ci2[k] + ci6[k];
    ai3 = ci3[k] + ci7[k];
    ai4 = ci0[k] - ci4[k];
    ai5 = ci1[k] - ci5[k];
    ai6 = ci2[k] - ci6[k];
    ai7 = ci3[k] - ci7[k];
    br0 = ar0 + ar2;
    br1 = ar1 + ar3;
    br2 = ar0 - ar2;
    br3 = ar1 - ar3;
    br4 = ar4 - ai6;
    br5 = ar5 - ai7;
    br6 = ar4 + ai6;
    br7 = ar5 + ai7;
    bi0 = ai0 + ai2;
    bi1 = ai1 + ai3;
    bi2 = ai0 - ai2;
    bi3 = ai1 - ai3;
    bi4 = ai4 + ar6;
    bi5 = ai5 + ar7;
    bi6 = ai4 - ar6;
    bi7 = ai5 - ar7;
    cr0[k] = br0 + br1;
    ci0[k] = bi0 + bi1;
    if (j)
    {
      cr1[k] = c4 * (br0 - br1) - s4 * (bi0 - bi1);
      ci1[k] = c4 * (bi0 - bi1) + s4 * (br0 - br1);
      cr2[k] = c2 * (br2 - bi3) - s2 * (bi2 + br3);
      ci2[k] = c2 * (bi2 + br3) + s2 * (br2 - bi3);
      cr3[k] = c6 * (br2 + bi3) - s6 * (bi2 - br3);
      ci3[k] = c6 * (bi2 - br3) + s6 * (br2 + bi3);
      tr = M_SQRT1_2 * (br5 - bi5);
      ti = M_SQRT1_2 * (br5 + bi5);
      cr4[k] = c1 * (br4 + tr) - s1 * (bi4 + ti);
      ci4[k] = c1 * (bi4 + ti) + s1 * (br4 + tr);
      cr5[k] = c5 * (br4 - tr) - s5 * (bi4 - ti);
      ci5[k] = c5 * (bi4 - ti) + s5 * (br4 - tr);
      tr = -M_SQRT1_2 * (br7 + bi7);
      ti = M_SQRT1_2 * (br7 - bi7);
      cr6[k] = c3 * (br6 + tr) - s3* (bi6+ti);
      ci6[k] = c3 * (bi6 + ti) + s3* (br6+tr);
      cr7[k] = c7 * (br6 - tr) - s7* (bi6-ti);
      ci7[k] = c7 * (bi6 - ti) + s7* (br6-tr);
    }
    else
    {
      cr1[k] = br0 - br1;
      ci1[k] = bi0 - bi1;
      cr2[k] = br2 - bi3;
      ci2[k] = bi2 + br3;
      cr3[k] = br2 + bi3;
      ci3[k] = bi2 - br3;
      tr = M_SQRT1_2 * (br5 - bi5);
      ti = M_SQRT1_2 * (br5 + bi5);
      cr4[k] = br4 + tr;
      ci4[k] = bi4 + ti;
      cr5[k] = br4 - tr;
      ci5[k] = bi4 - ti;
      tr = -M_SQRT1_2 * (br7 + bi7);
      ti = M_SQRT1_2 * (br7 - bi7);
      cr6[k] = br6 + tr;
      ci6[k] = bi6 + ti;
      cr7[k] = br6 - tr;
      ci7[k] = bi6 - ti;
    }
  }
}
}
void newr8tx (int32_t nxtlt, int32_t nthpo, int32_t length, float *cr, float *ci, int32_t n, double *Wr, double *Wi)
/*
 *-----------------------------------------------------------------------
 * subroutine:  newr8tx
 * radix 8 iteration subroutine
 *-----------------------------------------------------------------------
 */
{
double	c1, c2, c3, c4, c5, c6, c7, s1, s2, s3, s4, s5, s6, s7;
double	ar0, ar1, ar2, ar3, ar4, ar5, ar6, ar7,
	ai0, ai1, ai2, ai3, ai4, ai5, ai6, ai7,
	br0, br1, br2, br3, br4, br5, br6, br7,
	bi0, bi1, bi2, bi3, bi4, bi5, bi6, bi7;
double	tr, ti;
float	*cr0, *cr1, *cr2, *cr3, *cr4, *cr5, *cr6, *cr7,
	*ci0, *ci1, *ci2, *ci3, *ci4, *ci5, *ci6, *ci7;
int32_t	j, k;
int32_t     iscale, iarg;
int32_t miarg;
cr0 = cr;
ci0 = ci;
cr1 = cr0 + nxtlt;
ci1 = ci0 + nxtlt;
cr2 = cr1 + nxtlt;
ci2 = ci1 + nxtlt;
cr3 = cr2 + nxtlt;
ci3 = ci2 + nxtlt;
cr4 = cr3 + nxtlt;
ci4 = ci3 + nxtlt;
cr5 = cr4 + nxtlt;
ci5 = ci4 + nxtlt;
cr6 = cr5 + nxtlt;
ci6 = ci5 + nxtlt;
cr7 = cr6 + nxtlt;
ci7 = ci6 + nxtlt;
iscale = n/length;

for (j=0; j<nxtlt; j++)
{
  iarg = j * iscale;
  c1 = Wr[iarg];
  s1 = Wi[iarg];
  c2 = Wr[2 *iarg];
  s2 = Wi[2 * iarg];
  c3 = Wr[3 * iarg];
  s3 = Wi[3 * iarg];
  c4 = Wr[4 * iarg];
  s4 = Wi[4 * iarg];
  c5 = Wr[5 * iarg];
  s5 = Wi[5 * iarg];
  c6 = Wr[6 * iarg];
  s6 = Wi[6 * iarg];
  c7 = Wr[7 * iarg];
  s7 = Wi[7 * iarg];
  miarg = iarg > miarg ? iarg : miarg;
  for (k=j; k<nthpo; k+=length)
  {
    ar0 = cr0[k] + cr4[k];
    ar1 = cr1[k] + cr5[k];
    ar2 = cr2[k] + cr6[k];
    ar3 = cr3[k] + cr7[k];
    ar4 = cr0[k] - cr4[k];
    ar5 = cr1[k] - cr5[k];
    ar6 = cr2[k] - cr6[k];
    ar7 = cr3[k] - cr7[k];
    ai0 = ci0[k] + ci4[k];
    ai1 = ci1[k] + ci5[k];
    ai2 = ci2[k] + ci6[k];
    ai3 = ci3[k] + ci7[k];
    ai4 = ci0[k] - ci4[k];
    ai5 = ci1[k] - ci5[k];
    ai6 = ci2[k] - ci6[k];
    ai7 = ci3[k] - ci7[k];
    br0 = ar0 + ar2;
    br1 = ar1 + ar3;
    br2 = ar0 - ar2;
    br3 = ar1 - ar3;
    br4 = ar4 - ai6;
    br5 = ar5 - ai7;
    br6 = ar4 + ai6;
    br7 = ar5 + ai7;
    bi0 = ai0 + ai2;
    bi1 = ai1 + ai3;
    bi2 = ai0 - ai2;
    bi3 = ai1 - ai3;
    bi4 = ai4 + ar6;
    bi5 = ai5 + ar7;
    bi6 = ai4 - ar6;
    bi7 = ai5 - ar7;
    cr0[k] = br0 + br1;
    ci0[k] = bi0 + bi1;
    if (j)
    {
      cr1[k] = c4 * (br0 - br1) - s4 * (bi0 - bi1);
      ci1[k] = c4 * (bi0 - bi1) + s4 * (br0 - br1);
      cr2[k] = c2 * (br2 - bi3) - s2 * (bi2 + br3);
      ci2[k] = c2 * (bi2 + br3) + s2 * (br2 - bi3);
      cr3[k] = c6 * (br2 + bi3) - s6 * (bi2 - br3);
      ci3[k] = c6 * (bi2 - br3) + s6 * (br2 + bi3);
      tr = M_SQRT1_2 * (br5 - bi5);
      ti = M_SQRT1_2 * (br5 + bi5);
      cr4[k] = c1 * (br4 + tr) - s1 * (bi4 + ti);
      ci4[k] = c1 * (bi4 + ti) + s1 * (br4 + tr);
      cr5[k] = c5 * (br4 - tr) - s5 * (bi4 - ti);
      ci5[k] = c5 * (bi4 - ti) + s5 * (br4 - tr);
      tr = -M_SQRT1_2 * (br7 + bi7);
      ti = M_SQRT1_2 * (br7 - bi7);
      cr6[k] = c3 * (br6 + tr) - s3* (bi6+ti);
      ci6[k] = c3 * (bi6 + ti) + s3* (br6+tr);
      cr7[k] = c7 * (br6 - tr) - s7* (bi6-ti);
      ci7[k] = c7 * (bi6 - ti) + s7* (br6-tr);
    }
    else
    {
      cr1[k] = br0 - br1;
      ci1[k] = bi0 - bi1;
      cr2[k] = br2 - bi3;
      ci2[k] = bi2 + br3;
      cr3[k] = br2 + bi3;
      ci3[k] = bi2 - br3;
      tr = M_SQRT1_2 * (br5 - bi5);
      ti = M_SQRT1_2 * (br5 + bi5);
      cr4[k] = br4 + tr;
      ci4[k] = bi4 + ti;
      cr5[k] = br4 - tr;
      ci5[k] = bi4 - ti;
      tr = -M_SQRT1_2 * (br7 + bi7);
      ti = M_SQRT1_2 * (br7 - bi7);
      cr6[k] = br6 + tr;
      ci6[k] = bi6 + ti;
      cr7[k] = br6 - tr;
      ci7[k] = bi6 - ti;
    }
  }
}

}

void newfft842 (int32_t in, int32_t n, double *Wr, double *Wi, /* inputs  */
	        float *x, float *y)                 /* in-place in & out  */
/*
 *
 *  This program replaces the vector z = x + iy by its finite discrete,
 *    complex Fourier transform if in <= 0.  The inverse transform is
 *    calculated for in > 0.  It performs as many base 8 iterations
 *    as possible and then finishes with a base 4 iteration or a base 2
 *    iteration if needed.
 *  trig_init (n, Wr, Wi);  to initialize twiddle factors.
 *  The function is called as newfft842 (in, n, Wr, Wi, x, y).
 *    The integer n should be a power of 2, and x and y pointers to arrays
 *    of n floats each.
 *
 */
{
float fnv, t;
int32_t i, ij, ipass, ji, length, m, mmax, n2pow, n8pow, nt, nthpo, nxtlt;

i = n;
m = 0;
nt = 1;
while (i > 1)
{
  i >>= 1;
  m++;
  nt *= 2;
}
if (nt > n)
{
  fprintf (stderr, "Size error: # of elements n = %d for newfft842\n", n);
  exit (1);
}
if (nt < n)
{
  fprintf (stderr, "Dimension not a power of two for newfft842; truncated\n");
}
n2pow = m;
nthpo = nt;
mmax = nt >> 1;
if (in != 1)
{
  for (i=0; i<nthpo; i++)
    y[i] = -y[i];
}
n8pow = n2pow / 3;
if (n8pow)
/*
 *  Radix 8 passes, if any.
 */
{
  nxtlt = nt;
  for (ipass=0; ipass<n8pow; ipass++)
  {
    nxtlt >>= 3;
    length = 8 * nxtlt;
    newr8tx (nxtlt, nthpo, length, x, y, n, Wr, Wi);
  }
}

/*
 *  Is there a four factor left?  : Go through the base 4 iteration
 */
if ((n2pow - 3*n8pow) == 2)
  r4tx (nthpo, x, y);

else if ((n2pow - 3*n8pow) == 1)
/*
 *  Go through the base 2 iteration
 */
  r2tx (nthpo, x, y);

/*
 *  Now do the bit reversal: decimation in frequency
 */
ji = 0;
for (ij=0; ij<nthpo; ij++)
{
  if (ij < ji)							  /*  Swap  */
  {
    t = x[ij];
    x[ij] = x[ji];
    x[ji] = t;
    t = y[ij];
    y[ij] = y[ji];
    y[ji] = t;
  }
  m = mmax;
  while (m >= 2 && ji >= m)
  {
    ji -= m;
    m >>= 1;
  }
  ji += m;
}
if (in > 0)
{
  fnv = 1.0 / nt;
  for (i=0; i<nthpo; i++)
  {
    x[i] *= fnv;
    y[i] *= fnv;
  }
}
else
{
  for (i=0; i<nthpo; i++)
    y[i] = -y[i];
}
}
/*
 * trig_init.c
 * Initialize arrays of twiddle-factors (sparse arrays) for radix-8 FFT.
 * Allocate 7 * (N / 8 - 1) + 1 =  7 * N / 8 - 6  locations for trig
 * tables Wr and Wi.
 */

#define PI2 (M_PI + M_PI)

int32_t trig_init(int32_t N, double *Wr, double *Wi)

{
int32_t j;
int32_t iarg, iscale;
double arg, scale;
int32_t length;
int32_t i, m, n2pow, n8pow, nt, nxtlt;

i = N;
m = 0;
nt = 1;
while (i > 1)
{
  i >>= 1;
  m++;
  nt *= 2;
}
if (nt > N)
{
  fprintf (stderr, "Size error: # of elements N = %d for trig_init\n", N);
  exit (1);
}
if (nt < N)
{
  fprintf (stderr, "Dimension not a power of two for trig_init; truncated\n");
}
n2pow = m;
n8pow = n2pow / 3;


nxtlt = nt;
#ifdef DEBUG
printf ("\nn2pow = = %d n8pow = %d  ", n2pow, n8pow);
#endif

    nxtlt >>= 3;
    length = 8 * nxtlt;


/* printf ("\nlenght = %d nxtlt = %d  ", length, nxtlt);
     r8tx (nxtlt, nthpo, length, x, y); */


scale = PI2 / (float)length;
iscale = length/(N);
#ifdef DEBUG
printf(" iscale = %d scale = %lg\n", iscale, scale);
#endif
for (j=1; j<nxtlt; j++)
{
  arg = j * scale;
  iarg = j * iscale;
#ifdef DEBUG
printf("j = %d, nxtlt = %d iarg = %d arg = %lg\n", j, nxtlt, iarg, arg);
#endif

  Wr[iarg] = cos (arg);
  Wi[iarg] = sin (arg);
  Wr[2*iarg] = Wr[iarg] * Wr[iarg] -   Wi[iarg] * Wi[iarg];
  Wi[2*iarg] = Wr[iarg] * Wi[iarg] +   Wr[iarg] * Wi[iarg];
  Wr[3*iarg] = Wr[iarg] * Wr[2*iarg] - Wi[iarg] * Wi[2*iarg];
  Wi[3*iarg] = Wr[2*iarg]*Wi[iarg] +   Wi[2*iarg]*Wr[iarg];
  Wr[4*iarg] = Wr[2*iarg]*Wr[2*iarg] - Wi[2*iarg]*Wi[2*iarg];
  Wi[4*iarg] = Wr[2*iarg]*Wi[2*iarg] + Wr[2*iarg]*Wi[2*iarg];
  Wr[5*iarg] = Wr[2*iarg]*Wr[3*iarg] - Wi[2*iarg]*Wi[3*iarg];
  Wi[5*iarg] = Wr[3*iarg]*Wi[2*iarg] + Wi[3*iarg]*Wr[2*iarg];
  Wr[6*iarg] = Wr[3*iarg]*Wr[3*iarg] - Wi[3*iarg]*Wi[3*iarg];
  Wi[6*iarg] = Wr[3*iarg]*Wi[3*iarg] + Wr[3*iarg]*Wi[3*iarg];
  Wr[7*iarg] = Wr[3*iarg]*Wr[4*iarg] - Wi[3*iarg]*Wi[4*iarg];
  Wi[7*iarg] = Wr[4*iarg]*Wi[3*iarg] + Wi[4*iarg]*Wr[3*iarg];
}				/* End j           */

Wr[0] = 1.0; Wi[0] = 0.0;

#ifdef DEBUG
for (i = 0; i < N/8; i++) 
  printf("i = %d, W = %lg %lg\n", i, Wr[i], Wi[i]);
printf("\n");
#endif
return(0);
}


int32_t fft842 (int32_t in, int32_t n, float *x, float *y)
/*
 *  fft842 (in, n, x, y)
 *
 *  This program replaces the vector z = x + iy by its finite discrete,
 *    complex Fourier transform if in = 0.  The inverse transform is
 *    calculated for in = 1.  It performs as many base 8 iterations
 *    as possible and then finishes with a base 4 iteration or a base 2
 *    iteration if needed.
 *
 *  The function is called as fft842 (in, n, x, y).
 *    The integer n should be a power of 2, and x and y pointers to arrays
 *    of n floats each.  If n is non-positive, no transform is performed and
 *    the function returns -1.  If n is not a power of 2, only the values
 *    of x and y up to the largest power of 2 smaller than n are transformed
 *    and the function returns 1.  Otherwise, the function returns 0.
 *
 */
{
float fnv, t;
int32_t i, ij, ipass, ji, lengt, m, mmax, n2pow, n8pow, nt, nthpo, nxtlt;
int32_t returnval;

i = n;
m = 0;
nt = 1;
while (i > 1)
{
  i >>= 1;
  m++;
  nt *= 2;
}
if (nt > n)
{
			 /*  dimension negative or out of range for fft842  */
  return (-1);
}
if (nt < n)
		    /*  dimension not a power of two for fft842; truncated  */
  returnval = 1;
else
  returnval = 0;
n2pow = m;
nthpo = nt;
mmax = nt >> 1;
if (!in)
{
  for (i=0; i<nthpo; i++)
    y[i] = -y[i];
}
n8pow = n2pow / 3;
if (n8pow)
/*
 *  Radix 8 passes, if any.
 */
{
  nxtlt = nt;
  for (ipass=0; ipass<n8pow; ipass++)
  {
    nxtlt >>= 3;
    lengt = 8 * nxtlt;
    r8tx (nxtlt, nthpo, lengt, x, y);
  }
}
/*
 *  Is there a four factor left?  : Go through the base 4 iteration
 */
if ((n2pow - 3*n8pow) == 2)
  r4tx (nthpo, x, y);

else if ((n2pow - 3*n8pow) == 1)
/*
 *  Go through the base 2 iteration
 */
  r2tx (nthpo, x, y);
/*
 *  Now do the bit reversal: decimation in frequency
 */
ji = 0;
for (ij=0; ij<nthpo; ij++)
{
  if (ij < ji)							  /*  Swap  */
  {
    t = x[ij];
    x[ij] = x[ji];
    x[ji] = t;
    t = y[ij];
    y[ij] = y[ji];
    y[ji] = t;
  }
  m = mmax;
  while (m >= 2 && ji >= m)
  {
    ji -= m;
    m >>= 1;
  }
  ji += m;
}
if (in)
{
  fnv = 1.0 / nt;
  for (i=0; i<nthpo; i++)
  {
    x[i] *= fnv;
    y[i] *= fnv;
  }
}
else
{
  for (i=0; i<nthpo; i++)
    y[i] = -y[i];
}
return (returnval);
}

/*
 *  Revision History
 *	   92.10.20	Rick Bogart	created this file
 *  V 0.8  94.02.10	R Bogart	Version 0.8 - updated comments; made
 *		helper functions local
 *  V 0.9  94.11.07	R Bogart	included newfft842(), trig_init(),
 *		and newr8tx(); defined VERSION_NUMBER, modified includes.
 */
