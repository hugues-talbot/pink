/* $Id: relief.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file relief.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Sauvegarde dans un fichier le profil 3D d'une portion d'image */
/* inscrite dans une fenetre autour d'un point de coord. x,y     */
/* entree: image x y taille_fenetre nom_fichier_sortie           */
/* sortie: fichier du profil 3D (coord. x y z)                   */

/* Regis VINCIGUERRA - mai 98 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>  

int main(int argc, char **argv)
{
  struct xvimage * image;
  uint8_t *I;
  int32_t rs, cs, N;
  int32_t xp, yp;
  int32_t x,y;
  int32_t tf;
  FILE *profil;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in1.pgm xmax ymax taille_fenetre out \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  I = UCHARDATA(image);
  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;
  
  xp = atoi(argv[2]);
  yp = atoi(argv[3]);
  tf = atoi(argv[4])/2;
  profil = fopen(argv[5],"w");

  for (y=yp-tf;y<yp+tf;y++)
    for (x=xp-tf;x<xp+tf;x++)
      if ((y>=0) && (y<=cs) && (x>=0) && (x<=rs))
	fprintf(profil,"%d %d %d\n",x,y,I[x+y*rs]);
  
  fclose(profil);
  freeimage(image);
  
  return 0;
} /* main */
