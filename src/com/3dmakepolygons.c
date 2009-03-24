/* $Id: 3dmakepolygons.c,v 1.5 2009-03-24 15:34:09 mcouprie Exp $ */
/*! \file 3dmakepolygons.c

\brief identifies polygons from a labelled pure 2D cellular complex

<B>Usage:</B> 3dmakepolygons lab.pgm border.pgm psubdiv pmerge out.vtk

<B>Description:</B>

The input \b lab.pgm contains a labelled pure 2D complex, where each label 
marks a set of 2-facets which is a manifold with boundary

psubdiv: this parameter governs the subdivision of polygon edges 
pmerge: TODO

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - decembre 2008 */

#define MISEAUPOINT

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcliste.h>
#include <mcgeo.h>
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

#ifdef MISEAUPOINT1
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
#ifdef MISEAUPOINT1
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
#ifdef MISEAUPOINT1
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
  int32_t rs, cs, ds, ps, N, i, j, k, a, b, x, indx;
  uint32_t *L;
  uint8_t *B;
  int32_t u, v, nl, np, npol, spol, maxlab, label;
  int8_t *marklab;
  int32_t *markvert;
  Liste * Face;
  Liste * Poly;
  Liste * Labels;
  FILE *fd = NULL;
  double subdiv, merge, x1, y1, z1, x2, y2, z2;
  MCP *P;
  int32_t degenerate;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s lab.pgm border.pgm psubdiv pmerge out.vtk \n", argv[0]);
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
    fprintf(stderr, "%s: the input data %s must be VFF_TYP_1_BYTE\n", argv[0], argv[2]);
    exit(1);
  }
  B = UCHARDATA(bor);

  subdiv = atof(argv[3]);  
  merge = atof(argv[4]);  
  
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
#ifdef MISEAUPOINT1
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

  MCP_ComputeFaces(P);

MCP_Print(P);

  // FILTRAGE POUR FUSIONNER LES SOMMETS PROCHES
  
  markvert = (int32_t *)calloc(2 * P->Vertices->cur, sizeof(int32_t));
  // il faut plus de place car des sommets vont être créés
  if (markvert == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }
  for (i = 0; i < P->Vertices->cur; i++) markvert[i] = -1;

  for (i = 0; i < P->Faces->cur; i++)
  {  
    for (j = 0; j < P->Faces->f[i].n; j++)
    {  
      a = P->Faces->f[i].vert[j]; 
      b = P->Faces->f[i].vert[(j+1)%P->Faces->f[i].n];
      if ((markvert[a] == -1) && (markvert[b] == -1)) 
      { // a et b sont deux sommets consécutifs dans la face i
	x1 = P->Vertices->v[a].x; 
	y1 = P->Vertices->v[a].y; 
	z1 = P->Vertices->v[a].z; 
	x2 = P->Vertices->v[b].x; 
	y2 = P->Vertices->v[b].y; 
	z2 = P->Vertices->v[b].z; 
	if (dist3(x1, y1, z1, x2, y2, z2) < merge) 
	{ // on décide de fusionner a et b
	  indx = MCP_AddVertex(P, (x1+x2)/2, (y1+y2)/2, (z1+z2)/2);	  
	  markvert[a] = indx;
	  markvert[b] = indx;
	  markvert[indx] = -1;
#ifdef MISEAUPOINT
	  printf("Marque pour fusion: %d,%d -> %d\n", a, b, indx);
#endif
	}
      } // if ((markvert[a] != -1) && (markvert[b] != -1)) 
    } // for (j = 0; j < P->Faces->f[i].n; j++)
  } // for (i = 0; i < P->Faces->cur; i++)

  // fusion des sommets marqués dans toutes les faces
  for (i = 0; i < P->Faces->cur; i++)
  {  
    j = 0;
    while (j < P->Faces->f[i].n)
    {
      a = P->Faces->f[i].vert[j]; 
      b = P->Faces->f[i].vert[(j+1)%P->Faces->f[i].n];
      if ((markvert[a] != -1) && (markvert[a] == markvert[b]))
      { // a et b sont deux sommets consécutifs dans la face i marqués identiquement
#ifdef MISEAUPOINT
	  printf("Fusionne: %d,%d -> %d\n", a, b, markvert[a]);
#endif
	// on retire a de la face i
	for (k = j; k < P->Faces->f[i].n - 1; k++)
	  P->Faces->f[i].vert[k] = P->Faces->f[i].vert[k+1]; 
	P->Faces->f[i].n -= 1;
	// on remplace b par markvert[a]
	if (j < P->Faces->f[i].n)
	  P->Faces->f[i].vert[j] = markvert[a];
	else
	  P->Faces->f[i].vert[0] = markvert[a];
      }
      else
	j++;
    } // while (j < P->Faces->f[i].n)
  } // for (i = 0; i < P->Faces->cur; i++)

  // remplacement des sommets marqués dans toutes les faces
  for (i = 0; i < P->Faces->cur; i++)
  {  
    for (j = 0; j < P->Faces->f[i].n; j++)
    {
      a = P->Faces->f[i].vert[j]; 
      if (markvert[a] != -1)
	P->Faces->f[i].vert[j] = markvert[a]; // on remplace a par markvert[a]
    } // 
  } // for (i = 0; i < P->Faces->cur; i++)

MCP_Print(P);

 degenerate = 1;
 while (degenerate)
 {
  degenerate = 0;
  // FILTRAGE POUR REDUIRE A UN POINT LES FACES DEGENEREES (2 POINTS)
  for (i = 0; i < P->Faces->cur; i++)
  {  
    if (P->Faces->f[i].n == 2)
    {  
      degenerate = 1;
      a = P->Faces->f[i].vert[0]; 
      b = P->Faces->f[i].vert[1];
      x1 = P->Vertices->v[a].x; 
      y1 = P->Vertices->v[a].y; 
      z1 = P->Vertices->v[a].z; 
      x2 = P->Vertices->v[b].x; 
      y2 = P->Vertices->v[b].y; 
      z2 = P->Vertices->v[b].z; 
      indx = MCP_AddVertex(P, (x1+x2)/2, (y1+y2)/2, (z1+z2)/2);	  
      markvert[a] = indx;
      markvert[b] = indx;
      markvert[indx] = -1;
#ifdef MISEAUPOINT
      printf("Face degeneree: marque pour fusion: %d,%d -> %d\n", a, b, indx);
#endif
      P->Faces->f[i].n = 0; // Supprime la face
      P->Faces->f[i].na = 0;
    } // if (P->Faces->f[i].n == 2)
  } // for (i = 0; i < P->Faces->cur; i++)

  // remplacement des sommets marqués dans toutes les faces
  for (i = 0; i < P->Faces->cur; i++)
  {  
    for (j = 0; j < P->Faces->f[i].n; j++)
    {
      a = P->Faces->f[i].vert[j]; 
      if (markvert[a] != -1)
	P->Faces->f[i].vert[j] = markvert[a]; // on remplace a par markvert[a]
    } // 
  } // for (i = 0; i < P->Faces->cur; i++)
 } // while (degenerate)

MCP_Print(P);
 
  // SUBDIVISION DES ARETES

  P->Edges = MCP_AllocEdges(100);
  MCP_ComputeEdges(P);

  if (subdiv > 0) MCP_SubdivEdges(P, subdiv);

  // SAUVEGARDE RESULTAT

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
  free(markvert);
  return 0;
} /* main */
