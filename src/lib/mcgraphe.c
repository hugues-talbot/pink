/*! \file graph_algos.c
    \brief algorithmes fondamentaux
*/
/*
      Michel Couprie, septembre 1999

      janvier 2005 : DepthTree, MaxDiameterTree, Lee
      février 2005 : LCA (non efficace)
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mcfifo.h>
#include <mcrbt.h>
#include <mcutil.h>
#include <mcgraphe.h>

#define L_INFINITY LONG_MAX

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS SUR LES LISTES CHAINEES DE SOMMETS */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn pcell AlloueCell(pcell * plibre)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \return pointeur sur une cellule.
    \brief retire la premiere cellule de la liste pointée par plibre et retourne un pointeur sur cette cellule.
*/
pcell AlloueCell(pcell * plibre)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "AlloueCell"
{
  pcell p;
  if (*plibre == NULL) 
  {
    fprintf(stderr, "%s : plus de cellules libres\n", F_NAME);
    exit(0);
  }
  p = *plibre;
  *plibre = (*plibre)->next;
  return p;
} /* AlloueCell() */

/* ====================================================================== */
/*! \fn void LibereCell(pcell * plibre, pcell p)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \param p (entrée) : pointeur sur une cellule.
    \brief insère la cellule p au début de la liste pointée par 'plibre'. 
*/
void LibereCell(pcell * plibre, pcell p)
/* ====================================================================== */
{
  p->next = *plibre;
  *plibre = p;
} /* LibereCell() */

/* ====================================================================== */
/*! \fn void RetireTete(pcell * plibre, pcell * pliste)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \param pliste (entrée) : pointeur sur une liste.
    \brief retire la première cellule de la liste 'pliste'. La cellule est chaînee à la liste 'plibre'. 
*/
void RetireTete(pcell * plibre, pcell * pliste)
/* ====================================================================== */
{
  pcell p;
  p = (*pliste)->next;
  LibereCell(plibre, *pliste);
  *pliste = p;
} /* RetireTete() */

/* ====================================================================== */
/*! \fn void AjouteTete(pcell * plibre, pcell * pliste, int32_t a, TYP_VARC v)
    \param plibre (entrée) : pointeur sur une liste chaînee de cellules libres.
    \param pliste (entrée) : pointeur sur une liste.
    \param a (entrée) : un sommet.
    \param v (entrée) : une valeur.
    \brief ajoute une cellule contenant le sommet 'a' et la valeur 'v' en tête de la liste 'pliste'. La cellule est prise dans la liste 'plibre'. 
*/
void AjouteTete(pcell * plibre, pcell * pliste, int32_t a, TYP_VARC v)
/* ====================================================================== */
{
  pcell p;
  p = AlloueCell(plibre);
  p->next = *pliste;
  p->som = a;
  p->v_arc = v;
  *pliste = p;
} /* AjouteTete() */

/* ====================================================================== */
/*! \fn int32_t EstDansListe(pcell p, int32_t a) 
    \param p (entrée) : une liste chaînee de successeurs.
    \param a (entrée) : un sommet.
    \return booléen.
    \brief retourne 1 si le sommet 'a' se trouve dans la liste 'p', 0 sinon. 
*/
int32_t EstDansListe(pcell p, int32_t a) 
/* ====================================================================== */
{
  for (; p != NULL; p = p->next)
    if (p->som == a) return 1;
  return 0;
} /* EstDansListe() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ALLOCATION / LIBERATION POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * InitGraphe(int32_t nsom, int32_t nmaxarc)
    \param   nsom (entrée) : nombre de sommets.
    \param nmaxarc (entrée) : nombre maximum d'arcs.
    \return un graphe.
    \brief alloue la memoire nécessaire pour représenter un graphe a 'nsom' sommets,
              possédant au maximum 'nmaxarc' arcs. 
              Retourne un pointeur sur la structure allouée. 
*/
graphe * InitGraphe(int32_t nsom, int32_t nmaxarc)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "InitGraphe"
{
  graphe * g;
  int32_t i;
  
  g = (graphe *)malloc(sizeof(graphe));
  if (g == NULL)
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->reserve = (cell *)malloc(nmaxarc * sizeof(cell));
  if (g->reserve == NULL)
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->gamma = (pcell *)calloc(nsom, sizeof(pcell)); /* calloc met a 0 la memoire allouee */
  if (g->gamma == NULL)
  {   fprintf(stderr, "%s : calloc failed\n", F_NAME);
      exit(0);
  }

  g->tete = (int32_t *)malloc(nmaxarc * sizeof(int32_t));
  g->queue = (int32_t *)malloc(nmaxarc * sizeof(int32_t));
  if ((g->tete == NULL) || (g->tete == NULL))
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->v_arcs = (TYP_VARC *)malloc(nmaxarc * sizeof(TYP_VARC));
  if (g->v_arcs == NULL)
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->v_sommets = (TYP_VSOM *)malloc(nsom * sizeof(TYP_VSOM));
  if (g->v_sommets == NULL)
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->x = (double *)malloc(nsom * sizeof(double));
  g->y = (double *)malloc(nsom * sizeof(double));
  g->z = (double *)malloc(nsom * sizeof(double));
  if ((g->x == NULL) || (g->y == NULL) || (g->z == NULL))
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->nsom = nsom;
  g->nmaxarc = nmaxarc;
  g->narc = 0;

  /* construit la liste initiale de cellules libres */
  for (i = 0; i < nmaxarc - 1; i++)
    (g->reserve+i)->next = g->reserve+i+1;
  (g->reserve+i)->next = NULL;
  g->libre = g->reserve;  

  g->nomsommet = NULL;
  
  return g;
} /* InitGraphe() */

