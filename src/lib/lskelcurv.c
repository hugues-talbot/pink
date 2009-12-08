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
#include <math.h>
#include <assert.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <mclifo.h>
#include <mcutil.h>
#include <mcskelcurv.h>
#include <llabelextrema.h>
#include <lseltopo.h>
#include <lmoments.h>

#define VERBOSE
//#define DEBUG
//#define DEBUGDRAW
//#define DEBUG1
#ifdef DEBUG1
static uint32_t nptsisol=0;
static uint32_t nptsend=0;
static uint32_t nptscurv=0;
static uint32_t nptsjunc=0;
static uint32_t nadjisol=0;
static uint32_t nadjend=0;
static uint32_t nadjcurv=0;
static uint32_t nadjjunc=0;
#endif

#define VAL_ISOL 1
#define VAL_END  2
#define VAL_CURV 3
#define VAL_JUNC 4

/* ==================================== */
static double dist3(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
}

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
#ifdef DEBUG1
  nptsend++;
  printf("add point end %d nb %d\n", i, nptsend);
#endif
  S->tskel[m].pts = skeladdcell(S, i, NULL);
} // processend()

/* ====================================================================== */
static void trouve2voisins(int32_t i, int32_t rs, int32_t ps, int32_t N, int32_t connex, uint8_t *F, int32_t *v1, int32_t *v2)
/* ====================================================================== */
// retourne dans v1 et v2 les 2 voisins de i qui sont des points objet (F)
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
  assert(n == 2);
} // trouve2voisins()

/* ====================================================================== */
static int32_t trouve1voisin(int32_t i, int32_t rs, int32_t ps, int32_t N, int32_t connex, uint8_t *F)
/* ====================================================================== */
// retourne un voisin de i qui est un point objet (F), ou -1 si non trouvé
{
#undef F_NAME
#define F_NAME "trouve1voisin"
  int32_t j, k = 0;
  switch (connex)
  {
  case 4:
    for (k = 0; k < 8; k += 2)
    {
      j = voisin(i, k, rs, N);
      if ((j != -1) && F[j]) return j;
    } // for k
    break;
  case 8:
    for (k = 0; k < 8; k += 1)
    {
      j = voisin(i, k, rs, N);
      if ((j != -1) && F[j]) return j;
    } // for k
    break;
  case 6:
    for (k = 0; k <= 10; k += 2)
    {
      j = voisin6(i, k, rs, ps, N);
      if ((j != -1) && F[j]) return j;
    } // for k
    break;
  case 18:
    for (k = 0; k < 18; k += 1)
    {
      j = voisin18(i, k, rs, ps, N);
      if ((j != -1) && F[j]) return j;
    } // for k
    break;
  case 26:
    for (k = 0; k < 26; k += 1)
    {
      j = voisin26(i, k, rs, ps, N);
      if ((j != -1) && F[j]) return j;
    } // for k
    break;
  default:
    fprintf(stderr, "%s: bad connectivity: %d\n", F_NAME, connex);
    exit(0);
  } // switch (connex)
  return -1;
} // trouve1voisin()

/* ====================================================================== */
static void scancurve(int32_t i, struct xvimage *image, int32_t connex, uint8_t *T1, uint8_t *T2, uint8_t *T3, int32_t *length, int32_t *nbend, int32_t *nbjunc)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "scancurve"
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t ds = depth(image);
  int32_t N = ds * ps;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t j, k, jj, kk, le=1, ne=0, nj=0;

#ifdef DEBUG
  printf("%s: i=%d,%d\n", F_NAME, i%rs, i/rs);
#endif

  assert((T2[i] == NDG_MAX) && !T1[i]);
  T2[i] = 1;
  trouve2voisins(i, rs, ps, N, connex, F, &jj, &kk);
  // suit la branche jj
  i = jj;
  while(1)
  {
    if (T1[i]) { ne++; break; }
    if (T3[i]) { nj++; break; }
    assert((T2[i] == NDG_MAX) && !T1[i]);
    T2[i] = 1;
    le++;
    trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
    if (T2[j] == 1) i = k; else i = j;
    if (T2[i] == 1) break; // courbe fermée
  } // while(1)

  // suit la branche kk
  i = kk;
  while(1)
  {
    if (T2[i] == 1) break; // courbe fermée
    if (T1[i]) { ne++; break; }
    if (T3[i]) { nj++; break; }
    assert((T2[i] == NDG_MAX) && !T1[i]);
    T2[i] = 1;
    le++;
    trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
    if (T2[j] == 1) i = k; else i = j;
    assert(T2[i] != 1);
  } // while(1)

  *length = le;
  *nbend = ne;
  *nbjunc = nj;
} // scancurve()

/* ====================================================================== */
static void deletecurve(int32_t i, struct xvimage *image, int32_t connex, uint8_t *T1, uint8_t *T2, uint8_t *T3)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "deletecurve"
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t ds = depth(image);
  int32_t N = ds * ps;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t jj, kk;

#ifdef DEBUG
  printf("%s: i=%d,%d\n", F_NAME, i%rs, i/rs);
#endif
  assert(T2[i] == 1);
  T2[i] = F[i] = 0;
  trouve2voisins(i, rs, ps, N, connex, F, &jj, &kk);
  // suit la branche jj
  i = jj;
  while(1)
  {
    if (T1[i]) { T1[i] = T2[i] = F[i] = 0; break; }
    if (T3[i]) break;
    assert(T2[i] == 1);
    T2[i] = F[i] = 0;
    i = trouve1voisin(i, rs, ps, N, connex, F);
    if (i == -1) break;
  } // while(1)

  // suit la branche kk
  i = kk;
  while(1)
  {
    if (T1[i]) { T1[i] = T2[i] = F[i] = 0; break; }
    if (T3[i]) break;
    assert(T2[i] == 1);
    T2[i] = F[i] = 0;
    i = trouve1voisin(i, rs, ps, N, connex, F);
    if (i == -1) break;
  } // while(1)
} // deletecurve()

