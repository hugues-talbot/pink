/* $Id: reformat.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file reformat.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mccodimage.h>
#include <mcimage.h>

/* ==================================== */
struct xvimage * readimage_old(char *filename)
/* ==================================== */
#undef F_NAME
#define F_NAME "readimage_old"
{
#define BUFFERSIZE 10000
  char buffer[BUFFERSIZE];
  FILE *fd = NULL;
  int32_t rs, cs, d, ndgmax, N, i;
  struct xvimage * image;
  int32_t ascii;  
  int32_t typepixel;
  int32_t c;
  double xdim=1.0, ydim=1.0, zdim=1.0;

#ifdef UNIXIO
  fd = fopen(filename,"r");
#endif
#ifdef DOSIO
  fd = fopen(filename,"rb");
#endif
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", F_NAME, filename);
    return NULL;
  }

  fgets(buffer, BUFFERSIZE, fd); /* P5: raw byte bw  ; P2: ascii bw */
                                 /* P6: raw byte rgb ; P3: ascii rgb */
                                 /* P7: raw byte 3d  ; P4: ascii 3d  ==  extensions MC */
                                 /* P8: raw int32_t 2d-3d  ==  extension MC */
                                 /* P9: raw float 2d-3d  ==  extension MC */
                                 /* PA: ascii float 2d-3d  ==  extension LN */
                                 /* PB: ascii int32_t 2d-3d  ==  extension MC */

  if (buffer[0] != 'P')
  {   fprintf(stderr,"%s : invalid image format\n", F_NAME);
      return NULL;
  }
  switch (buffer[1])
  {
    case '2':
    case '4': ascii = 1; typepixel = VFF_TYP_1_BYTE; break;
    case '5':
    case '7': ascii = 0; typepixel = VFF_TYP_1_BYTE; break;
    case '8': ascii = 0; typepixel = VFF_TYP_4_BYTE; break;
    case '9': ascii = 0; typepixel = VFF_TYP_FLOAT; break;
    case 'A': ascii = 1; typepixel = VFF_TYP_FLOAT; break;
    case 'B': ascii = 1; typepixel = VFF_TYP_4_BYTE; break;
    default:
      fprintf(stderr,"%s : invalid image format\n", F_NAME);
      return NULL;
  } /* switch */

  do 
  {
    fgets(buffer, BUFFERSIZE, fd); /* commentaire */
    if (strncmp(buffer, "#xdim", 5) == 0)
      sscanf(buffer+5, "%lf", &xdim);
    else if (strncmp(buffer, "#ydim", 5) == 0)
      sscanf(buffer+5, "%lf", &ydim);
    else if (strncmp(buffer, "#zdim", 5) == 0)
      sscanf(buffer+5, "%lf", &zdim);
  } while (!isdigit(buffer[0]));

  c = sscanf(buffer, "%d %d %d", &rs, &cs, &d);
  if (c == 2) d = 1;
  else if (c != 3)
  {   fprintf(stderr,"%s : invalid image format\n", F_NAME);
      return NULL;
  }

  fgets(buffer, BUFFERSIZE, fd);

  sscanf(buffer, "%d", &ndgmax);
  N = rs * cs * d;

  image = allocimage(NULL, rs, cs, d, typepixel);
  if (image == NULL)
  {   fprintf(stderr,"%s : alloc failed\n", F_NAME);
      return(NULL);
  }
  image->xdim = xdim;
  image->ydim = ydim;
  image->zdim = zdim;

  if (typepixel == VFF_TYP_1_BYTE)
  {
    if (ascii)
    {
      if (ndgmax == 255)
        for (i = 0; i < N; i++)
        {
          fscanf(fd, "%d", &c);
          UCHARDATA(image)[i] = (uint8_t)c;
        } /* for i */
      else if (ndgmax == 65535)
        for (i = 0; i < N; i++)
        {
          fscanf(fd, "%d", &c);
          UCHARDATA(image)[i] = (uint8_t)(c/256);
        } /* for i */
      else
      {
        fprintf(stderr,"%s : wrong ndgmax = %d\n", F_NAME, ndgmax);
        return(NULL);
      }
    }
    else
    {
      int32_t ret = fread(UCHARDATA(image), sizeof(char), N, fd);
      if (ret != N)
      {
        fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, N, ret);
        return(NULL);
      }
    }
  } /* if (typepixel == VFF_TYP_1_BYTE) */
  else
  if (typepixel == VFF_TYP_4_BYTE)
  {
    if (ascii)
    {
      for (i = 0; i < N; i++)
      {
        fscanf(fd, "%ld", &(ULONGDATA(image)[i]));
      } /* for i */
    }
    else 
    {
      int32_t ret = fread(ULONGDATA(image), sizeof(int32_t), N, fd);
      if (ret != N)
      {
        fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, N, ret);
        return(NULL);
      }
    }
  } /* if (typepixel == VFF_TYP_4_BYTE) */
  else
  if (typepixel == VFF_TYP_FLOAT)
  {
    if (ascii)
    {
      for (i = 0; i < N; i++)
      {
        fscanf(fd, "%f", &(FLOATDATA(image)[i]));
      } /* for i */
    }
    else 
    {
      int32_t ret = fread(FLOATDATA(image), sizeof(float), N, fd);
      if (ret != N)
      {
        fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, N, ret);
        return(NULL);
      }
    }
  } /* if (typepixel == VFF_TYP_FLOAT) */

  fclose(fd);

  return image;
} /* readimage_old() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s file.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage_old(argv[1]);
  if (image == NULL)
  {   fprintf(stderr,"reformat : readimage failed\n");
      exit(1);
  }

  writeimage(image, argv[1]);
  freeimage(image);

  return 0;
} /* main */
