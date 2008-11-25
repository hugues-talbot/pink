/* $Id: l3dcollapse.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* 

   l3dcollapse: collapse guidÅÈ et contraint
     Michel Couprie - avril 2007

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mcrbt.h>
#include <mcindic.h>
#include <mcutil.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

#define EN_RBT        0

/* =============================================================== */
int32_t l3dcollapse(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit)
/* =============================================================== */
/* 
  collapse sÅÈquentiel, guidÅÈ et contraint
*/
#undef F_NAME
#define F_NAME "l3dcollapse"
{
  int32_t i, u, v, n, x, y, z, xv, yv, zv;
  int32_t rs, cs, ps, ds, N;
  uint8_t * K;
  uint32_t * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  int32_t taillemaxrbt;
  int32_t tab[GRS3D*GCS3D*GDS3D];

  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (prio == NULL)
  {
    fprintf(stderr, "%s : prio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(prio) != rs) || (colsize(prio) != cs) || (depth(prio) != ds))
  {
    fprintf(stderr, "%s : bad size for prio\n", F_NAME);
    return(0);
  }
  if (datatype(prio) == VFF_TYP_4_BYTE) 
    P = ULONGDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be uint32_t\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != ds))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
  }

  taillemaxrbt = 2 * (rs + cs + ds);
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (K[i] && ((I == NULL) || (!I[i])) && FaceLibre3d(k, x, y, z))
    {
      RbtInsert(&RBT, P[i], i);
      Set(i, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!RbtVide(RBT))
  {
    i = RbtPopMin(RBT);
    UnSet(i, EN_RBT);
    x = i % rs; y = (i % ps) / rs; z = i / ps;
    if (u = Collapse3d(k, x, y, z))
    {
      x = u % rs; y = (u % ps) / rs; z = u / ps;
      Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
      for (u = 0; u < n; u += 1)
      {
        v = tab[u];
	xv = v % rs; yv = (v % ps) / rs; zv = v / ps;
	if (K[v] && !IsSet(v, EN_RBT) && ((I == NULL) || (!I[v])) && FaceLibre3d(k, xv, yv, zv))
	{
	  RbtInsert(&RBT, P[v], v);
	  Set(v, EN_RBT);
	}
      }
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  return 1;

} /* l3dcollapse() */
