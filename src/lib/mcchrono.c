/* $Id: mcchrono.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
#include <mcchrono.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/*************************************************
	Fonctions de mesure de temps 
	----------------------------
        resolution: 4 microsecondes

    Michel Couprie - ESIEE - juillet 1996
**************************************************/

/* retourne le compteur microsecondes de l'horloge - utile pour la generation aleatoire */
/* ==================================== */
int32_t usecs()
/* ==================================== */
{
   struct timeval tp;
   struct timezone tzp;
   
   if ( gettimeofday(&tp, &tzp) != 0 ) /* code en retour: 0=Ok, -1=erreur */
     fprintf(stderr, "usecs() : gettimeofday failed\n");
   return(tp.tv_usec);
}

/*---------- demarrage chronometrage -------------------*/
/* ==================================== */
void start_chrono(
		 chrono *tp)
/* ==================================== */
{
   struct timezone tzp;
   
   if ( gettimeofday(tp, &tzp) != 0 ) /* code en retour: 0=Ok, -1=erreur */
     fprintf(stderr, "start_chrono() : gettimeofday failed\n");
}

/*----------- Lecture chronometrage : retourne en entier le nb de microsecondes */
/* ==================================== */
int32_t read_chrono(
		chrono *tp)
/* ==================================== */
{
   struct timeval tp2;
   struct timezone tzp;
   
   if ( gettimeofday(&tp2, &tzp) != 0 ) /* code en retour: 0=Ok, -1=erreur */
     fprintf(stderr, "read_chrono() : gettimeofday failed\n");
   if (tp->tv_usec >  tp2.tv_usec)
   {
      tp2.tv_usec += 1000000;
      tp2.tv_sec--;
   }
   return( (tp2.tv_sec - tp->tv_sec)*1000000 + (tp2.tv_usec - tp->tv_usec));
}

/*----------------------------------------------------------------*/
/* ==================================== */
void save_time(
  int32_t n,
  int32_t t,
  char *funcname,
  char *imagename)
/* ==================================== */
{
  char filename[256];
  char *pinkdir;
  FILE *fd = NULL;

  pinkdir = getenv("PINK");
  if (!pinkdir)
  {
    fprintf(stderr, "save_time: environment variable PINK not set\n");
    return;
  }
  strcpy(filename, pinkdir);
  strcat(filename, PERF_DIR);
  strcat(filename, funcname);
  strcat(filename, PERF_EXT);
  fd = fopen(filename,"a");
  if (!fd)
  {
    fprintf(stderr, "save_time: cannot open file: %s\n", filename);
    return;
  }
  /* fprintf(fd, "%9d %9d %s\n", n, t, imagename); */
  fprintf(fd, "%9d\t%9d\n", n, t);
  fclose(fd);
}

/*----------------------------------------------------------------*/
/* ==================================== */
void save_time2(
  int32_t n,
  int32_t n2,
  int32_t t,
  char *funcname,
  char *imagename)
/* ==================================== */
{
  char filename[256];
  char *pinkdir;
  FILE *fd = NULL;

  pinkdir = getenv("PINK");
  if (!pinkdir)
  {
    fprintf(stderr, "save_time: environment variable PINK not set\n");
    return;
  }
  strcpy(filename, pinkdir);
  strcat(filename, PERF_DIR);
  strcat(filename, funcname);
  strcat(filename, PERF_EXT);
  fd = fopen(filename,"a");
  if (!fd)
  {
    fprintf(stderr, "save_time: cannot open file: %s\n", filename);
    return;
  }
  /* fprintf(fd, "%9d %9d %9d %s\n", n, n2, t, imagename); */
  fprintf(fd, "%9d\t%9d\t%9d\n", n, n2, t);
  fclose(fd);
}

/*----------------------------------------------------------------*/
/* ==================================== */
void save_time4(
  int32_t n,
  int32_t n2,
  int32_t na,
  int32_t na2,
  int32_t t,
  char *funcname,
  char *imagename)
/* ==================================== */
{
  char filename[256];
  char *pinkdir;
  FILE *fd = NULL;

  pinkdir = getenv("PINK");
  if (!pinkdir)
  {
    fprintf(stderr, "save_time: environment variable PINK not set\n");
    return;
  }
  strcpy(filename, pinkdir);
  strcat(filename, PERF_DIR);
  strcat(filename, funcname);
  strcat(filename, PERF_EXT);
  fd = fopen(filename,"a");
  if (!fd)
  {
    fprintf(stderr, "save_time: cannot open file: %s\n", filename);
    return;
  }
  /* fprintf(fd, "%9d %9d %9d %9d %9d %s\n", n, n2, na, na2, t, imagename); */
  fprintf(fd, "%9d\t%9d\t%9d\t%9d\t%9d\n", n, n2, na, na2, t);
  fclose(fd);
}

/*----------- Exemple d'utilisation : 
void main()
{
  chrono Chrono1;
  int32_t i;
  start_chrono(&Chrono1);
  for(i = 0; i<1000; i++);
  printf("nb de usec ecoulees = %d\n", read_chrono(&Chrono1));

}
*/
