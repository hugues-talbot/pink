/* $Id: l3dcollapse.c,v 1.3 2009-06-30 05:15:47 mcouprie Exp $ */
/* 

   l3dcollapse: collapse guidÅÈ et contraint (OBSOLETE)
     Michel Couprie - avril 2007

   l3dpardircollapse: collapse parallÅËle par sous-ÅÈtapes directionnelles
   l3dpardircollapse_l: collapse guidÅÈ et contraint - prioritÅÈ ULONG
   l3dpardircollapse_f: collapse guidÅÈ et contraint - prioritÅÈ FLOAT
     Michel Couprie - juin 2009

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
#define EN_LIFO       1
#define BORDER        2
#define VERBOSE

/* =============================================================== */
int32_t l3dcollapse(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit)
/* =============================================================== */
/* 
  collapse sÅÈquentiel, guidÅÈ et contraint
  OBSOLETE - utiliser l3dpardircollapse_l
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
    u = Collapse3d(k, x, y, z);
    if (u != -1)
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
    }
  } /* while (!RbtVide(RBT)) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  return 1;

} /* l3dcollapse() */

/* =============================================================== */
int32_t l3dpardircollapse_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, uint32_t priomax)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  fonction de prioritÅÈ en entiers longs
  les ÅÈlÅÈments Å‡ prÅÈserver sont ceux de l'image "inhibit" ou, si celle-ci est "NULL", ceux supÅÈrieurs Å‡ "priomax" 
