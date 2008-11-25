/* $Id: mcmesh.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* 
  Gestion d'une triangulation
  Michel Couprie  -  Mai 2001
  Update Fevrier 2002 : Edges et lissage courbures
  Update Fevrier 2004 : Addition de bruit gaussien
  Update Fevrier 2004 : RegulMeshHamam
  Update F�vrier 2006 : mesures
  Update Avril 2007 : harmonisation de la d�tection de la convergence
    pour les m�thodes Vollmer et al. et Hamam. 
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcmesh.h>
#include <mcrbt1.h>
#include <mcgeo.h>
#include <mcprobas.h>
#include <mcutil.h>

#ifndef EPSILON
#define EPSILON 1E-20
#endif

#define RMH_EPSILON 1E-2

//#define OPTIMALSTEP
//#define PLOTCOSTFUNCTION

#define DEBUGHAM3
/*
#define DEBUG
*/
#define PARANO
#define VERBOSE
//#define MESURE

#define NITERMAX 20000

meshtabvertices *Vertices = NULL;
meshtabfaces *Faces = NULL;
meshtabedges *Edges = NULL;
meshtablinks *Links = NULL;
Rbt * RBT;

double normevect(double x, double y, double z)
{
  return sqrt(x*x + y*y + z*z);
}

/* ==================================== */
meshtabvertices * AllocVertices(int32_t taillemax)
/* ==================================== */
#undef F_NAME
#define F_NAME "AllocVertices"
{
  meshtabvertices * T = (meshtabvertices *)calloc(1,sizeof(meshtabvertices) + taillemax*sizeof(meshvertex));
  if (T == NULL) 
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  T->lab = (uint8_t *)calloc(taillemax, sizeof(char));
  if (T->lab == NULL) 
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  T->tmp = (uint8_t *)calloc(taillemax, sizeof(char));
  if (T->tmp == NULL) 
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  T->max = taillemax;
  T->cur = 0;
  return T;
} /* AllocVertices() */

/* ==================================== */
void ReAllocVertices(meshtabvertices **A)
/* ==================================== */
{
  int32_t i, taillemax;
  meshtabvertices * T, *Tmp;

  //printf("ReAllocVertices: ancienne taille %d nouvelle taille %d\n", (*A)->max, 2 * (*A)->max);

  taillemax = 2 * (*A)->max;  /* alloue le double de l'ancienne taille */ 
  T = AllocVertices(taillemax);
  T->cur = (*A)->cur;
  memcpy(T->v, (*A)->v, T->cur * sizeof(meshvertex));
  memcpy(T->lab, (*A)->lab, T->cur * sizeof(char));
  memcpy(T->tmp, (*A)->tmp, T->cur * sizeof(char));
  Tmp = *A;
  *A = T;
  free(Tmp->lab);
  free(Tmp->tmp);
  free(Tmp);
} /* ReAllocVertices() */

/* ==================================== */
meshtabfaces * AllocFaces(int32_t taillemax)
/* ==================================== */
#undef F_NAME
#define F_NAME "AllocFaces"
{
  meshtabfaces * T = (meshtabfaces *)calloc(1,sizeof(meshtabfaces) + taillemax*sizeof(meshface));
  if (T == NULL) 
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  T->max = taillemax;
  T->cur = 0;
  return T;
} /* AllocFaces() */

/* ==================================== */
void ReAllocFaces(meshtabfaces **A)
/* ==================================== */
{
  int32_t i, taillemax;
  meshtabfaces * T, *Tmp;

  //printf("ReAllocFaces: ancienne taille %d nouvelle taille %d\n", (*A)->max, 2 * (*A)->max);

  taillemax = 2 * (*A)->max;  /* alloue le double de l'ancienne taille */ 
  T = AllocFaces(taillemax);
  T->cur = (*A)->cur;
  memcpy(T->f, (*A)->f, T->cur * sizeof(meshface));
  Tmp = *A;
  *A = T;
  free(Tmp);
} /* ReAllocFaces() */

/* ==================================== */
meshtabedges * AllocEdges(int32_t taillemax)
/* ==================================== */
#undef F_NAME
#define F_NAME "AllocEdges"
{
  meshtabedges * T = (meshtabedges *)calloc(1,sizeof(meshtabedges) + taillemax*sizeof(meshedge));
  if (T == NULL) 
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  T->max = taillemax;
  T->cur = 0;
  return T;
} /* AllocEdges() */

/* ==================================== */
void ReAllocEdges(meshtabedges **A)
/* ==================================== */
{
  int32_t i, taillemax;
  meshtabedges * T, *Tmp;

  //printf("ReAllocEdges: ancienne taille %d nouvelle taille %d\n", (*A)->max, 2 * (*A)->max);

  taillemax = 2 * (*A)->max;  /* alloue le double de l'ancienne taille */ 
  T = AllocEdges(taillemax);
  T->cur = (*A)->cur;
  memcpy(T->e, (*A)->e, T->cur * sizeof(meshedge));
  Tmp = *A;
  *A = T;
  free(Tmp);
} /* ReAllocEdges() */

/* ==================================== */
meshtablinks * AllocLinks(int32_t nvert, int32_t nedge)
/* ==================================== */
#undef F_NAME
#define F_NAME "AllocLinks"
{
  meshtablinks * T = (meshtablinks *)calloc(1,sizeof(meshtablinks));
  if (T == NULL) 
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  T->nvert = nvert;
  T->nedge = nedge;
  T->lastneigh = (int32_t *)calloc(1,nvert * sizeof(int32_t));
  T->neigh = (int32_t *)calloc(1,nedge * sizeof(int32_t));
  if ((T->lastneigh == NULL) || (T->neigh == NULL))
  {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return NULL;
  }
  return T;
} /* AllocLinks() */

/* ==================================== */
void InitMesh(int32_t taillemax)
/* ==================================== */
{
  Vertices = AllocVertices(taillemax);
  Faces = AllocFaces(taillemax);
  RBT = CreeRbtVide(taillemax);
} /* InitMesh() */

/* ==================================== */
void TermineMesh()
/* ==================================== */
{
  free(Vertices->lab);
  free(Vertices->tmp);
  free(Vertices);
  free(Faces);
  if (Edges) free(Edges);
  if (Links) { free(Links->lastneigh); free(Links->neigh); free(Links); }
  RbtTermine(RBT);
} /* TermineMesh() */

/* ==================================== */
int32_t NotIn(
  int32_t e,
  int32_t *list,                   
  int32_t n)                       
/* ==================================== */
{
/* renvoie 1 si e n'est pas dans list, 0 sinon */
/* e : l'element a rechercher */
/* list : la liste (tableau d'entiers) */
/* n : le nombre d'elements dans la liste */
  while (n > 0)
    if (list[--n] == e) return 0;
  return 1;
} /* NotIn() */

/* ==================================== */
int32_t AddVertex(double x, double y, double z, int32_t indface)
/* ==================================== */
/* modifie les var. globales Faces, Vertices */
#undef F_NAME
#define F_NAME "AddVertex"
{
  int32_t i;
  TypRbtKey point;
  RbtElt * re;

  /* cherche si le sommet est deja la */
  point.x = x;
  point.y = y;
  point.z = z;
  re = RbtSearch(RBT, point); 
  if (re != RBT->nil)
  {
    i = re->auxdata; /* index du vertex */
    /* il est la : on lui ajoute la face si elle n'y est pas deja */
    if (NotIn(indface, Vertices->v[i].face, Vertices->v[i].nfaces)) 
    {
      if (Vertices->v[i].nfaces >= MAXADJFACES)
      {
        fprintf(stderr, "%s : WARNING: more than %d faces\n", F_NAME, MAXADJFACES);
        fprintf(stderr, "x=%g, y=%g, z=%g\n", x, y, z);
        goto skipadd;
      }
      Vertices->v[i].face[ Vertices->v[i].nfaces++ ] = indface;
    }
skipadd:
    return i;
  } /* if (re != RBT->nil) */
  if (Vertices->cur >= Vertices->max) ReAllocVertices(&Vertices);
  i = Vertices->cur;
  Vertices->cur += 1;
  Vertices->v[i].x = x;
  Vertices->v[i].y = y;
  Vertices->v[i].z = z;
  Vertices->v[i].face[ 0 ] = indface;
  Vertices->v[i].nfaces = 1;
  (void)RbtInsert(&RBT, point, i);
  return i;
} /* AddVertex() */


/* ==================================== */
int32_t AddVertexFixe(double x, double y, double z, int32_t indface)
/* ==================================== */
/* modifie les var. globales Faces, Vertices */
{
  int32_t i;
  i = AddVertex(x, y, z, indface);
  Vertices->lab[i] = 1;
  return i;
} /* AddVertexFixe() */

/* ==================================== */
void AddFace(double x1, double y1, double z1, 
             double x2, double y2, double z2, 
             double x3, double y3, double z3
            )
/* ==================================== */
/* modifie les var. globales Faces, Vertices */
{
  int32_t iv1, iv2, iv3, i, indface;
  if (Faces->cur >= Faces->max) ReAllocFaces(&Faces);
  indface = Faces->cur;
  Faces->cur += 1;
  iv1 = AddVertex(x1, y1, z1, indface);
  iv2 = AddVertex(x2, y2, z2, indface);
  iv3 = AddVertex(x3, y3, z3, indface);
  Faces->f[indface].vert[0] = iv1;
  Faces->f[indface].vert[1] = iv2;
  Faces->f[indface].vert[2] = iv3;
  Faces->f[indface].xn = Faces->f[indface].yn = Faces->f[indface].zn = 0.0;
} /* AddFace() */

/* ==================================== */
void AddFaceFixe(double x1, double y1, double z1, 
                 double x2, double y2, double z2, 
                 double x3, double y3, double z3,
                 int32_t fix1, int32_t fix2, int32_t fix3
                )
