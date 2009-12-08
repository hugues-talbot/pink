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
/*
      Librairie mcskelcurv :

      Gestion de squelettes curvilignes

      Michel Couprie, juin 2004
*/
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcskelcurv.h>

//#define DEBUG

/* ================================================ */
/* ================================================ */
/* fonctions privees */
/* ================================================ */
/* ================================================ */

/* ====================================================================== */
static int32_t tailleliste(SKC_pcell p)
/* ====================================================================== */
{
  int32_t n = 0;
  for (; p != NULL; p = p->next) n++;
  return n;
} /* tailleliste() */

/* ====================================================================== */
static void printliste(SKC_pcell p)
/* ====================================================================== */
{
  for (; p != NULL; p = p->next)
    printf("%d ", p->val);
} /* printliste() */

/* ====================================================================== */
static void printptsliste(SKC_pcell p, int32_t rs)
/* ====================================================================== */
{
  for (; p != NULL; p = p->next)
    printf("%d,%d ; ", p->val%rs, p->val/rs);
} /* printliste() */

/* ====================================================================== */
void printskel(skel * S)
/* ====================================================================== */
{
  uint32_t i;
  
  printf("isol:\n");
  for (i = 0; i < S->e_isol; i++)
  {
    printf("[%d] ", i); printf("adj: "); printliste(S->tskel[i].adj);
    printf("pts: "); printptsliste(S->tskel[i].pts, S->rs); printf("\n");
  }
  printf("end:\n");
  for (i = S->e_isol; i < S->e_end; i++)
  {
    printf("[%d] ", i); printf("adj: "); printliste(S->tskel[i].adj);
    printf("pts: "); printptsliste(S->tskel[i].pts, S->rs); printf("\n");
  }
  printf("curv:\n");
  for (i = S->e_end; i < S->e_curv; i++)
  {
    printf("[%d] ", i); printf("adj: "); printliste(S->tskel[i].adj);
    printf("pts: "); printptsliste(S->tskel[i].pts, S->rs); printf("\n");
  }
  printf("junc:\n");
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    printf("[%d] ", i); printf("adj: "); printliste(S->tskel[i].adj);
    printf("pts: "); printptsliste(S->tskel[i].pts, S->rs); printf("\n");
  }
} /* printskel() */

/* ================================================ */
/* ================================================ */
/* fonctions publiques */
/* ================================================ */
/* ================================================ */

/* ================================================ */
/* fonctions d'entrée-sortie */
/* ================================================ */

/*
FILE = 2Dskel | 3Dskel <CONNEX> <NVERTEX> <NCELL> <RS> <CS> <DS> <ISOL> <END> <CURV> <JUNC>
ISOL = "ISOL" <N> <VERTEX> ... <VERTEX>
END  = "END"  <N> <VERTEX> ... <VERTEX>
CURV = "CURV" <N> <VERTEX> ... <VERTEX>
JUNC = "JUNC" <N> <VERTEX> ... <VERTEX>
VERTEX = "vertex" <I> "adj" <N> <I> ... <I> "pts" <N> <POINT> ...  <POINT> 
CONNEX = 4 | 8 | 6 | 18 | 26
NVERTEX = int32_t
NCELL = int32_t
RS = int32_t
CS = int32_t
DS = int32_t
N = int32_t
I = int32_t
POINT = int32_t
*/

/* ====================================================================== */
void fprintliste(FILE *fd, SKC_pcell p)
/* ====================================================================== */
{
  int32_t n = tailleliste(p);
  fprintf(fd, "%d ", n);
  for (; p != NULL; p = p->next) fprintf(fd, "%d ", p->val);
} /* fprintliste() */

/* ====================================================================== */
void fprintvliste(FILE *fd, SKC_pcell p, float *V)
/* ====================================================================== */
{
  int32_t n = tailleliste(p);
  fprintf(fd, "%d ", n);
  for (; p != NULL; p = p->next) fprintf(fd, "%d %g ", p->val, V[p->val]);
} /* fprintvliste() */

/* ====================================================================== */
SKC_pcell skeladdcell(skel *S, int32_t val, SKC_pcell next)
/* ====================================================================== */
// alloue une nouvelle cellule de liste et met a jour ses champs val et next
{
#undef F_NAME
#define F_NAME "skeladdcell"
  SKC_pcell p;
  if (S->freecell >= S->nbcell)
  {
    fprintf(stderr, "%s: not enough cells\n", F_NAME);
    exit(0);
  }
  p = &(S->tcell[S->freecell]);
  S->freecell++;
  p->val = val;
  p->next = next;
  return p;
} // skeladdcell()

/* ====================================================================== */
void addadjlist(skel * S, uint32_t k, uint32_t v)
/* ====================================================================== */
{
  SKC_pcell p = skeladdcell(S, v, S->tskel[k].adj);
  S->tskel[k].adj = p;
} /* addadjlist() */

/* ====================================================================== */
void addptslist(skel * S, uint32_t k, uint32_t v)
/* ====================================================================== */
{
  SKC_pcell p = skeladdcell(S, v, S->tskel[k].pts);
  S->tskel[k].pts = p;
} /* addptslist() */

