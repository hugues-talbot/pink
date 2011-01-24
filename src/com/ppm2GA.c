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
  struct xvimage * r;
  struct xvimage * v;
  struct xvimage * b;
  struct xvimage * ga;
  int32_t param;
  if ( (argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s im.pgm param [alpha] out.ppm \nparam = 0 max (sur RGB) des differences entre pixel extremite, param = 1 distance euclidienne entre pixels extremités, param = 2 affinité floue.\n", 
            argv[0]);
    exit(1);
  }
  if ( readrgbimage(argv[1], &r,&v,&b) == 0 )
  {
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  if( (ga = allocGAimage(argv[2], r->row_size , r->col_size, depth(r), VFF_TYP_GABYTE )) == NULL )
  {
      fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
      freeimage(r); freeimage(v); freeimage(b);
      exit(1);
  }
  param = atoi(argv[2]);

  if(depth(r) == 1)
    {
      if( lppm2ga(r,v,b,ga,param) != 1 )
      {
	fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
	exit(1);
      }
    }
  else
    {
      fprintf(stderr, "%s: Ne traite pas les images 3D couleurs\n", argv[0]);
      exit(1);
    }
  writerawGAimage(ga, argv[3]); 
  freeimage(r); freeimage(v); freeimage(b);freeimage(ga);
  return 0;
}
