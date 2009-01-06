/* $Id: recalagerigide_num.c,v 1.5 2009-01-06 13:18:06 mcouprie Exp $ */

/*! \file recalagerigide_num.c

\brief rigid registration of two grayscale images

<B>Usage:</B> recalagerigide_num in1 in2 xmin ymin xmax ymax seuil [init] out

<B>Description:</B>

Rigid registration of two grayscale images. 
Let X and Y be two images, given respectively by \b in1 and \b in2.
This procedure identifies the parameters of a rigid deformation R such that
the "distance" between R(X) and Y is a local minimum.
The distance D(Z,Y) between two images Z and Y is defined by:

D(Z,Y) = sum { (Z[i] - Y[i])^2 ; for all i in support(Y) }

where support(Y) is a rectangular zone specified by arguments 
<B>xmin ymin xmax ymax<B>.

The rigid deformation R is defined as the composition (in this order) 
of a zoom, a rotation, and a translation. 

In 2d, the parameters are:
\li hx : parameter for the scaling in direction x
\li hy : parameter for the scaling in direction y
\li theta : angle (in degrees) of the rotation around (0,0)
\li tx : parameter for the translation in direction x
\li ty : parameter for the translation in direction y

The output parameter \b out is the name of a text file in which these 
values will be written (type "e" list format, see doc/formats.txt).

The parameter \b seuil makes it possible to eliminate outliers: 
points i such that (Z[i] - Y[i])^2 > seuil^2 are not taken into account.
A value 255 for this parameter means no outlier elimination.

The optional parameter \b init makes it possible to give, 
in the same format as the output, an initial deformation which is 
"close" to the expected one. The default initialisation is the identity
(parameters 0, 1, 1, 0, 0).

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
#include <lrotations.h>

//#define VERBOSE
const double PRECISION = 0.0001;

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t rs, cs, ds, ps, N, n;
  double Gamma[9] = {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // hx, hy, theta, tx, ty
  double seuil; 
  FILE *fd = NULL;

  if ((argc != 9) && (argc != 10))
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm x y w h seuil [init] out.lst \n", argv[0]);
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

  if ((rowsize(image2) != rowsize(image1)) || 
      (colsize(image2) != colsize(image1)) || 
      (depth(image2) != depth(image1)))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", argv[0]);
    exit(1);
  }

  image2->xmin = (uint32_t)atoi(argv[3]);
  image2->ymin = (uint32_t)atoi(argv[4]);
  image2->xmax = (uint32_t)atoi(argv[5]);
  image2->ymax = (uint32_t)atoi(argv[6]);
  seuil = atof(argv[7]);

  if (argc == 10)
  {
    char type;
    int32_t n;

    fd = fopen(argv[8],"r");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[8]);
      exit(1);
    }

    fscanf(fd, "%c", &type);
    if (type != 'e')
    {
      fprintf(stderr, "usage: %s: bad file format for %s: %c \n", argv[0], argv[8], type);
      exit(1);
    }

    fscanf(fd, "%d\n", &n);
    if (n != 5)
    {
      fprintf(stderr, "usage: %s: wrong number of parameters in %s: %d \n", argv[0], argv[8], n);
      exit(1);
    }

    fscanf(fd, "%lf%lf%lf%lf%lf", &(Gamma[0]), &(Gamma[1]), &(Gamma[2]), &(Gamma[3]), &(Gamma[4]));
    Gamma[2] = (Gamma[2] * M_PI) / 180;

    fclose(fd);
  }

  if (depth(image1) == 1) // 2D
  {
    (void)lrecalagerigide2d_num(image1, image2, Gamma, seuil, PRECISION);
#ifdef VERBOSE
    printf("hx = %g\n", Gamma[0]);
    printf("hy = %g\n", Gamma[1]);
    printf("theta = %g = %g deg\n", Gamma[2], (Gamma[2]*180)/M_PI);
    printf("tx = %g\n", Gamma[3]);
    printf("ty = %g\n", Gamma[4]);
#endif
  }
  else // 3D
  {
    fprintf(stderr, "%s: 3D not implemented\n", argv[0]);
    exit(1);
  }

#ifdef APPLY_TRANSFORMATION
  if (! laffinetransformation(image1, Gamma[0], Gamma[1], Gamma[2], Gamma[3], Gamma[4], image2))
  {
    fprintf(stderr, "%s: function laffinetransformation failed\n", argv[0]);
    exit(1);
  }
  writeimage(image2, argv[argc-1]);
#else
  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "e %d\n", 5); 
  fprintf(fd, "%g %g %g %g %g\n", 1.0, 1.0, (Gamma[2]*180)/M_PI, Gamma[3], Gamma[4]); 
  fclose(fd);
#endif

  freeimage(image1);
  freeimage(image2);
  return 0;
} /* main */


