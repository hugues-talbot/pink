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
/* \file 3dflowskeleton.c

\brief computes the flow skeleton (see [Cou10]) of a 3D complex

<B>Usage:</B> 3dflowskeleton in.pgm mode out.pgm

<B>Description:</B>

Computes the flow skeleton (see [Cou10]) of a 2D complex.

The parameter \b mode selects the function to be integrated in order to build the topological map. The possible choices are:
\li 0: uniform null function 
\li 1: uniform unity function 
\li 2: border indicator function
\li 3: border indicator function and substraction of lambda from the integrated map
\li 4: inverse opening function
\li 5: bisector function
\li 6: Euclidean distance map

\warning The input image \b in.pgm must be a complex, otherwise the result is meaningless (no verification is done)

References:<BR> 
[Cou10] M. Couprie: "Topological flows, maps and skeletons", in preparation.<BR>

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>
#include <mcgeo.h>
#include <mcdrawps.h>
#include <mcgraphe.h>
#include <ldist.h>
#include <lmedialaxis.h>
#include <l3dkhalimsky.h>
#include <l3dcollapse.h>

#define FS_EPSILON 0.1

#define DEBUG

/* ====================================================================== */
/*! \fn void Morsify(graphe * g, boolean * Vh, TYP_VSOM epsilon)
    \param g (entrée) : un graphe pondéré sur les sommets.
    \brief TODO
    \warning ne s'applique qu'aux graphes sans circuit
*/
static void Morsify(graphe * g, boolean * head, TYP_VSOM epsilon)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Morsify"
{
  int32_t x, y, i, r, rmax, cumul, n = g->nsom;
  int32_t * T;   // pour les sommets triés
  int32_t * H;   // histogramme des rangs
  TYP_VSOM * M;   // sauvegarde fonction sommets
  graphe * g_1 = Symetrique(g);
  pcell p;

  M = (TYP_VSOM *)malloc(n * sizeof(TYP_VSOM)); assert(M != NULL);
  for (x = 0; x < n; x++) M[x] = g->v_sommets[x];
  if (CircuitNiveaux(g))
  {
    fprintf(stderr, "%s: the graph is not acyclic\n", F_NAME);
    exit(0);
  }
  rmax = 0;
  for (x = 0; x < n; x++)
  {
    r = (int32_t)g->v_sommets[x];
    if (r > rmax) rmax = r;
  }
  H = (int32_t *)calloc(rmax + 1, sizeof(int32_t)); assert(H != NULL);
  for (x = 0; x < n; x++) H[(int32_t)g->v_sommets[x]]++; // calcule l'histo
  cumul = H[0];
  H[0] = 0;
  for (i = 1; i <= rmax; i++) // calcule l'histo cumulé
  {
    x = H[i];
    H[i] = cumul;
    cumul += x;
  }
  T = (int32_t *)malloc(n * sizeof(int32_t));
  if (T == NULL)
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    exit(0);
  }
  for (x = 0; x < n; x++)  // tri des sommets par rang croissant
  {
    r = (int32_t)g->v_sommets[x];
    T[H[r]] = x;
    H[r] += 1;
  }
  free(H);

  for (i = 0; i < n; i++)
  {
    x = T[i];
    for (p = g_1->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y prédécesseur de x */
      y = p->som;
      if (head[y])
      {
	M[x] = M[y] = mcmax(M[x],M[y]);
      }
      else
      {
	if (M[x] < (M[y] + epsilon))
	  M[x] = M[y] + epsilon;
      }
    }
  }

  for (x = 0; x < n; x++) g->v_sommets[x] = M[x];
  free(M);
  TermineGraphe(g_1);  
  free(T);
} /* Morsify() */

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * kk;
  struct xvimage * dist;
  struct xvimage * lambda;
  struct xvimage * func;
  int32_t mode;
  uint8_t *K;
  int32_t rs, cs, ds, ps, N, i;
  float * FUNC;
  float * LAMBDA;
  graphe * flow;
  boolean * perm;
  boolean * head;
  TYP_VSOM vmax;
  TYP_VSOM epsilon = FS_EPSILON;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm mode out.pgm\n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;
  K = UCHARDATA(k);
  assert(datatype(k) == VFF_TYP_1_BYTE);
  assert(depth(k) > 1);

  mode = atoi(argv[2]);

  lambda = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  assert(lambda != NULL);
  dist = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  assert(dist != NULL);

  if (! lsedt_meijster(k, dist))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
    exit(1);
  }

  if (!llambdamedialaxis(dist, lambda))
  {
    fprintf(stderr, "%s: llambdamedialaxis failed\n", argv[0]);
    exit(1);
  }

  // -----------------------------------------------------------
  // 1ERE ETAPE : CALCULE LE FLOW GRAPH
  // -----------------------------------------------------------
  kk = copyimage(k); // sauve k car l3dtopoflow modifie le complexe
  assert(kk != NULL);
  if (! (flow = l3dtopoflow_f(kk, lambda, NULL, FLT_MAX)))
  {
    fprintf(stderr, "%s: function l3dtopoflow_f failed\n", argv[0]);
    exit(1);
  }
  freeimage(kk);

  perm = (boolean *)calloc(N, sizeof(boolean)); assert(perm != NULL);
  head = (boolean *)calloc(N, sizeof(boolean)); assert(head != NULL);
  for (i = 0; i < N; i++)
    if (flow->v_sommets[i] == TF_PERMANENT)
      perm[i] = TRUE;
    else if (flow->v_sommets[i] == TF_HEAD)
      head[i] = TRUE;
  
  // -------------------------------------------------------------------
  // 2EME ETAPE : CALCULE LA FONCTION DE POIDS INITIALE POUR LES SOMMETS
  // -------------------------------------------------------------------
  LAMBDA = FLOATDATA(lambda);
  func = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  assert(func != NULL);
  FUNC = FLOATDATA(func);

  if (mode == 0)
  { // fonction uniformément nulle 
    razimage(func);
  }
  else if (mode == 1)
  { // fonction uniforme (unité)
    for (i = 0; i < N; i++) 
      if (K[i])
	FUNC[i] = (float)1;
      else
	FUNC[i] = (float)0;
  }
  else if ((mode == 2) || (mode == 3))
  { // fonction uniforme sur la frontière, nulle à l'intérieur 
    struct xvimage * border = copyimage(k);
    uint8_t *B;
    assert(border != NULL);
    if (! l3dborder(border))
    {
      fprintf(stderr, "%s: function l3dborder failed\n", argv[0]);
      exit(1);
    }
    B = UCHARDATA(border);
    for (i = 0; i < N; i++) 
      if (B[i]) 
	FUNC[i] = (float)1;
      else
	FUNC[i] = (float)0;
    freeimage(border);
  }
  else if (mode == 4)
  { // fonction d'ouverture inversée
    struct xvimage *of = lopeningfunction(k, 0);
    int32_t *OF, maxof;
    assert(of != NULL);
    OF = SLONGDATA(of);
    maxof = OF[0];
    for (i = 0; i < N; i++) if (OF[i] > maxof) maxof = OF[i];
    for (i = 0; i < N; i++) FUNC[i] = (float)(maxof - OF[i]) + 0.1;
    freeimage(of);
  }
  else if (mode == 5)
  { // fonction bisectrice
    if (! lmedialaxis_lbisector(dist, k, func))
    {
      fprintf(stderr, "%s: lmedialaxis_lbisector failed\n", F_NAME);
      exit(1);
    }
  }
  else if (mode == 6)
  { // distance map
    int32_t *D = SLONGDATA(dist);
    for (i = 0; i < N; i++) FUNC[i] = (float)sqrt(D[i]);
  }
  else
  {
    fprintf(stderr, "%s: bad mode: %d\n", argv[0], mode);
    exit(1);
  }

  for (i = 0; i < N; i++)
    flow->v_sommets[i] = (TYP_VSOM)FUNC[i];

  // -----------------------------------------------------------
  // 3EME ETAPE : INTEGRE LA FONCTION DE POIDS 
  // -----------------------------------------------------------
  IntegreGSC(flow);

  if (mode == 3)
  {
    MaxAlpha3d(lambda); // fermeture (en ndg)
    for (i = 0; i < N; i++)
      flow->v_sommets[i] = flow->v_sommets[i] - (TYP_VSOM)LAMBDA[i];
  }

  // -----------------------------------------------------------
  // 4EME ETAPE : TRANSFORME LA FONCTION SUR LES SOMMETS EN 
  // FONCTION DE MORSE (INVERSÉE) SUR LE COMPLEXE
  // -----------------------------------------------------------  
  Morsify(flow, head, epsilon);

  // met à vmax (infini) les sommets "permanents" (non collapsés)
  vmax = flow->v_sommets[0];

  for (i = 0; i < N; i++)
    if (flow->v_sommets[i] > vmax) vmax = flow->v_sommets[i];

  for (i = 0; i < N; i++)
    if (perm[i])
      flow->v_sommets[i] = vmax;
  
  for (i = 0; i < N; i++)
    FUNC[i] = (float)flow->v_sommets[i];

  writeimage(func, argv[argc-1]);

  freeimage(k);
  freeimage(dist);
  freeimage(lambda);
  freeimage(func);
  free(perm);
  free(head);
  TermineGraphe(flow);

  return 0;
} /* main */