/* ==================================== */
/* modifie les var. globales Faces, Vertices */
{
  int32_t iv1, iv2, iv3, i, indface;
  if (Faces->cur >= Faces->max) ReAllocFaces(&Faces);
  indface = Faces->cur;
  Faces->cur += 1;
  if (fix1)
    iv1 = AddVertexFixe(x1, y1, z1, indface);
  else 
    iv1 = AddVertex(x1, y1, z1, indface);
  if (fix2)
    iv2 = AddVertexFixe(x2, y2, z2, indface);
  else 
    iv2 = AddVertex(x2, y2, z2, indface);
  if (fix3)
    iv3 = AddVertexFixe(x3, y3, z3, indface);
  else 
    iv3 = AddVertex(x3, y3, z3, indface);
  Faces->f[indface].vert[0] = iv1;
  Faces->f[indface].vert[1] = iv2;
  Faces->f[indface].vert[2] = iv3;
  Faces->f[indface].xn = Faces->f[indface].yn = Faces->f[indface].zn = 0.0;
} /* AddFaceFixe() */

/* ==================================== */
int32_t AddEdge(int32_t v1, int32_t v2, int32_t f1, int32_t f2)
/* ==================================== */
/* modifie la var. globales Edges */
{
  int32_t indedge;
  if (Edges->cur >= Edges->max) ReAllocEdges(&Edges);
  indedge = Edges->cur;
  Edges->cur += 1;
  Edges->e[indedge].v1 = v1;
  Edges->e[indedge].v2 = v2;
  Edges->e[indedge].f1 = f1;
  Edges->e[indedge].f2 = f2;
  return indedge;
} /* AddEdge() */

/* ==================================== */
void SaveCoords()
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
  {
    Vertices->v[i].xp = Vertices->v[i].x;
    Vertices->v[i].yp = Vertices->v[i].y;
    Vertices->v[i].zp = Vertices->v[i].z;
  }
} /* SaveCoords() */

/* ==================================== */
void SaveOriginalCoords()
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }
} /* SaveOriginalCoords() */

/* ==================================== */
void RestoreCoords()
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
  {
    Vertices->v[i].x = Vertices->v[i].xp;
    Vertices->v[i].y = Vertices->v[i].yp;
    Vertices->v[i].z = Vertices->v[i].zp;
  }
} /* RestoreCoords() */

/* ==================================== */
void ComputeEdges()
/* ==================================== */
/*
  Contruit le tableau des cotes (edges),
  et met a jour le champ 'edge' des sommets.
*/
#undef F_NAME
#define F_NAME "ComputeEdges"
{
  int32_t i, j, k, n, e, nvertices;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  int32_t f1, f2;

  if (Edges == NULL) 
  {
    fprintf(stderr, "%s : Edges array must be allocated\n", F_NAME);
    exit(0);
  }  
  nvertices = Vertices->cur;
  for (i = 0; i < nvertices; i++) Vertices->v[i].nedges = 0;
  for (i = 0; i < nvertices; i++)
  {
    V = Vertices->v[i];
    n = 0;
    for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
    {                                         /* et calcule le link */
      F = Faces->f[V.face[j]];
      k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
      k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
      k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
    } /* for j */

    for (k = 0; k < n; k++)   /* parcourt le link et cree les cotes */
    {
      if (link[k] > i) /* pour ne compter un cote qu'une seule fois */
      {
        f2 = f1 = -1;
        for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
        {                    /* et trouve les 2 qui contiennent link[k] */
          F = Faces->f[V.face[j]];
          if ((F.vert[0] == link[k]) || (F.vert[1] == link[k]) || (F.vert[2] == link[k]))
	  {
            if (f1 == -1) f1 = V.face[j]; else { f2 = V.face[j]; break; }
	  }
        } /* for j */
        e = AddEdge(i, link[k], f1, f2);
        Vertices->v[i].edge[Vertices->v[i].nedges++] = e;
        Vertices->v[link[k]].edge[Vertices->v[link[k]].nedges++] = e;
      } /* if */    
    } /* for k */
  } /* for i */
} /* ComputeEdges() */

/* ==================================== */
void ComputeLinks()
/* ==================================== */
/*
  Contruit le graphe des voisins (links),
*/
#undef F_NAME
#define F_NAME "ComputeLinks"
{
  int32_t i, j, k, n, e, nvertices;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  int32_t f1, f2;

  if (Links != NULL) 
  {
    fprintf(stderr, "%s : Error : Links already exists\n", F_NAME);
    exit(0);
  }  
  nvertices = Vertices->cur;
  e = 0; // compte le nombre total de relations "successeur"
  for (i = 0; i < nvertices; i++)
  {
    V = Vertices->v[i];
    n = 0;
    for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
    {                                         /* et calcule le link */
      F = Faces->f[V.face[j]];
      k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
      k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
      k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
    } /* for j */
    e += n;
  }
  Links = AllocLinks(nvertices, e);
  if (Links == NULL) 
  {
    fprintf(stderr, "%s : AllocLinks failed\n", F_NAME);
    exit(0);
  }  
  e = 0; // pour indicer le tableau "neigh"
  for (i = 0; i < nvertices; i++)
  {
    V = Vertices->v[i];
    n = 0;
    for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
    {                                         /* et calcule le link */
      F = Faces->f[V.face[j]];
      k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
      k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
      k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
    } /* for j */
    for (k = 0; k < n; k++)   /* stocke le link */
      Links->neigh[e++] = link[k];
    Links->lastneigh[i] = e-1;
  } /* for i */
} /* ComputeLinks() */

/* ==================================== */
void NormaleFace(int32_t f, vec3 normale)
/* ==================================== */
#undef F_NAME
#define F_NAME "NormalFace"
{
  int32_t s0, s1, s2;
  meshface F = Faces->f[f];
  double norm;
  vec3 v1, v2;

  s0 = F.vert[0]; s1 = F.vert[1]; s2 = F.vert[2];

  /* calcule les vecteurs: v1 = s0 - s1, v2 = s0 - s2 */
  v1[0] = Vertices->v[s0].x - Vertices->v[s1].x;
  v1[1] = Vertices->v[s0].y - Vertices->v[s1].y;
  v1[2] = Vertices->v[s0].z - Vertices->v[s1].z;
  v2[0] = Vertices->v[s0].x - Vertices->v[s2].x;
  v2[1] = Vertices->v[s0].y - Vertices->v[s2].y;
  v2[2] = Vertices->v[s0].z - Vertices->v[s2].z;

  /* normale face: produit vectoriel de v1 et de v2 */
  normale[0] = v1[1] * v2[2] - v1[2] * v2[1];
  normale[1] = v1[2] * v2[0] - v1[0] * v2[2];
  normale[2] = v1[0] * v2[1] - v1[1] * v2[0];

  /* normalise la normale */
  norm = normale[0] * normale[0] + normale[1] * normale[1] + normale[2] * normale[2];
  norm = sqrt(norm);

  if (norm < EPSILON)
  {
    fprintf(stderr, "%s: warning: cannot compute normal\n", F_NAME);
  }
  else 
  {
    normale[0] = - normale[0] / norm; // signe - pour tourner la normale vers l'exterieur
    normale[1] = - normale[1] / norm;
    normale[2] = - normale[2] / norm;
  }

} // NormaleFace() 

/* ==================================== */
double AngleFaces(int32_t f1, int32_t f2)
/* ==================================== */
#undef F_NAME
#define F_NAME "AngleFaces"
{
  vec3 n1, n2;
  double s;
  mat33 m, mr;
  meshface F1 = Faces->f[f1];
  meshface F2 = Faces->f[f2];
  int32_t a, b, c; // indices des trois sommets formant la face f1, 
               // b et c etant communs a f1 et f2

  NormaleFace(f1, n1);
  NormaleFace(f2, n2);
  
  s = n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2];

  // retrouve les sommets a, b, c
  a = F1.vert[0]; 
  if ((a != F2.vert[0]) && (a != F2.vert[1]) && (a != F2.vert[2]))
  { b = F1.vert[1]; c = F1.vert[2]; }
  else
  {
    b = a;
    a = F1.vert[1];
    if ((a != F2.vert[0]) && (a != F2.vert[1]) && (a != F2.vert[2]))
      c = F1.vert[2];
    else
    {
      c = a;
      a = F1.vert[2];
    }
  }

  // calcule les vecteurs: b - a, b - c
  // et les range dans les 2 premieres colonnes de m
  m[0][0] = Vertices->v[b].x - Vertices->v[a].x;
  m[1][0] = Vertices->v[b].y - Vertices->v[a].y;
  m[2][0] = Vertices->v[b].z - Vertices->v[a].z;
  m[0][1] = Vertices->v[b].x - Vertices->v[c].x;
  m[1][1] = Vertices->v[b].y - Vertices->v[c].y;
  m[2][1] = Vertices->v[b].z - Vertices->v[c].z;
  // troisieme colonne: la normale n1
  m[0][2] = n1[0];
  m[1][2] = n1[1];
  m[2][2] = n1[2];
  // matrice de changement de base inverse: mr
  if (invmat3(m, mr) == 0)
  {
    fprintf(stderr, "%s: erreur matrice non inversible\n", F_NAME);
    exit(0);    
  }
  // coordonnees de n2 dans la base de la face f1 (resultat dans n1)
  multmat3vec3(mr, n2, n1);
  // l'angle est positif si la 1ere coordonnee du resultat l'est
  if (s > 1.0) s = 1.0;
  if (n1[0] >= 0)
    return acos(s);
  else
    return -acos(s);
} // AngleFaces()

/* ==================================== */
double MaxAngleFaces()
/* ==================================== */
#undef F_NAME
#define F_NAME "MaxAngleFaces"
{
  int32_t i;
  int32_t f1, f2;
  double angle, maxangle = 0.0;
  if (Edges == NULL) 
  {
    fprintf(stderr, "%s : Edges must be computed\n", F_NAME);
    exit(0);
  }  
  for (i = 0; i < Edges->cur; i++)
  {
    f1 = (Edges->e[i]).f1;
    f2 = (Edges->e[i]).f2;
    angle = AngleFaces(f1,f2);
    angle = mcabs(angle);
    if (angle > maxangle) maxangle = angle;
  }
  return maxangle;
} // MaxAngleFaces()

/* ==================================== */
void MeanAngleFaces(double *mean, double *standev)
/* ==================================== */
#undef F_NAME
#define F_NAME "MeanAngleFaces"
{
  int32_t i, N = Edges->cur;
  int32_t f1, f2;
  double angle, m1 = 0.0, m2 = 0.0;
  if (Edges == NULL) 
  {
    fprintf(stderr, "%s : Edges must be computed\n", F_NAME);
    exit(0);
  }  
  for (i = 0; i < N; i++)
  {
    f1 = (Edges->e[i]).f1;
    f2 = (Edges->e[i]).f2;
    angle = AngleFaces(f1,f2);
    angle = mcabs(angle);
    m1 += angle;
    m2 += angle * angle;
  }
  *mean = m1 / N;
  *standev = sqrt((m2 - (*mean * *mean) / N) / N);
} // MeanAngleFaces()

