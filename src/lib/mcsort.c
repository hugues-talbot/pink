/* $Id: mcsort.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* 
  Tri rapide et selection
  D'apres "Introduction a l'algorithmique", 
    T. Cormen, C. Leiserson, R. Rivest, pp. 152, Dunod Ed. 

  Michel Couprie  -  Decembre 1997
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mcsort.h>

/* =============================================================== */
int32_t Partitionner(int32_t *A, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : ceux <= A[p] et les autres.
*/
{
  int32_t t;
  int32_t x = A[p];
  int32_t i = p - 1;
  int32_t j = r + 1;
  while (1)
  {
    do j--; while (A[j] > x);
    do i++; while (A[i] < x);
    if (i < j) { t = A[i]; A[i] = A[j]; A[j] = t; }
    else return j;
  } /* while (1) */   
} /* Partitionner() */

/* =============================================================== */
int32_t PartitionStochastique(int32_t *A, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : ceux <= A[q] et les autres, avec q tire au hasard dans [p,r].
*/
{
  int32_t t, q;

  q = p + (rand() % (r - p + 1));
  t = A[p];         /* echange A[p] et A[q] */
  A[p] = A[q]; 
  A[q] = t;
  return Partitionner(A, p, r);
} /* PartitionStochastique() */

/* =============================================================== */
void TriRapide (int32_t * A, int32_t p, int32_t r)
/* =============================================================== */
/* 
  trie les valeurs du tableau A de l'indice m (compris) a l'indice n (compris) 
  par ordre croissant 
*/
{
  int32_t q; 
  if (p < r)
  {
    q = Partitionner(A, p, r);
    TriRapide (A, p, q) ;
    TriRapide (A, q+1, r) ;
  }
} /* TriRapide() */

/* =============================================================== */
void TriRapideStochastique (int32_t * A, int32_t p, int32_t r)
/* =============================================================== */
/* 
  trie les valeurs du tableau A de l'indice m (compris) a l'indice n (compris) 
  par ordre croissant 
*/
{
  int32_t q; 
  if (p < r)
  {
    q = PartitionStochastique(A, p, r);
    TriRapideStochastique (A, p, q) ;
    TriRapideStochastique (A, q+1, r) ;
  }
} /* TriRapideStochastique() */

/* =============================================================== */
int32_t SelectionStochastique (int32_t * A, int32_t p, int32_t r, int32_t i)
/* =============================================================== */
/* 
  retourne la valeur de rang i dans le tableau A 
  entre l'indice m (compris) a l'indice n (compris) 
*/
{
  int32_t q, k; 
  if (p == r) return A[p];
  q = PartitionStochastique(A, p, r);
  k = q - p + 1;
  if (i <= k) return SelectionStochastique (A, p, q, i);
  else        return SelectionStochastique (A, q+1, r, i - k) ;
} /* SelectionStochastique() */

/* =============================================================== */
int32_t ElimineDupliques(int32_t *A, int32_t n)
/* =============================================================== */
/*
  elimine les elements dupliques successifs de A[0..n[
  retourne le nouveau nombre d'elements 
*/
{
  int32_t s, t;

  if (n == 0) return 0;
  s = t = 1;
  while (s < n)
  {
    if (A[s] != A[s - 1])
    {
      A[t] = A[s];
      t++;
    }
    s++;    
  }
  return t;
} /* ElimineDupliques() */

#ifdef TEST
/* =============================================================== */
void main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  FILE *fd1 = NULL;
  int32_t ret1;
  int32_t in1;
  int32_t nb1, nb2;
  int32_t *tab;
  int32_t i;

  if ((argc < 2) || (argc > 3) || ((argc == 3) && (strcmp(argv[2], "-e") != 0)))
  {
    fprintf(stderr, "usage: %s filein [-e]\n", argv[0]);
    exit(0);
  }

  fd1 = fopen(argv[1],"r");
  if (!fd1)
  {
    fprintf(stderr, "%s: file not found: %s\n", argv[0], argv[1]);
    exit(0);
  }

  nb1 = 0;                         /* compte le nombre de configs */
  ret1 = fscanf(fd1, "%x", &in1);
  while (ret1 == 1)
  {
      nb1++;
      ret1 = fscanf(fd1, "%x", &in1);
  } /* while (ret1 == 1) */
  fclose(fd1);

  fprintf(stderr, "le fichier %s contient %d configs\n", argv[1], nb1);

  tab = (int32_t *)calloc(1,nb1 * sizeof(int32_t));
  if (tab == NULL)
  {   
    fprintf(stderr,"%s : malloc failed\n", argv[0]);
    exit(0);
  }  

  fd1 = fopen(argv[1],"r");          /* charge les configs dans le tableau tab */
  if (!fd1)
  {
    fprintf(stderr, "%s: file not found (very strange): %s\n", argv[0], argv[1]);
    exit(0);
  }
  nb1 = 0;
  ret1 = fscanf(fd1, "%x", &in1);
  while (ret1 == 1)
  {
    tab[nb1] = in1;
    nb1++;
    ret1 = fscanf(fd1, "%x", &in1);
  } /* while (ret1 == 1) */
  fclose(fd1);

  TriRapideStochastique(tab, 0, nb1-1);

  nb2 = nb1;
  if (argc == 3)    /* elimination des valeurs dupliquees */
  {
    nb2 = 1;
    for (i = 1; i < nb1; i++)
    {
      if (tab[i] != tab[i-1])
      {
        tab[nb2] = tab[i];
        nb2++;
      }
    }
  }

  for (i = 0; i < nb2; i++) printf("%x\n", tab[i]);

  free(tab);

} /* main */
#endif
