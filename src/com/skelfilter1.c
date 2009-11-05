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

<B>Usage:</B> skelfilter1 in.skel length angle out.pgm

<B>Description:</B>
The skeleton found in \b in.skel is searched for "small" branches which 
satisfies the following criteria:
\li Branch length is less than \b length parameter.
\li Extremities A, B are both junctions.
\li Let A' be the symmetric of A wrt B, and B' be the symmetric of B wrt A.
    The distance from A' to the skeleton is less than AB cos( \b angle ), and
    the distance from B' to the skeleton is less than AB cos( \b angle ).

Only the matching branches are kept.

Parameter \b length is given in pixels, parameter \b angle in degrees.

<B>Types supported:</B> skel 2d, skel 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/

/*
%TEST skelfilter1 %IMAGES/2dskel/s2skel4.skel 5 15 %RESULTS/skelfilter1_s2skel4.skel
%TEST skelfilter1 %IMAGES/2dskel/s2skel8.skel 5 15 %RESULTS/skelfilter1_s2skel8.skel
%TEST skelfilter1 %IMAGES/3dskel/s3skel6.skel 5 15 %RESULTS/skelfilter1_s3skel6.skel
%TEST skelfilter1 %IMAGES/3dskel/s3skel26.skel 5 15 %RESULTS/skelfilter1_s3skel26.skel
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
  double length, angle;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.skel length angle out.skel\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }
  
  length = atof(argv[2]);
  angle = atof(argv[3]);

  //  printskel(S);

  angle = (angle * M_PI) / 180;
  if (!lskelfilter1(S, length, angle))
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