/* ====================================================================== */
void writeskel(skel * S, char *filename)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "writeskel"
  uint32_t i;
  FILE *fd = NULL;

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    exit(0);
  }

  if (S->ds == 1) fprintf(fd, "2Dskel "); else fprintf(fd, "3Dskel ");

  fprintf(fd, "%d %d %d %d %d %d\n", S->connex, S->e_junc, S->freecell, S->rs, S->cs, S->ds);

  fprintf(fd, "ISOL %d\n", S->e_isol);
  for (i = 0; i < S->e_isol; i++)
  {
    fprintf(fd, "vertex %d\n", i); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintliste(fd, S->tskel[i].pts); fprintf(fd, "\n"); 
  }

  fprintf(fd, "END %d\n", S->e_end - S->e_isol);
  for (i = S->e_isol; i < S->e_end; i++)
  {
    fprintf(fd, "vertex %d\n", i); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintliste(fd, S->tskel[i].pts); fprintf(fd, "\n"); 
  }

  fprintf(fd, "CURV %d\n", S->e_curv - S->e_end);
  for (i = S->e_end; i < S->e_curv; i++)
  {
    fprintf(fd, "vertex %d\n", i); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintliste(fd, S->tskel[i].pts); fprintf(fd, "\n"); 
  }

  fprintf(fd, "JUNC %d\n", S->e_junc - S->e_curv);
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    fprintf(fd, "vertex %d\n", i); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintliste(fd, S->tskel[i].pts); fprintf(fd, "\n"); 
  }
  fclose(fd);
} /* writeskel() */

/* ====================================================================== */
void writevskel(skel * S, char *filename, struct xvimage *val)
/* ====================================================================== */
// idem writeskel, mais de plus chaque point est suivi d'une valeur trouvée dans l'image val,
// et chaque vertex est suivi d'une valeur (nulle par défaut).
{
#undef F_NAME
#define F_NAME "writevskel"
  uint32_t i;
  FILE *fd = NULL;
  float *V;

  if ((rowsize(val) != S->rs) || (colsize(val) != S->cs) || (depth(val) != S->ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  (void)convertfloat(&val);
  V = FLOATDATA(val);

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    exit(0);
  }

  if (S->ds == 1) fprintf(fd, "2Dvskel "); else fprintf(fd, "3Dvskel ");

  fprintf(fd, "%d %d %d %d %d %d\n", S->connex, S->e_junc, S->freecell, S->rs, S->cs, S->ds);

  fprintf(fd, "ISOL %d\n", S->e_isol);
  for (i = 0; i < S->e_isol; i++)
  {
    fprintf(fd, "vertex %d %d\n", i, 0); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintvliste(fd, S->tskel[i].pts, V); fprintf(fd, "\n"); 
  }

  fprintf(fd, "END %d\n", S->e_end - S->e_isol);
  for (i = S->e_isol; i < S->e_end; i++)
  {
    fprintf(fd, "vertex %d %d\n", i, 0); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintvliste(fd, S->tskel[i].pts, V); fprintf(fd, "\n"); 
  }

  fprintf(fd, "CURV %d\n", S->e_curv - S->e_end);
  for (i = S->e_end; i < S->e_curv; i++)
  {
    fprintf(fd, "vertex %d %d\n", i, 0); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintvliste(fd, S->tskel[i].pts, V); fprintf(fd, "\n"); 
  }

  fprintf(fd, "JUNC %d\n", S->e_junc - S->e_curv);
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    fprintf(fd, "vertex %d %d\n", i, 0); 
    fprintf(fd, "adj "); fprintliste(fd, S->tskel[i].adj); fprintf(fd, "\n"); 
    fprintf(fd, "pts "); fprintvliste(fd, S->tskel[i].pts, V); fprintf(fd, "\n"); 
  }
  fclose(fd);
} /* writevskel() */

/* ====================================================================== */
skel * readskel(char *filename)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "readskel"
#define BUFFERSIZE 1024
  skel * S;
  uint32_t i, j, k, v, n;
  char buf[BUFFERSIZE];
  FILE *fd = NULL;
  int32_t dim, rs, cs, ds;
  int32_t nbisol, nbend, nbcurv, nbjunc, nvertex, ncell, connex;

  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return NULL;
  }

  fscanf(fd, "%s", buf);
  if (strncmp(buf, "3Dskel", 6) == 0) dim = 3; else
  if (strncmp(buf, "2Dskel", 6) == 0) dim = 2; else
  {
    fprintf(stderr, "%s: bad file format : %s\n", F_NAME, buf);
    return NULL;
  }

  fscanf(fd, "%d %d %d %d %d %d", &connex, &nvertex, &ncell, &rs, &cs, &ds);

#ifdef DEBUG
  printf("connex : %d ; nvertex : %d ; ncell : %d ; rs : %d ; cs : %d ; ds : %d\n", 
	 connex, nvertex, ncell, rs, cs, ds);
