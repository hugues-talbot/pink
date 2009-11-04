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
/* analyse de squelettes curvilignes */
/* Michel Couprie - juin 2004 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <mclifo.h>
#include <mcskelcurv.h>
#include <llabelextrema.h>
#include <lseltopo.h>

#define PARANO
//#define VERBOSE
//#define DEBUG

/* ====================================================================== */
static void processend(skel *S, uint8_t *T1, uint8_t *T2, uint8_t *T3, int32_t *M,
		uint32_t start, uint32_t pos, uint32_t i)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "processend"
  int32_t m = start + pos; 

#ifdef DEBUG
  printf("%s: i = %d,%d\n", F_NAME, i%S->rs, i/S->rs);
#endif

  M[i] = m;
  S->tskel[m].adj = NULL; // les relations d'adjacence seront 
                                        // traitées plus tard
  S->tskel[m].pts = skeladdcell(S, i, NULL);
} // processend()

/* ====================================================================== */
static void trouve2voisins(int32_t i, int32_t rs, int32_t ps, int32_t N, int32_t connex, uint8_t *F, int32_t *v1, int32_t *v2)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "trouve2voisins"
  int32_t j, k, n = 0;
  switch (connex)
  {
  case 4:
    for (k = 0; k < 8; k += 2)
    {
      j = voisin(i, k, rs, N);
      if ((j != -1) && F[j])
      { if (n == 0) *v1 = j; else *v2 = j; n++; }
    } // for k
    break;
  case 8:
    for (k = 0; k < 8; k += 1)
    {
      j = voisin(i, k, rs, N);
      if ((j != -1) && F[j])
      { if (n == 0) *v1 = j; else *v2 = j; n++; }
    } // for k
    break;
  case 6:
    for (k = 0; k <= 10; k += 2)
    {
      j = voisin6(i, k, rs, ps, N);
      if ((j != -1) && F[j])
      { if (n == 0) *v1 = j; else *v2 = j; n++; }
    } // for k
    break;
  case 18:
    for (k = 0; k < 18; k += 1)
    {
      j = voisin18(i, k, rs, ps, N);
      if ((j != -1) && F[j])
      { if (n == 0) *v1 = j; else *v2 = j; n++; }
    } // for k
    break;
  case 26:
    for (k = 0; k < 26; k += 1)
    {
      j = voisin26(i, k, rs, ps, N);
      if ((j != -1) && F[j])
      { if (n == 0) *v1 = j; else *v2 = j; n++; }
    } // for k
    break;
  default:
    fprintf(stderr, "%s: bad connectivity: %d\n", F_NAME, connex);
    exit(0);
  } // switch (connex)
#ifdef PARANO
  if (n != 2)
  {
    fprintf(stderr, "%s: WARNING: curve point %d,%d,%d with %d neighbors\n", F_NAME, i%rs, (i%ps)/rs, i/ps, n);
  }
#endif
} // trouve2voisins()

/* ====================================================================== */
static void processcurv(skel *S, uint8_t *F, uint8_t *T1, uint8_t *T2, uint8_t *T3, int32_t *M,
		uint32_t start, uint32_t pos, uint32_t i)
