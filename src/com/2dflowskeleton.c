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
/* \file 2dflowskeleton.c

\brief computes the flow skeleton (see [Cou10]) of a 2D complex

<B>Usage:</B> 2dflowskeleton in.pgm mode [prio] out.pgm

<B>Description:</B>

Computes the flow skeleton (see [Cou10]) of a 2D complex.

The parameter \b mode selects the function to be integrated in order to build the topological map. The possible choices are:
\li 0: uniform null function 
\li 1: uniform unity function 
\li 2: border indicator function
\li 3: border indicator function and substraction of lambda from the integrated map
\li 4: inverse opening function
\li 5: bisector function
\li 6: inverse Euclidean distance map
\li 7: lambda function

\warning The input image \b in.pgm must be a complex, otherwise the result is meaningless (no verification is done)

References:<BR> 
[Cou10] M. Couprie: "Topological flows, maps and skeletons", in preparation.<BR>

<B>Types supported:</B> byte 2d

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
#include <mckhalimsky2d.h>
#include <mcgeo.h>
#include <mcdrawps.h>
#include <mcgraphe.h>
#include <ldist.h>
#include <lmedialaxis.h>
#include <l2dkhalimsky.h>
#include <l2dcollapse.h>

	      //#define SHOWGRAPHS
	      //#define SHOWCONFLUENTPOINTS
	      //#define SHOWIMAGES

#define FS_EPSILON 0.1
#define MARGEX1 5
#define MARGEY1 5
#define MARGEX2 25
#define MARGEY2 25
#define FONTSIZE 12

#ifdef SHOWGRAPHS
static void ShowGraphe(graphe * g, char *filename, double s, double r, double t, int noms_sommets, int v_sommets, int col_sommets, int all_arcs, uint8_t *K, int rs, uint8_t *head) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ShowGraphe"
{
  int i, j, xx, yy, n = g->nsom;
  double xmin, xmax, ymin, ymax;
  double x1, y1, x2, y2, x3, y3, x, y, a, b, d;
  pcell p;
  FILE * fd = NULL;
  char buf[80];
  
  if (g->gamma == NULL) 
  {  fprintf(stderr, "%s: representation successeurs absente\n", F_NAME);
     return;
  }
  
  if (g->x == NULL) 
  {  fprintf(stderr, "%s: coordonnees des sommets absentes\n", F_NAME);
     return;
  }

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return;
  }

  /* determine le rectangle englobant et genere le header */
  xmin = xmax = s*g->x[0];
  ymin = ymax = s*g->y[0];
  for (i = 1; i < n; i++) 
  {
    if (s*g->x[i] < xmin) xmin = s*g->x[i]; else if (s*g->x[i] > xmax) xmax = s*g->x[i];
    if (s*g->y[i] < ymin) ymin = s*g->y[i]; else if (s*g->y[i] > ymax) ymax = s*g->y[i];
  }
  EPSHeader(fd, xmax - xmin + MARGEX1 + MARGEX2, ymax - ymin + MARGEY1 + MARGEY2, 1.0, FONTSIZE);
  
  /* dessine le fond */
  PSSetColor (fd, 1.0);
  PSDrawRect (fd, 0, 0, xmax - xmin + MARGEX1 + MARGEX2, ymax - ymin + MARGEY1 + MARGEY2);
  PSSetColor (fd, 0.0);

  /* dessine le complexe */
  PSSetColor (fd, 0.80);
  PSSetLineWidth (fd, 5);    
  for (i = 0; i < n; i++) 
    if (K[i])
    {
      xx = i%rs;
      yy = i/rs;
      if ((xx%2 == 0) && (yy%2 == 0))
      {
	PSDrawdisc(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 5);
	if ((xx < rs-2) && K[i+2])
	  PSLine(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, s*g->x[i+2]-xmin+MARGEX1, s*g->y[i+2]-ymin+MARGEY1);
	if ((i < n-(rs+rs)) && K[i+rs+rs])
	  PSLine(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, s*g->x[i+rs+rs]-xmin+MARGEX1, s*g->y[i+rs+rs]-ymin+MARGEY1);
      }
    }
  PSSetColor (fd, 0.0);
  PSSetLineWidth (fd, 1);

  if (noms_sommets && g->nomsommet)
    for (i = 0; i < n; i++) 
      if (K[i])
	PSString(fd, s*g->x[i]-xmin+MARGEX1+2*r, s*g->y[i]-ymin+MARGEY1-2*r, g->nomsommet[i]);

  /* dessine les arcs */
  PSSetColor (fd, 0.50);
  for (i = 0; i < n; i++)
    if (all_arcs || head[i])
      for (p = g->gamma[i]; p != NULL; p = p->next)
      {
	j = p->som;
	PSLine(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, s*g->x[j]-xmin+MARGEX1, s*g->y[j]-ymin+MARGEY1);
      }

  /* dessine les fleches sur les arcs */
  if (t > 0.0)
  {
    for (i = 0; i < n; i++) 
    if (all_arcs || head[i])
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      x1 = s*g->x[i]-xmin+MARGEX1;
      y1 = s*g->y[i]-ymin+MARGEY1;
      x2 = s*g->x[j]-xmin+MARGEX1;
      y2 = s*g->y[j]-ymin+MARGEY1;
      x = (x2 + x1) / 2; // milieu de l'arc
      y = (y2 + y1) / 2;
      a = x2 - x1;
      b = y2 - y1;             /* (a,b) est un vecteur directeur de l'arc */
      d = sqrt(a * a + b * b); /* longueur de l'arc */
      if (d > 1) // sinon on ne dessine pas la fleche
      { 
        a /= d; b /= d;          /* norme le vecteur */
        x1 = x + 2 * t * a;
        y1 = y + 2 * t * b;      /* pointe de la fleche */
        x2 = x - 2 * t * a;
        y2 = y - 2 * t * b;      /* base de la fleche */
        x3 = x2 + t * -b;        /* (-b,a) est normal a (a,b) */
        y3 = y2 + t * a;
        x2 = x2 - t * -b;
        y2 = y2 - t * a;
        PSLine(fd, x1, y1, x2, y2);
        PSLine(fd, x2, y2, x3, y3);
        PSLine(fd, x3, y3, x1, y1);
      }
    }
  }
  PSSetColor (fd, 0.0);

  /* dessine les sommets */
  for (i = 0; i < n; i++) 
    if (K[i])
    {
      if (col_sommets && (g->v_sommets[i] == 1)) 
	PSDrawdisc(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 8*r);
      else if (col_sommets && (g->v_sommets[i] == 2)) 
      {
	PSSetColor(fd, 1.0);
	PSDrawdisc(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 8*r);
	PSSetColor(fd, 0.0);
	PSDrawcircle(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 8*r);
      }
      else
	PSDrawcircle(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, r);
    }
  if (v_sommets)
    for (i = 0; i < n; i++) 