/* ==================================== */
double MaxLengthEdges()
/* ==================================== */
#undef F_NAME
#define F_NAME "MaxLengthEdges"
{
  int32_t i;
  int32_t v1, v2;
  double length, maxlength = 0.0;
  if (Edges == NULL) 
  {
    fprintf(stderr, "%s : Edges must be computed\n", F_NAME);
    exit(0);
  }  
  for (i = 0; i < Edges->cur; i++)
  {
    v1 = (Edges->e[i]).v1;
    v2 = (Edges->e[i]).v2;
    length = dist3(Vertices->v[v1].x, Vertices->v[v1].y, Vertices->v[v1].z,
		   Vertices->v[v2].x, Vertices->v[v2].y, Vertices->v[v2].z);
    if (length > maxlength) maxlength = length;
  }
  return maxlength;
} // MaxLengthEdges()

/* ==================================== */
void ComputeCurvatures()
/* ==================================== */
/*
  Parcourt le tableau des cotes (edges) et calcule, le cas echeant,
  l'angle entre les deux facettes adjacentes a chaque cote.
*/
{
  int32_t i;
  meshedge E;

  for (i = 0; i < Edges->cur; i++)
  {
    E = Edges->e[i];
    if (E.f2 != -1)
      Edges->e[i].curv = AngleFaces(E.f1, E.f2);
  }
} // ComputeCurvatures()

/* ==================================== */
int32_t OppositeVertex(int32_t f, int32_t e)
/* ==================================== */
// retourne le vertex de la face f oppose au cote e
{
  int32_t a = Edges->e[e].v1;   
  int32_t b = Edges->e[e].v2;   
  meshface F = Faces->f[f];
  if ((F.vert[0] != a) && (F.vert[0] != b)) return F.vert[0];
  if ((F.vert[1] != a) && (F.vert[1] != b)) return F.vert[1];
  return F.vert[2];
} // OppositeVertex()

/* ==================================== */
double CurvVoisEdges(int32_t v)
/* ==================================== */
// retourne la moyenne des courbures des cotes "voisins" de v:
// - sens 1 : cotes adjacents
// - sens 2 : cotes opposes a v pour les faces adjacentes
// implemente pour le moment le sens 1
{
  double curv;
  int32_t i, ne, e;

  //printf("vertex %d : ", v);
  ne = Vertices->v[v].nedges;
  for (i = 0; i < ne; i++)
  {
    e = Vertices->v[v].edge[i];
    //printf("(%d %g) ", e, Edges->e[e].curv);
    curv += Edges->e[e].curv;
  }
  curv /= ne;
  //printf(" moy: %g\n", curv);
  return curv;
} // CurvVoisEdges()

/* ==================================== */
void MoveVertex(int32_t v, int32_t f, int32_t e, double dc)
/* ==================================== */
// calcule le deplacement du vertex v, egal a la normale a la face f, 
// ponderee par un facteur beta = dc * r / 5
// ou r est la distance de v au cote e et dc est la difference de courbure
// le deplacement est cumule dans les champs xp, yp, zp du vertex,
// et le nombre de deplacements est stocke dans le champ tmp associe au vertex.
{
  double nx = Faces->f[f].xn;
  double ny = Faces->f[f].yn;
  double nz = Faces->f[f].zn;
  point3 e1, e2, p;
  double r, beta;

  e1.x = Vertices->v[Edges->e[e].v1].x;
  e1.y = Vertices->v[Edges->e[e].v1].y;
  e1.z = Vertices->v[Edges->e[e].v1].z;
  e2.x = Vertices->v[Edges->e[e].v2].x;
  e2.y = Vertices->v[Edges->e[e].v2].y;
  e2.z = Vertices->v[Edges->e[e].v2].z;
  p.x = Vertices->v[v].x;
  p.y = Vertices->v[v].y;
  p.z = Vertices->v[v].z;
  r = distpointdroite3(p, e1, e2);  
  beta = (dc * r) / 5.0;

  //printf("sommet %d cote %d distance %g beta %g\n", v, e, r, beta);

  Vertices->tmp[v] += 1;
  Vertices->v[v].xp += beta * nx;
  Vertices->v[v].yp += beta * ny;
  Vertices->v[v].zp += beta * nz;
} // MoveVertex()

/* ==================================== */
void AddNoiseMesh(double alpha)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
  {

    Vertices->v[i].x += Normal(0.0, alpha);
    Vertices->v[i].y += Normal(0.0, alpha);
    Vertices->v[i].z += Normal(0.0, alpha);
  }
} /* AddNoiseMesh() */

/* ==================================== */
void LissageMesh()
/* ==================================== */
/* 
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
*/
{
#define MAXVOISEDGES 100
  int32_t e, i, f1, f2, v1, v2;
  double c, cv;

  //printf("%d %d\n", Vertices->cur, Edges->cur);

  for (i = 0; i < Vertices->cur; i++)
  {
    Vertices->tmp[i] = 0;
    Vertices->v[i].xp = Vertices->v[i].yp = Vertices->v[i].zp = 0.0;
  }

  for (e = 0; e < Edges->cur; e++)
  {
    c = Edges->e[e].curv;
    f2 = Edges->e[e].f2;
    if (f2 == -1) goto next;
    f1 = Edges->e[e].f1;
    v1 = OppositeVertex(f1, e);
    //printf("Vertices->lab[%d] = %d\n", v1, Vertices->lab[v1]);
    if (Vertices->lab[v1] == 0)
    {
      cv = CurvVoisEdges(v1);
      //printf("e = %d, c = %g, cv1 = %g\n", e, c, cv);
      if ((c >= 0) && (c > cv)) MoveVertex(v1, f1, e, c - cv);
      if ((c <= 0) && (c < cv)) MoveVertex(v1, f1, e, c - cv);
    }

    v2 = OppositeVertex(f2, e);
    if (Vertices->lab[v2] == 0)
    {
      cv = CurvVoisEdges(v2);
      //printf("e = %d, c = %g, cv2 = %g\n", e, c, cv);
      if ((c >= 0) && (c > cv)) MoveVertex(v2, f2, e, c - cv);
      if ((c <= 0) && (c < cv)) MoveVertex(v2, f2, e, c - cv);
    }
    next:;
  }

  for (i = 0; i < Vertices->cur; i++)
  if (Vertices->tmp[i] > 0)
  {
    Vertices->v[i].x += (Vertices->v[i].xp / Vertices->tmp[i]);
    Vertices->v[i].y += (Vertices->v[i].yp / Vertices->tmp[i]);
    Vertices->v[i].z += (Vertices->v[i].zp / Vertices->tmp[i]);
  }

} /* LissageMesh() */

/* ==================================== */
void RegulMeshLaplacian(int32_t niters)
/* ==================================== */
/* 
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
*/
#undef F_NAME
#define F_NAME "RegulMeshLaplacian"
{
  int32_t i, j, k, n, iter, a;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz, norme, alpha;

  a = 0; // calcule a = nb max de voisins pour 1 vertex
  for (i = 0; i < Vertices->cur; i++)
    if (Vertices->v[i].nfaces > a) a = Vertices->v[i].nfaces; 
  alpha = 1.0 / (4 * a);

#ifdef MESURE
  Edges = AllocEdges(1);
  ComputeEdges();
#endif

  for (iter = 0; iter < niters; iter++)
  {
#ifdef VERBOSE
    printf("%s: iter %d\n", F_NAME, iter);
#endif
#ifdef MESURE
    {
      double mean, stddev, meandc = MeanDistCenter();
      MeanAngleFaces(&mean, &stddev);
      printf("meanangle = %g ; std. deviation = %g ; mean dist center = %g\n", 
	     mean, stddev, meandc);
    }
#endif
    for (i = 0; i < Vertices->cur; i++)
    {
      if (Vertices->lab[i] == 0)
      {                              /* pour chaque sommet de la grille dont le label est nul */
        V = Vertices->v[i];
        n = 0;
        for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
          {                                      /* et calcule le link */
          F = Faces->f[V.face[j]];
          k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
        } /* for j */
#ifdef WARN_NON_CYCLE
        if (n != V.nfaces)
        {
          fprintf(stderr, "%s: warning: link[%d] non cycle\n", F_NAME, i);
        }
#endif                              
        sx = sy = sz = 0.0;
        for (j = 0; j < n; j++)
        {
          sx += Vertices->v[link[j]].x;
          sy += Vertices->v[link[j]].y;
          sz += Vertices->v[link[j]].z;
        } /* for j */
        x = sx / n; y = sy / n; z = sz / n;
#ifdef PAS_INDEPT_NB_VOIS
        Vertices->v[i].xp = (x + Vertices->v[i].x) / 2;
        Vertices->v[i].yp = (y + Vertices->v[i].y) / 2;
        Vertices->v[i].zp = (z + Vertices->v[i].z) / 2;
#endif
        Vertices->v[i].xp = (1.0 - n * alpha) * Vertices->v[i].x + n * alpha * x;
        Vertices->v[i].yp = (1.0 - n * alpha) * Vertices->v[i].y + n * alpha * y;
        Vertices->v[i].zp = (1.0 - n * alpha) * Vertices->v[i].z + n * alpha * z;
      } /* if ... */
      else
      {
        Vertices->v[i].xp = Vertices->v[i].x;
        Vertices->v[i].yp = Vertices->v[i].y;
        Vertices->v[i].zp = Vertices->v[i].z;
      } /* else */
    } /* for i */
    // stocke le resultat dans (x,y,z) 
    for (i = 0; i < Vertices->cur; i++)
    {
      Vertices->v[i].x = Vertices->v[i].xp;
      Vertices->v[i].y = Vertices->v[i].yp;
      Vertices->v[i].z = Vertices->v[i].zp;
    }
  } // for (iter = 0; iter < niters; iter++)
} /* RegulMeshLaplacian() */

