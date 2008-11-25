/* $Id: lisometry.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/*
   Rotations d'angles multiples de PI/2 autour des axes x, y, z

   Available modes: xzy, yxz, yzx, zxy, zyx, (3D), and
   0: identity,
   1: rotation 90 degrees clockwise,
   2: rotation 180 degrees clockwise,
   3: rotation 270 degrees clockwise,
   4: symmetry / vertical axis,             NYI
   5: symmetry / horizontal axis.           NYI

   Michel Couprie  -  Novembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lisometry.h>

/* ==================================== */
int32_t lisometry(struct xvimage * image1, char *mode, struct xvimage **im2)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "lisometry"
  struct xvimage *image2;
  uint8_t *I1, *I2;
  int32_t rs, cs, d, ps, nps;
  int32_t i, j, k;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  ps = rs * cs;
  I1 = UCHARDATA(image1);

  if (datatype(image1) != VFF_TYP_1_BYTE) 
  {   
    fprintf(stderr,"%s: bad image type\n", F_NAME);
    return 0;
  }

  if (strcmp(mode, "0") == 0)
  {
    image2 = copyimage(image1);
  }
  else
  if (strcmp(mode, "1") == 0) // rotation 90 clockwise
  {
    if (d != 1)
    {   
      fprintf(stderr,"%s: mode 1 is only for 2D images\n", F_NAME);
      return 0;
    }
    image2 = allocimage(NULL, cs, rs, d, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    I2 = UCHARDATA(image2);
    for (j = 0; j < rs; j++)
      for (i = 0; i < cs; i++)
	I2[j * cs + i] = I1[i * rs + rs-j-1]; 
  }
  else
  if (strcmp(mode, "2") == 0)  // rotation 180 clockwise
  {
    if (d != 1)
    {   
      fprintf(stderr,"%s: mode 1 is only for 2D images\n", F_NAME);
      return 0;
    }
    image2 = allocimage(NULL, rs, cs, d, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    I2 = UCHARDATA(image2);
    for (j = 0; j < cs; j++)
      for (i = 0; i < rs; i++)
	I2[j * rs + i] = I1[(cs-j-1) * rs + rs-i-1]; 
  }
  else
  if (strcmp(mode, "3") == 0)  // rotation 270 clockwise
  {
    if (d != 1)
    {   
      fprintf(stderr,"%s: mode 1 is only for 2D images\n", F_NAME);
      return 0;
    }
    image2 = allocimage(NULL, cs, rs, d, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    I2 = UCHARDATA(image2);
    for (j = 0; j < rs; j++)
      for (i = 0; i < cs; i++)
	I2[j * cs + i] = I1[(cs-i-1) * rs + j]; 
  }
  else
  if (strcmp(mode, "xzy") == 0)
  {
    image2 = allocimage(NULL, rs, d, cs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = rs * d;
    I2 = UCHARDATA(image2);
    for (k = 0; k < cs; k++)
      for (j = 0; j < d; j++)
        for (i = 0; i < rs; i++)
          I2[k * nps + j * rs + i] = I1[j * ps + k * rs + i]; 
  }
  else
  if (strcmp(mode, "yxz") == 0)
  {
    image2 = allocimage(NULL, cs, rs, d, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = rs * cs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < d; k++)
      for (j = 0; j < rs; j++)
        for (i = 0; i < cs; i++)
          I2[k * nps + j * cs + i] = I1[k * ps + i * rs + j]; 
  }
  else
  if (strcmp(mode, "yzx") == 0)
  {
    image2 = allocimage(NULL, cs, d, rs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = d * cs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < rs; k++)
      for (j = 0; j < d; j++)
        for (i = 0; i < cs; i++)
          I2[k * nps + j * cs + i] = I1[j * ps + i * rs + k]; 
  }
  else
  if (strcmp(mode, "zxy") == 0)
  {
    image2 = allocimage(NULL, d, rs, cs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = d * rs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < cs; k++)
      for (j = 0; j < rs; j++)
        for (i = 0; i < d; i++)
          I2[k * nps + j * d + i] = I1[i * ps + k * rs + j]; 
  }
  else
  if (strcmp(mode, "zyx") == 0)
  {
    image2 = allocimage(NULL, d, cs, rs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = d * cs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < rs; k++)
      for (j = 0; j < cs; j++)
        for (i = 0; i < d; i++)
          I2[k * nps + j * d + i] = I1[i * ps + j * rs + k]; 
  }
  else
  {   
    fprintf(stderr,"%s: bad mode : %s\n", F_NAME, mode);
    fprintf(stderr,"available modes: xzy, yxz, yzx, zxy, zyx, 0, 1, 2, 3\n");
    return 0;
  }
  *im2 = image2;
  return 1;
}





