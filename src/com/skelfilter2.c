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
/*! \file skelfilter2.c

\brief selection of an "aligned branch" in a curvilinear skeleton

<B>Usage:</B> skelfilter2 in.skel delta1 delta2 out.pgm

<B>Description:</B>

Selects a branch in the skeleton \b in.skel that has best aligned arcs.

\verbatim
  Let A0 be the arc in S having the greatest length
  Mark A0 (field "tag" = 1)
  Let E0, E1 be the vertices adjacent to A0
  For E in {E0, E1} do
    While E is a not yet marked junction do
      Mark E
      Let A1, ... Ak be the arcs other than A0 adjacent to E
      Let V0, ... Vk be the corresponding tangent vectors
      Let m be such that the angle <V0,-Vm> is minimal
      Let minangle be min{<Vi,-Vj> | 0 <= i < k and i < j <= k} 
      If <V0,-Vm> <= minangle then 
        E' = extremity of Am different from E
        A0 = Am; E = E'
        Mark A0
      EndIf
    EndWhile
    If E is a junction then Unmark E
  EndFor
\endverbatim

Parameters \b delta1 and \b delta2 are used to compute the tangent vectors:
the points taken into account for computing the vector for arc A have a 
distance to the nearest junction point that is between \b delta1 and \b delta2.

<B>Types supported:</B> skel 2d, skel 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/

/*
%TEST skelfilter2 %IMAGES/2dskel/s2skel4.skel 3 8 30 %RESULTS/skelfilter2_s2skel4.skel
%TEST skelfilter2 %IMAGES/2dskel/s2skel8.skel 3 8 30 %RESULTS/skelfilter2_s2skel8.skel
%TEST skelfilter2 %IMAGES/3dskel/s3skel6.skel 3 8 30 %RESULTS/skelfilter2_s3skel6.skel
%TEST skelfilter2 %IMAGES/3dskel/s3skel26.skel 3 8 30 %RESULTS/skelfilter2_s3skel26.skel
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
  double delta1, delta2;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.skel delta1 delta2 out.pgm\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }
  
  delta1 = atof(argv[2]);
  delta2 = atof(argv[3]);

  //  printskel(S);

  if (!lskelfilter2(S, delta1, delta2))
  {
    fprintf(stderr, "%s: function lskelfilter2 failed\n", argv[0]);
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