/* ==================================== */
void RegulMeshHamam(double theta)
/* ==================================== */
/* 
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
   Methode de Hamam & al [HC06]
*/
#undef F_NAME
#define F_NAME "RegulMeshHamam"
{
  int32_t a, n, i, j, k, iter, nv;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz, alphax, alphay, alphaz;
  double *tx, *ty, *tz;    // resultats intermediaires
  int32_t nitermax = NITERMAX; // garde-fou
  double normgradx, normgrady, normgradz, divisorx, divisory, divisorz;
  int32_t stabilite;
#ifdef OPTIMALSTEP
  double *ux, *uy, *uz;    // resultats intermediaires
#endif
  double *gx, *gy, *gz; // pour calculer la norme du vecteur des modifications

  gx = (double *)calloc(1,Vertices->cur * sizeof(double));
  gy = (double *)calloc(1,Vertices->cur * sizeof(double));
  gz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((gx == NULL) || (gy == NULL) || (gz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  tx = (double *)calloc(1,Vertices->cur * sizeof(double));
  ty = (double *)calloc(1,Vertices->cur * sizeof(double));
  tz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((tx == NULL) || (ty == NULL) || (tz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

#ifdef OPTIMALSTEP
  ux = (double *)calloc(1,Vertices->cur * sizeof(double));
  uy = (double *)calloc(1,Vertices->cur * sizeof(double));
  uz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((ux == NULL) || (uy == NULL) || (uz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }
#endif

  for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees initiales
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }

  a = 0; // calcule a = nb max de voisins pour 1 vertex
  for (i = 0; i < Vertices->cur; i++)
    if (Vertices->v[i].nfaces > a) a = Vertices->v[i].nfaces; 
  // calcule alpha pour une convergence monotone
  alphax = alphay = alphaz = 1.0 / (1.0 + 4.0 * a * a * theta);
  if (Links == NULL) ComputeLinks();
  iter = 0;
  stabilite = 0;
  while ((stabilite == 0) && (iter < nitermax))
  {
    stabilite = 1;
    iter++;
#ifdef DEBUG
    fprintf(stderr, "%s: iteration %d\n", F_NAME, iter);
#endif

    for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees en debut d'iteration
    {
      gx[i] = Vertices->v[i].x;
      gy[i] = Vertices->v[i].y;
      gz[i] = Vertices->v[i].z;
    }

    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule A x --- resultat dans tx, ty, tz
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * Vertices->v[i].x;
      sy = nv * Vertices->v[i].y;
      sz = nv * Vertices->v[i].z;
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= Vertices->v[j].x;
        sy -= Vertices->v[j].y;
        sz -= Vertices->v[j].z;
      }
      tx[i] = sx; ty[i] = sy; tz[i] = sz; 
    } // for (i = 0; i < Vertices->cur; i++)
    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule theta A^2 x --- resultat dans Vertices->v[].xp, yp, zp;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * tx[i];
      sy = nv * ty[i];
      sz = nv * tz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= tx[j];
        sy -= ty[j];
        sz -= tz[j];
      }
      Vertices->v[i].xp = theta * sx; 
      Vertices->v[i].yp = theta * sy; 
      Vertices->v[i].zp = theta * sz; 
    } // for (i = 0; i < Vertices->cur; i++)

#ifdef PLOTCOSTFUNCTION
 {
   double J = 0.0;
   for (i = 0; i < Vertices->cur; i++)
   {
     tx[i] = (Vertices->v[i].x - Vertices->v[i].xo) * 
             (Vertices->v[i].x - Vertices->v[i].xo) + 
              Vertices->v[i].x * Vertices->v[i].xp;
     ty[i] = (Vertices->v[i].y - Vertices->v[i].yo) * 
             (Vertices->v[i].y - Vertices->v[i].yo) + 
              Vertices->v[i].y * Vertices->v[i].yp;
     tz[i] = (Vertices->v[i].z - Vertices->v[i].zo) * 
             (Vertices->v[i].z - Vertices->v[i].zo) + 
              Vertices->v[i].z * Vertices->v[i].zp;
     J += tx[i]*tx[i] + ty[i]*ty[i] + tz[i]*tz[i]; 
   } // for (i = 0; i < Vertices->cur; i++)
   printf("%g\n", J);
 }
#endif

    normgradx = normgrady = normgradz = 0.0;
    for (i = 0; i < Vertices->cur; i++) // calcule le gradient nabla J  --- resultat dans t?[];
    {
      tx[i] = Vertices->v[i].x - Vertices->v[i].xo + Vertices->v[i].xp;
      ty[i] = Vertices->v[i].y - Vertices->v[i].yo + Vertices->v[i].yp;
      tz[i] = Vertices->v[i].z - Vertices->v[i].zo + Vertices->v[i].zp;

      normgradx += tx[i]*tx[i];
      normgrady += ty[i]*ty[i];
      normgradz += tz[i]*tz[i]; 
    } // for (i = 0; i < Vertices->cur; i++)

#ifdef OPTIMALSTEP
    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule A J --- resultat dans ux, uy, uz;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * tx[i];
      sy = nv * ty[i];
      sz = nv * tz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= tx[j];
        sy -= ty[j];
        sz -= tz[j];
      }
      ux[i] = sx; 
      uy[i] = sy; 
      uz[i] = sz; 
    } // for (i = 0; i < Vertices->cur; i++)

    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule (I + theta A^2) J --- resultat dans Vertices->v[].xp, yp, zp;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * ux[i];
      sy = nv * uy[i];
      sz = nv * uz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= ux[j];
        sy -= uy[j];
        sz -= uz[j];
      }
      Vertices->v[i].xp = tx[i] + (theta * sx); 
      Vertices->v[i].yp = ty[i] + (theta * sy); 
      Vertices->v[i].zp = tz[i] + (theta * sz); 
    } // for (i = 0; i < Vertices->cur; i++)

    divisorx = divisory = divisorz = 0.0;
    for (i = 0; i < Vertices->cur; i++) // calcule J^t (I + theta A^2) J
    {
      divisorx += Vertices->v[i].xp * tx[i];
      divisory += Vertices->v[i].yp * ty[i];
      divisorz += Vertices->v[i].zp * tz[i];
    } // for (i = 0; i < Vertices->cur; i++)
    alphax = normgradx / divisorx;
    alphay = normgrady / divisory;
    alphaz = normgradz / divisorz;
#endif

    for (i = 0; i < Vertices->cur; i++) // calcule x pour l'iteration suivante  --- resultat dans Vertices->v[].x, y, z;
    {
      Vertices->v[i].x = Vertices->v[i].x - (alphax * tx[i]);
      Vertices->v[i].y = Vertices->v[i].y - (alphay * ty[i]);
      Vertices->v[i].z = Vertices->v[i].z - (alphaz * tz[i]);
    } // for (i = 0; i < Vertices->cur; i++)

    // calcule la norme du vecteur d'evolution
    normgradx = normgrady = normgradz = 0.0;
    for (i = 0; i < Vertices->cur; i++)
    {
      sx = gx[i] - Vertices->v[i].x;
      sy = gy[i] - Vertices->v[i].y;
      sz = gz[i] - Vertices->v[i].z;
      normgradx += sx*sx;
      normgrady += sy*sy;
      normgradz += sz*sz; 
    }    

    if ((sqrt(normgradx) > RMH_EPSILON) || 
	(sqrt(normgrady) > RMH_EPSILON) ||
	(sqrt(normgradz) > RMH_EPSILON)) stabilite = 0;

#ifdef DEBUG
  fprintf(stderr, "%s: normgradx = %g ; normgrady = %g ; normgradz = %g ; niter = %d\n", 
	  F_NAME, sqrt(normgradx), sqrt(normgrady), sqrt(normgradz), iter);
#endif

  } // while (!stabilite && (iter < nitermax))

  if (iter == nitermax)
    fprintf(stderr, "%s: WARNING: non-convergence\n", F_NAME);

#ifdef VERBOSE
#ifndef PLOTCOSTFUNCTION 
  //  fprintf(stderr, "%s: alphax = %g ; niter = %d\n", F_NAME, alpha, iter);
  printf("%g\t%d\t", alphax, iter);
#endif
#endif

  free(gx); free(gy); free(gz); 
  free(tx); free(ty); free(tz); 
#ifdef OPTIMALSTEP
  free(ux); free(uy); free(uz); 
#endif
} /* RegulMeshHamam() */

