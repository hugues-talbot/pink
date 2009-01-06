/* $Id: 3dmakepolygons.c,v 1.1 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file 3dmakepolygons.c

\brief identifies polygons from a labelled pure 2D cellular complex

<B>Usage:</B> 3dmakepolygons lab.pgm border.pgm [subdiv] out.vtk

<B>Description:</B>

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - decembre 2008 */

//#define MISEAUPOINT

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcliste.h>
#include <mcrbt.h>
#include <mcpolygons.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>
#include <assert.h>

int32_t degrepoint(struct xvimage * b, int32_t i, int32_t j, int32_t k)
{
  int32_t a, u, n, tab[27];
  uint8_t *B = UCHARDATA(b);
  Betacarre3d(rowsize(b), colsize(b), depth(b), i, j, k, tab, &n);
  for (a = u = 0; u < n; u++) // parcourt les eventuels beta-voisins
    if (B[tab[u]]) a++;
  return a;
}

void getlabels(struct xvimage * lab, 
		  int32_t ip, int32_t jp, int32_t kp,
		  Liste * Labels
		 )
{
  int32_t rs = rowsize(lab);
  int32_t cs = colsize(lab);
  int32_t ds = depth(lab);
  int32_t ps = rs * cs;
  uint32_t *L = ULONGDATA(lab);
  int32_t u, n, tab[27];

  ListeFlush(Labels);
  Betacarre3d(rs, cs, ds, ip, jp, kp, tab, &n);
  for (u = 0; u < n; u++) 
    if (L[tab[u]] && CARRE3D((tab[u]%rs),((tab[u]%ps)/rs),(tab[u]/ps)))
      if (!ListeIn(Labels, L[tab[u]]))
	ListePush(Labels, L[tab[u]]);
}

void pointsegnextpoint(struct xvimage * lab, struct xvimage * b, 
		       int32_t label, 
		       int32_t ip, int32_t jp, int32_t kp,
		       int32_t is, int32_t js, int32_t ks,
		       int32_t *i, int32_t *j, int32_t *k
		      )
// retourne le point contenu dans le segment is,js,ks de l'image b 
// qui n'est pas le point ip,jp,kp
{
  int32_t rs = rowsize(b);
  int32_t cs = colsize(b);
  int32_t ds = depth(b);
  int32_t ps = rs * cs;
  int32_t u, n, tab[27];

  Alphacarre3d(rs, cs, ds, is, js, ks, tab, &n);
  assert(n == 2);
  for (u = 0; u < n; u++) // parcourt les deux alpha-voisins
  {
    *i = tab[u]%rs; *j = (tab[u]%ps)/rs; *k = tab[u]/ps;
    if ((*i != ip) || (*j != jp) || (*k != kp)) break;
  }
} // pointsegnextpoint()

int32_t segpointnextseg(struct xvimage * lab, struct xvimage * b, 
			int32_t label, 
			int32_t is, int32_t js, int32_t ks,
			int32_t ip, int32_t jp, int32_t kp,
			int32_t *it, int32_t *jt, int32_t *kt
		       )
// retourne (si possible) le segment de l'image b qui contient le point 
// ip,jp,kp, est adjacent à un carré marqué label, et n'est pas is,js,ks
{
  int32_t rs = rowsize(b);
  int32_t cs = colsize(b);
  int32_t ds = depth(b);
  uint8_t *B = UCHARDATA(b);
  uint32_t *L = ULONGDATA(lab);
  int32_t ps = rs * cs;
  int32_t u, n, tab[27], ii, jj, kk, u1, n1, tab1[27], ns;

  ns = 0;
  Betacarre3d(rs, cs, ds, ip, jp, kp, tab, &n);
  for (u = 0; u < n; u++) // parcourt les eventuels beta-voisins
  {
    ii = tab[u]%rs; jj = (tab[u]%ps)/rs; kk = tab[u]/ps;
    if (B[tab[u]] && INTER3D(ii,jj,kk))
    {
      Betacarre3d(rs, cs, ds, ii, jj, kk, tab1, &n1);
      for (u1 = 0; u1 < n1; u1++) // parcourt les eventuels beta-voisins
      {
	if (CARRE3D((tab1[u1]%rs),((tab1[u1]%ps)/rs),(tab1[u1]/ps))
	    && (L[tab1[u1]] == label))
	{
	  if ((ii != is) || (jj!= js) || (kk != ks))
	  {
	    *it = ii; *jt = jj; *kt = kk; 
	    ns++;
	  }
	}
      }
      assert(ns <= 1);
    }
  }
  return ns;
} // segpointnextseg()