#endif

  S = initskel(rs, cs, ds, nvertex, ncell, connex); 
  if (S == NULL)
  {   fprintf(stderr, "%s: initskel failed\n", F_NAME);
      return NULL;
  }

  // POINTS ISOLES
  fscanf(fd, "%s", buf);
  if ((strncmp(buf, "isol", 4) != 0) && (strncmp(buf, "ISOL", 4) != 0))
  {
    fprintf(stderr, "%s: bad file format (0) : %s\n", F_NAME, buf);
    return NULL;
  }
  fscanf(fd, "%d", &nbisol);
  S->e_isol = nbisol;

#ifdef DEBUG
  printf("points isolés : %d\n", nbisol);
#endif

  for (i = 0; i < nbisol; i++)
  {
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s: bad file format (1) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &k);
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (2) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addadjlist(S, k, v);
    }
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (3) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addptslist(S, k, v);
    }
  }

  // POINTS EXTREMITES
  fscanf(fd, "%s", buf);
  if ((strncmp(buf, "end", 3) != 0) && (strncmp(buf, "END", 3) != 0))
  {
    fprintf(stderr, "%s: bad file format (4) : %s\n", F_NAME, buf);
    return NULL;
  }
  fscanf(fd, "%d", &nbend);
  S->e_end = nbisol+nbend;

#ifdef DEBUG
  printf("points extrémités : %d\n", nbend);
#endif

  for (i = 0; i < nbend; i++)
  {
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s: bad file format (5) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &k);
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (6) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addadjlist(S, k, v);
    }
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (7) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addptslist(S, k, v);
    }
  }

  // POINTS DE COURBE
  fscanf(fd, "%s", buf);
  if ((strncmp(buf, "curv", 4) != 0) && (strncmp(buf, "CURV", 4) != 0))
  {
    fprintf(stderr, "%s: bad file format (8) : %s\n", F_NAME, buf);
    return NULL;
  }
  fscanf(fd, "%d", &nbcurv);
  S->e_curv = nbisol+nbend+nbcurv;

#ifdef DEBUG
  printf("courbes : %d\n", nbcurv);
#endif

  for (i = 0; i < nbcurv; i++)
  {
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s: bad file format (9) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &k);
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (10) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addadjlist(S, k, v);
    }
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (11) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addptslist(S, k, v);
    }
  }

  // POINTS DE JONCTION
  fscanf(fd, "%s", buf);
  if ((strncmp(buf, "junc", 4) != 0) && (strncmp(buf, "JUNC", 4) != 0))
  {
    fprintf(stderr, "%s: bad file format (12) : %s\n", F_NAME, buf);
    return NULL;
  }
  fscanf(fd, "%d", &nbjunc);
  S->e_junc = nbisol+nbend+nbcurv+nbjunc;

#ifdef DEBUG
  printf("jonctions : %d\n", nbjunc);
#endif

  for (i = 0; i < nbjunc; i++)
  {
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s: bad file format (13) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &k);
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (14) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addadjlist(S, k, v);
    }
    fscanf(fd, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s: bad file format (15) : %s\n", F_NAME, buf);
      return NULL;
    }
    fscanf(fd, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fd, "%d", &v);
      addptslist(S, k, v);
    }
  }

  fclose(fd);
  return S;
} /* readskel() */

/* ====================================================================== */
skel * initskel(uint32_t rs, uint32_t cs, uint32_t ds, uint32_t nbvertex, uint32_t nbcell, int32_t connex)
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "initskel"
  skel * S;
  uint32_t i;
  
  S = (skel *)calloc(1,sizeof(skel));
  if (S == NULL)
  {   fprintf(stderr, "%s: calloc failed for S\n", F_NAME);
      return(0);
  }
 
  S->tcell = (SKC_cell *)calloc(nbcell, sizeof(SKC_cell));
  if (S->tcell == NULL)
  {   fprintf(stderr, "%s: calloc failed for S->tcell\n", F_NAME);
      return(0);
  }

  S->connex = connex;
  S->rs = rs;
  S->cs = cs;
  S->ds = ds;
  S->freecell = 0;
  S->nbcell = nbcell;
 
  S->tskel = (skelpart *)calloc(nbvertex, sizeof(skelpart));
  if (S->tskel == NULL)
  {   fprintf(stderr, "%s: calloc failed for S->tskel\n", F_NAME);
      return(0);
  }

  for (i = 0; i < nbvertex; i++) S->tskel[i].pts = S->tskel[i].adj = NULL;

  return S;
} /* initskel() */

/* ====================================================================== */
void termineskel(skel * S)
/* ====================================================================== */
{
#ifdef DEBUG
  printf("termineskel: begin\n");
#endif
  free(S->tcell);
#ifdef DEBUG
  printf("termineskel 1\n");
#endif
  free(S->tskel);
#ifdef DEBUG
  printf("termineskel 2\n");
#endif
  free(S);
#ifdef DEBUG
  printf("termineskel: end\n");
#endif
} /* termineskel() */

#ifdef TESTSKEL
void main()
{
  skel * S;
  uint32_t s1, s2;
  
  S = initskel(16, 64);
  printskel(S);
  termineskel(S);
} /* main() */
#endif