/* ====================================================================== */
static void curve2junction(int32_t i, struct xvimage *image, int32_t connex, uint8_t *T2, uint8_t *T3)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "curve2junction"
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t ds = depth(image);
  int32_t N = ds * ps;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t j, k, jj, kk;

#ifdef DEBUG
  printf("%s: i=%d,%d\n", F_NAME, i%rs, i/rs);
#endif
  assert(T2[i] == 1);
  T2[i] = 0; T3[i] = NDG_MAX;
  trouve2voisins(i, rs, ps, N, connex, F, &jj, &kk);
  // suit la branche jj
  i = jj;
  while(1)
  {
    if (T2[i] != 1) break;
    T2[i] = 0; T3[i] = NDG_MAX;
    trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
    if (T2[j] != 1) i = k; else i = j;
  } // while(1)

  // suit la branche kk
  i = kk;
  while(1)
  {
    if (T2[i] != 1) break;
    T2[i] = 0; T3[i] = NDG_MAX;
    trouve2voisins(i, rs, ps, N, connex, F, &j, &k);
    if (T2[j] != 1) i = k; else i = j;
  } // while(1)
} // curve2junction()

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
  // l'extrémité est hors de la courbe : point "end" ou "junc"
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
    int32_t m = start + pos;
    S->tskel[m].adj = S->tskel[m].pts = NULL;

    assert(T2[i]);
#ifdef DEBUG1
    nptscurv++;
    printf("add 1 point curv %d,%d nb %d\n", m, i, nptscurv);
#endif
    addptslist(S, m, i);
    T2[i] = 3;
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
#ifdef DEBUG1
      nptscurv++;
      printf("add 2 point curv %d,%d nb %d\n", m, j, nptscurv);
#endif
      addptslist(S, m, j);
      i = j;
    } // while (!trouve)
  }
  else // courbe ouverte
  {
    int32_t m = start + pos; 
    S->tskel[m].adj = S->tskel[m].pts = NULL;

    if (T3[i]) // extrémité "junc"
    {
#ifdef DEBUG1
      nadjcurv++;
      printf("add 4 adj curv %d,%d nb %d\n", m, M[i], nadjcurv);
#endif
      addadjlist(S, m, M[i]);
      T2[i] = 3; // marque le point extrémité même si c'est une jonction
      oldi = i;
#ifdef DEBUG
      printf("%s: courbe %d adj junc %d\n", F_NAME, m, M[i]);
#endif
    }
    else  // extrémité "end"
    {
      assert(T1[i] && T2[i]); 

#ifdef DEBUG1
      nadjcurv++;
      printf("add 6 adj curv %d,%d nb %d\n", m, M[i], nadjcurv);
#endif
      addadjlist(S, m, M[i]);
      T2[i] = 3;
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
      printf("%s: courbe %d adj end %d\n", F_NAME, m, M[i]);
#endif
    }

    // trouve le premier "vrai point de courbe" adjacent à l'extrémité i
    if (T1[i]) // extrémité "end"
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
#ifdef DEBUG1
      nptscurv++;
      printf("add 7a point curv %d,%d nb %d\n", m, i, nptscurv);
#endif
      addptslist(S, m, i);
    }
    else // T3[i], extrémité "junc"
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

    i = j; // premier point de la courbe
    while ((!T1[i]) && (!T3[i]))
    {      
      T2[i] = 3;
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
#endif
#ifdef DEBUG1
      nptscurv++;
      printf("add 7 point curv %d,%d nb %d\n", m, i, nptscurv);
#endif
      addptslist(S, m, i);
      trouve2voisins(i, rs, ps, N, connex, F, &j, &k);

#ifdef DEBUG1
      printf("voisins: %d [%d,%d,%d] %d [%d,%d,%d] \n", 
	     j, T1[j], T2[j], T3[j], k, T1[k], T2[k], T3[k]);
#endif

      if ((T2[j] == 3) && ((T2[k] != 3) || T3[k] || T1[k])) i = k; 
      else if ((T2[k] == 3) && ((T2[j] != 3) || T3[j] || T1[j])) i = j; 
      else if (T2[k] != 3) i = k; 
      else if (T2[j] != 3) i = j; 
      else assert(0);
    } // while (!trouve)

    // traite dernier point
    if (T3[i]) 
    {
      //      addptslist(S, m, i);
#ifdef DEBUG1
      nadjcurv++;
      printf("add 8 adj curv %d,%d nb %d\n", m, M[i], nadjcurv);
#endif
      addadjlist(S, m, M[i]);
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
      printf("%s: courbe %d adj junc %d\n", F_NAME, m, M[i]);
#endif
    }
    else
    {
      T2[i] = 3;
#ifdef DEBUG1
      nptscurv++;
      printf("add 9 point curv %d,%d nb %d\n", m, i, nptscurv);
#endif
      addptslist(S, m, i);
#ifdef DEBUG1
      nadjcurv++;
      printf("add 10 adj curv %d,%d nb %d\n", m, M[i], nadjcurv);
#endif
      addadjlist(S, m, M[i]);
#ifdef DEBUG
      printf("%s: point i = %d,%d traité\n", F_NAME, i%S->rs, i/S->rs);
      printf("%s: courbe %d adj end %d\n", F_NAME, m, M[i]);
#endif
    }
    if (oldi != -1) T2[oldi] = 0;
  } // courbe ouverte
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
#ifdef DEBUG1
    nptsjunc++;
    printf("add point junc %d,%d nb %d\n", m, i, nptsjunc);
