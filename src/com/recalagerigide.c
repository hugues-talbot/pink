/* $Id: recalagerigide.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* ATTENTION: 
  version peu testée en 2D
  VERSION NON TESTEE EN 3D 
*/

/*! \file recalagerigide.c

\brief rigid registration of two closed contours

<B>Usage:</B> recalagerigide in1 in2 out

<B>Description:</B>

Rigid registration of two closed contours. 
Let X and Y be two point sets, given respectively by \b in1 and \b in2.
This procedure identifies the parameters of a rigid deformation R such that
the "distance" between R(X) and Y is a local minimum.
The distance D(Z,Y) between two sets Z and Y is defined by:

D(Z,Y) = sum { d2(z,Y) ; for all z in Z }

d2(z,Y) = min { d2(z,y) ; for all y in Y }

d2(z,y) = (z - y)^2 ; i.e., the square or the Euclidean distance between z and y.

The rigid deformation R is defined as the composition (in this order) 
of scalings, rotations and translations. 

In 2d, the parameters are:
\li sx : parameter for the scaling in direction x
\li sy : parameter for the scaling in direction y
\li theta : angle (in radians) of the rotation around the barycenter of X
\li tx : parameter for the translation in direction x
\li ty : parameter for the translation in direction y

In 3d, the parameters are:
\li sx : parameter for the scaling in direction x
\li sy : parameter for the scaling in direction y
\li sz : parameter for the scaling in direction z
\li theta : angle (in radians) of the rotation around the parallel to the the z axis passing by the barycenter of X
\li phi : angle (in radians) of the rotation around the parallel to the the y axis passing by the barycenter of X
\li tx : parameter for the translation in direction x
\li ty : parameter for the translation in direction y
\li tz : parameter for the translation in direction z

<B>Types supported:</B> byte 2d, byte 3d

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
  uint8_t *F;
  int32_t n1, n2;
  double * P1, * P2, *Gamma;

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

  P1 = image2list(image1, &n1);
  P2 = image2list(image2, &n2);

  if (depth(image1) == 1) // 2D
  {
    Gamma = lrecalagerigide2d(P1, n1, P2, n2);
    printf("sx = %g\n", Gamma[0]);
    printf("sy = %g\n", Gamma[1]);
    printf("theta = %g = %g deg\n", Gamma[2], (Gamma[2]*180)/M_PI);
    printf("tx = %g\n", Gamma[3]);
    printf("ty = %g\n", Gamma[4]);
  }
  else // 3D
  {
    Gamma = lrecalagerigide3d(P1, n1, P2, n2);
    printf("sx = %g\n", Gamma[0]);
    printf("sy = %g\n", Gamma[1]);
    printf("sz = %g\n", Gamma[2]);
    printf("theta = %g = %g deg\n", Gamma[3], (Gamma[3]*180)/M_PI);
    printf("phi   = %g = %g deg\n", Gamma[4], (Gamma[4]*180)/M_PI);
    printf("tx = %g\n", Gamma[5]);
    printf("ty = %g\n", Gamma[6]);
    printf("tz = %g\n", Gamma[7]);
  }

  razimage(image1);
  list2image(image1, P1, n1);
  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);
  free(P1);
  free(P2);
  free(Gamma);
  return 0;
} /* main */