/* ====================================================================== */
// ATTENTION : tous les autres types de points doivent avoir été traités avant (structure M)
// T2 contient les points de courbe proprement dit (2 voisins) et les points extrémités (1 voisin).
// après exécution, tous les points traités sont marqués '3' dans T2
{
#undef F_NAME
#define F_NAME "processcurv"
  int32_t j, k, oldi = -1;
  int32_t rs = S->rs, cs = S->cs, ds = S->ds, ps = rs * cs, N = ps * ds;
  int32_t trouve = 0, closed = 0, incr_vois, connex = S->connex;

#ifdef DEBUG
  printf("%s: i = %d,%d\n", F_NAME, i%S->rs, i/S->rs);
#endif

  if ((connex == 4) || (connex == 8))
  {
    if (ds != 1)
    {   
      fprintf(stderr,"%s: connexity 4 or 8 not defined for 3D\n", F_NAME);
      exit(0);
    }
    if (connex == 4) incr_vois = 2; else incr_vois = 1;
  }

  // 1ere étape: cherche une extrémité si elle existe
  while (!trouve)
  {
    if (T1[i] || T3[i]) { trouve = 1; }
    else // "vrai" point de courbe (non extrémité)
    {
      trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
      T2[i] = 2;
      if (T2[j] == 2) 
      {
	if (T2[k] == 2) trouve = closed = 1; else i = k;
      }
      else i = j;
    }    
  } // while (!trouve)

#ifdef DEBUG
  printf("%s: extremite i = %d,%d\n", F_NAME, i%S->rs, i/S->rs);
#endif

  // 2eme étape: parcourt la courbe et enregistre les infos
  if (closed) 
  {
    int32_t ii, m = start + pos;
    S->tskel[m].adj = S->tskel[m].pts = NULL;

    addptslist(S, m, i);
    T2[i] = 3;
    ii = i; // point de base
#ifdef DEBUG
  printf("%s: closed : point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
#endif
    trouve = 0;
    while (!trouve)
    {
      trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
      if (T2[j] != 2) j = k; 
      if (T2[j] != 2) { trouve = 1; break; }
      T2[j] = 3;
#ifdef DEBUG
  printf("%s: closed : point j = %d,%d traité\n", F_NAME, j%S->rs, j/S->rs);
#endif
      addptslist(S, m, j);
      i = j;
    } // while (!trouve)

    addptslist(S, m, ii);
#ifdef DEBUG
  printf("%s: closed : point ii = %d,%d traité (FIN)\n", F_NAME, ii%S->rs, ii/S->rs);
#endif

  }
  else // courbe ouverte
  {
    int32_t m = start + pos; 
    S->tskel[m].adj = S->tskel[m].pts = NULL;

    if (T3[i]) 
    {
      addadjlist(S, m, M[i]);
      T2[i] = 3;
      oldi = i;
#ifdef DEBUG
      printf("%s: courbe %d adj junc %d\n", F_NAME, m, M[i]);
#endif
    }
    else
    {
      addptslist(S, m, i);
      addadjlist(S, m, M[i]);
      T2[i] = 3;
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
      printf("%s: courbe %d adj end %d\n", F_NAME, m, M[i]);
#endif
    }

    if (T1[i]) 
    {
      if ((connex == 4) || (connex == 8))
	for (k = 0; k < 8; k += incr_vois)
	  { j = voisin(i, k, rs, N); if ((j != -1) && F[j]) break; }
      else if (connex == 6)
	for (k = 0; k < 10; k += 2)
	  { j = voisin6(i, k, rs, ps, N); if ((j != -1) && F[j]) break; }
      else if (connex == 18)
	for (k = 0; k < 18; k += 1)
	  { j = voisin18(i, k, rs, ps, N); if ((j != -1) && F[j]) break; }
      else if (connex == 26)
	for (k = 0; k < 26; k += 1)
	  { j = voisin26(i, k, rs, ps, N); if ((j != -1) && F[j]) break; }
    }
    else // T3
    {
      if ((connex == 4) || (connex == 8))
	for (k = 0; k < 8; k += incr_vois)
	  { j = voisin(i, k, rs, N); if ((j != -1) && (T2[j]==2)) break; }
      else if (connex == 6)
	for (k = 0; k < 10; k += 2)
	  { j = voisin6(i, k, rs, ps, N); if ((j != -1) && (T2[j]==2)) break; }
      else if (connex == 18)
	for (k = 0; k < 18; k += 1)
	  { j = voisin18(i, k, rs, ps, N); if ((j != -1) && (T2[j]==2)) break; }
      else if (connex == 26)
	for (k = 0; k < 26; k += 1)
	  { j = voisin26(i, k, rs, ps, N); if ((j != -1) && (T2[j]==2)) break; }
    }
    i = j;
    while ((!T1[i]) && (!T3[i]))
    {      
      T2[i] = 3;
#ifdef DEBUG
  printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
#endif
      addptslist(S, m, i);
      trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
      if (T2[j] == 3) j = k; 
      i = j;
    } // while (!trouve)

    // traite dernier point
    if (T3[i]) 
    {
      addptslist(S, m, i);
      addadjlist(S, m, M[i]);
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
      printf("%s: courbe %d adj junc %d\n", F_NAME, m, M[i]);
#endif
    }
    else
    {
      T2[i] = 3;
      addptslist(S, m, i);
      addadjlist(S, m, M[i]);
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
      printf("%s: courbe %d adj end %d\n", F_NAME, m, M[i]);
#endif
    }
    if (oldi != -1) T2[oldi] = 0;
  }
} // processcurv()

/* ====================================================================== */
static void processjunc(skel *S, uint8_t *T1, uint8_t *T2, uint8_t *T3, int32_t *M,
		uint32_t start, uint32_t pos, uint32_t i)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "processjunc"
  int32_t j, k;
  int32_t rs = S->rs, cs = S->cs, ds = S->ds, ps = rs * cs, N = ps * ds;
  Lifo * LIFO;
  int32_t incr_vois, connex = S->connex;
  int32_t m = start + pos; 

#ifdef DEBUG
  printf("%s: i = %d,%d ; m = %d\n", F_NAME, i%S->rs, i/S->rs, m);
#endif

  if ((connex == 4) || (connex == 8))
  {
    if (ds != 1)
    {   
      fprintf(stderr,"%s: connexity 4 or 8 not defined for 3D\n", F_NAME);
      exit(0);
    }
    if (connex == 4) incr_vois = 2; else incr_vois = 1;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   
    fprintf(stderr,"%s: CreeLifoVide failed\n", F_NAME);
    exit(0);
  }

  LifoPush(LIFO, i); 
  T3[i] = 2;

  S->tskel[m].adj = S->tskel[m].pts = NULL;

  while (! LifoVide(LIFO))
  {
    i = LifoPop(LIFO);

    // traiter point
    M[i] = m;     // enregistre i
    addptslist(S, m, i);

#ifdef DEBUG
  printf("%s: traite point i = %d,%d\n", F_NAME, i%S->rs, i/S->rs);
#endif

    if ((connex == 4) || (connex == 8))
      for (k = 0; k < 8; k += incr_vois)
      {
	j = voisin(i, k, rs, N);
	if ((j != -1) && (T3[j] == NDG_MAX)) { LifoPush(LIFO, j); T3[j] = 2; }
      } /* for k */
    else if (connex == 6)
      for (k = 0; k < 10; k += 2)
      {
	j = voisin6(i, k, rs, ps, N);
	if ((j != -1) && (T3[j] == NDG_MAX)) { LifoPush(LIFO, j); T3[j] = 2; }
      } /* for k */
    else if (connex == 18)
      for (k = 0; k < 18; k += 1)
      {
	j = voisin18(i, k, rs, ps, N);
	if ((j != -1) && (T3[j] == NDG_MAX)) { LifoPush(LIFO, j); T3[j] = 2; }
      } /* for k */
    else if (connex == 26)
      for (k = 0; k < 26; k += 1)
      {
	j = voisin26(i, k, rs, ps, N);
	if ((j != -1) && (T3[j] == NDG_MAX)) { LifoPush(LIFO, j); T3[j] = 2; }
      } /* for k */

  } /* while ! LifoVide */

  LifoTermine(LIFO);
} // processjunc()

