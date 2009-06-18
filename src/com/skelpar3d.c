/* $Id: skelpar3d.c,v 1.3 2009-06-18 06:34:55 mcouprie Exp $ */
/*! \file skelpar3d.c

\brief parallel 3D binary skeleton

<B>Usage:</B> skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel 3D binary thinning or skeleton. The parameter \b nsteps gives,
if positive, the number of parallel thinning steps to be processed.
If the value given for \b nsteps equals -1, the thinning is continued
until stability.

The parameter \b algorithm is a numerical code
indicating which method will be used for the thinning.
The possible choices are:
\li 0: ultimate, without constraint (MK3a)
\li 1: curvilinear, based on 1D isthmus (CK3a)
\li 2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered
\li 3: ultimate (MK3) - if nsteps = -2, returns the topological distance
\li 4: curvilinear based on ends (EK3)
\li 5: curvilinear based on ends, with end reconstruction (CK3b)
\li 6: topological axis (not homotopic)
\li 7: curvilinear, based on residual points and 2D isthmus (CK3)
\li 8: ultimate, asymetric (AMK3)
\li 9: curvilinear, asymetric, based on thin 1D isthmus (ACK3a)
\li 10: curvilinear, asymetric, based on thin 1D isthmus - variant (ACK3)
\li 11: surfacic, based on residual points (RK3)
\li 12: surfacic, based on 2D isthmuses (SK3)

In modes other than 2, if the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskelpar3d.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t mode, ret, nsteps;
  int32_t filter = 0;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm algorithm nsteps [inhibit] out.pgm\n", argv[0]);
    fprintf(stderr, "   0: ultimate, without constraint (MK3a)\n");
    fprintf(stderr, "   1: curvilinear, based on 1D isthmus (CK3a)\n");
    fprintf(stderr, "   2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered\n");
    fprintf(stderr, "   3: ultimate (MK3) - if nsteps = -2, returns the topological distance\n");
    fprintf(stderr, "   4: curvilinear based on ends (EK3)\n");
    fprintf(stderr, "   5: curvilinear based on ends, with end reconstruction (CK3b)\n");
    fprintf(stderr, "   6: topological axis (not homotopic)\n");
    fprintf(stderr, "   7: curvilinear, based on residual points and 2D isthmus (CK3)\n");
    fprintf(stderr, "   8: ultimate, asymetric (AMK3)\n");
    fprintf(stderr, "   9: curvilinear, asymetric, based on thin 1D isthmus (ACK3a)\n");
    fprintf(stderr, "  10: curvilinear, asymetric, based on thin 1D isthmus - variant (ACK3)\n");
    fprintf(stderr, "  11: surfacic, based on residual points (RK3)\n");
    fprintf(stderr, "  12: surfacic, based on 2D isthmuses (SK3)\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  nsteps = atoi(argv[3]);

  if ((argc == 6) && (mode == 2))
    filter = atoi(argv[4]);
  else if (argc == 6)
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
      if (! lskelMK3a(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelMK3a failed\n", argv[0]);
	exit(1);
      } break;
    case 1:
      if (! lskelCK3a(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3a failed\n", argv[0]);
	exit(1);
      } break;
    case 2:
      if (! lskelAK3(image, nsteps, inhibit, filter))
      {
	fprintf(stderr, "%s: lskelAK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 3:
      if (nsteps == -2)
      {
	struct xvimage *disttopo = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
	if (disttopo == NULL)
	{   
	  fprintf(stderr, "%s(): allocimage failed\n", argv[0]);
	  return 0;
	}
	if (! ldisttopo3(image, inhibit, disttopo))
        {
	  fprintf(stderr, "%s: ldistaxetopo3 failed\n", argv[0]);
	  exit(1);
	} 
	writeimage(disttopo, argv[argc-1]);
	freeimage(image);
	freeimage(disttopo);
	return 0;
      }
      else
      {
        if (! lskelMK3(image, nsteps, inhibit))
        {
	  fprintf(stderr, "%s: lskelMK3c failed\n", argv[0]);
	  exit(1);
        } break;
      }
    case 4:
      if (! lskelEK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelEK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 5:
      if (! lskelCK3b(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3b failed\n", argv[0]);
	exit(1);
      } break;
    case 6:
      {
	struct xvimage *disttopo = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
	if (disttopo == NULL)
	{   
	  fprintf(stderr, "%s(): allocimage failed\n", argv[0]);
	  return 0;
	}
	if (! ldistaxetopo3(image, inhibit, disttopo))
        {
	  fprintf(stderr, "%s: ldistaxetopo3 failed\n", argv[0]);
	  exit(1);
	} 
	freeimage(disttopo);
      } break;
    case 7:
      if (! lskelCK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 8:
      if (! lskelAMK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelAMK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 9:
      if (! lskelACK3a(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelACK3a failed\n", argv[0]);
	exit(1);
      } break;
    case 10:
      if (! lskelACK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelACK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 11:
      if (! lskelRK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelRK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 12:
      if (! lskelSK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelSK3 failed\n", argv[0]);
	exit(1);
      } break;
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

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
