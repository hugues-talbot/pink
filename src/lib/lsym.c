/* $Id: lsym.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/*
   Symetrie verticale, horizontale ou centrale

   Michel Couprie  -  Mai 1998 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <lsym.h>

/* ==================================== */
int32_t lsym(struct xvimage * image1, char mode)
/* ==================================== */
{
  int32_t i, j, k, ip, jp, kp;
  uint8_t *pt1;
  uint8_t tmp;
  int32_t rs, cs, ps, ds, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  ps = rs * cs;
  N = ps * ds;
  pt1 = UCHARDATA(image1);
  
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  if (mode != 'z')
  {
    for (k = 0; k < ds; k++)
    {  
      switch (mode)
      {
        case 'v': 
        case 'y': 
          for (j = 0, jp = cs - 1; j < jp; j++, jp--)
            for (i = 0; i < rs; i++)
            {
              tmp = pt1[k * ps + j * rs + i];
              pt1[k * ps + j * rs + i] = pt1[k * ps + jp * rs + i];
              pt1[k * ps + jp * rs + i] = tmp;
            }
          break;
        case 'h': 
        case 'x': 
          for (j = 0; j < cs; j++)
            for (i = 0, ip = rs - 1; i < ip; i++, ip--)
            {
              tmp = pt1[k * ps + j * rs + i];
              pt1[k * ps + j * rs + i] = pt1[k * ps + j * rs + ip];
              pt1[k * ps + j * rs + ip] = tmp;
            }
          break;
        case 'c': 
          for (j = 0, jp = cs - 1; j < jp; j++, jp--)
            for (i = 0; i < rs; i++)
            {
              tmp = pt1[k * ps + j * rs + i];
              pt1[k * ps + j * rs + i] = pt1[k * ps + jp * rs + i];
              pt1[k * ps + jp * rs + i] = tmp;
            }
          for (j = 0; j < cs; j++)
            for (i = 0, ip = rs - 1; i < ip; i++, ip--)
            {
              tmp = pt1[k * ps + j * rs + i];
              pt1[k * ps + j * rs + i] = pt1[k * ps + j * rs + ip];
              pt1[k * ps + j * rs + ip] = tmp;
            }
          break;
      } /* switch (mode) */
    } /* for (k = 0; k < d; k++) */
  }
  else
  {
    for (k = 0, kp = ds - 1; k < kp; k++, kp--)
      for (j = 0; j < cs; j++)
        for (i = 0; i < rs; i++)
        {
          tmp = pt1[k * ps + j * rs + i];
          pt1[k * ps + j * rs + i] = pt1[kp * ps + j * rs + i];
          pt1[kp * ps + j * rs + i] = tmp;
        }

  }
  return 1;
}





