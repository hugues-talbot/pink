/* $Id: radialopening.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file radialopening.c

\brief radial opening by line

<B>Usage:</B> radialopening in.pgm length [angle in degrees] [dilatmask.pgm] out.pgm

<B>Description:</B> 
Radial opening by line (or dilated line). Max between the openings of
in.pgm by a linear structuring element rotated by angle until 360 degrees
and dilated by dilatmask.pgm. The angle is normalized to an exact division
of 360 by an integer.

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author André Vital Saude jan 2005

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <ldilateros.h>
#include <ldraw.h>
#include <lsym.h>

//#define DEBUG 
//#define VERBOSE

#define DEFAULT_ANGLE 30.0

/* =============================================================== */
void drawline(struct xvimage *mask, int32_t length, double angle) 
/* =============================================================== */
{
  int32_t dx, dy, x1, y1, x2, y2;
  double rangle;

  rangle = (angle * M_PI) / 180;

  dx = (length/2) * cos(rangle);
  dy = (length/2) * sin(rangle);
  
  x1 = (length/2) - dx;
  y1 = (length/2) - dy;
  x2 = (length/2) + dx;
  y2 = (length/2) + dy;

  ldrawline(mask, x1, y1, x2, y2);

#ifdef VERBOSE
  printf("length=%d; angle=%f; rangle=%f; dx=%d; dy=%d; x1=%d; y1=%d; x2=%d; y2=%d \n", length, angle, rangle, dx, dy, x1, y1, x2, y2);
#endif

}

/* =============================================================== */
void open_image(struct xvimage * img, struct xvimage * org_img, struct xvimage * mask) 
/* open the org_image and write the result into img */
/* Adapter, that creates a copy of the original image */
/* and executes a dilation after an erosion on the copy */
/* Gets: a result_image, the original image, and a mask */
/* =============================================================== */
{	
  // Copy org_img to img
  copy2image(img, org_img);

  //Erosion
  leros(img, mask, rowsize(mask)/2, colsize(mask)/2);

  
  //As mask is a line dilated by an ordinary structuring element,
  //it is necessary:
  if (! lsym(mask, 'c')) {
    fprintf(stderr, "open_image: function lsym failed\n");
    exit(1);
  }

  //Dilation
  ldilat(img, mask, rowsize(mask)/2, colsize(mask)/2);

  return;
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image; 
  struct xvimage * opened_image; 
  struct xvimage * result_image;
  struct xvimage * mask;
  struct xvimage * dilatmask=NULL;

  int32_t rs, cs, ds, N, Nmask;
  int32_t sex, sey, sez;
  int32_t length, i, j, nangles; 
  uint8_t * mask_data;

  double angle=DEFAULT_ANGLE;

  if ((argc < 4) || (argc > 6))
  {
    fprintf(stderr, "usage: %s in.pgm length [angle in degrees] [dilatmask.pgm] out.pgm\n    default angle: %f\n", argv[0], DEFAULT_ANGLE);
    exit(1);
  }

  // read parameters
  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  length = atoi(argv[2]);	// length of structuring element
  if (argc > 4)
  {
    angle = atof(argv[3]);
  }

  if (argc > 5)
  {
    dilatmask = readse(argv[4], &sex, &sey, &sez);
    if (dilatmask == NULL) {
      fprintf(stderr, "%s: readse failed\n", argv[0]);
      exit(1);
    }
  }

  rs = rowsize(image);      /* taille ligne */
  cs = colsize(image);      /* taille colonne */
  ds = depth(image);        /* nombre plans */
  N = rs * cs * ds;         /* taille image */

  length = (length/2) * 2 +1; 	// has to be 2*x+1 (ungerade)
  Nmask = length*length;
  printf("Length is: %i\n", length);
 
  mask = allocimage(NULL, length, length, ds, datatype(image));
  mask_data = UCHARDATA(mask);
  opened_image = allocimage(NULL, rs, cs, ds, datatype(image));
  result_image = allocimage(NULL, rs, cs, ds, datatype(image));

#ifdef VERBOSE
  printf("angle=%f\n", angle);
#endif

  nangles = (int32_t)(360.0/angle);
  angle = 360.0 / (double)nangles;

#ifdef VERBOSE
  printf("angle=%f; nangles=%d\n", angle, nangles);
#endif

  // For every angle
  for(i=0; i<nangles; i++){
    //reset mask_data
    memset(mask_data, 0, Nmask);
#ifdef VERBOSE
    printf("%d: ", i);
#endif
    drawline(mask, length, angle*(double)i);
    if (dilatmask != NULL) {
      ldilat(mask, dilatmask, sex, sey);
    }
    open_image(opened_image, image, mask);
#ifdef DEBUG
{
char buf[128];
sprintf(buf, "_tmp%d\0", i);
writeimage(opened_image, buf);
}
#endif
    if (! lmax(result_image, opened_image)) {
      fprintf(stderr, "%s: function lmax failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(result_image, argv[argc-1]);
#ifdef DEBUG
  writeimage(mask, "/tmp/mask.pgm");
#endif
  freeimage(mask);
  freeimage(opened_image);
  freeimage(result_image);
  freeimage(image);
  
  return 0;
} /* main */
