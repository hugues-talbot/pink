#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lppm2GA.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  double alpha;
  struct xvimage * im;
  struct xvimage * ga;
  int32_t param;
  if ( (argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s im.pgm param [alpha] out.ppm \nparam = 0 difference entre pixel extremite, param = 1 max entre pixel extremite, param = 2 Deriche default for alpha is 1,\n", 
            argv[0]);
    exit(1);
  }
  if ((im =readimage(argv[1])) == NULL )
  {
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  //printf("taille de l'image 3d: %d\n", depth(im));
  printf("Datatype %d (et devrait etre %d ou %d)\n",datatype(im), VFF_TYP_FLOAT,VFF_TYP_1_BYTE);
  switch(datatype(im)){
  case VFF_TYP_1_BYTE:
    if( (ga = allocGAimage(argv[2], im->row_size , im->col_size, depth(im), VFF_TYP_GABYTE )) == NULL )
    {
      fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
      freeimage(im);
      exit(1);
    }
    break;
  case VFF_TYP_FLOAT:
    if( (ga = allocGAimage(argv[2], im->row_size , im->col_size, depth(im), VFF_TYP_GAFLOAT )) == NULL )
    {
      fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
      freeimage(im);
      exit(1);
    }
    break;
  default: fprintf(stderr,"Ne traite pas ce type d'image\n");
    exit(0);
  }
  param = atoi(argv[2]);

  if(argc == 5)
    alpha = atof(argv[3]);
  else alpha = 1;
  if(depth(im) == 1)
    {
      switch(datatype(im)){
      case VFF_TYP_1_BYTE:
	if( lpgm2ga(im, ga, param, alpha) != 1 )
	{
	  fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
	  exit(1);
	}
	break;
      case VFF_TYP_FLOAT:
	if( lpgm2gafloat(im, ga, param, alpha) != 1 )
	{
	  fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
	  exit(1);
	}
	break;
      }
    }
  else
    if( lpgm2ga3d(im,ga,param) != 1 )
    {
      fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
      exit(1);
    }
  if(argc == 5) 
    writerawGAimage(ga, argv[4]);
  else
    writerawGAimage(ga, argv[3]); 
  freeimage(im); freeimage(ga);
  return 0;
}
