/* $Id: fmm.c,v 1.1 2009-07-09 14:09:02 htalbot Exp $ */
/*! \file fmm.c

\brief  - Fast Marching Method

<B>Usage:</B> fmm speed.pgm seeds.pgm stop threshold seedout.pgm distanceout.pgm

<B>Description:</B>

The fast marching method of J.A. Sethian is a geodesic distance transform.
It integrates the constant arrival hyperbolic PDE. A rough physical analogy
is the following: Assume an anisotropic medium with varying propagation
celerities, and waves starting from various seeds travelling through that
medium. This function computes the successive arrival times of these
waves, as well as the propagation of the initial labels. The successive
arrival times are equivalent to a distance transform. The propagation
of the label yield a partition similar to a Voronoi.

If the speed function is constant and equal to 1, the arrival times would indeed
be the Euclidean distance function and the partition the Euclidean Voronoi.
Due to discretisation issue, the result is only approximately Euclidiean
(to second order).

<B>Stopping criteria:</B>
stop is the stopping criteria :

\li stop = 0 => no stop
\li stop = 1 => stop on metric (if speed function >= threshold)
\li stop = 2 => stop on distance (if distance > threshold)

the threshold is given after.

<B>Types supported:</B> integer, float Nd (N >= 2)
speed must be float, seeds must be integer.

<B>Category:</B> morpho
\ingroup morpho

\author Hugues Talbot & Ben Appleton
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include "pde_toolbox.h"
#include "lfmm.h"

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
    struct xvimage  *seeds, *speed ;
    struct xvimage  *voronoi, *distance;
    uint32_t  rs, cs, ss, dim[4], ndim;
    uint32_t *SeedIn, *SeedOut;
    float    *SpeedIn, *SpeedOut, threshold;
    int      error = 0, stop=0;

    if (argc != 7)
    {
        fprintf(stderr, "usage: %s seeds.pgm speed.pgm stop threshold seedout.pgm distanceout.pgm \n", argv[0]);
        exit(1);
    }

    seeds = readimage(argv[1]);  
    if (seeds == NULL)
    {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
    }
    speed = readimage(argv[2]);
    if (speed == NULL)
    {
        fprintf(stderr, "%s: readimage faild\n", argv[0]);
    }
    
    stop = atoi(argv[3]);
    threshold = (float)atof(argv[4]);
    
    /* ULONG is really only uint32_t */
    SeedIn = ULONGDATA(seeds);
    SpeedIn = FLOATDATA(speed);
    
    voronoi = copyimage(seeds);
    if (voronoi == NULL)
    {
        fprintf(stderr, "%s: copyimage failed\n", argv[0]);
        exit(1);
    }
    distance = copyimage(speed);
    if (distance == NULL)
    {
        fprintf(stderr, "%s: copyimage failed\n", argv[0]);
        exit(1);
    }
    
    SeedOut = ULONGDATA(voronoi);
    SpeedOut = FLOATDATA(distance);

    /* careful: rowsize is the size of a row => nx */
    rs = rowsize(seeds);
    cs = colsize(seeds);
    ss = depth(seeds);

#ifdef DEBUG 
    fprintf(stderr, "dimensions: [%d x %d x %d]\n", rs, cs, ss);
#endif
    
    ndim = 2;
    if (ss > 1)
        ++ndim;
    
    dim[0] = rs ; dim[1] = cs ; dim[2] = ss;
    
    if ((error = lfmmdist(SeedIn, SeedOut, dim, ndim, SpeedIn, stop, threshold, SpeedOut)) != 0)
    {
        fprintf(stderr, "%s: function lfmmdist failed with error code %d\n", argv[0], error);
        exit(1);
    }

    writeimage(voronoi, argv[5]);
    writeimage(distance, argv[6]);
        
    freeimage(seeds);
    freeimage(speed);
    freeimage(voronoi);
    freeimage(distance);

    return 0;
} /* main */