/* ====================================================================== */
/*! \fn void TermineGraphe(graphe * g)
    \param g (entrée) : un graphe.
    \brief libère la memoire occupée par le graphe g. 
*/
void TermineGraphe(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "TermineGraphe"
{
  int32_t i, n = g->nsom;
  free(g->reserve);
  if (g->gamma) free(g->gamma);
  if (g->tete) { free(g->tete); free(g->queue); }
  if (g->v_arcs) free(g->v_arcs);
  if (g->v_sommets) free(g->v_sommets);
  if (g->nomsommet)
  {
    for (i = 0; i < n; i++) free(g->nomsommet[i]);
    free(g->nomsommet);
  }
  if (g->x) free(g->x);
  if (g->y) free(g->y);
  if (g->z) free(g->z);
  free(g);
} /* TermineGraphe() */

/* ====================================================================== */
/*! \fn void CopyXY(graphe * g1, graphe * g2)
    \param g1 (entrée) : un graphe
    \param g2 (entrée/sortie) : un graphe
    \brief copie les coordonnées x, y des sommets du graphe 1 dans le graphe 2
*/
void CopyXY(graphe * g1, graphe * g2)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CopyXY"
{
  int32_t i, n = g1->nsom;
  if (g2->nsom != n)
  {   fprintf(stderr, "%s : graphs must have the same number of vertices\n", F_NAME);
      exit(0);
  }
  for (i = 0; i < n; i++) { g2->x[i] = g1->x[i]; g2->y[i] = g1->y[i]; }
} /* CopyXY() */

/* ====================================================================== */
/*! \fn void CopyVsom(graphe * g1, graphe * g2)
    \param g1 (entrée) : un graphe
    \param g2 (entrée/sortie) : un graphe
    \brief copie les valeurs associées aux sommets du graphe 1 dans le graphe 2
*/
void CopyVsom(graphe * g1, graphe * g2)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CopyVsom"
{
  int32_t i, n = g1->nsom;
  if (g2->nsom != n)
  {   fprintf(stderr, "%s : graphs must have the same number of vertices\n", F_NAME);
      exit(0);
  }
  for (i = 0; i < n; i++) g2->v_sommets[i] = g1->v_sommets[i];
} /* CopyVsom() */

/* ====================================================================== */
/*! \fn graphe * CopyGraphe(graphe * g1)
    \param g1 (entrée) : un graphe
    \return un graphe
    \brief retourne un clone du graphe g1
*/
graphe * CopyGraphe(graphe * g1)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CopyGraphe"
{
  graphe * g;
  int32_t i, j;
  TYP_VARC v;
  int32_t nsom = g1->nsom;
  int32_t nmaxarc = g1->nmaxarc;
  pcell p;

  g = InitGraphe(nsom, nmaxarc);

  if (g1->v_sommets)
    for (i = 0; i < nsom; i++) 
      g->v_sommets[i] = g1->v_sommets[i];

  if (g1->x)
    for (i = 0; i < nsom; i++) 
    {
      g->x[i] = g1->x[i];
      g->y[i] = g1->y[i];
      g->z[i] = g1->z[i];
    }

  for (i = 0; i < nsom; i++) 
    for (p = g1->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      v = p->v_arc;
      AjouteArcValue(g, i, j, (TYP_VARC)v);
    }
  
  return g;
} /* CopyGraphe() */

/* ====================================================================== */
/*! \fn void UnionGraphes(graphe * g1, graphe * g2)
    \param g1 (entrée) : un graphe
    \param g2 (entrée) : un graphe
    \brief calcule l'union des graphes g1 et g2, le résultat est stocké dans g1.
    Le champ v_sommet de l'union est le max des champs v_sommets des deux graphes.
*/
void UnionGraphes(graphe * g1, graphe * g2)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "UnionGraphes"
{
  int32_t i, j, n = g1->nsom, m1 = g1->narc, m2 = g2->narc;
  TYP_VARC v;
  pcell p;

  if (g2->nsom != n)
  {   fprintf(stderr, "%s: graphs must have the same number of vertices\n", F_NAME);
      exit(0);
  }
  if (m1 + m2 > g1->nmaxarc)
  {   fprintf(stderr, "%s: two many arcs in union\n", F_NAME);
      exit(0);
  }

  for (i = 0; i < n; i++) 
    for (p = g2->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      v = p->v_arc;
      AjouteArcValue(g1, i, j, v);
    }

  for (i = 0; i < n; i++) 
    if (g2->v_sommets[i] > g1->v_sommets[i])
      g1->v_sommets[i] = g2->v_sommets[i];
  
} /* UnionGraphes() */

/* ====================================================================== */
/*! \fn void UnitLength(graphe * g) 
    \param g (entrée) : un graphe.
    \brief met à 1 les longueurs de tous les arcs (représentation gamma seulement).
*/
void UnitLength(graphe * g) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "UnitLength"
{
  int32_t i, n = g->nsom;
  pcell p;
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
      p->v_arc = (TYP_VARC)1;
} /* UnitLength() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ENTREE / SORTIE FICHIER POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * ReadGraphe(char * filename)
    \param   filename (entrée) : nom du fichier graphe.
    \return un graphe.
    \brief Lit les données d'un graphe dans le fichier filename, retourne un pointeur sur la structure graphe construite. 
*/
graphe * ReadGraphe(char * filename)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ReadGraphe"
{
#define TAILLEBUF 4096
  graphe * g;
  int32_t i, n, m, t, q;
  char buf[TAILLEBUF];
  char *ret;

  FILE *fd = NULL;

  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", filename, F_NAME);
    return NULL;
  }

  fscanf(fd, "%d %d\n", &n, &m);
  g = InitGraphe(n, m);
  do
  {
    ret = fgets(buf, TAILLEBUF, fd);
    if ((ret != NULL) && (strncmp(buf, "noms sommets", 12) == 0))
    {
      g->nomsommet = (char **)malloc(n * sizeof(char *));
      if (g->nomsommet == NULL)
      {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
          exit(0);
      }
      for (i = 0; i < n; i++)
      {
        fscanf(fd, "%d\n", &t);
        fgets(buf, TAILLEBUF, fd);
        g->nomsommet[t] = (char *)malloc((strlen(buf)+1) * sizeof(char));
        if (g->nomsommet[t] == NULL)
        {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
            exit(0);
        }
        strcpy(g->nomsommet[t], buf);
      } /* for (i = 0; i < n; i++) */
    } /* if ((ret != NULL) && (strcmp(buf, "noms sommets") == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "val sommets", 11) == 0))
    {
      double v;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf\n", &t, &v);
        g->v_sommets[t] = (TYP_VSOM)v; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "val sommets", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0))
    {
      double x, y;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf %lf\n", &t, &x, &y);
        g->x[t] = x; 
        g->y[t] = y; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0))
    {
      double v;
      for (i = 0; i < m; i++)  
      {
        fscanf(fd, "%d %d %lf\n", &t, &q, &v);
        AjouteArcValue(g, t, q, (TYP_VARC)v);
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0))
    {
      for (i = 0; i < m; i++)  
      {
        fscanf(fd, "%d %d\n", &t, &q);
        AjouteArc(g, t, q);
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0)) */
  } while (ret != NULL);

  return g;
} /* ReadGraphe() */

/* ====================================================================== */
/*! \fn void SaveGraphe(graphe * g, char *filename) 
    \param g (entrée) : un graphe.
    \param filename (entrée) : nom du fichier à générer.
    \brief sauve le graphe g dans le fichier filename. 
*/
void SaveGraphe(graphe * g, char *filename) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "SaveGraphe"
{
  int32_t i, j, n = g->nsom, m = g->narc;
  pcell p;
  FILE * fd = NULL;
  TYP_VARC v;

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return;
  }
  
  fprintf(fd, "%d %d\n", n, m);

  if (g->v_sommets)
  {
    fprintf(fd, "val sommets\n");
    for (i = 0; i < n; i++) 
      fprintf(fd, "%d %d\n", i, g->v_sommets[i]);
  }

  if (g->x)
  {
    fprintf(fd, "coord sommets\n");
    for (i = 0; i < n; i++) 
      fprintf(fd, "%d %g %g\n", i, g->x[i], g->y[i]);
  }

  fprintf(fd, "arcs values\n");
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      v = p->v_arc;
      fprintf(fd, "%d %d %ld\n", i, j, v);
    }
  
  fclose(fd);
} /* SaveGraphe() */

