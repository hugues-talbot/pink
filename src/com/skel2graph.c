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

/*! \file skel2graph.c

\brief generation of a graph from a curvilinear skeleton

<B>Usage:</B> skel2graph in.skel [mode] out.graph

<B>Description:</B>
Generation of a graph from a curvilinear skeleton.

The parameter \b mode has the following meaning (default is 0):

\li 0: vertices of the graphs are elements (ends, curves, junctions, isolated points) of the skeleton

\li 1: vertices of the graph are only ends, isolated points and junctions of the skeleton

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgraphe.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

/* ====================================================================== */
graphe * skel2graph(skel * S)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés,  les extrémités, les arcs et les jonctions.
// Pour les jonctions, et les arcs, on prend pour coordonnées le barycentre des points.
{
#undef F_NAME
#define F_NAME "skel2graph"
  graphe * G;
  int32_t i, rs, ps, v, n;
  double x, y, z;
  int32_t nsom, ncurv;
  SKC_pt_pcell p;
  SKC_adj_pcell a;

  rs = S->rs;
  ps = rs * S->cs;
  ncurv = S->e_curv - S->e_end;
  nsom  = S->e_junc;

  G = InitGraphe(nsom, ncurv * 4);

  // pts isolés  
  for (i = 0; i < S->e_isol; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts extrémités  
  for (i = S->e_isol; i < S->e_end; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts de courbe
  for (i = S->e_end; i < S->e_curv; i++)
  {
    a = S->tskel[i].adj; 
    assert(a != NULL);
    v = a->val;
    AjouteArc(G, i, v);
    AjouteArc(G, v, i);

    a = a->next;
    assert(a != NULL);
    v = a->val;
    AjouteArc(G, i, v);
    AjouteArc(G, v, i);

    a = a->next;
    assert(a == NULL);

    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i] = x / n;
    G->y[i] = y / n;
    G->z[i] = z / n;    
  }
  // pts de jonction
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i] = x / n;
    G->y[i] = y / n;
    G->z[i] = z / n;
  }

  return G;
} // skel2graph()

/* ====================================================================== */
graphe * skel2graph1(skel * S)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés,  les extrémités et les jonctions.
// Pour les jonctions, et les arcs, on prend pour coordonnées le barycentre des points.
{
#undef F_NAME
#define F_NAME "skel2graph1"
  graphe * G;
  int32_t i, rs, ps, v, v2, n;
  double x, y, z;
  int32_t nsom, ncurv;
  SKC_pt_pcell p;
  SKC_adj_pcell a;

  rs = S->rs;
  ps = rs * S->cs;
  ncurv = S->e_curv - S->e_end;
  nsom  = S->e_junc - ncurv;

  G = InitGraphe(nsom, ncurv * 2);

  // pts isolés  
  for (i = 0; i < S->e_isol; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts extrémités  
  for (i = S->e_isol; i < S->e_end; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts de courbe
  for (i = S->e_end; i < S->e_curv; i++)
  {
    a = S->tskel[i].adj; 
    assert(a != NULL);
    v = a->val;
    if (IS_JUNC(v)) v = v - ncurv;

    a = a->next;
    assert(a != NULL);
    v2 = a->val;
    if (IS_JUNC(v2)) v2 = v2 - ncurv;
    AjouteArc(G, v, v2);
    AjouteArc(G, v2, v);

    a = a->next;
    assert(a == NULL);
  }
  // pts de jonction
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i-ncurv] = x / n;
    G->y[i-ncurv] = y / n;
    G->z[i-ncurv] = z / n;
  }

  return G;
} // skel2graph1()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  skel * s; 
  graphe * g; 
  int32_t mode = 0;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.skel [mode] fileout.graph\n", argv[0]);
    exit(1);
  }

  s = readskel(argv[1]);
  assert(s != NULL);

  mode = atoi(argv[2]);

  if (mode == 0)
    g = skel2graph(s);
  else if (mode == 1)
    g = skel2graph1(s);
  else
  {
    fprintf(stderr, "%s: bad mode %d\n", argv[0], mode);
    exit(1);
  }
    
  assert(g != NULL);

  SaveGraphe(g, argv[argc-1]);  

  return 0;
} /* main */
