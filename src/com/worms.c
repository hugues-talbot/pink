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
/* \file worms.c

\brief draw random non-intersecting spline segments

<B>Usage:</B> worms in.pgm nmax lenmin lenmax bendmin bendmax dmin out.pgm

<B>Types supported:</B> byte 3D

<B>Category:</B> draw geo experimental
\ingroup  draw geo experimental

\author Michel Couprie
*/

/*
%TEST worms %IMAGES/3dbyte/binary/b3empty_20_30_40.pgm 40 20 70 0.0 0.2 8 %RESULTS/worms_b3empty_20_30_40.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <mcprobas.h>
#include <mcgeo.h>
#include <ldist.h>
#include <ldraw.h>

#define VERBOSE

/* ==================================== */
static double scalarprod(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return (x1 * x2) + (y1 * y2) + (z1 * z2);
}

/* ==================================== */
static double norm(double x, double y, double z)
/* ==================================== */
{
  return sqrt((x * x) + (y * y) + (z * z));
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * img;
  struct xvimage * tmp;
  struct xvimage * dis;
  uint8_t *I, *T;
  uint32_t *D;
  int32_t i, j, rs, cs, ds, N, n, nmax, nt, ntmax;
  const int32_t npoints = 3;
  double x[npoints], y[npoints], z[npoints], t[npoints];
  double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
  double ux, uy, uz, vx, vy, vz;
  double Px[4], Py[4], Pz[4];
  double X0[2], X1[2], X2[2], X3[2];  
  double Y0[2], Y1[2], Y2[2], Y3[2];
  double Z0[2], Z1[2], Z2[2], Z3[2];
  double len, nor, scal;
  double lenmin, lenmax, bendmin, bendmax;
  int32_t dmin;

  if (argc != 11)
  {
    fprintf(stderr, "usage: %s rs cs ds nmax lenmin lenmax bendmin bendmax dmin out.pgm \n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);
  nmax = atoi(argv[4]);
  lenmin = atof(argv[5]);
  lenmax = atof(argv[6]);
  bendmin = atof(argv[7]);
  bendmax = atof(argv[8]);
  dmin = (int32_t)atof(argv[9]);
  ntmax = nmax * 20;
  N = rs * cs * ds;
  img = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(img != NULL);
  tmp = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(tmp != NULL);
  dis = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE); assert(dis != NULL);
  razimage(img);
  I = UCHARDATA(img);
  T = UCHARDATA(tmp);
  D = ULONGDATA(dis);

  n = 0; nt = 0;
  while ((n < nmax) && (nt < ntmax))
  {
    x1 = (double)Random(0, rs-1); y1 = (double)Random(0, cs-1); z1 = (double)Random(0, ds-1);
    x3 = (double)Random(0, rs-1); y3 = (double)Random(0, cs-1); z3 = (double)Random(0, ds-1);
    len = dist3(x1, y1, z1, x3, y3, z3);

    if ((len >= lenmin) && (len <= lenmax))
    {
      razimage(tmp);
      x2 = (x1 + x3) / 2; y2 = (y1 + y3) / 2; z2 = (z1 + z3) / 2;
      do 
      {
	x4 = (double)Random(0, rs-1); y4 = (double)Random(0, cs-1); z4 = (double)Random(0, ds-1);
      } while (dist3(x2, y2, z2, x4, y4, z4) < 1.0);

      ux = x3 - x2; uy = y3 - y2; uz = z3 - z2;
      vx = x4 - x2; vy = y4 - y2; vz = z4 - z2;
      // normalize u
      nor = norm(ux, uy, uz); assert(nor > 0.0);
      ux /= nor; uy /= nor; uz /= nor;
      // then v.u gives the magnitude of the orthogonal projection of v on u
      scal = scalarprod(vx, vy, vz, ux, uy, uz);
      // let u = proj(v,u)
      ux *= scal; uy *= scal; uz *= scal;
      // then v - u is a vector orthogonal to u in the plane u,v
      vx = vx - ux; vy = vy - uy; vz = vz - uz; 
      // normalize v
      nor = norm(vx, vy, vz); assert(nor > 0.0);
      vx /= nor; vy /= nor; vz /= nor;
      // multiply by the bending factor 
      scal = Uniform(bendmin, bendmax) * len;
      vx *= scal; vy *= scal; vz *= scal;
      // add to x2
      x2 += vx; y2 += vy; z2 += vz;

      x[0] = x1; y[0] = y1; z[0] = z1; t[0] = 0.0;
      x[1] = x2; y[1] = y2; z[1] = z2; t[1] = 1.0;
      x[2] = x3; y[2] = y3; z[2] = z3; t[2] = 2.0;

      scn_solvespline((double *)t, (double *)x, npoints, (double *)X0, (double *)X1, (double *)X2, (double *)X3);
      scn_solvespline((double *)t, (double *)y, npoints, (double *)Y0, (double *)Y1, (double *)Y2, (double *)Y3);
      scn_solvespline((double *)t, (double *)z, npoints, (double *)Z0, (double *)Z1, (double *)Z2, (double *)Z3);
    
      for (j = 0; j < npoints-1; j++)
      {
	Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
	Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];
	Pz[0] = Z0[j]; Pz[1] = Z1[j]; Pz[2] = Z2[j]; Pz[3] = Z3[j];
	ldrawcubic3d(tmp, (double *)Px, (double *)Py, (double *)Pz, 10, t[j], t[j+1]);
      }
    
      if (n > 1)
      {
	if (! ldisteuc3d(img, dis))
	{
	  fprintf(stderr, "%s: ldisteuc3d failed\n", argv[0]);
	  exit(1);
	}

	for (i = 0; i < N; i++) if (T[i] && (D[i] <= dmin)) break;
	if (i == N)
	{
	  n++;
	  for (i = 0; i < N; i++) if (T[i]) I[i] = n;
	}
      }
      else
      {
	n++;
	for (i = 0; i < N; i++) if (T[i]) I[i] = n;
      }
      nt++;
    } // if ((len >= dmin) && (len <= dmax))
  } // while ((n < nmax) && (nt < ntmax))
#ifdef VERBOSE
  printf("n = %d segments generated; %d trials\n", n, nt);
#endif

  writeimage(img, argv[argc-1]);
  freeimage(img);
  freeimage(tmp);
  freeimage(dis);

  return 0;
} /* main */