/* ====================================================================== */
/*! \fn graphe * Image2Graphe(struct xvimage *image, int32_t mode, int32_t connex)
    \param image (entrée) : structure image
    \param mode (entrée) : mode de valuation des arcs
    \param connex (entrée) : relation d'adjacence (4 ou 8 en 2D, 6 ou 26 en 3D)
    \return un graphe.
    \brief Lit les données d'une image dans la structure image, 
    retourne un pointeur sur la structure graphe construite. 
*/
graphe * Image2Graphe(struct xvimage *image, int32_t mode, int32_t connex)
/* ====================================================================== */
/*
A weighted graph G = (V,E) is built with V = set of pixels of \b image
and E = {(P,Q) in VxV ; P and Q are directly adjacent}. 
Let F(P) be the value of pixel P in \b image. 
A weight W(P,Q) is assigned to each edge, according to the value of \b mode:

\li max : W(P,Q) = max{F(P),F(Q)} 
\li min : W(P,Q) = min{F(P),F(Q)} 
\li avg : W(P,Q) = (F(P) + F(Q)) / 2 
*/
#undef F_NAME
#define F_NAME "Image2Graphe"
{
  int32_t rs, cs, ds, ps, N, M;
  uint8_t *F;
  graphe * g;
  int32_t i, j, k, p;
  TYP_VARC v;

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad data type (only byte is available)\n", F_NAME);
    return NULL;
  }
  
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  F = UCHARDATA(image);

  if ((ds == 1) && (connex == 4))
  {
    N = rs * cs;
    M = 2 * ((rs-1) * cs + (cs-1) * rs); 
    g = InitGraphe(N, M);
    for (j = 0; j < cs; j++)  
      for (i = 0; i < rs; i++)  
      {
	p = j*rs + i;
        g->x[p] = (double)i; 
        g->y[p] = (double)j; 	// coord sommet
	if (i < (rs-1)) 
	{
	  switch(mode)
	  {
	  case SP_MIN: v = min((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
	  case SP_MAX: v = max((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[j*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+1, v*v);
	  AjouteArcValue(g, p+1, p, v*v);
	}
	if (j < (cs-1))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = min((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
	  case SP_MAX: v = max((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j+1)*rs+i]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+rs, v*v);
	  AjouteArcValue(g, p+rs, p, v*v);
	}
      }
  }
  else if ((ds == 1) && (connex == 8))
  {
    N = rs * cs;
    M = 4 * ((rs-1) * cs + (cs-1) * rs); 
    g = InitGraphe(N, M);
    for (j = 0; j < cs; j++)  
      for (i = 0; i < rs; i++)  
      {
	p = j*rs + i;
        g->x[p] = (double)i; 
        g->y[p] = (double)j; 	// coord sommet
	if (i < (rs-1)) 
	{
	  switch(mode)
	  {
	  case SP_MIN: v = min((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
	  case SP_MAX: v = max((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[j*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+1, v);
	  AjouteArcValue(g, p+1, p, v);
	}
	if (j < (cs-1))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = min((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
	  case SP_MAX: v = max((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j+1)*rs+i]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+rs, v);
	  AjouteArcValue(g, p+rs, p, v);
	}
	if ((i < (rs-1)) && (j < (cs-1)))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = min((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i+1]); break;
	  case SP_MAX: v = max((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j+1)*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+rs+1, v);
	  AjouteArcValue(g, p+rs+1, p, v);
	}
	if ((i < (rs-1)) && (j > 0))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = min((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j-1)*rs+i+1]); break;
	  case SP_MAX: v = max((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j-1)*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j-1)*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p-rs+1, v);
	  AjouteArcValue(g, p-rs+1, p, v);
	}
      }
  }
  else
  {
    fprintf(stderr, "%s: 3D not yet implemented\n", F_NAME);
    return NULL;
  }

  return g;
} /* Image2Graphe() */

/* ====================================================================== */
/*! \fn struct xvimage *Graphe2Image(graphe * g, int32_t rs)
    \param g (entrée) : un graphe
    \return une structure image
    \brief génère une image à partir du graphe g 
    (d'après les valeurs associées aux sommets) 
    et retourne un pointeur sur la structure image construite. 
*/
struct xvimage *Graphe2Image(graphe * g, int32_t rs)
/* ====================================================================== */
/*

*/
#undef F_NAME
#define F_NAME "Graphe2Image"
{
  int32_t N;
  struct xvimage *image;
  uint8_t *F;
  int32_t i;

  N = g->nsom;
  if (N % rs != 0)
  {
    fprintf(stderr, "%s: rs and g->nsom must be congruent\n", F_NAME);
    return NULL;
  }

  image = allocimage(NULL, rs, N / rs, 1, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    exit(1);
  }
  F = UCHARDATA(image);
  memset(F, 0, N);
  
  for (i = 0; i < N; i++)  
    if (g->v_sommets[i]) F[i] = 255; 

  return image;
} /* Graphe2Image() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE MANIPULATION DES ARCS (APPLICATION GAMMA UNIQUEMENT) */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void AjouteArc(graphe * g, int32_t i, int32_t s)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : extrémité initiale de l'arc.
    \param s (entrée) : extrémité finale de l'arc.
    \brief ajoute l'arc (i,s) au graphe g (application gamma seulement). 
*/
void AjouteArc(graphe * g, int32_t i, int32_t s)
/* ====================================================================== */
{
  AjouteTete(&(g->libre), &(g->gamma[i]), s, 0);
  g->narc++;
} /* AjouteArc() */

/* ====================================================================== */
/*! \fn void AjouteArcValue(graphe * g, int32_t i, int32_t s, TYP_VARC v)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : extrémité initiale de l'arc.
    \param s (entrée) : extrémité finale de l'arc.
    \param v (entrée) : une valeur pour l'arc.
    \brief ajoute l'arc (i,s) au graphe g (application gamma seulement). 
*/
void AjouteArcValue(graphe * g, int32_t i, int32_t s, TYP_VARC v)
/* ====================================================================== */
{
  AjouteTete(&(g->libre), &(g->gamma[i]), s, v);
  g->narc++;
} /* AjouteArcValue() */

/* ====================================================================== */
/*! \fn void RetireArc(graphe * g, int32_t i, int32_t s)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : un sommet de g.
    \param s (entrée) : un sommet de g.
    \brief retire l'arc (i,s) du graphe g (application gamma seulement),
              si celui-ci etait présent. Sinon, pas d'action. 
*/
void RetireArc(graphe * g, int32_t i, int32_t s)
/* ====================================================================== */
{
  pcell * pliste;
  pliste = g->gamma + i;
  while ((*pliste != NULL) && (((*pliste)->som) != s))
    pliste = &((*pliste)->next);
  if (*pliste != NULL) RetireTete(&(g->libre), pliste);
  g->narc--;
} /* RetireArc() */

/* ====================================================================== */
/*! \fn int32_t PopSuccesseur(graphe * g, int32_t i)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : un sommet de g.
    \brief retire un arc (i,s) du graphe g (application gamma seulement),
           et retourne le sommet s
    \warning le sommet i doit avoir au moins un successeur (pas de vérification)
*/
int32_t PopSuccesseur(graphe * g, int32_t i)
/* ====================================================================== */
{
  int32_t s = g->gamma[i]->som;
  RetireTete(&(g->libre), &(g->gamma[i]));
  return s;
} /* PopSuccesseur() */

/* ====================================================================== */
/*! \fn int32_t EstSuccesseur(graphe *g, int32_t i, int32_t s) 
    \param g (entrée) : un graphe.
    \param i (entrée) : un sommet de g.
    \param s (entrée) : un sommet de g.
    \return booléen.
    \brief retourne 1 si le sommet 's' est un successeur du sommet 'i', 0 sinon.
*/
int32_t EstSuccesseur(graphe *g, int32_t i, int32_t s) 
/* ====================================================================== */
{
  return EstDansListe(g->gamma[i], s);
} /* EstSuccesseur() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE MANIPULATION DES ARCS (LISTE D'ARCS) */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void Gamma2ListArcs(graphe *g) 
    \param g (entrée) : un graphe.
    \brief génère la représentation "liste d'arcs" du graphe g.
*/
void Gamma2ListArcs(graphe *g) 
/* ====================================================================== */
{
  int32_t i, a, n = g->nsom, m = g->narc;
  pcell p;

  a = 0;
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      g->tete[a] = i;
      g->queue[a] = p->som;
      g->v_arcs[a] = p->v_arc;
      a++;
    }
} /* Gamma2ListArcs() */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE GENERATION DE GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * GrapheAleatoire(int32_t nsom, int32_t narc)
    \param nsom (entrée) : nombre de sommets.
    \param narc (entrée) : nombre d'arcs.
    \return un graphe.
    \brief retourne un graphe aléatoire à 'nsom' sommets et 'narc' arcs. 
              Le graphe est antisymétrique et sans boucle.
              Le nombre d'arcs doit être <= nsom (nsom - 1) / 2.
              Les arcs sont pondérés (valeur aléatoire entre 0.0 et 1.0).
    \warning la méthode employée ici est naïve, tres inefficace du point de vue
              temps de calcul et ne garantit aucune propriété statistique. 
*/
graphe * GrapheAleatoire(int32_t nsom, int32_t narc)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "GrapheAleatoire"
{
  graphe * g;
  int32_t i, j, m;
  double mmax = ((double)nsom * ((double)nsom - 1)) / 2;

  if (narc > mmax)
  {
    fprintf(stderr, "%s : pas plus de %g arcs pour %d sommets\n", 
                     mmax, nsom, F_NAME);
    exit(0);
  }

  if ((mmax >= (1<<31)) || (narc <= ((int32_t)mmax) / 2))
  {
    g = InitGraphe(nsom, narc);
    while (narc > 0)
    {
      narc--;
      do
      {
        i = rand() % nsom;
        j = rand() % nsom;
      } while ((i == j) || EstSuccesseur(g, i, j) || EstSuccesseur(g, j, i));
      AjouteArc(g, i, j);
      g->tete[narc] = i;
      g->queue[narc] = j;
      if (g->v_arcs) g->v_arcs[narc] = (TYP_VARC)(rand()*100.0);
    }
  }
  else /* on part du graphe complet et on retire des arcs au hasard */
  {
    /* construit un graphe complet antisymetrique sans boucle */
    g = InitGraphe(nsom, (int32_t)mmax);
    for (i = 0; i < nsom ; i++)
      for (j = i+1; j < nsom; j++)
        AjouteArc(g, i, j);         

    /* retire mmax - narc arcs */
    m = (int32_t)mmax;
    while (m > narc)
    {
      m--;
      do
      {
        i = rand() % nsom;
        j = rand() % nsom;
      } while ((i == j) || !EstSuccesseur(g, min(i,j), max(i,j)));
      RetireArc(g, min(i,j), max(i,j));
    }

    /* rajoute la liste des arcs et les poids */
    m = 0;
    for (i = 0; i < nsom ; i++)
      for (j = i+1; j < nsom; j++)
        if (EstSuccesseur(g, i, j))
	{
          g->tete[m] = i;
          g->queue[m] = j;
          if (g->v_arcs) g->v_arcs[narc] = (TYP_VARC)(rand()*100.0);
          m++;
	}
  }

  return g;
} /* GrapheAleatoire() */

/* ====================================================================== */
/* ====================================================================== */
/* OPERATEURS DE BASE SUR LES GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn boolean * EnsembleVide(int32_t n)
    \param n (entrée) : taille du tableau booléen.
    \return un tableau de \b n booléens, tous égaux à FALSE.
    \brief alloue et initialise un tableau de \b n booléens, représentant l'ensemble vide.
*/
boolean * EnsembleVide(int32_t n)
/* ====================================================================== */
{
  boolean *Ca = (boolean *)calloc(n, sizeof(boolean));
  if (Ca == NULL)
  {   fprintf(stderr, "EnsembleVide : calloc failed\n");
      exit(0);
  }
  return Ca;
} /* EnsembleVide() */

/* ====================================================================== */
/*! \fn int32_t * ListeVide(int32_t n)
    \param n (entrée) : taille max de la liste.
    \return un tableau de \b n entiers
    \brief alloue et initialise un tableau de \b n entiers
*/
int32_t * ListeVide(int32_t n)
/* ====================================================================== */
{
  int32_t *Ca = (int32_t *)calloc(n, sizeof(int32_t));
  if (Ca == NULL)
  {   fprintf(stderr, "EnsembleVide : calloc failed\n");
      exit(0);
  }
  return Ca;
} /* ListeVide() */

/* ====================================================================== */
/*! \fn int32_t Degre(graphe * g, int32_t s)
    \param g (entrée) : un graphe
    \param s (entrée) : un sommet
    \return un entier
    \brief retourne le degré du sommet s dans le graphe graphe g (nombre de successeurs)
    \warning seule la représentation 'gamma' est utilisée
*/
int32_t Degre(graphe * g, int32_t s)
/* ====================================================================== */
{
  int32_t d = 0;
  pcell p;
  for (p = g->gamma[s]; p != NULL; p = p->next) d++;
  return d;
} /* Degre() */

/* ====================================================================== */
/*! \fn graphe * Symetrique(graphe * g)
    \param g (entrée) : un graphe
    \return un graphe
    \brief construit et retourne le graphe g_1 symétrique du graphe g (algorithme linéaire)
    \warning seule la représentation 'gamma' est utilisée
*/
graphe * Symetrique(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Symetrique"
{
  graphe *g_1;
  int32_t nsom, narc, al_arcs, k, i, j;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  g_1 = InitGraphe(nsom, narc);
  for (i = 0; i < nsom; i++) /* pour tout i sommet de g */
  {
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout j successeur de i */
      j = p->som;
      AjouteArcValue(g_1, j, i, p->v_arc);
    } // for p
  } // for i

  return g_1;
} /* Symetrique() */

/* ====================================================================== */
/*! \fn graphe * FermetureSymetrique(graphe * g)
    \param g (entrée) : un graphe
    \return un graphe
    \brief construit et retourne la fermeture symétrique du graphe g (algorithme linéaire)
    \warning seule la représentation 'gamma' est utilisée
*/
graphe * FermetureSymetrique(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "FermetureSymetrique"
{
  graphe *gs;
  int32_t nsom, narc, al_arcs, k, i, j;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  gs = InitGraphe(nsom, 2 * narc);
  for (i = 0; i < nsom; i++) /* pour tout i sommet de g */
  {
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout j successeur de i */
      j = p->som;
      AjouteArcValue(gs, i, j, p->v_arc);
      AjouteArcValue(gs, j, i, p->v_arc);
    } // for p
  } // for i

  return gs;
} /* FermetureSymetrique() */

/* ====================================================================== */
/*! \fn void CompFortConnexe(graphe * g, graphe *g_1, int32_t a, boolean * Ca)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique de g
    \param a (entrée) : un sommet du graphe g
    \param Ca (sortie) : un sous-ensemble des sommets de g (tableau de booléens)
    \brief retourne dans Ca la composante fortement connexe de g contenant a 
          (sous la forme d'un tableau de booléens)
    \warning Ca doit avoir été alloué correctement (pas de vérification)
*/
void CompFortConnexe(graphe * g, graphe *g_1, int32_t a, boolean * Ca)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CompFortConnexe"
{
  boolean * D;   /* pour les "descendants" (successeurs a N niveaux) */
  boolean * A;   /* pour les "ascendants" (predecesseurs a N niveaux) */
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int32_t i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  A = EnsembleVide(n);
  D = EnsembleVide(n);
  memset(Ca, 0, n);   // Ca := vide

  /* calcule les descendants de a */
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout sommet s successeur de i */
      s = p->som;
      if (!D[s]) 
      {
        D[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  /* calcule les ascendants de a */
  LifoPush(T, a);
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g_1->gamma[i]; p != NULL; p = p->next) 
    { /* pour tout s successeur de i */
      s = p->som;
      if (!A[s]) 
      {
        A[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  /* intersection de D et de A, union { a } */
  for (i = 0; i < n; i++) Ca[i] = (D[i] && A[i]);
  Ca[a] = TRUE;
    
  free(A);
  free(D);
  LifoTermine(T);
} /* CompFortConnexe() */

/* ====================================================================== */
/*! \fn boolean ExisteCircuit(graphe * g, int32_t a)
    \param g (entrée) : un graphe
    \param a (entrée) : un sommet du graphe g
    \return booléen
    \brief teste l'existence d'un circuit dans g contenant a 
*/
boolean ExisteCircuit(graphe * g, int32_t a)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ExisteCircuit"
{
  boolean * D;   /* pour les "descendants" (successeurs a N niveaux) */
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int32_t i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  D = EnsembleVide(n);

  /* calcule les descendants de a */
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout sommet s successeur de i */
      s = p->som;
      if (s == a) { free(D); LifoTermine(T); return TRUE; }
      if (!D[s]) 
      {
        D[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  free(D);
  LifoTermine(T);
  return FALSE;
} /* ExisteCircuit() */

/* ====================================================================== */
/*! \fn void CompConnexe(graphe * g, graphe *g_1, int32_t a, boolean * Ca)
    \param   g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique de g
    \param a (entrée) : un sommet du graphe g
    \return un sous-ensemble de sommets de g (tableau de booléens)
    \brief retourne la composante connexe de g contenant a
          (sous la forme d'un tableau de booléens)
*/
void CompConnexe(graphe * g, graphe *g_1, int32_t a, boolean * Ca)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CompConnexe"
{
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int32_t i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  memset(Ca, 0, n); // Ca := vide

  Ca[a] = TRUE;
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (!Ca[s]) 
      {
        Ca[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i dans g_1 */
      s = p->som;
      if (!Ca[s]) 
      {
        Ca[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  LifoTermine(T);
} /* CompConnexe() */

/* ====================================================================== */
/*! \fn int32_t Connexe(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique de g
    \return booléen
    \brief retourne 1 si le graphe est connexe, 0 sinon
*/
boolean Connexe(graphe * g, graphe *g_1)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Connexe"
{
  int32_t i;
  boolean ret = TRUE;
  int32_t n = g->nsom;
  boolean * C = EnsembleVide(n);
  CompConnexe(g, g_1, 0, C);
  for (i = 0; i < n; i++) if (!C[i]) { ret = FALSE; break; }
  free(C);
  return ret;
} /* Connexe() */

/* ====================================================================== */
/*! \fn boolean CircuitNiveaux(graphe * g)
    \param g (entrée) : un graphe
    \return booléen
    \brief si le graphe possède au moins un circuit, retourne FALSE ;
    sinon, calcule les niveaux des sommets du graphes (dans v_sommets) et retourne TRUE
*/
boolean CircuitNiveaux(graphe * g, int32_t a)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CircuitNiveaux"
{
  int32_t * D;   /* pour les degrés intérieurs */
  int32_t i, n, s;
  pcell p;

  n = g->nsom;

  // A FINIR

  free(D);
  return FALSE;
} /* CircuitNiveaux() */

/* ====================================================================== */
/* ====================================================================== */
/* ARBRES */
/* ====================================================================== */
/* ====================================================================== */

/*************************************************
  Tri rapide et selection
  D'apres "Introduction a l'algorithmique", 
    T. Cormen, C. Leiserson, R. Rivest, pp. 152, Dunod Ed. 

  Michel Couprie  -  Decembre 1997

  Version avec index et cle - Septembre 1999
  Les donnees cle sont contenues dans un tableau T.
  Le tri s'effectue sur un tableau A contenant les index
    des elements de T.
**************************************************/

#define TypeCle TYP_VARC

/* =============================================================== */
static int Partitionner(int *A, TypeCle *T, int p, int r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements q tq T[A[q]] <= T[A[p]] et les autres.
*/
{
  int t;
  TypeCle x = T[A[p]];
  int i = p - 1;
  int j = r + 1;
  while (1)
  {
    do j--; while (T[A[j]] > x);
    do i++; while (T[A[i]] < x);
    if (i < j) { t = A[i]; A[i] = A[j]; A[j] = t; }
    else return j;
  } /* while (1) */   
} /* Partitionner() */

/* =============================================================== */
static int PartitionStochastique(int *A, TypeCle *T, int p, int r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements k tels que T[A[k]] <= T[A[q]] et les autres, 
  avec q tire au hasard dans [p,r].
*/
{
  int t, q;

  q = p + (rand() % (r - p + 1));
  t = A[p];         /* echange A[p] et A[q] */
  A[p] = A[q]; 
  A[q] = t;
  return Partitionner(A, T, p, r);
} /* PartitionStochastique() */

/* =============================================================== */
/*! \fn void TriRapideStochastique (int * A, TypeCle *T, int p, int r)
    \param A (entrée/sortie) : un tableau d'entiers
    \param T (entrée) : un tableau de valeurs de type TypeCle.
    \param p (entrée) : indice du début de la zone à trier.
    \param r (entrée) : indice de fin de la zone à trier.
    \brief tri par ordre croissant des valeurs du tableau \b T.
           Le tri s'effectue sur un tableau \b A contenant les index
           des elements de \b T, l'indice \b p (compris) à l'indice \b r (compris).
*/
static void TriRapideStochastique (int * A, TypeCle *T, int p, int r)
/* =============================================================== */
{
  int q; 
  if (p < r)
  {
    q = PartitionStochastique(A, T, p, r);
    TriRapideStochastique (A, T, p, q) ;
    TriRapideStochastique (A, T, q+1, r) ;
  }
} /* TriRapideStochastique() */

/* ====================================================================== */
/*! \fn graphe * Kruskal1(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe pondéré connexe antisymétrique antiréflexif
    \param g_1 (entrée) : le graphe symétrique de g
    \return un arbre
    \brief retourne un arbre de poids maximal pour g
*/
graphe * Kruskal1(graphe * g, graphe *g_1)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Kruskal1"
{
  int32_t n = g->nsom;
  int32_t m = g->narc;
  graphe * apm;    /* pour le resultat */
  graphe * apm_1;  /* pour la detection de cycles */
  int32_t *A;          /* tableau pour ranger les index des arcs */
  int32_t i, j, t, q;
  boolean *Ct = EnsembleVide(n);

  /* tri des arcs par poids croissant */
  A = (int32_t *)malloc(m * sizeof(int32_t)); /* allocation index */
  if (A == NULL)
  {   fprintf(stderr, "%s: malloc failed\n", F_NAME);
      exit(0);
  }  
  for (i = 0; i < m; i++) A[i] = i; /* indexation initiale */
  TriRapideStochastique(A, g->v_arcs, 0, m-1);

  /* construit le graphe resultat, initialement vide */
  apm = InitGraphe(n, n-1);
  apm_1 = InitGraphe(n, n-1);

  /* Boucle sur les arcs pris par ordre decroissant:
     on ajoute un arc dans apm, si cela ne cree pas un cycle dans apm.
     Arret lorsque nb arcs ajoutes = n-1. */

  j = 0; 
  i = m - 1;
  while (j < n-1)
  {
    t = g->tete[A[i]]; q = g->queue[A[i]]; 
    CompConnexe(apm, apm_1, t, Ct);
    if (!Ct[q]) 
    {
      AjouteArcValue(apm, t, q, g->v_arcs[A[i]]);
      AjouteArc(apm_1, q, t);
      j++; 
    }
    i--;
    if ((i < 0) && (j < (n-1)))
    {   fprintf(stderr, "%s: graphe d'origine non connexe\n", F_NAME);
        exit(0);
    }  
  } // while (j < n-1)

  if (g->x != NULL) // recopie les coordonnees des sommets pour l'affichage
    for (i = 0; i < n; i++) { apm->x[i] = g->x[i];  apm->y[i] = g->y[i]; }

  free(A);
  free(Ct);
  TermineGraphe(apm_1);
  return apm;
} /* Kruskal1() */

/* ====================================================================== */
/*! \fn graphe * Kruskal2(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe pondéré connexe antisymétrique antiréflexif
    \param g_1 (entrée) : le graphe symétrique de g
    \return un arbre
    \brief retourne un arbre de poids maximal pour g
    \warning les données du graphe d'origine seront détruites
*/
graphe * Kruskal2(graphe * g, graphe *g_1)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Kruskal2"
{
  int32_t n = g->nsom;
  int32_t msav, m = g->narc;
  graphe * apm;    /* pour le resultat */
  int32_t *A;          /* tableau pour ranger les index des arcs */
  int32_t i, j, t, q;
  msav = m;

  if (!Connexe(g, g_1)) 
  {   fprintf(stderr, "%s: graphe d'origine non connexe\n", F_NAME);
      exit(0);
  }  

  /* tri des arcs par poids croissant */
  A = (int32_t *)malloc(m * sizeof(int32_t)); /* allocation index */
  if (A == NULL)
  {   fprintf(stderr, "%s: malloc failed\n", F_NAME);
      exit(0);
  }  
  for (i = 0; i < m; i++) A[i] = i; /* indexation initiale */
  TriRapideStochastique(A, g->v_arcs, 0, m-1);

  /* Boucle sur les arcs pris par ordre croissant:
       on enleve un arc, si cela deconnecte le graphe on le remet.
     Arret lorsque m = n-1. */
  i = 0;
  while (m > n-1)
  {
    t = g->tete[A[i]]; q = g->queue[A[i]]; 
    RetireArc(g, t, q);
    RetireArc(g_1, q, t);
    if (Connexe(g, g_1)) 
      m--; 
    else 
    {
      AjouteArc(g, t, q);
      AjouteArc(g_1, q, t);
    }
    i++;
  } // while (m > n-1)

  /* construit le graphe resultat */
  apm = InitGraphe(n, m);
  for (i = 0, j = 0; i < msav; i++)
  {
    t = g->tete[i]; q = g->queue[i]; 
    if (EstSuccesseur(g, t, q)) 
    {
      AjouteArc(apm, t, q);
      apm->tete[j] = t; 
      apm->queue[j] = q; 
      apm->v_arcs[j] = g->v_arcs[i]; 
      j++;
    }
  } // for i

  if (g->x)
    for (i = 0; i < n; i++) { apm->x[i] = g->x[i];  apm->y[i] = g->y[i]; }

  free(A);
  return apm;
} /* Kruskal2() */

/* ====================================================================== */
/*! \fn void DepthTree(graphe * g, graphe *g_1, int32_t a, TYP_VARC *depth, int32_t *farthest)
    \param g (entrée) : un graphe (arbre), symétrique ou non
    \param a (entrée) : un sommet du graphe g
    \param depth (sortie) : la profondeur de l'arbre de racine a
    \param farthest (sortie) : un sommet le plus éloigné de a 
    \brief calcule la profondeur de l'arbre de racine a
*/
void DepthTree(graphe * g, int32_t a, TYP_VARC *depth, int32_t *farthest)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "DepthTree"
{
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int32_t i, f, n, s;
  pcell p;
  TYP_VARC* dist;
  TYP_VARC d;

  n = g->nsom;
  dist = (TYP_VARC *)malloc(n * sizeof(TYP_VARC));
  T = CreeLifoVide(n);
  for (i = 0; i < n; i++) dist[i] = -1;

  d = dist[a] = 0;
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (dist[s] == -1) 
      {
        dist[s] = dist[i] + p->v_arc;
	if (dist[s] > d) { d = dist[s]; f = s; }
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  free(dist);
  LifoTermine(T);
  *depth = d;
  *farthest = f;
} /* DepthTree() */

/* ====================================================================== */
/*! \fn void MaxDiameterTree(graphe * g)
    \param g (entrée) : un graphe (arbre) symétrique
    \brief calcule un diametre maximal
*/
graphe * MaxDiameterTree(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "MaxDiameterTree"
{
  int32_t n = g->nsom;
  int32_t i, imd;
  TYP_VARC * diam, d, maxdiam = 0;
  int32_t *opp_som, f;
  graphe *pcc;

  diam = (TYP_VARC *)malloc(n * sizeof(TYP_VARC *));
  opp_som = (int32_t *)malloc(n * sizeof(int32_t *));
  for (i = 0; i < n; i++) diam[i] = -1;
  
  for (i = 0; i < n; i++)
    if (Degre(g, i) == 1)
    {
      DepthTree(g, i, &d, &f);
      diam[i] = d;
      opp_som[i] = f;
      if (d > maxdiam) { maxdiam = d; imd = i; }
    }
  Lee(g, imd);
  pcc = PCCna(g, imd, opp_som[imd]);
  return pcc;
} /* MaxDiameterTree() */

/* ====================================================================== */
/*! \fn int32_t LCA(graphe * g, int32_t i, int32_t j)
    \param g (entrée) : un graphe (arbre orienté des feuilles vers la racine)
    \param i (entrée) : un sommet de \b g
    \param j (entrée) : un sommet de \b g
    \brief retourne le plus proche ancêtre commun de i et j (least common ancestor)
*/
int32_t LCA(graphe * g, int32_t i, int32_t j)
/* ====================================================================== */
/* algorithme linéaire, efficace pour quelques requêtes mais 
   pour un algo en temps constant avec pre-processing linéaire voir:
   author = 	 {M.A. Bender and M. Farach-Colton},
   title = 	 {The {LCA} problem revisited},
   booktitle = 	 {procs. Latin American Theoretical Informatics, LNCS, Springer Verlag},
   volume = {1776},
   pages = 	 {88--94},
   year = 	 {2000}   
 */
{
#undef F_NAME
#define F_NAME "LCA"  
#define CHECK_TREE
  int32_t k = -1, n = g->nsom;
  pcell p;
  boolean *M = EnsembleVide(n);

  M[i] = TRUE; // met i et les ancêtres de i dans l'ensemble M
  p = g->gamma[i]; 
  while (p != NULL)
  {
#ifdef CHECK_TREE
    if (p->next != NULL)
    {
      fprintf(stderr,"%s: non-tree structure\n", F_NAME);
      exit(0);
    }
#endif
    i = p->som;
    M[i] = TRUE;
    p = g->gamma[i]; 
  } 

  if (M[j]) k = j; // recherche un element de M parmi les ancêtres de j
  else
  {
    p = g->gamma[j]; 
    while (p != NULL) 
    {
#ifdef CHECK_TREE
      if (p->next != NULL)
      {
	fprintf(stderr,"%s: non-tree structure\n", F_NAME);
	exit(0);
      }
#endif
      j = p->som;
      if (M[j]) { k = j; break; }
      p = g->gamma[j];
    } 
  }

#ifdef CHECK_TREE
  if (k == -1)
  {
    fprintf(stderr,"%s: no LCA found\n", F_NAME);
    exit(0);
  }
#endif

  free(M);
  return k;
} /* LCA() */

/* ====================================================================== */
/*! \fn graphe * RootTree(graphe * g, graphe * g_1, int32_t i)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique du graphe g
    \param i (entrée) : un sommet de \b g
    \brief retourne un arbre orienté (arboresence) de racine i
    dont les arcs correspondent aux arêtes du graphe non-orienté représenté par g et g_1.
    Les distances (nombre d'arcs) de la racine aux sommets sont de plus stockés dans les champs v_sommets.
*/
graphe * RootTree(graphe * g, graphe * g_1, int32_t i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "RootTree"
{
  int32_t k, s, n;
  pcell p;
  Fifo * T;
  graphe * tree;

  if ((g == NULL) || (g_1 == NULL))
  {
    fprintf(stderr,"%s: input graph not available\n", F_NAME);
    exit(0);
  }
  n = g->nsom;

  T = CreeFifoVide(n);
  if (T == NULL)
  {
    fprintf(stderr,"%s: CreeFifoVide failed\n", F_NAME);
    exit(0);
  }

  tree = InitGraphe(n, n-1);
  if (tree == NULL)
  {
    fprintf(stderr,"%s: InitGraphe failed\n", F_NAME);
    exit(0);
  }

  for (k = 0; k < n; k++) tree->v_sommets[k] = -1;
  tree->v_sommets[i] = 0;
  FifoPush(T, i);

  while (!FifoVide(T))
  {
    i = FifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (tree->v_sommets[s] == -1) 
      {
	AjouteArcValue(tree, i, s, p->v_arc);
	tree->v_sommets[s] = tree->v_sommets[i] + 1;
	FifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s prédécesseur de i */
      s = p->som;
      if (tree->v_sommets[s] == -1) 
      {
	AjouteArcValue(tree, i, s, p->v_arc);
	tree->v_sommets[s] = tree->v_sommets[i] + 1;
	FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
  return tree;
} /* RootTree() */

/* ====================================================================== */
/*! \fn void DistRoot(graphe * g, int32_t r)
    \param g (entrée) : un graphe (arbre orienté ou arborescence)
    \param r (entrée) : racine de \b g
    \brief calcule, pour chaque sommet x de g, la longueur (somme des poids d'arcs) d'un plus court
           chemin de r vers x. Cette longueur est stockée dans le champ
           \b v_sommets de \b g
*/
void DistRoot(graphe * g, int32_t r)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "DistRoot"
{
  int32_t k, s, n = g->nsom;
  pcell p;
  Fifo * T = CreeFifoVide(n);

  for (k = 0; k < n; k++) g->v_sommets[k] = -1;
  g->v_sommets[r] = 0;
  FifoPush(T, r);

  while (!FifoVide(T))
  {
    r = FifoPop(T);
    for (p = g->gamma[r]; p != NULL; p = p->next)
    { /* pour tout s successeur de r */
      s = p->som;
      if (g->v_sommets[s] == -1) 
      {
        g->v_sommets[s] = g->v_sommets[r] + p->v_arc;
        FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
} /* DistRoot() */

/* ====================================================================== */
/* ====================================================================== */
/* PLUS COURTS CHEMINS */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/* \fn void Dijkstra1(graphe * g, int32_t i)
    \param g (entrée) : un graphe pondéré. La pondération de chaque arc doit 
                        se trouver dans le champ \b v_arc de la structure \b cell
    \param i (entrée) : un sommet de \b g
    \brief calcule, pour chaque sommet x de g, la longueur d'un plus court
           chemin de i vers x. Cette longueur est stockée dans le champ
           \b v_sommets de \b g
*/
void Dijkstra1(graphe * g, int32_t i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Dijkstra1"
{
  int32_t n = g->nsom;
  boolean * S = EnsembleVide(n);
  int32_t k, x, y;
  pcell p;
  TYP_VSOM vmin;

  S[i] = TRUE;
  for (k = 0; k < n; k++) g->v_sommets[k] = L_INFINITY;
  g->v_sommets[i] = 0;
  k = 1;
  x = i;
  while ((k < n) && (g->v_sommets[x] < L_INFINITY))
  {
    for (p = g->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y successeur de x */
      y = p->som;
      if (!S[y])
      {
        g->v_sommets[y] = min((g->v_sommets[y]),(g->v_sommets[x]+p->v_arc));
      } // if (!S[y])
    } // for p
    // extraire un sommet x hors de S de valeur g->v_sommets minimale
    vmin = L_INFINITY;
    for (y = 0; y < n; y++)
      if (!S[y]) 
        if (g->v_sommets[y] <= vmin) { x = y; vmin = g->v_sommets[y]; }
    k++; 
    S[x] = TRUE;
  } // while ((k < n) && (g->v_sommets[x] < L_INFINITY))

} /* Dijkstra1() */

/* ====================================================================== */
/*! \fn void Dijkstra(graphe * g, int32_t i)
    \param g (entrée) : un graphe pondéré. La pondération de chaque arc doit 
                        se trouver dans le champ \b v_arc de la structure \b cell
    \param i (entrée) : un sommet de \b g
    \brief calcule, pour chaque sommet x de g, la longueur d'un plus court
           chemin de i vers x. Cette longueur est stockée dans le champ
           \b v_sommets de \b g
*/
void Dijkstra(graphe * g, int32_t i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Dijkstra"
{
  int32_t n = g->nsom;
  boolean * S = EnsembleVide(n);
  boolean * TT = EnsembleVide(n);
  int32_t k, x, y;
  pcell p;
  TYP_VSOM vmin;
  int32_t *T = ListeVide(n);
  int32_t tx, ty, tt = 0;

  if ((i >= n) || (i < 0))
  {
    fprintf(stderr, "%s: bad vertex index: %d\n", F_NAME, i);
    exit(0);
  }

  S[i] = TRUE;
  TT[i] = TRUE;
  for (k = 0; k < n; k++) g->v_sommets[k] = L_INFINITY;
  g->v_sommets[i] = 0;
  x = i;
  do
  {
    for (p = g->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y successeur de x */
      y = p->som;
      if (!S[y])
      {
        g->v_sommets[y] = min((g->v_sommets[y]),(g->v_sommets[x]+p->v_arc));
        if (!TT[y]) 
	{  
	  TT[y] = TRUE;
	  T[tt] = y; // insere y dans T
	  tt++;
	}
      } // if (!S[y])
    } // for p

    // extraire un sommet x de T de valeur g->v_sommets minimale
    tx = 0; 
    x = T[0];
    vmin = g->v_sommets[x];
    for (ty = 1; ty < tt; ty++)
    {
      y = T[ty];
      if (g->v_sommets[y] <= vmin) { x = y; tx = ty; vmin = g->v_sommets[y]; }
    }
    S[x] = TRUE;
    for (; tx < tt-1; tx++) T[tx] = T[tx+1]; // retire x de T
    tt--;
  } while (tt > 0);

  free(T);
  free(S);
  free(TT);

} /* Dijkstra() */

/* ====================================================================== */
/*! \fn void Lee(graphe * g, int32_t i)
    \param g (entrée) : un graphe
    \param i (entrée) : un sommet de \b g
    \brief calcule, pour chaque sommet x de g, la longueur (nombre d'arcs) d'un plus court
           chemin de i vers x. Cette longueur est stockée dans le champ
           \b v_sommets de \b g
*/
void Lee(graphe * g, int32_t i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Lee"
{
  int32_t k, s, n = g->nsom;
  pcell p;
  Fifo * T = CreeFifoVide(n);

  for (k = 0; k < n; k++) g->v_sommets[k] = -1;
  g->v_sommets[i] = 0;
  FifoPush(T, i);

  while (!FifoVide(T))
  {
    i = FifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (g->v_sommets[s] == -1) 
      {
        g->v_sommets[s] = g->v_sommets[i] + 1;
        FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
} /* Lee() */

/* ====================================================================== */
/*! \fn void LeeNO(graphe * g, graphe * g_1, int32_t i)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique du graphe g
    \param i (entrée) : un sommet de \b g
    \brief calcule, pour chaque sommet x de g, la longueur (nombre d'arêtes) 
    d'un plus court chemin de i vers x dans le graphe non orienté représenté 
    par (g, g_1). Cette longueur est stockée dans le champ \b v_sommets de \b g
*/
void LeeNO(graphe * g, graphe * g_1, int32_t i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "LeeNO"
{
  int32_t k, s, n = g->nsom;
  pcell p;
  Fifo * T = CreeFifoVide(n);

  for (k = 0; k < n; k++) g->v_sommets[k] = -1;
  g->v_sommets[i] = 0;
  FifoPush(T, i);

  while (!FifoVide(T))
  {
    i = FifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (g->v_sommets[s] == -1) 
      {
        g->v_sommets[s] = g->v_sommets[i] + 1;
        FifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s prédécesseur de i */
      s = p->som;
      if (g->v_sommets[s] == -1) // sic: g
      {
        g->v_sommets[s] = g->v_sommets[i] + 1;  // sic: g
        FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
} /* LeeNO() */

/* ====================================================================== */
/*! \fn graphe * PCC(graphe * g, int32_t d, int32_t a)
    \param g (entrée) : un graphe pondéré, représenté par son application successeurs,
            et dont les sommets ont été valués par la distance au sommet \b d
    \param d (entrée) : un sommet (départ)
    \param a (entrée) : un sommet (arrivée)
    \return un plus court chemin de \b d vers \b a dans \b g , représenté par un graphe
    \brief retourne un plus court chemin de \b d vers \b a dans \b g
*/
graphe * PCC(graphe * g, int32_t d, int32_t a)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "PCC"
{
  int32_t n = g->nsom;
  int32_t m = g->narc;
  graphe * pcc = InitGraphe(n, n-1); /* pour le resultat */
  graphe * g_1 = Symetrique(g);
  int32_t i, y, x = a;
  pcell p;

  while (x != d)
  {
    for (p = g_1->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y predecesseur de x */
      y = p->som;
      if ((g->v_sommets[x]-g->v_sommets[y]) == p->v_arc) 
      {
        AjouteArcValue(pcc, y, x, p->v_arc);
        x = y;
        break;
      }
    } // for p
    if (p == NULL)
    {
      printf("WARNING: pas de pcc trouve de %d vers %d\n", d, a);
      TermineGraphe(g_1);
      return pcc;
    }
  }
  TermineGraphe(g_1);
  for (i = 0; i < n; i++) { pcc->x[i] = g->x[i]; pcc->y[i] = g->y[i]; }
  return pcc;
} /* PCC() */

/* ====================================================================== */
/*! \fn graphe * PCCna(graphe * g, int32_t d, int32_t a)
    \param g (entrée) : un graphe, représenté par son application successeurs,
            et dont les sommets ont été valués par la distance (nombre d'arcs) au sommet \b d
    \param d (entrée) : un sommet (départ)
    \param a (entrée) : un sommet (arrivée)
    \return un plus court chemin (au sens du nombre d'arcs) de \b d vers \b a dans \b g , 
    représenté par un graphe
    \brief retourne un plus court chemin de \b d vers \b a dans \b g
*/
graphe * PCCna(graphe * g, int32_t d, int32_t a)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "PCCna"
{
  int32_t n = g->nsom;
  int32_t m = g->narc;
  graphe * pcc = InitGraphe(n, n-1); /* pour le resultat */
  graphe * g_1 = Symetrique(g);
  int32_t i, y, x = a;
  pcell p;

  while (x != d)
  {
    for (p = g_1->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y predecesseur de x */
      y = p->som;
      if ((g->v_sommets[x]-g->v_sommets[y]) == 1) 
      {
        AjouteArcValue(pcc, y, x, p->v_arc);
        x = y;
        break;
      }
    } // for p
    if (p == NULL)
    {
      printf("WARNING: pas de pcc trouve \n");
      TermineGraphe(g_1);
      return pcc;
    }
  }
  TermineGraphe(g_1);
  for (i = 0; i < n; i++) { pcc->x[i] = g->x[i]; pcc->y[i] = g->y[i]; }
  return pcc;
} /* PCCna() */

/* ====================================================================== */
/* ====================================================================== */
/* PROGRAMME DE TEST */
/* ====================================================================== */
/* ====================================================================== */

#ifdef TEST
int main(int argc, char **argv)
{
  graphe * g, *g_1, *a;
  int32_t s1, s2, na, ns;
  boolean *Cs;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <nombre sommets> <nombre arcs>\n", argv[0]);
    exit(0);
  }

  ns = atoi(argv[1]);
  na = atoi(argv[2]);
  
  g = GrapheAleatoire(ns, na);
  AfficheSuccesseurs(g);
  AfficheArcs(g);
  g_1 = Symetrique(g);
  AfficheSuccesseurs(g_1);
  AfficheArcs(g_1);

  if (Connexe(g, g_1))
  {
    printf("graphe connexe\n");
    a = Kruskal2(g, g_1);
    AfficheSuccesseurs(a);
    AfficheArcs(a);
    TermineGraphe(a);
  }
  else printf("graphe NON connexe\n");

  do
  {
    printf("entrer un sommet : ");
    scanf("%d", &s1);
    if (s1 >= 0) 
    {
      Cs = CompFortConnexe(g, g_1, s1);
      AfficheEnsemble(Cs, g->nsom);
      free(Cs);
    }
  } while (s1 >= 0);

  do
  {
    printf("entrer un sommet : ");
    scanf("%d", &s1);
    if (s1 >= 0) 
    {
      Cs = CompConnexe(g, g_1, s1);
      AfficheEnsemble(Cs, g->nsom);
      free(Cs);
    }
  } while (s1 >= 0);

  TermineGraphe(g);
  TermineGraphe(g_1);

} /* main() */
#endif

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'AFFICHAGE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void AfficheEnsemble(boolean *s, int32_t n)
    \brief affiche à l'écran l'ensemble representé par le tableau de booléens s.
    \param s (entrée) : un tableau de valeurs booléennes.
    \param n (entrée) : la taille du tableau. 
*/
void AfficheEnsemble(boolean *s, int32_t n)
/* ====================================================================== */
{
  int32_t i;
  for (i = 0; i < n; i++) if (s[i]) printf("%d ", i);
  printf("\n");
} /* AfficheEnsemble() */

/* ====================================================================== */
/*! \fn void AfficheListe(pcell p) 
    \param p (entrée) : une liste chaînee de successeurs.
    \brief affiche le contenu de la liste p. */
void AfficheListe(pcell p) 
/* ====================================================================== */
{
  for (; p != NULL; p = p->next) printf("%d (%g); ", p->som, (double)(p->v_arc));
  printf("\n");
} /* AfficheListe() */

/* ====================================================================== */
/*! \fn void AfficheSuccesseurs(graphe * g) 
    \param   g (entrée) : un graphe.
    \brief affiche le graphe dans sa représentation "listes de successeurs". 
*/
void AfficheSuccesseurs(graphe * g) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "AfficheSuccesseurs"
{
  int32_t i;
  
  if (g->gamma)
  {
    for (i = 0; i < g->nsom; i++)
    {
      printf("[%d] ", i);
      AfficheListe(g->gamma[i]);
    }
    printf("\n");
  }
  else fprintf(stderr, "%s: representation successeurs absente\n", F_NAME);
} /* AfficheSuccesseurs() */

/* ====================================================================== */
/*! \fn void AfficheArcs(graphe * g)
    \param   g (entrée) : un graphe.
    \brief affiche le graphe dans sa représentation "listes d'arcs". 
*/
void AfficheArcs(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "AfficheArcs"
{
  int32_t i;

  if (g->tete && g->v_arcs)
  {
    for (i = 0; i < g->narc; i++)
      printf("[%d, %d] %g\n", g->tete[i], g->queue[i], (double)(g->v_arcs[i]));
    printf("\n");
  }
  else if (g->tete)
  {
    for (i = 0; i < g->narc; i++)
      printf("[%d, %d]\n", g->tete[i], g->queue[i]);
    printf("\n");
  }
  else fprintf(stderr, "%s: representation arcs absente\n", F_NAME);
} /* AfficheArcs() */

/* ====================================================================== */
/*! \fn void AfficheValeursSommets(graphe * g)
    \param   g (entrée) : un graphe.
    \brief affiche les valeurs associées aux sommets. 
*/
void AfficheValeursSommets(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "AfficheValeursSommets"
{
  int32_t i;

  if (g->v_sommets)
  {
    for (i = 0; i < g->nsom; i++)
      printf("sommet %d : valeur %g\n", i, (double)(g->v_sommets[i]));
    printf("\n");
  }
  else fprintf(stderr, "%s: valeurs sommets absentes\n", F_NAME);
} /* AfficheValeursSommets() */