#endif
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
static int32_t is_end(int32_t x, uint8_t *F, int32_t rs, int32_t ps, int32_t N, int32_t connex)
/* ====================================================================== */
{
  switch (connex)
  {
  case 4:
    if (nbvois4(F, x, rs, N) == 1) return 1; else return 0;
  case 8:
    if (nbvois8(F, x, rs, N) == 1) return 1; else return 0;
  case 6:
    if (nbvoiso6(F, x, rs, ps, N) == 1) return 1; else return 0;
  case 18:
    if (nbvoiso18(F, x, rs, ps, N) == 1) return 1; else return 0;
  case 26:
    if (nbvoiso26(F, x, rs, ps, N) == 1) return 1; else return 0;
  default: assert(0);
  }
} // is_end()

/* ====================================================================== */
static int32_t is_simple(int32_t x, uint8_t *F, int32_t rs, int32_t ps, int32_t N, int32_t connex)
/* ====================================================================== */
{
  int32_t t, tb;
  switch (connex)
  {
  case 4:
    top4(F, x, rs, N, &t, &tb);
    if ((t == 1) && (tb == 1)) return 1; else return 0;
  case 8:
    top8(F, x, rs, N, &t, &tb);
    if ((t == 1) && (tb == 1)) return 1; else return 0;
  case 6:
    if (simple6(F, x, rs, ps, N)) return 1; else return 0;
  case 18:
    if (simple18(F, x, rs, ps, N)) return 1; else return 0;
  case 26:
    if (simple26(F, x, rs, ps, N)) return 1; else return 0;
  default: assert(0);
  }
} // is_simple()

/* ====================================================================== */
static int32_t tailleliste(SKC_pcell p)
/* ====================================================================== */
{
  int32_t n = 0;
  for (; p != NULL; p = p->next) n++;
  return n;
} /* tailleliste() */

