/* \file GAwatershed.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> GA byte 2D 

<B>Category:</B> 
\ingroup  

\author Jean Cousty
*/
/* 
NAME

<B>GAwatershed</B> - watershed of a 4-connected edge-weighted graph

SYNOPSIS

<B>GAwatershed</B> GAin.pgm out.pgm [type]

DESCRIPTION

If type = 0, then outputs a watershed by an M-border algorithm. The
file out.pgm is a GA.

If type = 1, then outputs a watershed by a non-recursive algorithm
based on streams. The file out.pgm is a GA.

If type = 2 outputs a watershed by a recursive algorithm based on
streams (generally the most efficient one). The file out.pgm is a GA.

If type = 3 outputs an M-border watershed. The file out.pgm is a GA.

If type = 4 outputs a flow mapping. The file out.pgm is a long integer
image.

Types supported: GA byte 2D.

CLASS 

connect

*/
//#define TIME_WATERSHED_TEST 1

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
// #include <mcfifo.h>
#include <jcgraphes.h>
#include <llpeGA.h>
#include <lppm2GA.h>
//#include <jclabelextrema.h>

#ifdef TIME_WATERSHED_TEST
#include <time.h>
#endif


/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  FILE *fd = NULL;
  struct xvimage *image;
  struct xvimage *ga;
  struct xvimage *gaout;
  struct xvimage *watershed;
  double *F;
  double TA, TB,r,rprime;
  int32_t type,i,j,k,y;
#ifdef TIME_WATERSHED_TEST
  clock_t t1,t2;
#endif
  int32_t rs;               /* taille ligne */
  int32_t cs;               /* taille colonne */
  uint32_t *FlowMapping; 
  double max = 0;
  double *G;
  if (argc != 5 )
  {
            fprintf(stderr, "usage: %s imageRawDouble rs cs gaout\n", argv[0]); 
	    exit(1);
  }
  rs = atoi(argv[2]);
  cs = atoi(argv[3]);
  printf("rs %d et cs %d \n",rs, cs);
#ifdef UNIXIO
  fd = fopen(argv[1],"r");
#endif
#ifdef DOSIO
  fd = fopen(argv[1],"rb");
#endif
  image = allocimage(NULL, rs, cs, 1, VFF_TYP_DOUBLE);    
  if(fread(DOUBLEDATA(image), sizeof(double), rs * cs, fd) != rs * cs)
  {
    printf("Erreur de fread \n");
    exit(0);
  }
  fclose(fd);
  F = DOUBLEDATA(image);
#define TITI 1
#define CONSTANTE1 (double)100000000000000000.0
#define CONSTANTE (double)0.001
#ifdef TITI
  for(j = 0; j < cs; j++)
    for(i = 0; i < rs; i++){
      r = ((i - 115) * (i - 115)) + ((j - 90) * (j - 90)); 
      rprime = ((i - 115) * (i - 115)) + ((j + 92) * (j + 92)); 
      if((rprime - r) <= 0) printf("A: (i,j) = (%d,%d)",i,j);
      TA = CONSTANTE1 * (double)(CONSTANTE * (rprime + r)) / ((CONSTANTE * (double)(rprime - r)));
    
      r = ( (i - 60) * (i - 60)) + ((j - 150) * (j - 150)); 
      rprime = ((i - 60) * (i - 60))+ ((j + 152) * (j + 152)); 
      if((rprime - r) <= 0) printf("B: (i,j) = (%d,%d)",i,j);
      TB = CONSTANTE1 * (double)(CONSTANTE * (rprime + r)) / ((CONSTANTE * (double)(rprime - r)));
      F[j*rs +i] = min(TA,TB); 
      if( F[j*rs +i] > max) max =  F[j*rs +i];
      //      printf("val %lf \t", F[j*rs +i]);
    }

  //Dilatation de la fonction
  G = (double *)malloc(sizeof(double)*rs*cs);
  for(j = 0; j < cs; j++)
    for(i = 0; i < rs; i++){
      G[j*rs +i] = 0.0;
      for(k = 0; k < 8; k+=2){
	y = voisin(j*rs +i, k, rs, rs*cs);
	if ((y != -1) && ( F[y] >  G[j*rs +i]) ) {G[j*rs +i] = F[y]; printf("toto");}
      }
      //      printf("val %lf \t", F[j*rs +i]);
    }
  for(j = 0; j < cs; j++)
    for(i = 0; i < rs; i++) F[j*rs + i] = G[j*rs +i];
  free(G);

  i = 240; j =151;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 239; j =152;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 240; j =152;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 241; j =152;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 239; j =153;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 240; j =153;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 241; j =153;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);
  i = 240; j =154;
  printf("(%d,%d) = %lf\n",i,j,F[j*rs +i]);

  printf("max %lf",max);
#endif
  ga = allocGAimage(NULL, rs, cs, 1, VFF_TYP_GADOUBLE);
  if( lpgm2gaDouble(image,ga, 1, 18) != 1 )
  {
    fprintf(stderr, "%s: erreur de lppm2gadouble \n", argv[0]);
    exit(1);
  } 
  if( (watershed = allocimage(NULL, rs,cs, 2, VFF_TYP_4_BYTE)) == NULL){
    fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
    exit(0);
  }

  FlowMapping = ULONGDATA(watershed);
  flowMappingDouble(ga, FlowMapping); 
  printf("Je suis dans le bon type\n");
  fflush(stdout);
  gaout = SeparatingEdge(watershed);
  printf("%s (devrait etre _gaout)\n", argv[2]);
  writerawGAimage(gaout, argv[4]);
  printf("Super totosizeofdouble %d \n", sizeof(double));  
  if(!gaout) free(gaout);
  free(watershed); free(ga);
  return 0;
}
