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
/*! \file skelfilter1.c

\brief selection of "junction branches" in a curvilinear skeleton

<B>Usage:</B> skelfilter1 in.skel length delta1 delta2 out.pgm

<B>Description:</B>

The skeleton \b in.skel is searched for arcs A which 
satisfy the following criteria:
\li The length of A is not greater than parameter \b length.
\li Both extremities of A are junctions.
\li For at least one extremity E of A, 
    the arc A is the worst-aligned arc adjacent to E.

Parameters \b delta1 and \b delta2 are used to compute the tangent vectors:
the points taken into account for computing the vector for arc A have a 
distance to the nearest junction point that is between \b delta1 and \b delta2.

Matching arcs are written in \b out.pgm.

<B>Types supported:</B> skel 2d, skel 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/

/*
%TEST skelfilter1 %IMAGES/2dskel/s2skel4.skel 35 2 12 %RESULTS/skelfilter1_s2skel4.skel
%TEST skelfilter1 %IMAGES/2dskel/s2skel8.skel 35 2 12 %RESULTS/skelfilter1_s2skel8.skel
%TEST skelfilter1 %IMAGES/3dskel/s3skel6.skel 35 2 12 %RESULTS/skelfilter1_s3skel6.skel
%TEST skelfilter1 %IMAGES/3dskel/s3skel26.skel 35 2 12 %RESULTS/skelfilter1_s3skel26.skel
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  skel * S;
  double length, delta1, delta2;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.skel length delta1 delta2 out.pgm\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }
  
  length = atof(argv[2]);
  delta1 = atof(argv[3]);
  delta2 = atof(argv[4]);

  //  printskel(S);

  if (!lskelfilter1(S, length, delta1, delta2))
  {
    fprintf(stderr, "%s: function lskelfilter1 failed\n", argv[0]);
    exit(1);
  }

  if (! (image = lskelmarked2image(S)))
  {
    fprintf(stderr, "%s: function lskelmarked2image failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  termineskel(S);
  freeimage(image);

  return 0;
} /* main */