*/
#undef F_NAME
#define F_NAME "l3dpardircollapse_l"
{
  int32_t i, g, f, u, v, n, xf, yf, zf, xg, yg, zg, xv, yv, zv;
  int32_t rs, cs, ps, ds, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  uint32_t * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  Lifo * LIFO;
  Lifo * LIFOb;
  int32_t taillemaxrbt;
  int32_t tab[GRS3D*GCS3D*GDS3D];
  TypRbtKey p;
  uint32_t nbcol;

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

  taillemaxrbt = 4 * (rs*cs + cs*ds + ds*rs);
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  LIFO = CreeLifoVide(taillemaxrbt);
  LIFOb = CreeLifoVide(taillemaxrbt);
  if ((LIFO == NULL) || (LIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
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

  for (zg = 0; zg < ds; zg++)
  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = zg*ps + yg*rs + xg;
    if (K[g] && 
	(((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax)) ) && 
	FaceLibre3d(k, xg, yg, zg))
    {
      RbtInsert(&RBT, (TypRbtKey)P[g], g);
      Set(g, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  nbcol = 1;
  while (!RbtVide(RBT) && nbcol)
  {
    nbcol = 0;
    // construit la liste de toutes les paires libres ayant la prioritÅÈ courante
    p = RbtMinLevel(RBT); 
    while (!RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      g = RbtPopMin(RBT);
      UnSet(g, EN_RBT);
      xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
      f = PaireLibre3d(k, xg, yg, zg);
      if (f != -1)
      {
	LifoPush(LIFO, f);
	LifoPush(LIFO, g);
	Set(g, EN_LIFO);
      }
    }

    for (dir = 0; dir <= 2; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 3; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < LIFO->Sp; i += 2) // Scan the free faces list (dim 3)
	  {
	    f = LIFO->Pts[i];
	    g = LIFO->Pts[i+1];
	    xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
	    if (K[f] && K[g] && 
		(((I != NULL) && (!I[g] && !I[f])) || 
		 ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
	    {
	      xg = g % rs; yg = (g % ps) / rs; zg = g / ps;  
	      if (xf - xg)      { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else if (yf - yg) { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      else              { direc = 2; if (zf > zg) orien = 0; else orien = 1; }
	      if ((DIM3D(xf,yf,zf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE;
		nbcol++;
		// PrÅÈparation sous-ÅÈtapes suivantes
		Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
		  if (K[g] && (P[g] <= p) && 
		      (((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax))) )
		  {
		    f = PaireLibre3d(k, xg, yg, zg);
		    if ((f != -1) && IsSet(f, BORDER) && !IsSet(g, EN_LIFO))
		    { 
		      LifoPush(LIFOb, f); 
		      LifoPush(LIFOb, g);
		    }
		  }
		} // for u
	      } // if ((DIM3D(xf,yf,zf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < LIFO->Sp; i += 2)
	  while (!LifoVide(LIFOb))
	  { 
	    g = LifoPop(LIFOb);
	    f = LifoPop(LIFOb);
	    LifoPush(LIFO, f); 
	    LifoPush(LIFO, g); 
	  }
      } // for (dim = 3; dim >= 1; dim--)
    } // for for

    // PREPARATION ETAPE SUIVANTE
    for (i = 0; i < LIFO->Sp; i += 2)
    {
      f = LIFO->Pts[i];
      xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
      Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
	if (K[g] && !IsSet(g, EN_RBT) && 
	    (((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax)) ) && 
	    FaceLibre3d(k, xg, yg, zg))
	{
	  RbtInsert(&RBT, (TypRbtKey)P[g], g);
	  Set(g, EN_RBT);
	}
      } // for u
    } // for (i = 0; i < LIFO->Sp; i += 2)
    LifoFlush(LIFO);
  } /* while (!RbtVide(RBT)) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  LifoTermine(LIFO);
  LifoTermine(LIFOb);
  return 1;

} /* l3dpardircollapse_l() */
 
/* =============================================================== */
int32_t l3dpardircollapse_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  fonction de prioritÅÈ en flottants
*/
#undef F_NAME
#define F_NAME "l3dpardircollapse_f"
{
  int32_t i, g, f, u, v, n, xf, yf, zf, xg, yg, zg, xv, yv, zv;
  int32_t rs, cs, ps, ds, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  float * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  Lifo * LIFO;
  Lifo * LIFOb;
  int32_t taillemaxrbt;
  int32_t tab[GRS3D*GCS3D*GDS3D];
  TypRbtKey p;
  uint32_t nbcol;

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
  if (datatype(prio) == VFF_TYP_FLOAT) 
    P = FLOATDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be float\n", F_NAME);
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

  taillemaxrbt = 4 * (rs*cs + cs*ds + ds*rs);
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  LIFO = CreeLifoVide(taillemaxrbt);
  LIFOb = CreeLifoVide(taillemaxrbt);
  if ((LIFO == NULL) || (LIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
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

  for (zg = 0; zg < ds; zg++)
  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = zg*ps + yg*rs + xg;
    if (K[g] && 
	(((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax)) ) && 
	FaceLibre3d(k, xg, yg, zg))
    {
      RbtInsert(&RBT, (TypRbtKey)P[g], g);
      Set(g, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  nbcol = 1;
  while (!RbtVide(RBT) && nbcol)
  {
    nbcol = 0;
    // construit la liste de toutes les paires libres ayant la prioritÅÈ courante
    p = RbtMinLevel(RBT); 
    while (!RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      g = RbtPopMin(RBT);
      UnSet(g, EN_RBT);
      xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
      f = PaireLibre3d(k, xg, yg, zg);
      if (f != -1)
      {
	LifoPush(LIFO, f);
	LifoPush(LIFO, g);
	Set(g, EN_LIFO);
      }
    }

    for (dir = 0; dir <= 2; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 3; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < LIFO->Sp; i += 2) // Scan the free faces list (dim 3)
	  {
	    f = LIFO->Pts[i];
	    g = LIFO->Pts[i+1];
	    xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
	    if (K[f] && K[g] && 
		(((I != NULL) && (!I[g] && !I[f])) || 
		 ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
	    {
	      xg = g % rs; yg = (g % ps) / rs; zg = g / ps;  
	      if (xf - xg)      { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else if (yf - yg) { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      else              { direc = 2; if (zf > zg) orien = 0; else orien = 1; }
	      if ((DIM3D(xf,yf,zf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE;
		nbcol++;
		// PrÅÈparation sous-ÅÈtapes suivantes
		Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
		  if (K[g] && (P[g] <= p) && 
		      (((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax))) )
		  {
		    f = PaireLibre3d(k, xg, yg, zg);
		    if ((f != -1) && IsSet(f, BORDER) && !IsSet(g, EN_LIFO))
		    { 
		      LifoPush(LIFOb, f); 
		      LifoPush(LIFOb, g);
		    }
		  }
		} // for u
	      } // if ((DIM3D(xf,yf,zf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < LIFO->Sp; i += 2)
	  while (!LifoVide(LIFOb))
	  { 
	    g = LifoPop(LIFOb);
	    f = LifoPop(LIFOb);
	    LifoPush(LIFO, f); 
	    LifoPush(LIFO, g); 
	  }
      } // for (dim = 3; dim >= 1; dim--)
    } // for for

    // PREPARATION ETAPE SUIVANTE
    for (i = 0; i < LIFO->Sp; i += 2)
    {
      f = LIFO->Pts[i];
      xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
      Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
	if (K[g] && !IsSet(g, EN_RBT) && 
	    (((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax)) ) && 
	    FaceLibre3d(k, xg, yg, zg))
	{
	  RbtInsert(&RBT, (TypRbtKey)P[g], g);
	  Set(g, EN_RBT);
	}
      } // for u
    } // for (i = 0; i < LIFO->Sp; i += 2)
    LifoFlush(LIFO);
  } /* while (!RbtVide(RBT)) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  LifoTermine(LIFO);
  LifoTermine(LIFOb);
  return 1;

} /* l3dpardircollapse_f() */

/* =============================================================== */
int32_t l3dpardircollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  sans fonction de prioritÅÈ
*/
#undef F_NAME
#define F_NAME "l3dpardircollapse"
{
  int32_t i, g, f, u, v, n, xf, yf, zf, xg, yg, zg, xv, yv, zv;
  int32_t rs, cs, ps, ds, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  uint8_t * I = NULL;
  Lifo * LIFO;
  Lifo * LIFOb;
  Lifo * LIFOt;
  int32_t taillemax;
  int32_t tab[GRS3D*GCS3D*GDS3D];

  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;
  K = UCHARDATA(k);

  IndicsInit(N);

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

  taillemax = 4 * (rs*cs + cs*ds + ds*rs);
  LIFO = CreeLifoVide(taillemax);
  LIFOb = CreeLifoVide(taillemax);
  if ((LIFO == NULL) || (LIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
    return(0);
  }

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /* INITIALISATION DE LA LIFO ET DE LA "BORDER" */
  /* ========================================================= */

  for (zg = 0; zg < ds; zg++)
  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = zg*ps + yg*rs + xg;
    if (K[g] && ((I == NULL) || (!I[g])))
    {
      f = PaireLibre3d(k, xg, yg, zg);
      if (f != -1) 
      { 
	LifoPush(LIFO, f); 
	LifoPush(LIFO, g);
	Set(g, EN_LIFO);
	xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
	Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
	for (u = 0; u < n; u += 1)
	{
	  g = tab[u];
	  Set(g, BORDER);
	} // for u
      }
    }
  }

  /* ================================================ */
  /*              DEBUT BOUCLE PRINCIPALE             */
  /* ================================================ */

  while (!LifoVide(LIFO) && nsteps > 0)
  {
    nsteps --;

    for (dir = 0; dir <= 2; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 3; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < LIFO->Sp; i += 2) // Scan the free faces list (dim 3)
	  {
	    f = LIFO->Pts[i];
	    g = LIFO->Pts[i+1];
	    xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
	    if (K[f] && K[g] && ((I == NULL) || (!I[g] && !I[f])))
	    {
	      xg = g % rs; yg = (g % ps) / rs; zg = g / ps;  
	      if (xf - xg)      { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else if (yf - yg) { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      else              { direc = 2; if (zf > zg) orien = 0; else orien = 1; }
	      if ((DIM3D(xf,yf,zf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE;
		// PrÅÈparation sous-ÅÈtapes suivantes
		Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
		  if (K[g] && ((I == NULL) || (!I[g])))
		  {
		    f = PaireLibre3d(k, xg, yg, zg);
		    if ((f != -1) && IsSet(f, BORDER) && !IsSet(g, EN_LIFO))
		    { 
		      LifoPush(LIFOb, f); 
		      LifoPush(LIFOb, g);
		    }
		  }
		} // for u
	      } // if ((DIM3D(xf,yf,zf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < LIFO->Sp; i += 2)
	  while (!LifoVide(LIFOb))
	  { 
	    g = LifoPop(LIFOb);
	    f = LifoPop(LIFOb);
	    LifoPush(LIFO, f); 
	    LifoPush(LIFO, g); 
	  }
      } // for (dim = 3; dim >= 1; dim--)
    } // for for

    // PREPARATION ETAPE SUIVANTE
    for (i = 0; i < LIFO->Sp; i++) UnSet(LIFO->Pts[i], EN_LIFO);
    for (i = 0; i < LIFO->Sp; i += 2)
    {
      f = LIFO->Pts[i];
      UnSet(f, BORDER);
      xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
      Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	UnSet(g, BORDER);
	if (K[g] && !IsSet(g, EN_LIFO) && ((I == NULL) || (!I[g])))
	{
	  xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
	  f = PaireLibre3d(k, xg, yg, zg);
	  if (f != -1) 
	  { 
	    LifoPush(LIFOb, f); 
	    LifoPush(LIFOb, g);
	    Set(g, EN_LIFO);
	  }
	}
      } // for u
    }

    for (i = 0; i < LIFOb->Sp; i += 2)
    {
      f = LIFOb->Pts[i];
      xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
      Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	Set(g, BORDER);
      } // for u
    }

    LifoFlush(LIFO);

    LIFOt = LIFOb;
    LIFOb = LIFO;
    LIFO = LIFOt;

  } // while (!LifoVide(LIFO) && nsteps > 0)

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  LifoTermine(LIFO);
  LifoTermine(LIFOb);
  return 1;

} /* l3dpardircollapse() */

/* =============================================================== */
int32_t l3dsurfacecollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  sans fonction de prioritÅÈ
  detection et prÅÈservation de surfaces
*/
#undef F_NAME
#define F_NAME "l3dsurfacecollapse"
{
  int32_t i, g, f, u, v, n, xf, yf, zf, xg, yg, zg, xv, yv, zv;
  int32_t rs, cs, ps, ds, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  uint8_t * I = NULL;
  Lifo * LIFO;
  Lifo * LIFOb;
  Lifo * LIFOt;
  int32_t taillemax;
  int32_t tab[GRS3D*GCS3D*GDS3D];
  uint32_t nbcol;

  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;
  K = UCHARDATA(k);

  IndicsInit(N);

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
  else
  {
    fprintf(stderr, "%s : inhibit image must be allocated\n", F_NAME);
    return(0);
  }

  taillemax = 4 * (rs*cs + cs*ds + ds*rs);
  LIFO = CreeLifoVide(taillemax);
  LIFOb = CreeLifoVide(taillemax);
  if ((LIFO == NULL) || (LIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
    return(0);
  }

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /* INITIALISATION DE LA LIFO ET DE LA "BORDER" */
  /* ========================================================= */

  for (zg = 0; zg < ds; zg++)
  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = zg*ps + yg*rs + xg;
    if (K[g] && ((I == NULL) || (!I[g])))
    {
      f = PaireLibre3d(k, xg, yg, zg);
      if (f != -1) 
      { 
	LifoPush(LIFO, f); 
	LifoPush(LIFO, g);
	Set(g, EN_LIFO);
	xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
	Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
	for (u = 0; u < n; u += 1)
	{
	  g = tab[u];
	  Set(g, BORDER);
	} // for u
      }
    }
  }

  /* ================================================ */
  /*              DEBUT BOUCLE PRINCIPALE             */
  /* ================================================ */

  nbcol = 1;
  while (!LifoVide(LIFO) && nsteps > 0 && nbcol)
  {
    nsteps --;
    nbcol = 0;

    for (dir = 0; dir <= 2; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 3; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < LIFO->Sp; i += 2) // Scan the free faces list (dim 3)
	  {
	    f = LIFO->Pts[i];
	    g = LIFO->Pts[i+1];
	    xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
	    if (K[f] && K[g] && ((I == NULL) || (!I[g] && !I[f])))
	    {
	      xg = g % rs; yg = (g % ps) / rs; zg = g / ps;  
	      if (xf - xg)      { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else if (yf - yg) { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      else              { direc = 2; if (zf > zg) orien = 0; else orien = 1; }
	      if ((DIM3D(xf,yf,zf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE;
		nbcol++;
		// PrÅÈparation sous-ÅÈtapes suivantes
		Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
		  if (K[g] && ((I == NULL) || (!I[g])))
		  {
		    f = PaireLibre3d(k, xg, yg, zg);
		    if ((f != -1) && IsSet(f, BORDER) && !IsSet(g, EN_LIFO))
		    { 
		      LifoPush(LIFOb, f); 
		      LifoPush(LIFOb, g);
		    }
		  }
		} // for u
	      } // if ((DIM3D(xf,yf,zf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < LIFO->Sp; i += 2)
	  while (!LifoVide(LIFOb))
	  { 
	    g = LifoPop(LIFOb);
	    f = LifoPop(LIFOb);
	    LifoPush(LIFO, f); 
	    LifoPush(LIFO, g); 
	  }
      } // for (dim = 3; dim >= 1; dim--)
    } // for for

    // PREPARATION ETAPE SUIVANTE
    for (i = 0; i < LIFO->Sp; i++) UnSet(LIFO->Pts[i], EN_LIFO);
    for (i = 0; i < LIFO->Sp; i += 2)
    {
      f = LIFO->Pts[i];
      UnSet(f, BORDER);
      xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
      Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	UnSet(g, BORDER);
	xg = g % rs; yg = (g % ps) / rs; zg = g / ps;
	if (K[g] && (DIM3D(xg,yg,zg) == 2) && BetaTerminal3d(K, rs, cs, ds, xg, yg, zg))
	  I[g] = 1;
	else if (K[g] && !IsSet(g, EN_LIFO) && !I[g])
	{
	  f = PaireLibre3d(k, xg, yg, zg);
	  if (f != -1) 
	  { 
	    LifoPush(LIFOb, f); 
	    LifoPush(LIFOb, g);
	    Set(g, EN_LIFO);
	  }
	}
      } // for u
    }

    for (i = 0; i < LIFOb->Sp; i += 2)
    {
      f = LIFOb->Pts[i];
      xf = f % rs; yf = (f % ps) / rs; zf = f / ps;
      Alphacarre3d(rs, cs, ds, xf, yf, zf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	Set(g, BORDER);
      } // for u
    }

    LifoFlush(LIFO);

    LIFOt = LIFOb;
    LIFOb = LIFO;
    LIFO = LIFOt;

  } // while (!LifoVide(LIFO) && nsteps > 0)

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  LifoTermine(LIFO);
  LifoTermine(LIFOb);
  return 1;

} /* l3dsurfacecollapse() */