int32_t point2segs(struct xvimage * lab, struct xvimage * b, int32_t label, 
		   int32_t i, int32_t j, int32_t k,
		   int32_t *i1, int32_t *j1, int32_t *k1,
		   int32_t *i2, int32_t *j2, int32_t *k2
		  )
// retourne deux (si possible) segments de l'image b qui contiennent le point 
// i,j,k et sont adjacents à un carré marqué label
{
  int32_t rs = rowsize(b);
  int32_t cs = colsize(b);
  int32_t ds = depth(b);
  int32_t ps = rs * cs;
  uint8_t *B = UCHARDATA(b);
  uint32_t *L = ULONGDATA(lab);
  int32_t u, n, tab[27], ii, jj, kk, u1, n1, tab1[27], ns;

  ns = 0;
  Betacarre3d(rs, cs, ds, i, j, k, tab, &n);
  for (u = 0; u < n; u++) // parcourt les eventuels beta-voisins
  {
    ii = tab[u]%rs; jj = (tab[u]%ps)/rs; kk = tab[u]/ps;
    if (B[tab[u]] && INTER3D(ii,jj,kk))
    {
      Betacarre3d(rs, cs, ds, ii, jj, kk, tab1, &n1);
      for (u1 = 0; u1 < n1; u1++) // parcourt les eventuels beta-voisins
      {
	if (CARRE3D((tab1[u1]%rs),((tab1[u1]%ps)/rs),(tab1[u1]/ps))
	    && (L[tab1[u1]] == label))
	{
	  if (ns == 0)
	  {
	    *i1 = ii; *j1 = jj; *k1 = kk; 
	  } 
	  else
	  {
	    *i2 = ii; *j2 = jj; *k2 = kk; 
	  } 
	  ns++;
	}
      }
      assert(ns <= 2);
    }
  }
} // point2segs()

int32_t scanpolygon(struct xvimage * lab, struct xvimage * b, 
		 int32_t label, int32_t i, int32_t j, int32_t k,
		 Liste *poly
		)
{
  int32_t rs = rowsize(b);
  int32_t ps = rs * colsize(b);
  int32_t i1, j1, k1, i2, j2, k2, n, d, npoints = 0;
  int32_t ip, jp, kp, iq, jq, kq, is, js, ks, it, jt, kt;

  ListeFlush(poly);

#ifdef MISEAUPOINT
  printf("scanpolygon : label %d\n", label);
  printf("  sommet %d,%d,%d (%d): degre > 2\n", i, j, k, k*ps + j*rs + i);
#endif
  npoints++;
  ListePush(poly, k*ps + j*rs + i);
  n = point2segs(lab, b, label, i, j, k, &i1, &j1, &k1, &i2, &j2, &k2);
  assert(n > 0);
  ip = i; jp = j; kp = k;
  is = i1; js = j1; ks = k1;
  do
  {
    pointsegnextpoint(lab, b, label, ip, jp, kp, is, js, ks, &iq, &jq, &kq);
    d = degrepoint(b, iq, jq, kq);
    if (d != 2)
    {
#ifdef MISEAUPOINT
      printf("  sommet %d,%d,%d (%d): degre %d\n", iq, jq, kq, kq*ps + jq*rs + iq, d);
#endif	
      if ((iq != i) || (jq != j) || (kq != k))
      {
	npoints++;
	ListePush(poly, kq*ps + jq*rs + iq);
      }
    }
    n = segpointnextseg(lab, b, label, is, js, ks, iq, jq, kq, &it, &jt, &kt);
    ip = iq; jp = jq; kp = kq;
    is = it; js = jt; ks = kt;
  } while (n && ((is != i1) || (js != j1) || (ks != k1)));
  if (n == 0) 
  {
    ListePush(poly, -1); // marqueur pour le changement de sens
    ip = i; jp = j; kp = k;
    is = i2; js = j2; ks = k2;
    do
    {
      pointsegnextpoint(lab, b, label, ip, jp, kp, is, js, ks, &iq, &jq, &kq);
      d = degrepoint(b, iq, jq, kq);
      if (d != 2)
      {
#ifdef MISEAUPOINT
	printf("  -sommet %d,%d,%d (%d): degre %d\n", iq, jq, kq, kq*ps + jq*rs + iq, d);
#endif	
	npoints++;
	ListePush(poly, kq*ps + jq*rs + iq);
      }
      n = segpointnextseg(lab, b, label, is, js, ks, iq, jq, kq, &it, &jt, &kt);
      ip = iq; jp = jq; kp = kq;
      is = it; js = jt; ks = kt;
    } while (n);
  }
  return npoints;
} // scanpolygon()