/* ====================================================================== */
skel * limage2skel(struct xvimage *image, int32_t connex, int32_t len)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "limage2skel"
  int32_t i, j, x, y, z;
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
  int32_t length, ne, nj, ret;
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
  temp0 = copyimage(image); assert(temp0);
  temp1 = copyimage(image); assert(temp1);
  temp2 = copyimage(image); assert(temp2);
  temp3 = copyimage(image); assert(temp3);
  T0 = UCHARDATA(temp0);
  T1 = UCHARDATA(temp1);
  T2 = UCHARDATA(temp2);
  T3 = UCHARDATA(temp3);
  M = (int32_t *)calloc(N, sizeof(int32_t)); assert (M);
  for (i = 0; i < N; i++) M[i] = -1;
  ret = lptisolated(temp0, connex); assert(ret);
  ret = lptend(temp1, connex); assert(ret);
  ret = lptcurve(temp2, connex); assert(ret);
  ret = lptsimple(temp3, connex); assert(ret);

  for (i = 0; i < N; i++)
    if (T3[i] && !T1[i])
    {
      fprintf(stderr, "%s: input image is not a curvilinear skeleton\n", F_NAME);
      return NULL;
    }
      
  copy2image(temp3, image);

  // detection des points de jonction (T3) par complementation
  // réalise de plus l'union de T2 et de T1 (résulat dans T2)
  for (i = 0; i < N; i++) 
  {
    if (F[i])
    {
      if (T0[i]) { T3[i] = 0; } else
      if (T1[i]) { T3[i] = 0; T2[i] = NDG_MAX; } else
      if (T2[i]) T3[i] = 0; else
      if (T3[i]) T3[i] = NDG_MAX;
    }
  } // for (i = 0; i < N; i++) 

  if (len != INT32_MAX)
  {
    // élimination des courbes de longueur inférieure à len
    for (i = 0; i < N; i++) 
    {
      if (T0[i] && (len > 0)) F[i] = T0[i] = 0; // points isolés
      if (T1[i] && (len > 1)) // branches de longueur 1
      {
	j = trouve1voisin(i, rs, ps, N, connex, F);
	assert(j != -1);
	if (T3[j]) F[i] = T1[i] = T2[i] = 0;
      } 
      if ((T2[i] == NDG_MAX) && !T1[i])
      {
	scancurve(i, image, connex, T1, T2, T3, &length, &ne, &nj); // etiquette à 1
	if (length < len)
	{
	  if (((ne == 0) && (nj == 0)) || (ne > 0))
	    deletecurve(i, image, connex, T1, T2, T3);
	  else 
	    curve2junction(i, image, connex, T2, T3);
	}
      }
    } // for (i = 0; i < N; i++) 
    for (i = 0; i < N; i++) if (T2[i]) T2[i] = NDG_MAX;

    do { // retire itérativement les points simples non end
      nbpoints = 0;
      for (i = 0; i < N; i++)
	if (F[i] && !is_end(i, F, rs, ps, N, connex) && is_simple(i, F, rs, ps, N, connex))
	{
	  nbpoints++;
	  F[i] = 0;
	}
    } while (nbpoints);

#ifdef RECARAC
    // Re-caractérisation des points
    // Une jonction qui n'est adjacente qu'à deux courbes est recaractérisée courbe
    // Une jonction qui n'est adjacente qu'à une courbe est recaractérisée extrémité
    // Une jonction qui n'est adjacente à aucune courbe est recaractérisée isolé

    for (i = 0; i < N; i++) 
    {
      if (T3[i] == NDG_MAX)
      {
	nc = scanjunction(i, image, connex, T2, T3); // etiquette à 1
	if (nc == 2) junction2curve(i, image, connex, T2, T3);
	else if (nc == 1) junction2end(i, image, connex, T1, T2, T3);
	else if (nc == 0) junction2isol(i, image, connex, T0, T3);
      }
    } // for (i = 0; i < N; i++) 
    for (i = 0; i < N; i++) if (T3[i]) T3[i] = NDG_MAX;
#endif

  } // if (len != INT32_MAX)

  // comptage des points isoles et extremites et 
  nbisol = nbend = nbcurv = nbjunc = nbpoints = 0;
  for (i = 0; i < N; i++) 
  {
    if (F[i])
    {
      nbpoints++;
      if (T0[i]) nbisol++;
      if (T1[i]) nbend++;
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
#ifdef DEBUG1
  printf("initskel nbvertex %d, nbpoints %d, nbcurv %d, cells %d\n", 
	 nbvertex, nbpoints, nbcurv, nbpoints + nbend + 2*nbcurv + nbvertex * (nbvertex-1));
#endif
  S = initskel(rs, cs, ds, nbvertex, nbpoints + nbend + 2*nbcurv + nbvertex * (nbvertex-1), connex);
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
#ifdef DEBUG1
      nptsisol++;
      printf("add point isol %d,%d nb %d\n", nbisol, i, nptsisol);
#endif
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
    {
#ifdef DEBUG1
      nadjcurv++;
      printf("add adj curv %d,%d nb %d\n", p->val, i, nadjcurv);
#endif
      addadjlist(S, p->val, i);
    }

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

  for (i = S->e_curv; i < S->e_junc; i++)
  {
    for (p = S->tskel[i].pts; p != NULL; p = p->next) 
      F[p->val] = VAL_JUNC;
  }
  for (i = S->e_end; i < S->e_curv; i++)
  {
    for (p = S->tskel[i].pts; p != NULL; p = p->next) 
    {
      if (S->tskel[i].tag)
	F[p->val] = VAL_ISOL;
      else
	F[p->val] = VAL_CURV;
    }
  }
  for (i = S->e_isol; i < S->e_end; i++)
  {
    for (p = S->tskel[i].pts; p != NULL; p = p->next) 
      F[p->val] = VAL_END;
  }
  for (i = 0; i < S->e_isol; i++)
  {
    for (p = S->tskel[i].pts; p != NULL; p = p->next) 
      F[p->val] = VAL_ISOL;
  }

  return image;
} /* lskel2image() */

/* ====================================================================== */
struct xvimage * lskelmarked2image(skel *S)
/* ====================================================================== */
// only the marked items are transfered
{
#undef F_NAME
#define F_NAME "lskelmarked2image"
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
  {
    if (S->tskel[i].tag)
      for (p = S->tskel[i].pts; p != NULL; p = p->next) 
	F[p->val] = NDG_MAX;
  }

  return image;
} /* lskelmarked2image() */

/* ====================================================================== */
static void coordvertex(skel *S, int32_t V, double *Vx, double *Vy, double *Vz)
/* ====================================================================== */
// computes the coordinates (Vx, Vy, Vz) of vertex V in S 
{
  int32_t rs = S->rs;
  int32_t cs = S->cs;
  int32_t ps = rs*cs;
  int32_t n = 0;
  double x=0.0, y=0.0, z=0.0;
  SKC_pcell p = S->tskel[V].pts;

  assert(p != NULL);
  for (; p != NULL; p = p->next)
  {
    x += (double)(p->val % rs);
    y += (double)((p->val % ps) / rs);
    z += (double)(p->val / ps);
    n += 1;
  }  
  *Vx = x/n; *Vy = y/n; *Vz = z/n; 
} // coordvertex()

/* ====================================================================== */
static double distancetoskel(skel *S, double x, double y, double z)
/* ====================================================================== */
//
{
  int32_t rs = S->rs;
  int32_t cs = S->cs;
  int32_t ps = rs*cs;
  int32_t i;
  double xx, yy, zz, d, min = (double)(ps * S->ds);
  SKC_pcell p;

  for (i = 0; i < S->e_junc; i++)
  {
    for (p = S->tskel[i].pts; p != NULL; p = p->next)
    {
      xx = (double)(p->val % rs);
      yy = (double)((p->val % ps) / rs);
      zz = (double)(p->val / ps);
      d = dist3(xx, yy, zz, x, y, z);
      if (d < min) min = d;
    }
  }
  return min;
} // distancetoskel()

/* ====================================================================== */
int32_t lskelmarkvertex(skel *S, int32_t vertex_id)
/* ====================================================================== */
// Mark the specified vertex
{
#undef F_NAME
#define F_NAME "lskelmarkvertex"
  int32_t i;

  if (vertex_id < 0) return 0;
  if (vertex_id >= S->e_junc) return 0;

  for (i = 0; i < S->e_junc; i++)
    S->tskel[i].tag = 0; // not marked

  S->tskel[vertex_id].tag = 1; // marked
  return 1;
} /* lskelmarkvertex() */

/* ====================================================================== */
int32_t lskelfilter1(skel *S, double length, double angle)
/* ====================================================================== */
/*
The skeleton found in S is searched for "small" branches which 
satisfies the following criteria:
\li Extremities A, B are both junctions.
\li Branch length (AB) is less than \b length parameter.
\li Let AA be the symmetric of A wrt B, and BB be the symmetric of B wrt A.
    The distance from AA to the skeleton is more than AB sin( \b angle ), or
    the distance from BB to the skeleton is more than AB sin( \b angle ).

The matching branches are marked (field "tag" = 1)

Parameter \b length is given in pixels, parameter \b angle in radians.
*/
{
#undef F_NAME
#define F_NAME "lskelfilter1"
  int32_t i, A, B;
  SKC_pcell p;
  double AB, Ax, Ay, Az, Bx, By, Bz, AAx, AAy, AAz, BBx, BBy, BBz;
  double dAA, dBB;

#ifdef DEBUG
  printf("lskelfilter1: length = %g, angle %g, l sin a %g\n", 
	 length, angle, length * sin(angle));
#endif	  

  for (i = S->e_end; i < S->e_curv; i++)
  {
    S->tskel[i].tag = 0; // not marked
    p = S->tskel[i].adj;
    A = p->val;
    assert(p->next != NULL);
    p = p->next;
    B = p->val;
    assert(p->next == NULL);
    if (IS_JUNC(A) && IS_JUNC(B))
    {
      coordvertex(S, A, &Ax, &Ay, &Az);
      coordvertex(S, B, &Bx, &By, &Bz);
      AB = dist3(Ax, Ay, Az, Bx, By, Bz);
#ifdef DEBUG
      printf("arc %d, (%g,%g,%g)-(%g,%g,%g), length %g\n", 
	     i, Ax, Ay, Az, Bx, By, Bz, AB);
#endif	  
      if (AB <= length)
      {
	// symétrique de A par rapport à B
	AAx = Bx + Bx - Ax; BBy = By + By - Ay; BBz = Bz + Bz - Az; 
	// symétrique de B par rapport à A
	BBx = Ax + Ax - Bx; AAy = Ay + Ay - By; AAz = Az + Az - Bz; 
	dAA = distancetoskel(S, AAx, AAy, AAz);
	dBB = distancetoskel(S, BBx, BBy, BBz);

#ifdef DEBUG
	printf("dAA = %g, dBB = %g, AB * sin(angle) = %g\n", 
	       dAA, dBB, AB * sin(angle));
#endif	  
	
	if ((dAA >= (AB * sin(angle))) || (dBB >= (AB * sin(angle))))
	{
	  S->tskel[i].tag = 1; // mark for deletion
#ifdef DEBUG
	  printf("mark %d\n", i);
#endif	  
	}
      } // if (AB >= length)
    } // if (IS_JUNC(A) && IS_JUNC(B))
  } // for (i = S->e_end; i < S->e_curv; i++)

  return 1;
} /* lskelfilter1() */

/* ====================================================================== */
static void points_at_head(skel *S, int32_t Ai, double delta, int32_t *e, int32_t *f)
/* ====================================================================== */
// détermine deux points e et f au "début" de l'arc de courbe Ai, 
// séparés d'une distance euclidienne d'au moins delta
{
  int32_t rs = S->rs;
  int32_t ps = rs * S->cs;
  SKC_pcell p = S->tskel[Ai].pts;
  double x, y, z, xx, yy, zz;

  assert(p != NULL);
  *e = p->val;
  if (p->next == NULL)
  {
    *f = p->val;
#ifdef VERBOSE
    printf("Warning: arc %d length 1\n", Ai);
#endif    
    return;
  }
  x = (double)(*e % rs);
  y = (double)((*e % ps) / rs);
  z = (double)(*e / ps);
  for (p = p->next; p != NULL; p = p->next)
  {
    xx = (double)(p->val % rs);
    yy = (double)((p->val % ps) / rs);
    zz = (double)(p->val / ps);
    if (dist3(x, y, z, xx, yy, zz) >= delta)
    {
      *f = p->val;
      break;
    }
    if (p->next == NULL)
    {
      *f = p->val;
#ifdef VERBOSE
      printf("Warning: arc %d length %g\n", Ai, dist3(x, y, z, xx, yy, zz));
#endif    
      return;
    }
  }
} // points_at_head()

/* ====================================================================== */
static void points_at_tail(skel *S, int32_t Ai, double delta, int32_t *e, int32_t *f)
/* ====================================================================== */
// détermine deux points e et f en "fin" de l'arc de courbe Ai, 
// séparés d'une distance euclidienne d'au moins delta
{
  int32_t rs = S->rs;
  int32_t ps = rs * S->cs;
  SKC_pcell p = S->tskel[Ai].pts, pp;
  double x, y, z, xx, yy, zz;

  assert(p != NULL);
  if (p->next == NULL)
  {
    *e = *f = p->val;
#ifdef VERBOSE
    printf("Warning: arc %d length 1\n", Ai);
#endif    
    return;
  }

  for (; p != NULL; p = p->next) *e = p->val;

  x = (double)(*e % rs);
  y = (double)((*e % ps) / rs);
  z = (double)(*e / ps);

  pp = S->tskel[Ai].pts;
  xx = (double)(pp->val % rs);
  yy = (double)((pp->val % ps) / rs);
  zz = (double)(pp->val / ps);
  if (dist3(x, y, z, xx, yy, zz) < delta)
  {
    *f = pp->val;
#ifdef VERBOSE
    printf("Warning: arc %d length %g\n", Ai, dist3(x, y, z, xx, yy, zz));
#endif    
    return;
  }

  for (p = pp->next; p != NULL; p = p->next)
  {
    xx = (double)(p->val % rs);
    yy = (double)((p->val % ps) / rs);
    zz = (double)(p->val / ps);
    if (dist3(x, y, z, xx, yy, zz) < delta)
      break;
    pp = p;
  }
  *f = pp->val;
} // points_at_tail()

/* ====================================================================== */
static int32_t adj_point_junc(skel *S, int32_t e, int32_t J)
/* ====================================================================== */
{
  SKC_pcell p;
  int32_t connex = S->connex;
  int32_t rs = S->rs;
  int32_t ps = rs * S->cs;

  switch(connex)
  {
    case 4:
      for (p = S->tskel[J].pts; p != NULL; p = p->next)
	if (sont4voisins(p->val, e, rs)) return 1;
      break;
    case 8:
      for (p = S->tskel[J].pts; p != NULL; p = p->next)
	if (sont8voisins(p->val, e, rs)) return 1;
      break;
    case 6:
      for (p = S->tskel[J].pts; p != NULL; p = p->next)
	if (sont6voisins(p->val, e, rs, ps)) return 1;
      break;
    case 18:
      for (p = S->tskel[J].pts; p != NULL; p = p->next)
	if (sont18voisins(p->val, e, rs, ps)) return 1;
      break;
    case 26:
      for (p = S->tskel[J].pts; p != NULL; p = p->next)
	if (sont26voisins(p->val, e, rs, ps)) return 1;
      break;
  }
  return 0;
} // adj_point_junc()

/* ====================================================================== */
int32_t lskelfilter2_old(skel *S, double delta, double theta)
/* ====================================================================== */
/*
  For each junction J
    For each arc Ai adjacent to J
      compute and store the vector Vi tangent to Ai starting from J
    For each couple (Vi,Vj) of vectors 
      compute the cosine similarity Cij between Vi and -Vj
        (see http://en.wikipedia.org/wiki/Cosine_similarity)
      if Cij <= theta then mark the arcs Ai and Aj as "aligned"
*/
{
#undef F_NAME
#define F_NAME "lskelfilter2_old"
  int32_t rs = S->rs;
  int32_t ps = rs * S->cs;
  int32_t J, Ai, nadj, e, f, i, j, A[26];
  SKC_pcell p;
  double Vx[26], Vy[26], Vz[26], Cij;

#ifdef DEBUG
  printf("lskelfilter2: delta = %g, theta %g\n", delta, theta);
#endif	  

  for (Ai = S->e_end; Ai < S->e_curv; Ai++)
    S->tskel[Ai].tag = 1; // mark all arcs as "not aligned"

  for (J = S->e_curv; J < S->e_junc; J++)
  {
    for (p = S->tskel[J].adj,nadj = 0; p != NULL; p = p->next, nadj++)
    {
      Ai = p->val;
      A[nadj] = Ai;
      points_at_head(S, Ai, delta, &e, &f);
      if (adj_point_junc(S, e, J))
      {
	Vx[nadj] = (double)((e % rs) - (f % rs));
	Vy[nadj] = (double)(((e % ps) / rs) - ((f % ps) / rs));
	Vz[nadj] = (double)((e / ps) - (f / ps));
      }
      else
      {
	points_at_tail(S, Ai, delta, &e, &f);
	assert(adj_point_junc(S, e, J));
	Vx[nadj] = (double)((e % rs) - (f % rs));
	Vy[nadj] = (double)(((e % ps) / rs) - ((f % ps) / rs));
	Vz[nadj] = (double)((e / ps) - (f / ps));	
      }
    } // for (p = S->tskel[J].adj,nadj = 0; p != NULL; p = p->next, nadj++)
    for (i = 0; i < nadj-1; i++)
      for (j = i+1; j < nadj; j++)
      {
	Cij = acos(scalarprod(Vx[i], Vy[i], Vz[i], -Vx[j], -Vy[j], -Vz[j]) / 
		      (norm(Vx[i], Vy[i], Vz[i]) * norm(Vx[j], Vy[j], Vz[j])));
	if (Cij <= theta)
	{
	  S->tskel[A[i]].tag = 0;
	  S->tskel[A[j]].tag = 0;
#ifdef DEBUG
	  printf("mark %d and %d\n", A[i], A[j]);
#endif	  
	}
      }
  } // for (J = S->e_curv; J < S->e_junc; J++)

  return 1;
} /* lskelfilter2_old() */

/* ====================================================================== */
static void list_points_at_head(skel *S, int32_t Ai, double delta, int32_t *listpoints, int32_t *npoints)
/* ====================================================================== */
// Renvoie dans listpoints la liste des points du "début" de l'arc de courbe Ai, 
// séparés d'une distance euclidienne de moins (<=) de delta du premier.
// Renvoie dans npoints le nombre de points trouvés.
// Le tableau listpoints doit avoir été alloué.
// En entrée, *npoints contient la taille du tableau listpoints.
{
  int32_t rs = S->rs;
  int32_t ps = rs * S->cs;
  SKC_pcell p = S->tskel[Ai].pts;
  double x, y, z, xx, yy, zz;
  int32_t n, nmax;

  nmax = *npoints;
  assert(nmax > 0);

  assert(p != NULL);
  listpoints[0] = p->val;
  n = 1;

  x = (double)(p->val % rs);
  y = (double)((p->val % ps) / rs);
  z = (double)(p->val / ps);

  p = p->next;
  for (; p != NULL; p = p->next)
  {
    xx = (double)(p->val % rs);
    yy = (double)((p->val % ps) / rs);
    zz = (double)(p->val / ps);
    if (dist3(x, y, z, xx, yy, zz) > delta) break;
    assert(n < nmax);
    listpoints[n] = p->val;
    n++;
  }
  *npoints = n;
} // list_points_at_head()

/* ====================================================================== */
static void list_points_at_tail(skel *S, int32_t Ai, double delta, int32_t *listpoints, int32_t *npoints)
/* ====================================================================== */
// Renvoie dans listpoints la liste des points de la "fin" de l'arc de courbe Ai, 
// séparés d'une distance euclidienne de moins (<=) de delta du dernier.
// Le dernier point de l'arc est le premier point de listpoints.
// Renvoie dans npoints le nombre de points trouvés.
// Le tableau listpoints doit avoir été alloué.
// En entrée, *npoints contient la taille du tableau listpoints.
{
  int32_t rs = S->rs;
  int32_t ps = rs * S->cs;
  SKC_pcell p = S->tskel[Ai].pts;
  double x, y, z, xx, yy, zz;
  int32_t i, n, nmax, e;

  nmax = *npoints;
  assert(nmax > 0);

  assert(p != NULL);
  for (; p != NULL; p = p->next) e = p->val; // atteint le dernier point
  x = (double)(e % rs);
  y = (double)((e % ps) / rs);
  z = (double)(e / ps); 

  for (n = 0, p = S->tskel[Ai].pts; p != NULL; p = p->next) // repart du début
  {
    xx = (double)(p->val % rs);
    yy = (double)((p->val % ps) / rs);
    zz = (double)(p->val / ps);
    if (dist3(x, y, z, xx, yy, zz) <= delta)
    {
      assert(n < nmax);
      listpoints[n] = p->val;
      n++;
    }
  }
  // retourne la liste des points mémorisés
  for (i = 0; i < n/2; i++)
  {
    e = listpoints[i];
    listpoints[i] = listpoints[n-1-i];
    listpoints[n-1-i] = e;
  }
  *npoints = n;
} // list_points_at_tail()

/* ====================================================================== */
int32_t lskelfilter2(skel *S, double delta1, double delta2, double theta, int32_t length)
/* ====================================================================== */
/*
  For each junction J
    For each arc Ai adjacent to J
      compute and store the vector Vi tangent to Ai starting from J
    For each couple (Vi,Vj) of vectors 
      compute the cosine similarity Cij between Vi and -Vj
        (see http://en.wikipedia.org/wiki/Cosine_similarity)
      if Cij <= theta then mark the arcs Ai and Aj as "aligned"

  The matching branches are marked (field "tag" = 1)
  Any arc that is closed (cycle), isolated (two ends) or longer than 
  parameter length is also marked as "aligned"
  For the sake of connectivity, junctions are also marked.
*/
{
#undef F_NAME
#define F_NAME "lskelfilter2"
  int32_t rs = S->rs, ps = rs * S->cs;
  int32_t ret, J, Ai, nadj, i, j, A[26], npoints, nmaxpoints, *listpoints;
  SKC_pcell p;
  double Vx[26], Vy[26], Vz[26], Cij;
  double *X, *Y, *Z, x, y, z, xc, yc, zc, xx, yy, zz, dmy;

#ifdef DEBUGDRAW
  struct xvimage *dbg = allocimage(NULL, S->rs, S->cs, S->ds, VFF_TYP_1_BYTE); assert(dbg != NULL);
  razimage(dbg);
#endif	  
#ifdef DEBUG
  printf("lskelfilter2: delta1 = %g, delta2 = %g, theta %g\n", delta1, delta2, theta);
#endif	  

  nmaxpoints = delta2 * 4;
  listpoints = (int32_t *)malloc(nmaxpoints * sizeof(int32_t)); assert(listpoints != NULL);
  X = (double *)malloc(nmaxpoints * sizeof(double)); assert(X != NULL);
  Y = (double *)malloc(nmaxpoints * sizeof(double)); assert(Y != NULL);
  Z = (double *)malloc(nmaxpoints * sizeof(double)); assert(Z != NULL);

  for (J = S->e_curv; J < S->e_junc; J++) // mark all junctions
    S->tskel[J].tag = 1; 

  for (Ai = S->e_end; Ai < S->e_curv; Ai++) // scan all arcs
  {
    p = S->tskel[Ai].adj;
    if (p == NULL) // arc fermé (cycle)
    {
      S->tskel[Ai].tag = 1; // mark as "aligned"
      break;
    }
    assert(p->next != NULL); // soit 0, soit 2 adjacences
    if (((!IS_JUNC(p->val)) && (!IS_JUNC(p->next->val))) ||
	(tailleliste(S->tskel[Ai].pts) >= length))
      S->tskel[Ai].tag = 1; // mark as "aligned"
    else
      S->tskel[Ai].tag = 0; // mark as "not aligned"
  }

  for (J = S->e_curv; J < S->e_junc; J++)
  { // scan all junctions
    for (p = S->tskel[J].adj,nadj = 0; p != NULL; p = p->next, nadj++)
    { // scan arcs Ai adjacent to junction J
      Ai = p->val;
      A[nadj] = Ai;

      npoints = nmaxpoints;
      list_points_at_head(S, Ai, delta2, listpoints, &npoints);
      assert(npoints > 0);
      if (adj_point_junc(S, listpoints[0], J))
      { // arc partant de la jonction 
	x = (double)(listpoints[0] % rs);
	y = (double)((listpoints[0] % ps) / rs);
	z = (double)(listpoints[0] / ps);
	for (j = i = 0; i < npoints; i++)
	{
	  xx = (double)(listpoints[i] % rs);
	  yy = (double)((listpoints[i] % ps) / rs);
	  zz = (double)(listpoints[i] / ps);
	  if (dist3(x, y, z, xx, yy, zz) >= delta1)
	  { // origine pour le calcul de la direction principale : (x,y,z) 
	    X[j] = xx - x; Y[j] = yy - y; Z[j] = zz - z; 
	    j++;
	  }
	}
	if (j > 1)
	{
	  ret = ldirectionsprincipales3d(X, Y, Z, j, 
					 &xc, &yc, &zc, 
					 &(Vx[nadj]), &(Vy[nadj]), &(Vz[nadj]),
					 &dmy, &dmy, &dmy, 
					 &dmy, &dmy, &dmy); assert(ret != 0);
#ifdef DEBUG
	printf("Arc Ai = %d ; V=(%g,%g,%g) ; c=(%g,%g,%g) ; prod=%g\n", 
	       Ai, Vx[nadj], Vy[nadj], Vz[nadj], xc, yc, zc, 
	       scalarprod(Vx[nadj], Vy[nadj], Vz[nadj], xc, yc, zc));
#endif	  
	  if (scalarprod(Vx[nadj], Vy[nadj], Vz[nadj], xc, yc, zc) < 0)
	  {
	    Vx[nadj] = -Vx[nadj]; Vy[nadj] = -Vy[nadj]; Vz[nadj] = -Vz[nadj];
	  }
#ifdef DEBUGDRAW
	  ldrawline(dbg, arrondi(x), arrondi(y), arrondi((x+(10*Vx[nadj]))), arrondi((y+(10*Vy[nadj]))));
#endif
	}
	else
	{
#ifdef VERBOSE
	  printf("%s: warning: arc %d too short (length %d)\n", F_NAME, Ai, tailleliste(S->tskel[Ai].pts));
#endif
	  Vx[nadj] = Vy[nadj] = Vz[nadj] = 0.0;
	}
      } // if (adj_point_junc(S, listpoints[0], J))
      else
      { // arc arrivant à la jonction 
	npoints = nmaxpoints;
	list_points_at_tail(S, Ai, delta2, listpoints, &npoints);
	assert(npoints > 0);
	assert(adj_point_junc(S, listpoints[0], J));
	x = (double)(listpoints[0] % rs);
	y = (double)((listpoints[0] % ps) / rs);
	z = (double)(listpoints[0] / ps);
	for (j = i = 0; i < npoints; i++)
	{
	  xx = (double)(listpoints[i] % rs);
	  yy = (double)((listpoints[i] % ps) / rs);
	  zz = (double)(listpoints[i] / ps);
	  if (dist3(x, y, z, xx, yy, zz) >= delta1)
	  { // origine pour le calcul de la direction principale : (x,y,z) 
	    X[j] = xx - x; Y[j] = yy - y; Z[j] = zz - z; 
	    j++;
	  }
	}
	if (j > 1)
	{
	  ret = ldirectionsprincipales3d(X, Y, Z, j, 
					 &xc, &yc, &zc, 
					 &(Vx[nadj]), &(Vy[nadj]), &(Vz[nadj]),
					 &dmy, &dmy, &dmy, 
					 &dmy, &dmy, &dmy); assert(ret != 0);
#ifdef DEBUG
	printf("Arc Ai = %d ; V=(%g,%g,%g) ; c=(%g,%g,%g) ; prod=%g\n", 
	       Ai, Vx[nadj], Vy[nadj], Vz[nadj], xc, yc, zc, 
	       scalarprod(Vx[nadj], Vy[nadj], Vz[nadj], xc, yc, zc));
#endif	  
	  if (scalarprod(Vx[nadj], Vy[nadj], Vz[nadj], xc, yc, zc) < 0)
	  {
	    Vx[nadj] = -Vx[nadj]; Vy[nadj] = -Vy[nadj]; Vz[nadj] = -Vz[nadj];
	  }
#ifdef DEBUGDRAW
	  ldrawline(dbg, arrondi(x), arrondi(y), arrondi((x+(10*Vx[nadj]))), arrondi((y+(10*Vy[nadj]))));
#endif
	}
	else
	{
#ifdef VERBOSE
	  printf("%s: warning: arc %d too short (length %d)\n", F_NAME, Ai, tailleliste(S->tskel[Ai].pts));
#endif
	  Vx[nadj] = Vy[nadj] = Vz[nadj] = 0.0;
	}
      } // else
    } // for (p = S->tskel[J].adj,nadj = 0; p != NULL; p = p->next, nadj++)

#ifdef DEBUGDRAW
    writeimage(dbg, "_dbg");
#endif

    for (i = 0; i < nadj-1; i++)
    {
#ifdef DEBUG
      printf("Arc i = %d V=(%g,%g,%g)\n", A[i], Vx[i], Vy[i], Vz[i]);
#endif	  
      for (j = i+1; j < nadj; j++)
      {
	Cij = acos(scalarprod(Vx[i], Vy[i], Vz[i], -Vx[j], -Vy[j], -Vz[j]) / 
		      (norm(Vx[i], Vy[i], Vz[i]) * norm(Vx[j], Vy[j], Vz[j])));
#ifdef DEBUG
	printf("  Arc j = %d V=(%g,%g,%g), Cij = %g(%g)\n", A[j], Vx[j], Vy[j], Vz[j], Cij, (Cij*180)/M_PI);
#endif	  
	if (Cij <= theta)
	{
	  S->tskel[A[i]].tag = 1;
	  S->tskel[A[j]].tag = 1;
#ifdef DEBUG
	  printf("mark %d and %d\n", A[i], A[j]);
#endif	  
	}
      } // for j
    } // for i
  } // for (J = S->e_curv; J < S->e_junc; J++)

  free(listpoints); free(X); free(Y); free(Z);
  return 1;
} /* lskelfilter2() */
