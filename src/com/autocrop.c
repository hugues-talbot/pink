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
/*! \file autocrop.c

\brief eliminates null regions at the periphery of an image

<B>Usage:</B> autocrop in.pgm [threshold [out.pgm]]

<B>Description:</B>
Selects the minimum rectangle (cuboid in 3d) 
that contain all values of the image \b in.pgm strictly above the \b threshold (default 0).

If \b out.pgm is not specified, then out.pgm = in.pgm.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  double seuil;

  if ((argc != 2) && (argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [threshold [out.pgm]]\n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if (argc > 2) 
    seuil = atof(argv[2]);
  else 
    seuil = 0;

  out = lautocrop(in, seuil);
  if (out == NULL)
  {
    fprintf(stderr, "%s: lautocrop failed\n", argv[0]);
    exit(1);
  }

  if (argc > 3) 
    writeimage(out, argv[argc-1]);
  else
    writeimage(out, argv[1]);

  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
