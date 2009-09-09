/* $Id: ltopotypes.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* calcul des types topologiques des points d' une image */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mctopo3d.h>

/* ==================================== */
int32_t ltopotypes(
  struct xvimage * image,
  struct xvimage * types, 
  int32_t connex,
  int32_t bin)
/* ==================================== */
#undef F_NAME
#define F_NAME "ltopotypes"
{
  int32_t i;
  uint8_t *pti = UCHARDATA(image);
  uint8_t *ptt = UCHARDATA(types);
  int32_t rs = image->row_size;
  int32_t N = rs * image->col_size;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (bin)
  {
    if (connex == 4)
    {
      for (i = 0; i < N; i++)
      {
        ptt[i] = (uint8_t)typetopobin(pti, i, rs, N);
      }
    }
    else
    {
      for (i = 0; i < N; i++)
      {
        ptt[i] = (uint8_t)typetopobin8(pti, i, rs, N);
      }
    }
  }
  else
  {
    if (connex == 4)
    {
      for (i = 0; i < N; i++)
      {
        ptt[i] = (uint8_t)typetopo(pti, i, rs, N);
      }
    }
    else
    {
      for (i = 0; i < N; i++)
      {
        ptt[i] = (uint8_t)typetopo8(pti, i, rs, N);
      }
    }
  }
  return 1;
} /* ltopotypes() */

/* ==================================== */
int32_t lt4pp(struct xvimage * image)
/* ==================================== */
#undef F_NAME
#define F_NAME "lt4pp"
{
  int32_t i, j;
  struct xvimage * temp;
  uint8_t *pti;
  uint8_t *ptt;
  uint32_t *pti_l;
  uint32_t *ptt_l;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t N = rs * cs;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  temp = copyimage(image);
  if (temp == NULL) 
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    pti = UCHARDATA(image);
    ptt = UCHARDATA(temp);
    memset(pti, 0, N);
    for (j = 1; j < cs - 1; j++) 
      for (i = 1; i < rs - 1; i++) 
        pti[j * rs + i] = (uint8_t)t4pp(ptt, j * rs + i, rs, N);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    pti_l = ULONGDATA(image);
    ptt_l = ULONGDATA(temp);
    memset(pti_l, 0, 4*N);
    for (j = 1; j < cs - 1; j++) 
      for (i = 1; i < rs - 1; i++) 
        pti_l[j * rs + i] = (uint32_t)t4pp_l(ptt_l, j * rs + i, rs, N);
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", F_NAME, datatype(image));
    return(0);
  }

  freeimage(temp);
  return 1;
} /* lt4pp() */

/* ==================================== */
int32_t lt8pp(struct xvimage * image)
/* ==================================== */
#undef F_NAME
#define F_NAME "lt8pp"
{
  int32_t i, j;
  struct xvimage * temp;
  uint8_t *pti;
  uint8_t *ptt;
  uint32_t *pti_l;
  uint32_t *ptt_l;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t N = rs * cs;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  temp = copyimage(image);
  if (temp == NULL) 
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    pti = UCHARDATA(image);
    ptt = UCHARDATA(temp);
    memset(pti, 0, N);
    for (j = 1; j < cs - 1; j++) 
      for (i = 1; i < rs - 1; i++) 
        pti[j * rs + i] = (uint8_t)t8pp(ptt, j * rs + i, rs, N);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    pti_l = ULONGDATA(image);
    ptt_l = ULONGDATA(temp);
    memset(pti_l, 0, 4*N);
    for (j = 1; j < cs - 1; j++) 
      for (i = 1; i < rs - 1; i++) 
        pti_l[j * rs + i] = (uint32_t)t8pp_l(ptt_l, j * rs + i, rs, N);
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", F_NAME, datatype(image));
    return(0);
  }

  freeimage(temp);
  return 1;
} /* lt8pp() */