#define MAXSOMMETS 100000
#define MAXPPOLY 1000
#define MAXLPOINT 1000

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * lab;
  struct xvimage * bor;
  int32_t rs, cs, ds, ps, N, i, j, k, x, indx;
  uint32_t *L;
  uint8_t *B;
  int32_t u, v, nl, np, npol, spol, maxlab, label;
  int8_t *marklab;
  Liste * Face;
  Liste * Poly;
  Liste * Labels;
  FILE *fd = NULL;
  double param;
  MCP *P;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s lab.pgm border.pgm [subdiv] out.vtk \n", argv[0]);
    exit(1);
  }

  lab = readimage(argv[1]);  
  if (lab == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(lab) != VFF_TYP_4_BYTE) 
  {
    fprintf(stderr, "%s: the input data %s must be VFF_TYP_4_BYTE\n", argv[0], argv[1]);
    exit(1);
  }
  rs = rowsize(lab);
  cs = colsize(lab);
  ds = depth(lab);
  ps = rs * cs;
  N = ps * ds;
  L = ULONGDATA(lab);

  bor = readimage(argv[2]);  
  if (bor == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((rowsize(bor) != rs) || (colsize(bor) != cs) || (depth(bor) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes \n", argv[0]);
    exit(1);
  }

  if (datatype(bor) != VFF_TYP_1_BYTE) 
  {
    fprintf(stderr, "%s: the input data %s must be VFF_TYP_4_BYTE\n", argv[0], argv[2]);
    exit(1);
  }
  B = UCHARDATA(bor);

  if (argc == 5) 
    param = atof(argv[3]);  
  else
    param = 0;

  P = MCP_Init(100);

  Face = CreeListeVide(MAXSOMMETS);
  Poly = CreeListeVide(MAXPPOLY);
  Labels = CreeListeVide(MAXLPOINT);
  if ((Face == NULL) || (Poly == NULL) || (Labels == NULL))
  {
    fprintf(stderr, "%s: CreeListeVide failed\n", argv[0]);
    exit(1);
  }
  
  for (maxlab = x = 0; x < N; x++)
    if (L[x] > maxlab) maxlab = L[x];

  marklab = (int8_t *)calloc((maxlab + 1), sizeof (int8_t));
  if (marklab == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  // 1er parcours pour les points

  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = k*ps + j*rs + i;
    if (B[x] && SINGL3D(i,j,k))
      if (degrepoint(bor, i, j, k) != 2) // point multiple ou extrémité
      {
	indx = MCP_AddVertex(P, (double)i, (double)j, (double)k);
	L[x] = indx; // on stocke l'index du point dans l'image lab
      }
  }

  // 2eme parcours pour les faces

  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = k*ps + j*rs + i;
    if (B[x] && SINGL3D(i,j,k))
    {
      if (degrepoint(bor, i, j, k) > 2) // point multiple du bord
      { // récupère l'ensemble des labels adjacents
	getlabels(lab, i, j, k, Labels);
	nl = ListeTaille(Labels);
	assert(nl > 0);
	for (u = 0; u < nl; u++)
	{
	  label = ListeElt(Labels, u);
	  if (!marklab[label]) // nouveau label
	  {
	    marklab[label] = 1;
	    spol = scanpolygon(lab, bor, label, i, j, k, Poly);	    
#ifdef MISEAUPOINT
	    printf("point multiple %d,%d,%d  ", i, j, k);
	    printf("label %d\n", label);
	    ListePrintLine(Poly);
#endif	
	    ListeFlush(Face);
	    for (v = 0; v < spol; v++)
	    {
	      x = ListeElt(Poly, v);
	      if (x == -1) break;
	      indx = L[x];
	      ListePush(Face, indx);
	    } // for (v = 0; v < spol; v++)
	    if (x == -1)
	    {
	      for (v = ListeTaille(Poly)-1; v >= 0 ; v--)
	      {
		x = ListeElt(Poly, v);
		if (x == -1) break;
		indx = L[x];
		ListePush(Face, indx);
	      }	// for v
	    } // if (x == -1)

	    (void)MCP_AddFace(P, Face);

	  } // if (!marklab[label])
	} // for (u = 0; u < nl; u++)
      } // if (degrepoint(bor, i, j, k) > 2)
    } // if (B[x] && SINGL3D(i,j,k))
  } // for i, j, k

  P->Edges = MCP_AllocEdges(100);
  MCP_ComputeEdges(P);
  if (param != 0)
    MCP_SubdivEdges(P, param);

  fd = fopen(argv[argc - 1], "w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  MCP_SaveVTK(P, fd);
  fclose(fd);

  MCP_Termine(P);
  ListeTermine(Face);
  ListeTermine(Poly);
  ListeTermine(Labels);
  freeimage(lab);
  freeimage(bor);
  free(marklab);
  return 0;
} /* main */