/* ==================================== */
void RegulMeshHamam1(double theta)
/* ==================================== */
/* 
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
   Methode de Hamam & al [HC06], variante avec A au lieu de AA
*/
#undef F_NAME
#define F_NAME "RegulMeshHamam1"
{
  int32_t a, n, i, j, k, iter, nv;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz, alphax, alphay, alphaz;
  double *tx, *ty, *tz;    // resultats intermediaires
  int32_t nitermax = NITERMAX; // garde-fou
  double normgradx, normgrady, normgradz, divisorx, divisory, divisorz;
  int32_t stabilite;
  double *gx, *gy, *gz; // pour calculer la norme du vecteur des modifications

  gx = (double *)calloc(1,Vertices->cur * sizeof(double));
  gy = (double *)calloc(1,Vertices->cur * sizeof(double));
  gz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((gx == NULL) || (gy == NULL) || (gz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  tx = (double *)calloc(1,Vertices->cur * sizeof(double));
  ty = (double *)calloc(1,Vertices->cur * sizeof(double));
  tz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((tx == NULL) || (ty == NULL) || (tz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees initiales
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }

  a = 0; // calcule a = nb max de voisins pour 1 vertex
  for (i = 0; i < Vertices->cur; i++)
    if (Vertices->v[i].nfaces > a) a = Vertices->v[i].nfaces; 
  // calcule alpha pour une convergence monotone
  alphax = alphay = alphaz = 1.0 / (1.0 + 2.0 * a * theta);

  if (Links == NULL) ComputeLinks();

  iter = 0;
  stabilite = 0;
  while ((stabilite == 0) && (iter < nitermax))
  {
    stabilite = 1;
    iter++;
#ifdef DEBUG
    fprintf(stderr, "%s: iteration %d\n", F_NAME, iter);
#endif

    for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees en debut d'iteration
    {
      gx[i] = Vertices->v[i].x;
      gy[i] = Vertices->v[i].y;
      gz[i] = Vertices->v[i].z;
    }

    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule theta A x --- resultat dans Vertices->v[].xp, yp, zp;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * Vertices->v[i].x;
      sy = nv * Vertices->v[i].y;
      sz = nv * Vertices->v[i].z;
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= Vertices->v[j].x;
        sy -= Vertices->v[j].y;
        sz -= Vertices->v[j].z;
      }
      Vertices->v[i].xp = theta * sx; 
      Vertices->v[i].yp = theta * sy; 
      Vertices->v[i].zp = theta * sz; 
    } // for (i = 0; i < Vertices->cur; i++)

#ifdef PLOTCOSTFUNCTION
 {
   double J = 0.0;
   for (i = 0; i < Vertices->cur; i++)
   {
     tx[i] = (Vertices->v[i].x - Vertices->v[i].xo) * 
             (Vertices->v[i].x - Vertices->v[i].xo) + 
              Vertices->v[i].x * Vertices->v[i].xp;
     ty[i] = (Vertices->v[i].y - Vertices->v[i].yo) * 
             (Vertices->v[i].y - Vertices->v[i].yo) + 
              Vertices->v[i].y * Vertices->v[i].yp;
     tz[i] = (Vertices->v[i].z - Vertices->v[i].zo) * 
             (Vertices->v[i].z - Vertices->v[i].zo) + 
              Vertices->v[i].z * Vertices->v[i].zp;
     J += tx[i]*tx[i] + ty[i]*ty[i] + tz[i]*tz[i]; 
   } // for (i = 0; i < Vertices->cur; i++)
   printf("%g\n", J);
 }
#endif

    normgradx = normgrady = normgradz = 0.0;
    for (i = 0; i < Vertices->cur; i++) // calcule le gradient nabla J  --- resultat dans t?[];
    {
      tx[i] = Vertices->v[i].x - Vertices->v[i].xo + Vertices->v[i].xp;
      ty[i] = Vertices->v[i].y - Vertices->v[i].yo + Vertices->v[i].yp;
      tz[i] = Vertices->v[i].z - Vertices->v[i].zo + Vertices->v[i].zp;

      normgradx += tx[i]*tx[i];
      normgrady += ty[i]*ty[i];
      normgradz += tz[i]*tz[i]; 
    } // for (i = 0; i < Vertices->cur; i++)

#ifdef OPTIMALSTEP
    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule (I + theta A) Jx --- resultat dans Vertices->v[].xp, yp, zp;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * tx[i];
      sy = nv * ty[i];
      sz = nv * tz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= tx[j];
        sy -= ty[j];
        sz -= tz[j];
      }
      Vertices->v[i].xp = tx[i] + (theta * sx); 
      Vertices->v[i].yp = ty[i] + (theta * sy); 
      Vertices->v[i].zp = tz[i] + (theta * sz); 
    } // for (i = 0; i < Vertices->cur; i++)
    divisorx = divisory = divisorz = 0.0;
    for (i = 0; i < Vertices->cur; i++) // calcule Jx^t (I + theta A) Jx
    {
      divisorx += Vertices->v[i].xp * tx[i];
      divisory += Vertices->v[i].yp * ty[i];
      divisorz += Vertices->v[i].zp * tz[i];
    } // for (i = 0; i < Vertices->cur; i++)
    alphax = normgradx / divisorx;
    alphay = normgrady / divisory;
    alphaz = normgradz / divisorz;
#endif

    for (i = 0; i < Vertices->cur; i++) // calcule x pour l'iteration suivante  --- resultat dans Vertices->v[].x, y, z;
    {
      Vertices->v[i].x = Vertices->v[i].x - (alphax * tx[i]);
      Vertices->v[i].y = Vertices->v[i].y - (alphay * ty[i]);
      Vertices->v[i].z = Vertices->v[i].z - (alphaz * tz[i]);
    } // for (i = 0; i < Vertices->cur; i++)

    // calcule la norme du vecteur d'evolution
    normgradx = normgrady = normgradz = 0.0;
    for (i = 0; i < Vertices->cur; i++)
    {
      sx = gx[i] - Vertices->v[i].x;
      sy = gy[i] - Vertices->v[i].y;
      sz = gz[i] - Vertices->v[i].z;
      normgradx += sx*sx;
      normgrady += sy*sy;
      normgradz += sz*sz; 
    }    

    if ((sqrt(normgradx) > RMH_EPSILON) || 
	(sqrt(normgrady) > RMH_EPSILON) ||
	(sqrt(normgradz) > RMH_EPSILON)) stabilite = 0;

#ifdef DEBUG
  fprintf(stderr, "%s: normgradx = %g ; normgrady = %g ; normgradz = %g ; niter = %d\n", 
	  F_NAME, sqrt(normgradx), sqrt(normgrady), sqrt(normgradz), iter);
#endif

  } // while (!stabilite && (iter < nitermax))

#ifdef VERBOSE
#ifndef PLOTCOSTFUNCTION 
  //  fprintf(stderr, "%s: alpha = %g ; niter = %d\n", F_NAME, alpha, iter);
  printf("%g\t%d\t", alphax, iter);
#endif
#endif

  if (iter == nitermax)
    fprintf(stderr, "%s: WARNING: non-convergence\n", F_NAME);

  free(gx); free(gy); free(gz); 
  free(tx); free(ty); free(tz); 
} /* RegulMeshHamam1() */

/* ==================================== */
void RegulMeshHamam2(int32_t nitermax)
/* ==================================== */
/* 
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
   Methode de Hamam & al [HC06], test d'une variante (theta = infini)
*/
#undef F_NAME
#define F_NAME "RegulMeshHamam2"
{
  int32_t a, n, i, j, k, iter, nv;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz, alpha, dx, dy, dz;
  double *tx, *ty, *tz; // resultats intermediaires
  double normgradx, normgrady, normgradz;
  int32_t stabilite;

  if (nitermax == -1) nitermax = NITERMAX;

  tx = (double *)calloc(1,Vertices->cur * sizeof(double));
  ty = (double *)calloc(1,Vertices->cur * sizeof(double));
  tz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((tx == NULL) || (ty == NULL) || (tz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees initiales
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }

  a = 0; // calcule a = nb max de voisins pour 1 vertex
  for (i = 0; i < Vertices->cur; i++)
    if (Vertices->v[i].nfaces > a) a = Vertices->v[i].nfaces; 
  // calcule alpha pour une convergence monotone
  alpha = 1.0 / (1.0 + 4.0 * a * a);
  if (Links == NULL) ComputeLinks();

#ifdef MESURE
  Edges = AllocEdges(1);
  ComputeEdges();
#endif

  iter = 0;
  stabilite = 0;
  while ((stabilite == 0) && (iter < nitermax))
  {
    stabilite = 1;
    normgradx = normgrady = normgradz = 0.0;
    iter++;
#ifdef DEBUG
    fprintf(stderr, "%s: iteration %d\n", F_NAME, iter);
#endif
#ifdef MESURE
    {
      double mean, stddev, meandc = MeanDistCenter();
      MeanAngleFaces(&mean, &stddev);
      printf("meanangle = %g ; std. deviation = %g ; mean dist center = %g\n", 
	     mean, stddev, meandc);
    }
#endif

    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule A x --- resultat dans tx, ty, tz
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * Vertices->v[i].x;
      sy = nv * Vertices->v[i].y;
      sz = nv * Vertices->v[i].z;
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= Vertices->v[j].x;
        sy -= Vertices->v[j].y;
        sz -= Vertices->v[j].z;
      }
      tx[i] = sx; ty[i] = sy; tz[i] = sz; 
    } // for (i = 0; i < Vertices->cur; i++)
    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule A^2 x --- resultat dans Vertices->v[].xp, yp, zp;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * tx[i];
      sy = nv * ty[i];
      sz = nv * tz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= tx[j];
        sy -= ty[j];
        sz -= tz[j];
      }
      Vertices->v[i].xp = sx; 
      Vertices->v[i].yp = sy; 
      Vertices->v[i].zp = sz; 
    } // for (i = 0; i < Vertices->cur; i++)
    for (i = 0; i < Vertices->cur; i++) // calcule x pour l'iteration suivante  --- resultat dans Vertices->v[].x, y, z;
    {
      dx = Vertices->v[i].xp;
      dy = Vertices->v[i].yp;
      dz = Vertices->v[i].zp;

      normgradx += dx*dx;
      normgrady += dy*dy;
      normgradz += dz*dz; 

      Vertices->v[i].x = Vertices->v[i].x - (alpha * dx);
      Vertices->v[i].y = Vertices->v[i].y - (alpha * dy);
      Vertices->v[i].z = Vertices->v[i].z - (alpha * dz);

    } // for (i = 0; i < Vertices->cur; i++)

    if ((sqrt(normgradx) > RMH_EPSILON) || 
	(sqrt(normgrady) > RMH_EPSILON) ||
	(sqrt(normgradz) > RMH_EPSILON)) stabilite = 0;

#ifdef DEBUG
  fprintf(stderr, "%s: normgradx = %g ; normgrady = %g ; normgradz = %g ; niter = %d\n", 
	  F_NAME, sqrt(normgradx), sqrt(normgrady), sqrt(normgradz), iter);
#endif

  } // while (!stabilite && (iter < nitermax))

#ifdef VERBOSE
  if (iter < nitermax)
    fprintf(stderr, "%s: convergence reached at iteration %d\n", F_NAME, iter);
#endif

  free(tx); free(ty); free(tz); 
} /* RegulMeshHamam2() */