/* ==================================== */
int32_t lt6pp(struct xvimage * image)
/* ==================================== */
#undef F_NAME
#define F_NAME "lt6pp"
{
  int32_t i, j, k;
  struct xvimage * temp;
  uint8_t *pti;
  uint8_t *ptt;
  uint32_t *pti_l;
  uint32_t *ptt_l;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t ds = depth(image);
  int32_t ps = rs * cs;
  int32_t N = ps * ds;

  temp = copyimage(image);
  if (temp == NULL) 
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }

  init_topo3d();

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    pti = UCHARDATA(image);
    ptt = UCHARDATA(temp);
    memset(pti, 0, N);
    for (k = 1; k < ds - 1; k++) 
      for (j = 1; j < cs - 1; j++) 
        for (i = 1; i < rs - 1; i++) 
          pti[k*ps + j*rs + i] = (uint8_t)t6pp(ptt, k*ps + j*rs + i, rs, ps, N);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    pti_l = ULONGDATA(image);
    ptt_l = ULONGDATA(temp);
    memset(pti_l, 0, 4*N);
    for (k = 1; k < ds - 1; k++) 
      for (j = 1; j < cs - 1; j++) 
        for (i = 1; i < rs - 1; i++) 
          pti_l[k*ps + j*rs + i] = (uint32_t)t6pp_l(ptt_l, k*ps + j*rs + i, rs, ps, N);
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", F_NAME, datatype(image));
    return(0);
  }

  termine_topo3d();
  freeimage(temp);
  return 1;
} /* lt6pp() */

/* ==================================== */
int32_t lt26pp(struct xvimage * image)
/* ==================================== */
#undef F_NAME
#define F_NAME "lt26pp"
{
  int32_t i, j, k;
  struct xvimage * temp;
  uint8_t *pti;
  uint8_t *ptt;
  uint32_t *pti_l;
  uint32_t *ptt_l;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t ds = depth(image);
  int32_t ps = rs * cs;
  int32_t N = ps * ds;

  temp = copyimage(image);
  if (temp == NULL) 
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }

  init_topo3d();

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    pti = UCHARDATA(image);
    ptt = UCHARDATA(temp);
    memset(pti, 0, N);
    for (k = 1; k < ds - 1; k++) 
      for (j = 1; j < cs - 1; j++) 
        for (i = 1; i < rs - 1; i++) 
          pti[k*ps + j*rs + i] = (uint8_t)t26pp(ptt, k*ps + j*rs + i, rs, ps, N);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    pti_l = ULONGDATA(image);
    ptt_l = ULONGDATA(temp);
    memset(pti_l, 0, 4*N);
    for (k = 1; k < ds - 1; k++) 
      for (j = 1; j < cs - 1; j++) 
        for (i = 1; i < rs - 1; i++) 
          pti_l[k*ps + j*rs + i] = (uint32_t)t26pp_l(ptt_l, k*ps + j*rs + i, rs, ps, N);
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", F_NAME, datatype(image));
    return(0);
  }

  termine_topo3d();
  freeimage(temp);
  return 1;
} /* lt26pp() */

/* ==================================== */
int32_t lalpha(struct xvimage * image, int32_t connex, char sign)
/* ==================================== */
#undef F_NAME
#define F_NAME "lalpha"
{
  int32_t k;
  struct xvimage * temp;
  uint8_t *pti;
  uint8_t *ptt;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t ds = depth(image);
  int32_t ps = rs * cs;
  int32_t N = ps * ds;

  temp = copyimage(image);
  if (temp == NULL) 
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    pti = UCHARDATA(image);
    ptt = UCHARDATA(temp);
    memset(pti, 0, N);

    if (sign == 'm')
    {
      if (connex == 26)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha26m(ptt, k, rs, ps, N);
      else
      if (connex == 6)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha6m(ptt, k, rs, ps, N);
      else
      if (connex == 8)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha8m(ptt, k, rs, N);
      else
      if (connex == 4)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha4m(ptt, k, rs, N);
      else
      {
	fprintf(stderr, "%s: bad connexity: %d\n", F_NAME, connex);
	return(0);
      }
    }
    else
    if (sign == 'p')
    {
      if (connex == 26)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha26p(ptt, k, rs, ps, N);
      else
      if (connex == 6)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha6p(ptt, k, rs, ps, N);
      else
      if (connex == 8)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha8p(ptt, k, rs, N);
      else
      if (connex == 4)
	for (k = 0; k < N; k++) 
	  pti[k] = (uint8_t)alpha4p(ptt, k, rs, N);
      else
      {
	fprintf(stderr, "%s: bad connexity: %d\n", F_NAME, connex);
	return(0);
      }
    }
    else
    {
      fprintf(stderr, "%s: bad sign: %c\n", F_NAME, sign);
      return(0);
    }
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", F_NAME, datatype(image));
    return(0);
  }

  freeimage(temp);
  return 1;
} /* lalpha() */
