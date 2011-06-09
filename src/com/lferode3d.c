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
/* \file lferode3d.c

\brief

<B>Usage:</B>

<B>Description:</B>

<B>Types supported:</B>

<B>Category:</B>
\ingroup

\author Hugues Talbot
*/
/*
NAME

<B>dilat</B> - morphological erosion by a rectangle structuring element

SYNOPSIS

<B>dilat</B> in.pgm el.pgm x y out.pgm

DESCRIPTION

The rectangle structuring element is given by the non-null values x and y, which are the size of the rectangle
<B>x</B> and <B>y</B>.

Types supported: all type

CLASS

morph

*/

/* Hugues Talbot, June 2011 */

# include <stdio.h>
# include <stdint.h>
# include <sys/types.h>
# include <stdlib.h>
# include <mccodimage.h>
# include <mcimage.h>
# include <liarp.h>


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int dimx=0, dimy=0;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s f.pgm px py out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  dimx=atof(argv[2]);
  dimy=atof(argv[3]);
  if ( (dimx==0) ||(dimy==0) )
  {
    fprintf(stderr, "%s wrong dimension for the rectangle \n", argv[0]);
    exit(1);
  }


  if (imferode3D_rect(image, dimx, dimy,1, image)!=0 )
  {
      fprintf(stderr, "%s: function imferode3D_rect failed\n", argv[0]);
      exit(1);
  }


  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