/* ==================================== */
void RegulMeshHamam3(double theta)
/* ==================================== */
/* 
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale.
   Methode de Hamam & al [HC06], variante utilisant le gradient conjugu�
*/
#undef F_NAME
#define F_NAME "RegulMeshHamam3"
{
  int32_t a, n, i, j, k, iter, nv;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz;
  double *dx, *dy, *dz;    // resultats intermediaires
  double *ex, *ey, *ez;    // resultats intermediaires
  double *fx, *fy, *fz;    // resultats intermediaires
  double *tx, *ty, *tz;    // resultats intermediaires
  double alphax, betax, deltax, gamma_1x, gamma_nx, gamma_n1x;
  double alphay, betay, deltay, gamma_1y, gamma_ny, gamma_n1y;
  double alphaz, betaz, deltaz, gamma_1z, gamma_nz, gamma_n1z;
  // PLUS D'UTILITE POUR gamma_1

  dx = (double *)calloc(1,Vertices->cur * sizeof(double));
  dy = (double *)calloc(1,Vertices->cur * sizeof(double));
  dz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((dx == NULL) || (dy == NULL) || (dz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  ex = (double *)calloc(1,Vertices->cur * sizeof(double));
  ey = (double *)calloc(1,Vertices->cur * sizeof(double));
  ez = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((ex == NULL) || (ey == NULL) || (ez == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  fx = (double *)calloc(1,Vertices->cur * sizeof(double));
  fy = (double *)calloc(1,Vertices->cur * sizeof(double));
  fz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((fx == NULL) || (fy == NULL) || (fz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  tx = (double *)calloc(1,Vertices->cur * sizeof(double));
  ty = (double *)calloc(1,Vertices->cur * sizeof(double));
  tz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((tx == NULL) || (ty == NULL) || (tz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees initiales
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }

  if (Links == NULL) ComputeLinks();

  for (n = 0, i = 0; i < Vertices->cur; i++) // calcule A x --- resultat dans tx, ty, tz
  {
    nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
    sx = nv * Vertices->v[i].x;
    sy = nv * Vertices->v[i].y;
    sz = nv * Vertices->v[i].z;
    for (; n <= Links->lastneigh[i]; n++)
    {
      j = Links->neigh[n];
      sx -= Vertices->v[j].x;
      sy -= Vertices->v[j].y;
      sz -= Vertices->v[j].z;
    }
    tx[i] = sx; ty[i] = sy; tz[i] = sz; 
  } // for (i = 0; i < Vertices->cur; i++)

  for (n = 0, i = 0; i < Vertices->cur; i++) // calcule -theta A^2 x --- resultat dans ex, ey, ez et dans dx, dy, dz;
  {
    nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
    sx = nv * tx[i];
    sy = nv * ty[i];
    sz = nv * tz[i];
    for (; n <= Links->lastneigh[i]; n++)
    {
      j = Links->neigh[n];
      sx -= tx[j];
      sy -= ty[j];
      sz -= tz[j];
    }
    dx[i] = ex[i] = -theta * sx; 
    dy[i] = ey[i] = -theta * sy; 
    dz[i] = ez[i] = -theta * sz; 
  } // for (i = 0; i < Vertices->cur; i++)

  gamma_1x = gamma_1y = gamma_1z = 0; // calcule gamma_1 = gamma_n = e^t e
  for (i = 0; i < Vertices->cur; i++) 
  { 
    gamma_1x += ex[i] * ex[i];
    gamma_1y += ey[i] * ey[i];
    gamma_1z += ez[i] * ez[i];
  }
  gamma_nx = gamma_1x; gamma_ny = gamma_1y; gamma_nz = gamma_1z;

  // =================================================
  // BOUCLE PRINCIPALE
  // =================================================
  for (iter = 1; iter < Vertices->cur; iter++)
  {
#ifdef DEBUGHAM3
    fprintf(stderr, "%s: iteration %d\n", F_NAME, iter);
#endif

    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule A d --- resultat dans tx, ty, tz
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * dx[i];
      sy = nv * dy[i];
      sz = nv * dz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= dx[j];
        sy -= dy[j];
        sz -= dz[j];
      }
      tx[i] = sx; ty[i] = sy; tz[i] = sz; 
    } // for (i = 0; i < Vertices->cur; i++)

    for (n = 0, i = 0; i < Vertices->cur; i++) // calcule d + theta A^2 d --- resultat dans fx, fy, fz;
    {
      nv = Links->lastneigh[i] - n + 1; // nb de voisins de i
      sx = nv * tx[i];
      sy = nv * ty[i];
      sz = nv * tz[i];
      for (; n <= Links->lastneigh[i]; n++)
      {
        j = Links->neigh[n];
        sx -= tx[j];
        sy -= ty[j];
        sz -= tz[j];
      }
      fx[i] = dx[i] + theta * sx; 
      fy[i] = dy[i] + theta * sy; 
      fz[i] = dz[i] + theta * sz; 
    } // for (i = 0; i < Vertices->cur; i++)

    alphax = alphay = alphaz = 0; // calcule d^t f, r�sultat dans alpha
    for (i = 0; i < Vertices->cur; i++)
    {
      alphax += dx[i] * fx[i];
      alphay += dy[i] * fy[i];
      alphaz += dz[i] * fz[i];
    }
    alphax = gamma_nx / alphax; // calcule alpha
    alphay = gamma_ny / alphay;
    alphaz = gamma_nz / alphaz;

    for (i = 0; i < Vertices->cur; i++) // x = x + alpha d
    {
      Vertices->v[i].x += alphax * dx[i];
      Vertices->v[i].y += alphay * dy[i];
      Vertices->v[i].z += alphaz * dz[i];
    }

    for (i = 0; i < Vertices->cur; i++) // e = e - alpha f
    {
      ex[i] -= alphax * fx[i];
      ey[i] -= alphay * fy[i];
      ez[i] -= alphaz * fz[i];
    }

    gamma_n1x = gamma_n1y = gamma_n1z = 0; // calcule gamma_n1 = e^t e
    for (i = 0; i < Vertices->cur; i++)
    {
      gamma_n1x += ex[i] * ex[i];
      gamma_n1y += ey[i] * ey[i];
      gamma_n1z += ez[i] * ez[i];
    }

    betax = gamma_n1x / gamma_nx; // calcule beta = gamma_n1 / gamma_n
    betay = gamma_n1y / gamma_ny;
    betaz = gamma_n1z / gamma_nz;

    gamma_nx = gamma_n1x; // gamma_n = gamma_n1
    gamma_ny = gamma_n1y;
    gamma_nz = gamma_n1z;

    for (i = 0; i < Vertices->cur; i++) // d = e + beta d
    {
      dx[i] = ex[i] + betax * dx[i];
      dy[i] = ey[i] + betay * dy[i];
      dz[i] = ez[i] + betaz * dz[i];
    }
  } // for (iter = 1; iter < Vertices->cur; iter++)

  free(tx); free(ty); free(tz); 
  free(dx); free(dy); free(dz); 
  free(ex); free(ey); free(ez); 
  free(fx); free(fy); free(fz); 
} /* RegulMeshHamam3() */

/* ==================================== */
void RegulMeshHC(double alpha, double beta)
/* ==================================== */
/* 
   Methode de Vollmer, Mencl et Mueller
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
*/
#undef F_NAME
#define F_NAME "RegulMeshHC"
{
  int32_t i, j, k, n, iter;
  int32_t stabilite, nitermax = NITERMAX;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz, normgradx, normgrady, normgradz, dx, dy, dz;
  double *tx, *ty, *tz; // pour calculer la norme du vecteur des modifications

#ifdef VERBOSE
  //printf("%s: alpha = %g ; beta = %g\n", F_NAME, alpha, beta);
  printf("%g\t%g\t", alpha, beta);
#endif

  tx = (double *)calloc(1,Vertices->cur * sizeof(double));
  ty = (double *)calloc(1,Vertices->cur * sizeof(double));
  tz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((tx == NULL) || (ty == NULL) || (tz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees initiales
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }


  iter = 0;
  stabilite = 0;
  while ((stabilite == 0) && (iter < nitermax))
  {
    stabilite = 1;
    iter++;

    for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees en debut d'iteration
    {
      tx[i] = Vertices->v[i].x;
      ty[i] = Vertices->v[i].y;
      tz[i] = Vertices->v[i].z;
    }

#ifdef DEBUG
    fprintf(stderr, "%s: iter = %d\n", F_NAME, iter);
#endif

    for (i = 0; i < Vertices->cur; i++) // calcule p (terminologie VMM) et stocke dans (xp,yp,zp)
    {
      if (Vertices->lab[i] == 0)
      {                              /* pour chaque sommet de la grille dont le label est nul */
        V = Vertices->v[i];
        n = 0;
        for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
          {                                      /* et calcule le link */
          F = Faces->f[V.face[j]];
          k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
        } /* for j */
#ifdef WARN_NON_CYCLE
        if (n != V.nfaces)
        {
          fprintf(stderr, "%s: warning: link[%d] non cycle\n", F_NAME, i);
        }
#endif
        if (n == 0) break; // point isole
                                
        sx = sy = sz = 0.0;
        for (j = 0; j < n; j++)
        {
          sx += Vertices->v[link[j]].x;
          sy += Vertices->v[link[j]].y;
          sz += Vertices->v[link[j]].z;
        } /* for j */
        x = sx / n; y = sy / n; z = sz / n;
        Vertices->v[i].xp = x;
        Vertices->v[i].yp = y;
        Vertices->v[i].zp = z;
      } /* if ... */
      else
      {
        Vertices->v[i].xp = Vertices->v[i].x;
        Vertices->v[i].yp = Vertices->v[i].y;
        Vertices->v[i].zp = Vertices->v[i].z;
      } /* else */
    } /* for i */
  
    // calcule b = p - (alpha o + (1-alpha) q) et stocke dans (x,y,z) 
    for (i = 0; i < Vertices->cur; i++)
    {
      Vertices->v[i].x = Vertices->v[i].xp - (alpha * Vertices->v[i].xo + (1.0 - alpha) * Vertices->v[i].x);
      Vertices->v[i].y = Vertices->v[i].yp - (alpha * Vertices->v[i].yo + (1.0 - alpha) * Vertices->v[i].y);
      Vertices->v[i].z = Vertices->v[i].zp - (alpha * Vertices->v[i].zo + (1.0 - alpha) * Vertices->v[i].z);
    }
  
    // p est dans (xp,yp,zp) et b est dans (x,y,z)
    // calcule p - (beta b + (1 - beta) * mean(b, link)) et stocke dans (xp,yp,zp)
    for (i = 0; i < Vertices->cur; i++) 
    {
      if (Vertices->lab[i] == 0)
      {                              /* pour chaque sommet de la grille dont le label est nul */
        V = Vertices->v[i];
        n = 0;
        for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
          {                                      /* et calcule le link */
          F = Faces->f[V.face[j]];
          k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
        } /* for j */
        if (n == 0) break; // point isole
                                
        sx = sy = sz = 0.0;
        for (j = 0; j < n; j++)
        {
          sx += Vertices->v[link[j]].x;
          sy += Vertices->v[link[j]].y;
          sz += Vertices->v[link[j]].z;
        } /* for j */
        x = sx / n; y = sy / n; z = sz / n;
        Vertices->v[i].xp -= (beta * Vertices->v[i].x + (1.0-beta) * x);
        Vertices->v[i].yp -= (beta * Vertices->v[i].y + (1.0-beta) * y);
        Vertices->v[i].zp -= (beta * Vertices->v[i].z + (1.0-beta) * z);
      } /* if ... */
    } /* for i */
  
    // stocke le resultat dans (x,y,z) 
    for (i = 0; i < Vertices->cur; i++)
    {
      Vertices->v[i].x = Vertices->v[i].xp;
      Vertices->v[i].y = Vertices->v[i].yp;
      Vertices->v[i].z = Vertices->v[i].zp;
    }

    // calcule la norme du vecteur d'evolution
    normgradx = normgrady = normgradz = 0.0;
    for (i = 0; i < Vertices->cur; i++)
    {
      dx = tx[i] - Vertices->v[i].x;
      dy = ty[i] - Vertices->v[i].y;
      dz = tz[i] - Vertices->v[i].z;
      normgradx += dx*dx;
      normgrady += dy*dy;
      normgradz += dz*dz; 
    }


    if ((sqrt(normgradx) > RMH_EPSILON) || 
	(sqrt(normgrady) > RMH_EPSILON) ||
	(sqrt(normgradz) > RMH_EPSILON)) stabilite = 0;

  } // while ((stabilite == 0) && (iter < nitermax))

#ifdef VERBOSE
  if (iter < nitermax
)
    printf("%d\t", iter);
  //printf("%s: convergence reached at iteration %d\n", F_NAME, iter);
#endif

  free(tx); free(ty); free(tz); 

} /* RegulMeshHC() */

/* ==================================== */
void RegulMeshTaubin(double lambda, double mu, int nitermax)
/* ==================================== */
/* 
   Methode de Taubin (Tau95)
   ATTENTION : utilise et modifie les champs xp, yp, zp du vertex V.
   Les sommets dont les labels sont non nuls resteront a leur position initiale .
*/
#undef F_NAME
#define F_NAME "RegulMeshTaubin"
{
  int32_t i, j, k, n, iter;
  int32_t stabilite;
  meshvertex V;
  meshface F;
  int32_t link[MAXADJFACES];
  double x, y, z, sx, sy, sz, normgradx, normgrady, normgradz, dx, dy, dz;
  double *tx, *ty, *tz; // pour calculer la norme du vecteur des modifications

#ifdef VERBOSE
  //printf("%s: lambda = %g ; mu = %g\n", F_NAME, lambda, mu);
  printf("%g\t%g\t", lambda, mu);
#endif

  tx = (double *)calloc(1,Vertices->cur * sizeof(double));
  ty = (double *)calloc(1,Vertices->cur * sizeof(double));
  tz = (double *)calloc(1,Vertices->cur * sizeof(double));
  if ((tx == NULL) || (ty == NULL) || (tz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);    
  }

  for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees initiales
  {
    Vertices->v[i].xo = Vertices->v[i].x;
    Vertices->v[i].yo = Vertices->v[i].y;
    Vertices->v[i].zo = Vertices->v[i].z;
  }


  iter = 0;
  stabilite = 0;
  while ((stabilite == 0) && (iter < nitermax))
  {
    stabilite = 1;
    iter++;

    for (i = 0; i < Vertices->cur; i++)  // sauve les coordonnees en debut d'iteration
    {
      tx[i] = Vertices->v[i].x;
      ty[i] = Vertices->v[i].y;
      tz[i] = Vertices->v[i].z;
    }

#ifdef DEBUG
    fprintf(stderr, "%s: iter = %d\n", F_NAME, iter);
#endif

    for (i = 0; i < Vertices->cur; i++) // 1ere sous-iteration : SHRINK
    {
      if (Vertices->lab[i] == 0)
      {                              /* pour chaque sommet de la grille dont le label est nul */
        V = Vertices->v[i];
        n = 0;
        for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
          {                                      /* et calcule le link */
          F = Faces->f[V.face[j]];
          k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
        } /* for j */
#ifdef WARN_NON_CYCLE
        if (n != V.nfaces)
        {
          fprintf(stderr, "%s: warning: link[%d] non cycle\n", F_NAME, i);
        }
#endif
        if (n == 0) break; // point isole
                                
        sx = sy = sz = 0.0;
        for (j = 0; j < n; j++)
        {
          sx += Vertices->v[link[j]].x - V.x;
          sy += Vertices->v[link[j]].y - V.y;
          sz += Vertices->v[link[j]].z - V.z;
        } /* for j */
        x = sx / n; y = sy / n; z = sz / n;
        Vertices->v[i].xp = V.x + lambda * x;
        Vertices->v[i].yp = V.y + lambda * y;
        Vertices->v[i].zp = V.z + lambda * z;
      } /* if ... */
      else
      {
        Vertices->v[i].xp = Vertices->v[i].x;
        Vertices->v[i].yp = Vertices->v[i].y;
        Vertices->v[i].zp = Vertices->v[i].z;
      } /* else */
    } /* for i */

    // stocke le resultat dans (x,y,z) 
    for (i = 0; i < Vertices->cur; i++)
    {
      Vertices->v[i].x = Vertices->v[i].xp;
      Vertices->v[i].y = Vertices->v[i].yp;
      Vertices->v[i].z = Vertices->v[i].zp;
    }



    for (i = 0; i < Vertices->cur; i++) // 2eme sous-iteration : EXPAND
    {
      if (Vertices->lab[i] == 0)
      {                              /* pour chaque sommet de la grille dont le label est nul */
        V = Vertices->v[i];
        n = 0;
        for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes */
          {                                      /* et calcule le link */
          F = Faces->f[V.face[j]];
          k = F.vert[0]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[1]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
          k = F.vert[2]; if ((k != i) && NotIn(k, link, n)) link[n++] = k;
        } /* for j */
#ifdef WARN_NON_CYCLE
        if (n != V.nfaces)
        {
          fprintf(stderr, "%s: warning: link[%d] non cycle\n", F_NAME, i);
        }
#endif
        if (n == 0) break; // point isole
                                
        sx = sy = sz = 0.0;
        for (j = 0; j < n; j++)
        {
          sx += Vertices->v[link[j]].x - V.x;
          sy += Vertices->v[link[j]].y - V.y;
          sz += Vertices->v[link[j]].z - V.z;
        } /* for j */
        x = sx / n; y = sy / n; z = sz / n;
        Vertices->v[i].xp = V.x + mu * x;
        Vertices->v[i].yp = V.y + mu * y;
        Vertices->v[i].zp = V.z + mu * z;
      } /* if ... */
      else
      {
        Vertices->v[i].xp = Vertices->v[i].x;
        Vertices->v[i].yp = Vertices->v[i].y;
        Vertices->v[i].zp = Vertices->v[i].z;
      } /* else */
    } /* for i */

    // stocke le resultat dans (x,y,z) 
    for (i = 0; i < Vertices->cur; i++)
    {
      Vertices->v[i].x = Vertices->v[i].xp;
      Vertices->v[i].y = Vertices->v[i].yp;
      Vertices->v[i].z = Vertices->v[i].zp;
    }

    // calcule la norme du vecteur d'evolution
    normgradx = normgrady = normgradz = 0.0;
    for (i = 0; i < Vertices->cur; i++)
    {
      dx = tx[i] - Vertices->v[i].x;
      dy = ty[i] - Vertices->v[i].y;
      dz = tz[i] - Vertices->v[i].z;
      normgradx += dx*dx;
      normgrady += dy*dy;
      normgradz += dz*dz; 
    }

    if ((sqrt(normgradx) > RMH_EPSILON) || 
	(sqrt(normgrady) > RMH_EPSILON) ||
	(sqrt(normgradz) > RMH_EPSILON)) stabilite = 0;

  } // while ((stabilite == 0) && (iter < nitermax))

#ifdef VERBOSE
  if (iter < nitermax
)
    printf("%d\t", iter);
  //printf("%s: convergence reached at iteration %d\n", F_NAME, iter);
#endif

  free(tx); free(ty); free(tz); 

} /* RegulMeshTaubin() */

/* ==================================== */
void BoundingBoxMesh(meshbox *B)
/* ==================================== */
{
  int32_t i;
  meshvertex V;
  B->bxmin = B->bxmax = Vertices->v[0].x; 
  B->bymin = B->bymax = Vertices->v[0].y; 
  B->bzmin = B->bzmax = Vertices->v[0].z; 
  for (i = 1; i < Vertices->cur; i++)
  {                              /* pour chaque sommet de la grille */
    V = Vertices->v[i];
    if (V.x < B->bxmin) B->bxmin = V.x; else if (V.x > B->bxmax) B->bxmax = V.x;
    if (V.y < B->bymin) B->bymin = V.y; else if (V.y > B->bymax) B->bymax = V.y;
    if (V.z < B->bzmin) B->bzmin = V.z; else if (V.z > B->bzmax) B->bzmax = V.z;
  } /* for i */
} /* BoundingBoxMesh() */

/* ==================================== */
void IsobarMesh(double *X, double *Y, double *Z)
/* ==================================== */
{
  int32_t i;
  meshvertex V;
  double sx, sy, sz;

  sx = sy = sz = 0;
  for (i = 0; i < Vertices->cur; i++)
  {                              /* pour chaque sommet de la grille */
    V = Vertices->v[i];
    sx += V.x;
    sy += V.y;
    sz += V.z;
  } /* for i */
  *X = sx / Vertices->cur;
  *Y = sy / Vertices->cur;
  *Z = sz / Vertices->cur;
} /* IsobarMesh() */

/* ==================================== */
double MeanDistCenter()
/* ==================================== */
// mean distance from vertices to the geometric center of the mesh
{
  int32_t i;
  meshvertex V;
  double sx, sy, sz, md;

  sx = sy = sz = 0.0;
  for (i = 0; i < Vertices->cur; i++)
  {
    V = Vertices->v[i];
    sx += V.x; sy += V.y; sz += V.z;
  } /* for i */
  sx = sx / Vertices->cur;
  sy = sy / Vertices->cur;
  sz = sz / Vertices->cur;
  md = 0.0;
  for (i = 0; i < Vertices->cur; i++)
  {
    V = Vertices->v[i];
    md += dist3(sx, sy, sz, V.x, V.y, V.z);
  } /* for i */
  return md / Vertices->cur;
} /* IsobarMesh() */

/* ==================================== */
void TranslateMesh(double x, double y, double z)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
  {                              /* pour chaque sommet de la grille */
    Vertices->v[i].x += x;
    Vertices->v[i].y += y;
    Vertices->v[i].z += z;
  } /* for i */
} /* TranslateMesh() */

/* ==================================== */
void ZoomMesh(double k)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
  {                              /* pour chaque sommet de la grille */
    Vertices->v[i].x *= k;
    Vertices->v[i].y *= k;
    Vertices->v[i].z *= k;
  } /* for i */
} /* ZoomMesh() */

/* ==================================== */
void ZoomMeshX(double k)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
    Vertices->v[i].x *= k;
} /* ZoomMeshX() */

/* ==================================== */
void ZoomMeshY(double k)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
    Vertices->v[i].y *= k;
} /* ZoomMeshY() */

/* ==================================== */
void ZoomMeshZ(double k)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < Vertices->cur; i++)
    Vertices->v[i].z *= k;
} /* ZoomMeshZ() */

