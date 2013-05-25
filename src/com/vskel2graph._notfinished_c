PAS FINI


/* $Id: vskel2graph.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file vskel2graph.c

\brief generation of a graph from a curvilinear skeleton

<B>Usage:</B> vskel2graph in.skel out.graph

<B>Description:</B>
Generation of a graph from a curvilinear skeleton.

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2004
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

/* ====================================================================== */
int32_t vskel2graph(char *filename_in, char *filename_out)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés, les jonctions et les extrémités.
// Il y a un arc entre deux sommets s'il y a au moins une branche du squelette entre les deux.
// Pour les jonctions, on prend pour coordonnées le barycentre des points, et
// pour valuation le max des valuations des points.
{
#undef F_NAME
#define F_NAME "vskel2graph"
#define BUFFERSIZE 1024
  skel * S;
  uint32_t i, j, k, v, n;
  char buf[BUFFERSIZE];
  FILE *fdin = NULL;
  FILE *fdout = NULL;
  int32_t ret, dim, rs, cs, ds;
  int32_t nbisol, nbend, nbcurv, nbjunc, nvertex, ncell, connex;
  float fval;
  int32_t nsommets = 0, narcs = 0;

  fdin = fopen(filename_in,"r");
  if (!fdin)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename_in);
    return 0;
  }

  fdout = fopen(filename_out,"w");
  if (!fdout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename_out);
    return 0;
  }

  fscanf(fdin, "%s", buf);
  if (strncmp(buf, "3Dvskel", 6) == 0) dim = 3; else
  if (strncmp(buf, "2Dvskel", 6) == 0) dim = 2; else
  {
    fprintf(stderr, "%s : bad file format : %s\n", F_NAME, buf);
    return 0;
  }

  fscanf(fdin, "%d %d %d %d %d %d", &connex, &nvertex, &ncell, &rs, &cs, &ds);

#ifdef DEBUG
  printf("connex : %d ; nvertex : %d ; ncell : %d ; rs : %d ; cs : %d ; ds : %d\n", 
	 connex, nvertex, ncell, rs, cs, ds);
#endif

  S = initskel(rs, cs, ds, nvertex, ncell, connex); 
  if (S == NULL)
  {
    fprintf(stderr, "%s : initskel failed\n", F_NAME);
    return 0;
  }

  // POINTS ISOLES
  fscanf(fdin, "%s", buf);
  if (strncmp(buf, "isol", 4) != 0)
  {
    fprintf(stderr, "%s : bad file format (0) : %s\n", F_NAME, buf);
    return 0;
  }
  fscanf(fdin, "%d", &nbisol);
  S->e_isol = nbisol;

#ifdef DEBUG
  printf("points isolés : %d\n", nbisol);
#endif

  for (i = 0; i < nbisol; i++)
  {
    nsommets++;
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s : bad file format (1) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d %f", &k, &fval);
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (2) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d", &v);
      addadjlist(S, k, v);
      narcs++;
    }
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (3) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d %f", &v, &fval);
      addptslist(S, k, v);
    }
    S->tskel[k].fval = fval;
  }

  // POINTS EXTREMITES
  fscanf(fdin, "%s", buf);
  if (strncmp(buf, "end", 3) != 0)
  {
    fprintf(stderr, "%s : bad file format (4) : %s\n", F_NAME, buf);
    return 0;
  }
  fscanf(fdin, "%d", &nbend);
  S->e_end = nbisol+nbend;

#ifdef DEBUG
  printf("points extrémités : %d\n", nbend);
#endif

  for (i = 0; i < nbend; i++)
  {
    nsommets++;
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s : bad file format (5) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &k);
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (6) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d", &v);
      addadjlist(S, k, v);
      narcs++;
    }
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (7) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d %f", &v, &fval);
      addptslist(S, k, v);
    }
    S->tskel[k].fval = fval;
  }

  // POINTS DE COURBE
  fscanf(fdin, "%s", buf);
  if (strncmp(buf, "curv", 4) != 0)
  {
    fprintf(stderr, "%s : bad file format (8) : %s\n", F_NAME, buf);
    return 0;
  }
  fscanf(fdin, "%d", &nbcurv);
  S->e_curv = nbisol+nbend+nbcurv;

#ifdef DEBUG
  printf("courbes : %d\n", nbcurv);
#endif

  for (i = 0; i < nbcurv; i++)
  {
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s : bad file format (9) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &k);
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (10) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d", &v);
      addadjlist(S, k, v);
      narcs++;
    }
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (11) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d %f", &v, &fval);
      addptslist(S, k, v);
    }
    S->tskel[k].fval = 0.0;
  }

  // POINTS DE JONCTION
  fscanf(fdin, "%s", buf);
  if (strncmp(buf, "junc", 4) != 0)
  {
    fprintf(stderr, "%s : bad file format (12) : %s\n", F_NAME, buf);
    return 0;
  }
  fscanf(fdin, "%d", &nbjunc);
  S->e_junc = nbisol+nbend+nbcurv+nbjunc;

#ifdef DEBUG
  printf("jonctions : %d\n", nbjunc);
#endif

  for (i = 0; i < nbjunc; i++)
  {
    nsommets++;
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "vertex", 6) != 0)
    {
      fprintf(stderr, "%s : bad file format (13) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &k);
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "adj", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (14) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d", &v);
      addadjlist(S, k, v);
      narcs++;
    }
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "pts", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format (15) : %s\n", F_NAME, buf);
      return 0;
    }
    fscanf(fdin, "%d", &n);
    fmax = 0.0;
    for (j = 0; j < n; j++)
    {
      fscanf(fdin, "%d %f", &v, &fval);
      if (fval > fmax) fmax = fval;
      addptslist(S, k, v);
    }
    S->tskel[k].fval = fmax;
  }

  // ECRITURE DU GRAPHE

  fdout = fopen(filenameout,"w");
  if (!fdout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return;
  }
  
  fprintf(fdout, "%d %d\n", nsommets, narcs);

  fprintf(fdout, "val sommets\n");
  for (i = 0; i < S->e_isol; i++) printf("%d 0\n", i); 
  for (i = S->e_isol; i < S->e_end; i++) printf("%d 1\n", i); 
  for (i = S->e_curv; i < S->e_junc; i++) printf("%d 2\n", i); 

  fprintf(fdout, "coord sommets\n");

  for (i = 0; i < S->e_isol; i++) 
  { 
    printf("%d ", i); printptsliste(S->tskel[i].pts, S->rs); printf("\n"); 
  }
  for (i = S->e_isol; i < S->e_end; i++) 
  { 
    printf("%d ", i); printptsliste(S->tskel[i].pts, S->rs); printf("\n"); 
  }
  for (i = S->e_curv; i < S->e_junc; i++) 
  { 
    printf("%d ", i); printptsliste(S->tskel[i].pts, S->rs); printf("\n"); 
  }







  PAS FINI 






  fprintf(fdout, "arcs values\n");
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      v = p->v_arc;
      fprintf(fdout, "%d %d %ld\n", i, j, v);
    }


  fclose(fdin);
  fclose(fdout);
  termineskel(S);
  return 1;
} /* readvskel() */

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.skel fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (!vskel2graph(argv[1], argv[2]))
  {
    fprintf(stderr, "%s: function vskel2graph failed\n", argv[0]);
    exit(1);
  }

  return 0;
} /* main */
