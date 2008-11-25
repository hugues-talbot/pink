/* $Id: recalagerigide_num.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */

/*! \file recalagerigide_num.c

\brief rigid registration of two grayscale images

<B>Usage:</B> recalagerigide_num in1 in2 out

<B>Description:</B>

Rigid registration of two grayscale images. 
Let X and Y be two images, given respectively by \b in1 and \b in2.
This procedure identifies the parameters of a rigid deformation R such that
the "distance" between R(X) and Y is a local minimum.
The distance D(Z,Y) between two images Z and Y is defined by:

D(Z,Y) = sum { (Z[i] - Y[i])^2 ; for all i in support(Z) }

The rigid deformation R is defined as the composition (in this order) 
of a rotation and a translation. 

In 2d, the parameters are:
\li theta : angle (in radians) of the rotation around the barycenter of X
\li tx : parameter for the translation in direction x
\li ty : parameter for the translation in direction y

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcgeo.h>
#include <lrecalagerigide.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t rs, cs, ds, ps, N, x, y, z, n;
  double *Gamma;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if ((datatype(image1) != VFF_TYP_1_BYTE) || (datatype(image2) != VFF_TYP_1_BYTE))
  {
    fprintf(stderr, "%s: incompatible data types\n", argv[0]);
    exit(1);
  }

  if (depth(image1) == 1) // 2D
  {
    Gamma = lrecalagerigide2d_num(image1, image2);
    printf("theta = %g = %g deg\n", Gamma[2], (Gamma[2]*180)/M_PI);
    printf("tx = %g\n", Gamma[3]);
    printf("ty = %g\n", Gamma[4]);
  }
  else // 3D
  {
    fprintf(stderr, "%s: 3D not implemented\n", argv[0]);
    exit(1);
  }

  freeimage(image1);
  freeimage(image2);
  free(Gamma);
  return 0;
} /* main */