/* ==================================== */
double VolMesh()
/* ==================================== */
/*
  Calcule le volume contenu dans le maillage. 
  Suppose que le maillage constitue une surface combinatoire orientable.
*/
{
  int32_t i, j, k0, k1, k2, n;
  meshvertex V;
  meshface F;
  double x0, y0, z0, x1, y1, z1, x2, y2, z2, vol=0.0;

  for (i = 0; i < Faces->cur; i++)
  {                              /* pour chaque face */
    F = Faces->f[i];
    k0 = F.vert[0]; k1 = F.vert[1]; k2 = F.vert[2];
    V = Vertices->v[k0];
    x0 = V.x; y0 = V.y; z0 = V.z;
    V = Vertices->v[k1];
    x1 = V.x; y1 = V.y; z1 = V.z;
    V = Vertices->v[k2];
    x2 = V.x; y2 = V.y; z2 = V.z;
    vol += x0*y1*z2 + y0*z1*x2 + z0*x1*y2 - z0*y1*x2 - y0*x1*z2 - x0*z1*y2;
  } /* for i */

  return -vol/6; /* - a cause de l'orientation des faces */
} /* VolMesh() */

/* ==================================== */
void CalculNormales()
/* ==================================== */
/* Stocke les normales dans les champs xp, yp, zp du vertex V */
#undef F_NAME
#define F_NAME "CalculNormales"
{
  int32_t i, j, s1, s2;
  meshvertex V;
  meshface F;
  double x, y, z, norm;
  vec3 v1, v2, normaleface, normale;

  for (i = 0; i < Vertices->cur; i++)
  {                              /* pour chaque sommet de la grille */
    V = Vertices->v[i];
    normale[0] = normale[1] = normale[2] = 0;
    for (j = 0; j < V.nfaces; j++) /* parcourt les faces adjacentes au sommet i */
    {
      F = Faces->f[V.face[j]];
      s1 = -1;
      /* range dans s1 et s2 les indices des 2 autres sommets de la face */
      /* attention a respecter l'ordre */
      if (F.vert[0] == i) { s1 = F.vert[1]; s2 = F.vert[2]; }
      else if (F.vert[1] == i) { s1 = F.vert[2]; s2 = F.vert[0]; }
      else if (F.vert[2] == i) { s1 = F.vert[0]; s2 = F.vert[1]; }
#ifdef PARANO
      else
      {
        fprintf(stderr, "%s: unexpected error in mesh structure\n", F_NAME);
        exit(0);
      }
#endif
      /* calcule les vecteurs: v1 = i - s1, v2 = i - s2 */
      v1[0] = V.x - Vertices->v[s1].x;
      v1[1] = V.y - Vertices->v[s1].y;
      v1[2] = V.z - Vertices->v[s1].z;
      v2[0] = V.x - Vertices->v[s2].x;
      v2[1] = V.y - Vertices->v[s2].y;
      v2[2] = V.z - Vertices->v[s2].z;
      /* normale face: produit vectoriel de v1 et de v2 */
      normaleface[0] = v1[1] * v2[2] - v1[2] * v2[1];
      normaleface[1] = v1[2] * v2[0] - v1[0] * v2[2];
      normaleface[2] = v1[0] * v2[1] - v1[1] * v2[0];
      /* cumule pour le calcul de la normale moyenne */
      normale[0] += normaleface[0];
      normale[1] += normaleface[1];
      normale[2] += normaleface[2];
    } /* for j */
    /* normalise la normale */
    norm = normale[0] * normale[0] + normale[1] * normale[1] + normale[2] * normale[2];
    norm = sqrt(norm);
    if (norm < EPSILON)
    {
      fprintf(stderr, "%s: warning: cannot compute normal for vertex %d\n", F_NAME, i);
    }
    else 
    {
      Vertices->v[i].xp = -normale[0] / norm;
      Vertices->v[i].yp = -normale[1] / norm;
      Vertices->v[i].zp = -normale[2] / norm;
    }
  } /* for i */
} /* CalculNormales() */

