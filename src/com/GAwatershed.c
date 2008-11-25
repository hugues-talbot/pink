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
//#include <jclabelextrema.h>

#define TIME_WATERSHED_TEST 1
#ifdef TIME_WATERSHED_TEST
#include <time.h>
#endif


/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage *ga;
  struct xvimage *gaout;
  struct xvimage *watershed;
  int32_t type;
#ifdef TIME_WATERSHED_TEST
  clock_t t1,t2;
#endif
  int32_t rs;               /* taille ligne */
  int32_t cs;               /* taille colonne */
  uint32_t *FlowMapping; 


  if ( (argc != 3) && (argc != 4) )
  {
            fprintf(stderr, "usage: %s GAin.pgm out.pgm [type] \n\tif type = 0, then produces a watershed by M-border algorithm;\n\tif type = 1, then outputs a watershed by a non-recursive algorithm based on stream;\n\tif type = 2 outputs a watershed by a recursive algorithm based on streams;\n\tif type = 3 outputs an M-border watershed;\n\tif type = 4 outputs a flow mapping, out.pgm is a long integer image;\n\tdefault type = 2.\n", argv[0]); 
	    exit(1);
  }
  ga = readGAimage(argv[1]);
  if (ga == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if(argc == 4)
    type = atoi(argv[3]);
  else
    type = 2;
  if( (type > 4) || (type < 0)){
    fprintf(stderr, "usage: %s GAin.pgm out.pgm [type] \n\tif type = 0, then produces a watershed by M-border algorithm;\n\tif type = 1, then outputs a watershed by a non-recursive algorithm based on stream;\n\tif type = 2 outputs a watershed by a recursive algorithm based on streams;\n\tif type = 3 outputs an M-border watershed;\n\tif type = 4 outputs a flow mapping, out.pgm is a long integer image;\n\tdefault type = 2.\n", argv[0]); 
  }
  switch(datatype(ga)){
  case VFF_TYP_GABYTE:
#ifndef TIME_WATERSHED_TEST
    switch(type){
    case 0: // Separation par M-border watershed
      watershed = mBorderWshed2d(ga); 
      gaout = SeparatingEdge(watershed);
      writerawGAimage(gaout,argv[2]); 
      break; 
    case 1: // Separation par flow mapping non recursif    
      rs = rowsize(ga);
      cs = colsize(ga);
    if( (watershed = allocimage(NULL, rs,cs, 1, VFF_TYP_4_BYTE)) == NULL){
      fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
      exit(0);
    }
    FlowMapping = ULONGDATA(watershed);
    flowMapping(ga, FlowMapping);
    gaout = SeparatingEdge(watershed);
    writerawGAimage(gaout,argv[2]);
    break; 
  case 2: // Separation par flow mapping recursif
    rs = rowsize(ga);
    cs = colsize(ga);
    if( (watershed = allocimage(NULL, rs,cs, 1, VFF_TYP_4_BYTE)) == NULL){
      fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
      exit(0);
    }
    FlowMapping = ULONGDATA(watershed);
    flowMappingRecursif(ga, FlowMapping); 
    gaout = SeparatingEdge(watershed);
    writerawGAimage(gaout,argv[2]);
    break;
  case 3: // M-border watershed
    watershed = mBorderWshed2d(ga);  
    writerawGAimage(ga,argv[2]);
    break;
  case 4: // flow mapping 
    rs = rowsize(ga);
    cs = colsize(ga);
    if( (watershed = allocimage(NULL, rs,cs, 1, VFF_TYP_4_BYTE)) == NULL){
      fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
      exit(0);
    }
    FlowMapping = ULONGDATA(watershed);
    flowMappingRecursif(ga, FlowMapping);
    writeimage(watershed,argv[2]);
  }
#else
  switch(type){
  case 0: 
    t1 = clock();   
    watershed = mBorderWshed2d(ga);
    t2 = clock(); 
    printf("temps (border_watershed) %d ms\n", 
	   (t2 - t1)/ (CLOCKS_PER_SEC / 1000)); 
    gaout = SeparatingEdge(watershed);
    writerawGAimage(gaout,argv[2]);
    break; 
  case 1: 
    t1 = clock();  
    rs = rowsize(ga);
    cs = colsize(ga);
    if( (watershed = allocimage(NULL, rs,cs, 1, VFF_TYP_4_BYTE)) == NULL){
      fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
      exit(0);
    }
    FlowMapping = ULONGDATA(watershed);
    flowMapping(ga, FlowMapping);
    t2 = clock();
    printf("temps (stream_non_recursif) %d ms\n", 
	   (t2 - t1)/ (CLOCKS_PER_SEC / 1000));
    gaout = SeparatingEdge(watershed);
    writerawGAimage(gaout,argv[2]);
    break;
  case 2: 
    t1 = clock();  
    rs = rowsize(ga);
    cs = colsize(ga);
    if( (watershed = allocimage(NULL, rs,cs, 1, VFF_TYP_4_BYTE)) == NULL){
      fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
      exit(0);
    }
    FlowMapping = ULONGDATA(watershed);
    flowMappingRecursif(ga, FlowMapping);
    t2 = clock();
    printf("temps (stream_recursif) %d ms\n", 
	   (t2 - t1)/ (CLOCKS_PER_SEC / 1000));
    gaout = SeparatingEdge(watershed);
    writerawGAimage(gaout,argv[2]);
    break;
    case 4: 
      t1 = clock();   
      watershed = mBorderWshed2drapide(ga);
      t2 = clock(); 
      printf("temps (border_watershed_rapide) %d ms\n", 
	     (t2 - t1)/ (CLOCKS_PER_SEC / 1000)); 
      gaout = SeparatingEdge(watershed);
      writerawGAimage(gaout,argv[2]);
      break;
  }
#endif
  break;
  case VFF_TYP_GAFLOAT:    
    printf("Je suis dans le bon type\n");
    fflush(stdout);
    rs = rowsize(ga);
    cs = colsize(ga);
    if( (watershed = allocimage(NULL, rs,cs, 1, VFF_TYP_4_BYTE)) == NULL){
      fprintf(stderr, "%s : erreur de malloc\n",argv[0]);
      exit(0);
    }
    FlowMapping = ULONGDATA(watershed);
    printf("Je suis dans le bon type\n"); 
    fflush(stdout);
    flowMappingFloat(ga, FlowMapping); // Il faut l'ecrire
    printf("Je suis dans le bon type\n");
    fflush(stdout);
    gaout = SeparatingEdge(watershed);
    printf("%s (devrait etre _gaout)\n", argv[2]);
    writerawGAimage(gaout, argv[2]);
    printf("Super totosizeofdouble %d \n", sizeof(double));
    break;
  }
  if(!gaout) free(gaout);
  free(watershed); free(ga);
  return 0;
}
