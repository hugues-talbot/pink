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
/*! \file skelfindelbows.c

\brief detection of "elbows" in a curvilinear skeleton

<B>Usage:</B> skelfindelbows in.skel len sharp out.pgm

<B>Description:</B>

Find "elbows" (points making sharp angles) in the curves of skeleton \b in.skel.
Matching points are written as voxels in the returned image \b out.pgm.

\verbatim
Let <C[0], ... C[n-1]> be the points of the curve C. 
Let j be an index between len and n-1-len, 
let i = j - len, let k = j + len.
If angle(C[j]-C[i], C[j]-C[k]) <= sharp then output C[j].
\endverbatim

Parameter \b len is given in pixels, parameter \b sharp in degrees.

<B>Types supported:</B> skel 2d, skel 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/

/*
%TEST skelfindelbows %IMAGES/2dskel/s2skel4.skel 7 110 %RESULTS/skelfindelbows_s2skel4.skel
%TEST skelfindelbows %IMAGES/2dskel/s2skel8.skel 7 110 %RESULTS/skelfindelbows_s2skel8.skel
%TEST skelfindelbows %IMAGES/3dskel/s3skel6.skel 7 110 %RESULTS/skelfindelbows_s3skel6.skel
%TEST skelfindelbows %IMAGES/3dskel/s3skel26.skel 7 110 %RESULTS/skelfindelbows_s3skel26.skel
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
  struct xvimage * res;
  skel * S;
  double length, angle;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.skel len sharp out.pgm\n", argv[0]);
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
  if (! (res = lskelfindelbows(S, length, angle)))
  {
    fprintf(stderr, "%s: function lskelfindelbows failed\n", argv[0]);
    exit(1);
  }

  writeimage(res, argv[argc-1]);
  termineskel(S);
  freeimage(res);

  return 0;
} /* main */
