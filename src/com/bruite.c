/* $Id: bruite.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file bruite.c

\brief adds noise to an image

<B>Usage:</B> bruite in.pgm n alpha p out.pgm

<B>Description:</B>
Adds a uniform or gaussian noise, of mean 0 and standard deviation <B>alpha</B>, 
to a proportion <B>p</B> of the pixels of the image in.pgm .

  OPTIONS
    <B>n</B> = 0  uniform noise
    <B>n</B> = 1  gaussian noise

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/

/* Michel Couprie - mai 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcprobas.h>

//#define RANDOMIZE
#ifdef RANDOMIZE
#include <mcchrono.h>
#endif

inline double bruite(double f, int32_t type, double alpha, double p) 
{
  double tmp;

  switch(type)
  {
    case 0:
      tmp = Prob();
      if (tmp <= p) {
	tmp = ((double)f) + Uniform(-2*alpha,2*alpha);
	//if (tmp>255) printf("tmp = %lg\n", tmp);
      }
      else 
	tmp = f;
      break;
    case 1:
      tmp = Prob();
      if (tmp <= p)
	tmp = ((double)f) + Normal(0.0, alpha);
      else 
	tmp = f;
      break;
    default:
      exit(1);
  }
  return tmp;
}

void usage(int argc, char **argv)
{
    fprintf(stderr, "usage: %s in.pgm n alpha p out.pgm\n", argv[0]);
    fprintf(stderr, "       n = 0 (uniforme) | 1 (gaussien) \n");
    fprintf(stderr, "       alpha = ecart-type ; p = proportion de pixels a changer (0 < p <= 1) \n");
    fprintf(stderr, "Note that for ULONG image, the pixel value is limited to %d\n", UINT_MAX);
    exit(1);
}

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i,j;
  uint8_t *ImUC;
  uint32_t *ImUL;
  
  int32_t rs, cs, d, N;
  int32_t n;
  double alpha, tmp, p;

  if (argc != 6)
    usage(argc, argv);

#ifdef RANDOMIZE
  ResetProb(usecs());
#endif

  n = atoi(argv[2]);
  if ((n!=0) && (n!=1))
    usage(argc, argv);
  
  alpha = atof(argv[3]);
  p = atof(argv[4]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;

  switch(datatype(image)) {
  case VFF_TYP_1_BYTE:
    ImUC = UCHARDATA(image);
    for (i = 0; i < N; i++) {
      tmp = bruite((double)(ImUC[i]), n, alpha, p);
      ImUC[i] = (uint8_t)max(NDG_MIN,min(NDG_MAX,tmp));
    }
    break;
  case VFF_TYP_4_BYTE:
    ImUL = ULONGDATA(image);
    for (i = 0; i < N; i++) {
      tmp = bruite((double)(ImUL[i]), n, alpha, p);
      ImUL[i] = (uint32_t)max(NDG_MIN,min(UINT_MAX,tmp));
    }
    break;
  default:
    fprintf(stderr, "Type of image not implemented\n");
    exit(1);
  }
  image->name = NULL;
  writeimage(image, argv[5]);
  freeimage(image);

  return 0;
} /* main */

