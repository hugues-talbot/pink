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
/*! \file lantuejoul.c

\brief Lantuejoul's skeleton

<B>Usage:</B> lantuejoul in.pgm [dist] out.pgm

<B>Description:</B>
Let X be the set in \b in.pgm .
The result is union{Si(X), i in N} where
Si(X) = erosball(X,i) \ openball(erosball(X,i),1). 
Structuring elements are balls defined after a distance.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: approximate euclidean distance (truncated)
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie août 2009

*/

#define VERBOSE

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <mcutil.h>
#include <ldist.h>

/* ==================================== */
int32_t llantuejoul(
  struct xvimage *img,   /* donnee/résultat: image binaire */
  int32_t mode
)
/* ==================================== */
/* 
The result is union{Si(X), i in N} where
Si(X) = erosball(X,i) \ openball(erosball(X,i),1). 

The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: approximate euclidean distance
\li 2: chamfer distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.
*/
#undef F_NAME
#define F_NAME "llantuejoul"
{ 
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t ds = depth(img); 
  int32_t N = rs*cs*ds;
  struct xvimage *ori;
  struct xvimage *tmp;
  struct xvimage *tmp1;
  uint8_t *T, *T1, *R;
  uint32_t r, i, ok, connex, rmax;

#ifdef VERBOSE
    printf("%s: mode = %d\n", F_NAME, mode);
#endif

  if (ds == 1) connex = 4; else connex = 6; 
  if (ds == 1) rmax = min((rs/2),(cs/2)); 
  else         rmax = min((ds/2),min((rs/2),(cs/2)));

  ori = copyimage(img); assert(ori != NULL);
  tmp = copyimage(img); assert(tmp != NULL);
  tmp1 = copyimage(img); assert(tmp1 != NULL);
  T = UCHARDATA(tmp);
  T1 = UCHARDATA(tmp1);
  R = UCHARDATA(img);
  razimage(img);
  
  r = 0;
  do 
  {
#ifdef VERBOSE
    printf("%s: r = %d\n", F_NAME, r);
#endif
    copy2image(tmp, ori);
    ok = lerosball(tmp, r, mode); assert(ok);
    copy2image(tmp1, tmp);
    ok = lopenball(tmp1, 1, mode); assert(ok);
    for (i = 0; i < N; i++)
      if (T[i] && !T1[i])
	R[i] = 255;
    r++;
  } while (r <= rmax);

  freeimage(ori);
  freeimage(tmp);
  freeimage(tmp1);
  return 1;
} // llantuejoul()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t mode;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s f.pgm [dist] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4) mode = atoi(argv[2]); else mode = 0;
  if ((mode != 0) && (mode != 2) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|2|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  llantuejoul(image, mode);

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
