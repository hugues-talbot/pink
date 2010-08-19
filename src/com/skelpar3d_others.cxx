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
/*! \file skelpar3d_others.cxx

\brief parallel 3D binary skeleton
DirectionalSkeletonizer
<B>Usage:</B> skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel 3D binary thinning or skeleton. The parameter \b nsteps gives,
if positive, the number of parallel thinning steps to be processed.
If the value given for \b nsteps equals -1, the thinning is continued
until stability.

The parameter \b algorithm is a numerical code
indicating which method will be used for the thinning.
The possible choices are:
\li 0: Palagyi (curvilinear, directional, 1998 - 6-subiterations directional curve-thinning)
\li 1: Palagyi (curvilinear, sequential, 2006)
\li 2: Palagyi (surfacic, parallel directional, 2002)
\li 3: Palagyi (surfacic, fully parallel, 2008)

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged.

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/

// I'm confused, this should be a C++ file...
// Hugues Talbot	18 Aug 2010

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskelpar3d_others.h>
#include <lmedialaxis.h>
#include <DirectionalSkeletonizer.hpp>
#include <FIFO.hpp>

/* =============================================================== */
int main(int32_t argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t mode, nsteps;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm algorithm nsteps [inhibit] out.pgm\n", argv[0]);
    fprintf(stderr, "   0: Palagyi (curvilinear, parallel directional, 1998)\n");
    fprintf(stderr, "   1: Palagyi (curvilinear, sequential, 2006)\n");
    fprintf(stderr, "   2: Palagyi (surfacic, parallel directional, 2002)\n");
    fprintf(stderr, "   3: Palagyi (surfacic, fully parallel, 2008)\n");
    fprintf(stderr, "   4: Raynal (curvilinear, directional, 2010)\n");
    fprintf(stderr, "   5: Raynal (surfacic, directional, 2010)\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  DirectionalSkeletonizer dskel1(image->row_size, image->col_size, image->depth_size);

  mode = atoi(argv[2]);
  nsteps = atoi(argv[3]);

  if (argc == 6)
  {
    inhibit = readimage(argv[4]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (depth(image) != 1)
  {
    switch (mode)
    {
    case 0:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      if (palagyi_skelpar_curv_98(image)!=0)
      {
	fprintf(stderr, "%s: palagyi_skelpar_curv_98 failed\n", argv[0]);
	exit(1);
      } break;
	case 1:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      if (palagyi_skelpar_curv_06(image)!=0)
      {
	fprintf(stderr, "%s: palagyi_skelpar_curv_06 failed\n", argv[0]);
	exit(1);
      } break;
	case 2:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      if (palagyi_skelpar_surf_02(image)!=0)
      {
	fprintf(stderr, "%s: palagyi_skelpar_surf_02 failed\n", argv[0]);
	exit(1);
      } break;
	case 3:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      if (palagyi_skelpar_surf_08(image)!=0)
      {
	fprintf(stderr, "%s: palagyi_skelpar_surf_08 failed\n", argv[0]);
	exit(1);
      } break;
     case 4:
      if (nsteps==-1) nsteps=0;
      if(inhibit!=NULL)
      dskel1.skeletonize((unsigned char*)(image->image_data), true, nsteps, (unsigned char*)(inhibit->image_data));
      else
      dskel1.skeletonize((unsigned char*)(image->image_data), true, nsteps);
      break;
     case 5:
      if (nsteps==-1) nsteps=0;
      if(inhibit!=NULL)
      dskel1.skeletonize((unsigned char*)(image->image_data), false, nsteps, (unsigned char*)(inhibit->image_data));
      else
      dskel1.skeletonize((unsigned char*)(image->image_data), false, nsteps);
      break;
    default:
      fprintf(stderr, "%s: mode %d not implemented\n", argv[0], mode);
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "%s: image must be 3D\n", argv[0]);
    exit(1);
  }

  if(inhibit!=NULL) freeimage(inhibit);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */








