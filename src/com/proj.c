/* $Id: proj.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file proj.c

\brief projection parallel to one of the main axes

<B>Usage:</B> proj in.pgm dir mode out.pgm

<B>Description:</B>

Projection parallel to one of the main axes.

<B>dir</B> = x | y | z

<B>mode</B> = 0 (mean) | 1 (max)

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - avril 2001 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * result;
  struct xvimage * image;
  int32_t i, j, k;
  uint8_t *R;
  uint8_t *I;
  int32_t rs, cs, ds, ps, N;
  char dir;
  int32_t mode;
  double tmp;
  uint8_t t;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm dir mode out.pgm \n", argv[0]);
    fprintf(stderr, "       dir = x|y|z ; mode = 0 (mean) | 1 (max)\n");
    exit(1);
  }

  image = readimage(argv[1]);

  rs = rowsize(image);   /* taille ligne */
  cs = colsize(image);   /* taille colonne */
  ds = depth(image);     /* nb plans */
  ps = rs * cs;          /* taille plan */
  N = ps * ds;           /* taille image */
  I = UCHARDATA(image);

  dir = argv[2][0];
  mode = atoi(argv[3]);

  switch(dir)
  {
    case 'x':
      result = allocimage(NULL, cs, ds, 1, VFF_TYP_1_BYTE);
      if (result == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", argv[0]);
        exit(1);
      }
      R = UCHARDATA(result);

      for (k = 0; k < ds; k++)
      for (j = 0; j < cs; j++)
      {
        tmp = 0.0;
        if (mode == 0)
	{
          for (i = 0; i < rs; i++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for i */
          R[k*cs + j] = (uint8_t)arrondi((tmp/cs));
	}
        else
	{
          t = 0;
          for (i = 0; i < rs; i++)
          {
            t = max(t,I[k*ps+j*rs+i]);
          } /* for i */
          R[k*cs + j] = t;
	}
      } /* for k, j */
      break;

    case 'y':
      result = allocimage(NULL, rs, ds, 1, VFF_TYP_1_BYTE);
      if (result == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", argv[0]);
        exit(1);
      }
      R = UCHARDATA(result);

      for (k = 0; k < ds; k++)
      for (i = 0; i < rs; i++)
      {
        tmp = 0.0;
        if (mode == 0)
	{
          for (j = 0; j < cs; j++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for j */
          R[k*rs + i] = (uint8_t)arrondi((tmp/cs));
	}
        else
	{
          t = 0;
          for (j = 0; j < cs; j++)
          {
            t = max(t,I[k*ps+j*rs+i]);
          } /* for j */
          R[k*rs + i] = t;
	}
      } /* for k, i */
      break;

    case 'z':
      result = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
      if (result == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", argv[0]);
        exit(1);
      }
      R = UCHARDATA(result);

      for (j = 0; j < cs; j++)
      for (i = 0; i < rs; i++)
      {
        tmp = 0.0;
        if (mode == 0)
	{
          for (k = 0; k < ds; k++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for k */
          R[j*rs + i] = (uint8_t)arrondi((tmp/cs));
	}
        else
	{
          t = 0;
          for (k = 0; k < ds; k++)
          {
            t = max(t,I[k*ps+j*rs+i]);
          } /* for k */
          R[j*rs + i] = t;
	}
      } /* for j, i */
      break;
    
    default:
      fprintf(stderr, "dir = x|y|z \n");
      exit(1);

  } /* switch(dir) */

  writeimage(result, argv[argc - 1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */

