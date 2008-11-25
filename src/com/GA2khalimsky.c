#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lga2khalimsky.h>

/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * ga;
  struct xvimage * out;
  int32_t bar;                    /* booleen indiquant si l'on traite un objet ou un complementaire */
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s GA.pgm type (0 pour un objet ou 1 pour un complementaire) out.pgm \n", 
            argv[0]);
    exit(1);
  }
  
  if( (ga = readGAimage(argv[1])) == NULL )
  {
     fprintf(stderr, "%s : ne peut lire %s \n", 
	     argv[0], argv[1]);
     exit(1);
  }
  if(datatype(ga) != VFF_TYP_GABYTE)
  {
    fprintf(stderr, "%s : mauvais format pour %s \n", 
            argv[0], argv[1]);
    freeimage(ga);
    exit(1);
  }
  bar = atoi(argv[2]);
  if(depth(ga) == 1){
    out = allocimage(argv[3], rowsize(ga)*2, colsize(ga)*2, 1, VFF_TYP_1_BYTE);
    if(!lga2khalimsky(ga, out, bar))
    {
      fprintf(stderr, "%s : echec de lga2khalimsky \n", 
	      argv[0]);
      freeimage(ga); freeimage(out);
      exit(1);
    }
  }
  else
  {
    out = allocimage(argv[3], rowsize(ga)*2, colsize(ga)*2, depth(ga)*2, VFF_TYP_1_BYTE);
    if(!lga2khalimsky3d(ga, out, bar))
    {
      fprintf(stderr, "%s : echec de lga2khalimsky \n", 
	      argv[0]);
      freeimage(ga); freeimage(out);
      exit(1);
    }
  }
  writeimage(out, argv[3]);
  freeimage(ga); freeimage(out);
  return 0;
}
