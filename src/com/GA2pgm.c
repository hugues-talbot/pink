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
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  double alpha;
  struct xvimage * im;
  struct xvimage * ga;
  int32_t param;
  if(argc != 4) {
    fprintf(stderr, "usage: %s ga.ga param out.pgm \n param = 1 max des sommets contenus dans l'arete, param = 2 min des sommets contenus dans l'aretes\n", 
            argv[0]);
    exit(1);
  }
  param = atoi(argv[2]);
  if( (param!= 1) && (param != 2)){
    fprintf(stderr, "Bad parameter param\nusage: %s ga.ga param out.pgm \n param = 1 max des sommets contenus dans l'arete, param = 2 min des sommets contenus dans l'aretes\n", 
            argv[0]);
    exit(1);
  }

  ga = readGAimage(argv[1]);
  if (ga == NULL) {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if( (im = allocimage(argv[2], ga->row_size , ga->col_size, depth(ga), VFF_TYP_1_BYTE )) == NULL ) {
    fprintf(stderr, "%s: ne peut pas allouer d'image de sortie \n", argv[0]);
    freeimage(ga);
    exit(1);
  }
  if(depth(im) == 1) {
    
    if( lga2pgm(im,ga,param) != 1 ) {
      fprintf(stderr, "%s: erreur de lga2pgm \n", argv[0]);
      exit(1);
    }
  } else {
    if( lga2pgm3d(im,ga,param) != 1 ) {
      fprintf(stderr, "%s: erreur de lga2pgm3d \n", argv[0]);
      exit(1);
    }
  }
  writerawimage(im, argv[3]); 
  freeimage(im); freeimage(ga);
  return 0;
}