#ifdef DESSINECOLSEQ
      if (K[i] && !head[i] && (g->v_sommets[i] < 0))
      { //pour dessiner une col. seq.
	sprintf(buf, "%g", -(double)(g->v_sommets[i]));
	PSString(fd, s*g->x[i]-xmin+MARGEX1+2*r, s*g->y[i]-ymin+MARGEY1+2*r, buf);
      }
#else
      if (K[i])
      {
	sprintf(buf, "%.1f", (double)(g->v_sommets[i]));
	PSString(fd, s*g->x[i]-xmin+MARGEX1+2*r, s*g->y[i]-ymin+MARGEY1+2*r+2, buf);
      }
#endif
  
  PSFooter(fd);
  fclose(fd);
} // ShowGraphe()
#endif

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
  int32_t rs, cs, ds=1, N, i;
  float * FUNC;
  float * LAMBDA;
  graphe * flow;
  boolean * perm;
  boolean * head;
  TYP_VSOM vmax;
  TYP_VSOM epsilon = FS_EPSILON;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm mode [prio] out.pgm\n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]); 
  assert(k != NULL);
  assert(datatype(k) == VFF_TYP_1_BYTE);
  assert(depth(k) == 1);
  rs = rowsize(k);
  cs = colsize(k);
  N = rs*cs;
  K = UCHARDATA(k);

  mode = atoi(argv[2]);

  dist = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  assert(dist != NULL);
  if (! lsedt_meijster(k, dist))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    lambda = readimage(argv[3]); 
    assert(lambda != NULL);
    assert(datatype(lambda) == VFF_TYP_FLOAT);
    assert(rowsize(lambda) == rs);
    assert(colsize(lambda) == cs);
    assert(depth(lambda) == 1);
  }
  else
  {
    lambda = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
    assert(lambda != NULL);
    if (!llambdamedialaxis(dist, lambda))
    {
      fprintf(stderr, "%s: llambdamedialaxis failed\n", argv[0]);
      exit(1);
    }
  }

