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
/*! \file skelfilter3.c

\brief detection of "rectilinear branches" in a curvilinear skeleton

<B>Usage:</B> skelfilter3 in.skel delta1 delta2 maxbridgelength maxelbowangle out.pgm

<B>Description:</B>

A ECRIRE

Parameters \b delta1 and \b delta2 are used to compute the tangent vectors:
the points taken into account for computing the vector for arc A have a 
distance to the nearest junction point that is between \b delta1 and \b delta2.

Parameter \b maxelbowangle is given in degrees.

<B>Types supported:</B> skel 2d, skel 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/

/*
%TEST skelfilter3 %IMAGES/2dskel/s2skel4.skel 3 8 30 120 %RESULTS/skelfilter3_s2skel4.skel
%TEST skelfilter3 %IMAGES/2dskel/s2skel8.skel 3 8 30 120 %RESULTS/skelfilter3_s2skel8.skel
%TEST skelfilter3 %IMAGES/3dskel/s3skel6.skel 3 8 30 120 %RESULTS/skelfilter3_s3skel6.skel
%TEST skelfilter3 %IMAGES/3dskel/s3skel26.skel 3 8 30 120 %RESULTS/skelfilter3_s3skel26.skel
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
  struct xvimage * result;
  skel * S;
  double delta1, delta2, maxbridgelength, maxelbowangle;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.skel delta1 delta2 maxbridgelength maxelbowangle out.pgm\n", argv[0]);
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
  maxbridgelength = atof(argv[4]);
  maxelbowangle = atof(argv[5]);
  maxelbowangle = (maxelbowangle * M_PI) / 180;

  //  printskel(S);

  result = lskelfilter3(S, delta1, delta2, maxbridgelength, maxelbowangle);
  if (result == NULL)
  {
    fprintf(stderr, "%s: function lskelfilter3 failed\n", argv[0]);
    exit(1);
  }

  writeimage(result, argv[argc-1]);
  termineskel(S);
  freeimage(result);

  return 0;
} /* main */
