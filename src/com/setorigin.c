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
/*! \file setorigin.c

\brief adds origin to file header

<B>Usage:</B> setorigin in.pgm [source_se | xorig yorig zorig] 

<B>Description:</B> Adds origin (the one of \b se or \b xorig \b yorig \b zorig ) 
to file header of \b in.pgm.

<B>Types supported:</B> byte 3d, int32_t 3d

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t x, y, z;

  if ((argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s image [source_se | xorig yorig zorig] \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    struct xvimage * se;
    se = readse(argv[2], &x, &y, &z);  
    if (se == NULL)
    {
      fprintf(stderr, "%s: readse failed\n", argv[0]);
      exit(1);
    }
    freeimage(se);
  }
  else
  {
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    z = atoi(argv[4]);
  }

  writese(image, argv[1], x, y, z);
  freeimage(image);

  return 0;
} /* main */