/* ====================================================================== */
skel * limage2skel(struct xvimage *image, int32_t connex)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "limage2skel"
  int32_t i, x, y, z;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t ds = depth(image);
  int32_t N = ds * ps;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *M; // carte des parties de squelette
  struct xvimage *temp0, *temp1, *temp2, *temp3;
  struct xvimage *lab2, *lab3;
  uint8_t *T0, *T1, *T2, *T3;
  int32_t nbisol, nbend, nbcurv, nbjunc, nbvertex, nbpoints;
  skel * S;
  SKC_pcell p;

#ifdef DEBUG
  printf("%s: begin\n", F_NAME);
#endif

  // filtre les bords de l'image
  if (ds > 1)
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      F[0 * ps + y * rs + x] = 0;          /* plan z = 0 */
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      F[(ds-1) * ps + y * rs + x] = 0;     /* plan z = ds-1 */

    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      F[z * ps + 0 * rs + x] = 0;          /* plan y = 0 */
    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      F[z * ps + (cs-1) * rs + x] = 0;     /* plan y = cs-1 */

    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      F[z * ps + y * rs + 0] = 0;          /* plan x = 0 */
    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      F[z * ps + y * rs + (rs-1)] = 0;     /* plan x = rs-1 */
  }
  else
  {
    for (x = 0; x < rs; x++) F[x] = 0;
    for (x = 0; x < rs; x++) F[(cs - 1) * rs + x] = 0;

    for (y = 1; y < cs - 1; y++) F[y * rs] = 0;
    for (y = 1; y < cs - 1; y++) F[y * rs + rs - 1] = 0;
  }

  // detection des differents types de points
  temp0 = copyimage(image);
  temp1 = copyimage(image);
  temp2 = copyimage(image);
  temp3 = copyimage(image);
  if ((temp0 == NULL) || (temp1 == NULL) || (temp2 == NULL) || (temp3 == NULL))
  {
    fprintf(stderr, "%s: function copyimage failed\n", F_NAME);
    return NULL;
  }
  T0 = UCHARDATA(temp0);
  T1 = UCHARDATA(temp1);
  T2 = UCHARDATA(temp2);
  T3 = UCHARDATA(temp3);
  M = (int32_t *)calloc(N, sizeof(int32_t));
  if (M == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", F_NAME);
    return NULL;
  }
  for (i = 0; i < N; i++) M[i] = -1;

  if (! lptisolated(temp0, connex))
  {
    fprintf(stderr, "%s: function lptisolated failed\n", F_NAME);
    return NULL;
  }

  if (! lptend(temp1, connex))
  {
    fprintf(stderr, "%s: function lptend failed\n", F_NAME);
    return NULL;
  }

  if (! lptcurve(temp2, connex))
  {
    fprintf(stderr, "%s: function lptcurve failed\n", F_NAME);
    return NULL;
  }

  if (! lptsimple(temp3, connex))
  {
    fprintf(stderr, "%s: function lpsimple failed\n", F_NAME);
    return NULL;
  }
  for (i = 0; i < N; i++)
    if (T3[i] && !T1[i])
    {
      fprintf(stderr, "%s: input image is not a curvilinear skeleton\n", F_NAME);
      return NULL;
    }
      
  copy2image(temp3, image);

  // comptage des points isoles et extremites et 
  // detection des points de jonction (T3) par complementation
  // réalise de plus l'union de T2 et de T1 (résulat dans T2)
  nbisol = nbend = nbcurv = nbjunc = nbpoints = 0;
  for (i = 0; i < N; i++) 
  {
    if (F[i])
    {
      nbpoints++;
      if (T0[i]) { nbisol++; T3[i] = 0; } else
      if (T1[i]) { nbend++; T3[i] = 0; T2[i] = NDG_MAX; } else
      if (T2[i]) T3[i] = 0; else
      if (T3[i]) T3[i] = NDG_MAX;
    }
  } // for (i = 0; i < N; i++) 

  // etiquetage des courbes et des jonctions
  lab2 = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  lab3 = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if ((lab2 == NULL) || (lab3 == NULL))
  {
    fprintf(stderr, "%s: function allocimage failed\n", F_NAME);
    return NULL;
  }

  if (! llabelextrema(temp2, connex, 0, lab2, &nbcurv))
  {
    fprintf(stderr, "%s: function llabelextrema failed\n", F_NAME);
    return NULL;
  }
  nbcurv -= 1;

  if (! llabelextrema(temp3, connex, 0, lab3, &nbjunc))
  {
    fprintf(stderr, "%s: function llabelextrema failed\n", F_NAME);
    return NULL;
  }
  nbjunc -= 1;