/* ==================================== */
void CalculNormalesFaces()
/* ==================================== */
{
  int32_t i, j, s0, s1, s2;
  meshvertex V;
  meshface F;
  double x, y, z, norm;
  vec3 v1, v2, normale;

  for (i = 0; i < Faces->cur; i++)
  {                              /* pour chaque sommet de la grille */
    F = Faces->f[i];
    s0 = F.vert[0]; s1 = F.vert[1]; s2 = F.vert[2];
    /* calcule les vecteurs: v1 = s0 - s1, v2 = s0 - s2 */
    v1[0] = Vertices->v[s0].x - Vertices->v[s1].x;
    v1[1] = Vertices->v[s0].y - Vertices->v[s1].y;
    v1[2] = Vertices->v[s0].z - Vertices->v[s1].z;
    v2[0] = Vertices->v[s0].x - Vertices->v[s2].x;
    v2[1] = Vertices->v[s0].y - Vertices->v[s2].y;
    v2[2] = Vertices->v[s0].z - Vertices->v[s2].z;
    /* normale face: produit vectoriel de v1 et de v2 */
    normale[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normale[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normale[2] = v1[0] * v2[1] - v1[1] * v2[0];

    /* normalise la normale */
    norm = normale[0] * normale[0] + normale[1] * normale[1] + normale[2] * normale[2];
    norm = sqrt(norm);
    if (norm < EPSILON)
    {
      fprintf(stderr, "%s: warning: cannot compute normal for vertex %d\n", F_NAME, i);
    }
    else 
    {
      Faces->f[i].xn = -normale[0] / norm;
      Faces->f[i].yn = -normale[1] / norm;
      Faces->f[i].zn = -normale[2] / norm;
    }
  } /* for i */
} /* CalculNormalesFaces() */

/* ==================================== */
void PrintMesh()
/* ==================================== */
{
  int32_t i, j;
  printf(" ========== VERTICES ===========\n");
  for (i = 0; i < Vertices->cur; i++)
  {
    printf("v[%d]: x=%g, y=%g, z=%g;  ", i, 
           Vertices->v[i].x, Vertices->v[i].y, Vertices->v[i].z);
    printf("v[%d]: xp=%g, yp=%g, zp=%g;  faces ", i, 
       Vertices->v[i].xp, Vertices->v[i].yp, Vertices->v[i].zp);
    for (j = 0; j < Vertices->v[i].nfaces; j++)
      printf("%d  ", Vertices->v[i].face[j]);
    printf("\n");
  }
  printf(" ============ FACES ===========\n");
  for (i = 0; i < Faces->cur; i++)
  {
    printf("f[%d]: v1=%d, v2=%d, v3=%d ; normale = %g %g %g\n", i, 
           Faces->f[i].vert[0], Faces->f[i].vert[1], Faces->f[i].vert[2],
           Faces->f[i].xn, Faces->f[i].yn, Faces->f[i].zn);
  }
  if (Edges)
  {
    printf(" ============ EDGES ===========\n");
    for (i = 0; i < Edges->cur; i++)
    {
      printf("e[%d]: v1=%d, v2=%d, f1=%d, f2=%d, curv=%g\n", i, 
             Edges->e[i].v1, Edges->e[i].v2, Edges->e[i].f1, Edges->e[i].f2,
             Edges->e[i].curv);
    }
  }
  if (Links)
  {
    printf(" ============ LINKS ===========\n");
    j = 0;
    for (i = 0; i < Vertices->cur; i++)
    {
      printf("neigh[%d]: ", i);
      for (; j <= Links->lastneigh[i]; j++)
        printf("%d ", Links->neigh[j]);
      printf("\n");
    }
  }
} /* PrintMesh() */

#ifdef DEBUG
int32_t main()
{
  char r[80];
  double x1, y1, z1, x2, y2, z2, x3, y3, z3 ;

  InitMesh(1);
  (void)AddFace(0,0,0, 1,0,0, 0,1,0);
  (void)AddFace(0,0,0, 1,0,0, 0,0,1);
  (void)AddFace(0,0,0, 0,0,1, 0,1,0);
  Edges = AllocEdges(1);
  ComputeEdges();
  ComputeLinks();
  ComputeCurvatures();
  do
  {
    printf("commande (qUIT, PuSH, rEGUL, pRINT)\n");
    scanf("%s", r);
    switch (r[0])
    {
      case 'u':
        printf("sommets (9) > ");
        scanf("%lf %lf %lf", &x1, &y1, &z1);
        scanf("%lf %lf %lf", &x2, &y2, &z2);
        scanf("%lf %lf %lf", &x3, &y3, &z3);
        (void)AddFace(x1, y1, z1, x2, y2, z2, x3, y3, z3);
        break;
      case 'r': RegulMeshHamam(1.0); break;
      case 'p': PrintMesh(); break;
      case 'q': break;
    }
  } while (r[0] != 'q');
  TermineMesh();
}
#endif