#ifdef SHOWIMAGES
  writeimage(lambda, "_lambda");
#endif

  // -----------------------------------------------------------
  // 1ERE ETAPE : CALCULE LE FLOW GRAPH
  // -----------------------------------------------------------
  kk = copyimage(k); // sauve k car l2dtopoflow modifie le complexe
  assert(kk != NULL);
  if (! (flow = l2dtopoflow_f(kk, lambda, NULL, FLT_MAX)))
  {
    fprintf(stderr, "%s: function l2dtopoflow_f failed\n", argv[0]);
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

#ifdef SHOWGRAPHS
#ifdef DESSINECOLSEQ
  ShowGraphe(flow, "_flow", 30, 1, 3, 0, 1, 0, 0, K, rs, head);//pour dessiner une col. seq.
#else
  ShowGraphe(flow, "_flow1", 30, 1, 3, 0, 0, 0, 1, K, rs, head);
#endif
#ifdef SHOWCONFLUENTPOINTS
  {
    graphe * flow_s = Symetrique(flow);
    int32_t a;
    char buf[1024];
    //    struct xvimage *img = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    //    uint8_t *I = UCHARDATA(img);
    //    for (i = 0; i < N; i++) I[i] = (D[i]?255:0);
    //    writeimage(img, "_amont");
    //    freeimage(img);
    assert(N < 1000);
    for (a = 0; a < N; a++) if (K[a])
    {
      boolean *D = Descendants(flow_s, a);
      for (i = 0; i < N; i++) 
	if (D[i]) 
	  flow->v_sommets[i] = (TYP_VSOM)1; 
	else 
	  flow->v_sommets[i] = (TYP_VSOM)0;
      flow->v_sommets[a] = (TYP_VSOM)2; 
      sprintf(buf, "_amont%d_%d", a%rs, a/rs);
      printf("showing: %s\n", buf);
      ShowGraphe(flow, buf, 30, 1, 3, 0, 0, 1, 1, K, rs, head);
      free(D);
    }
    TermineGraphe(flow_s);
  }
#endif
#endif
  
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
    if (! l2dborder(border))
    {
      fprintf(stderr, "%s: function l2dborder failed\n", argv[0]);
      exit(1);
    }
#ifdef SHOWIMAGES
    writeimage(border, "_border");
#endif
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
    uint32_t *OF, maxof;
    assert(of != NULL);
    OF = ULONGDATA(of);
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
    MaxAlpha2d(func); // fermeture (en ndg)
  }
  else if (mode == 6)
  { // distance map
    uint32_t *D = ULONGDATA(dist);
    for (i = 0; i < N; i++) FUNC[i] = (float)(1.0/sqrt(D[i]));
  }
  else if (mode == 7)
  { // lambda function
    copy2image(func, lambda);
    MaxAlpha2d(func); // fermeture (en ndg)
  }
  else
  {
    fprintf(stderr, "%s: bad mode: %d\n", argv[0], mode);
    exit(1);
  }

  for (i = 0; i < N; i++)
    flow->v_sommets[i] = (TYP_VSOM)FUNC[i];

#ifdef SHOWGRAPHS
  ShowGraphe(flow, "_flow2", 30, 1, 3, 0, 1, 0, 1, K, rs, head);
#endif

  // -----------------------------------------------------------
  // 3EME ETAPE : INTEGRE LA FONCTION DE POIDS 
  // -----------------------------------------------------------
  IntegreGSC(flow);

#ifdef SHOWGRAPHS
  ShowGraphe(flow, "_flow3", 30, 1, 3, 0, 1, 0, 1, K, rs, head);
#endif
#ifdef SHOWIMAGES
  for (i = 0; i < N; i++)
    FUNC[i] = (float)flow->v_sommets[i];
  writeimage(func, "_func");
#endif
    
  if (mode == 3)
  {
    MaxAlpha2d(lambda); // fermeture (en ndg)
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

#ifdef SHOWGRAPHS
  ShowGraphe(flow, "_flow4", 30, 1, 3, 0, 1, 0, 1, K, rs, head);
#endif
  
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