#ifdef DEBUG
  printf("%s: llabelextrema done\n", F_NAME);
  writeimage(lab2, "_labelcurv");
  writeimage(lab3, "_labeljunc");
#endif

#ifdef VERBOSE
  printf("nb isol : %d ; nb end : %d ; nb curves : %d ; nb junctions : %d\n", nbisol, nbend, nbcurv, nbjunc);
#endif

  // construction de la structure "squelette"
  nbvertex = nbisol + nbend + nbcurv + nbjunc; 
  S = initskel(rs, cs, ds, nbvertex, nbpoints + 2*nbcurv + nbvertex * (nbvertex-1), connex);
  if (S == NULL)
  {
    fprintf(stderr, "%s: function initskel failed\n", F_NAME);
    return NULL;
  }

  S->e_isol = nbisol;
  S->e_end  = nbisol + nbend;
  S->e_curv = nbisol + nbend + nbcurv;
  S->e_junc = nbisol + nbend + nbcurv + nbjunc;

  nbisol = nbend = nbjunc = 0;
  for (i = 0; i < N; i++) 
  {
    if (T0[i])
    {
      addptslist(S, nbisol, i);
      M[i] = nbisol;
      nbisol++;
    }
    else if (T1[i])
    {
      processend(S, T1, T2, T3, M, S->e_isol, nbend, i);
      nbend++;
    }
    else if (T3[i] == NDG_MAX)
    {
      processjunc(S, T1, T2, T3, M, S->e_curv, nbjunc, i);
      nbjunc++;
    }
  } // for i

  nbcurv = 0;
  for (i = 0; i < N; i++) 
  {
    if (T2[i] == NDG_MAX)
    {
      processcurv(S, F, T1, T2, T3, M, S->e_end, nbcurv, i);
      nbcurv++;
    }
  } // for i

  // ajoute les relations d'adjacence réciproques
  for (i = S->e_end; i < S->e_curv; i++) 
    for (p = S->tskel[i].adj; p != NULL; p = p->next)
      addadjlist(S, p->val, i);

  // ménage
  free(M);
  free(lab2);
  free(lab3);
  free(temp0);
  free(temp1);
  free(temp2);
  free(temp3);

  return S;
} /* limage2skel() */

/* ====================================================================== */
struct xvimage * lskel2image(skel *S)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "lskel2image"
  int32_t i;
  int32_t rs = S->rs;
  int32_t cs = S->cs;
  int32_t ds = S->ds;
  int32_t N = ds * cs * rs;
  uint8_t *F;      /* l'image de depart */
  SKC_pcell p;
  struct xvimage * image;

  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return NULL;
  }
  F = UCHARDATA(image);      /* l'image de depart */
  memset(F, 0, N);

  for (i = 0; i < S->e_junc; i++)
    for (p = S->tskel[i].pts; p != NULL; p = p->next) 
      F[p->val] = NDG_MAX;

  return image;
} /* lskel2image() */
