/* $Id: lenoir.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* 

\brief Calcul d'une géodésique dans un espace discret (2D ou 3D)

<B>Usage:</B> Lenoir in.pgm source_point_x source_point_y [source_point_z] target_point_x target_point_y [target_point_z] connex out.pgm

<B>Description:</B> Calcul de la géodésique entre le point source et le point target

<B>Types supported:</B> byte 3d

<B>Category:</B> Topology
\ingroup  orders

\author Massieu Julien
\author Parly Sébastien
*/
/* Massieu Julien & Parly Sébastien - Février 2005 */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include "llenoir.h"

int main(int argc, char **argv) 
{
	struct xvimage *image, *out; //xvimages
	int32_t xb,yb,zb,xe,ye,ze;
	int32_t x,y;//Points entre lesquels on applique l'algorithme Lenoir1
	int32_t rs, cs, ds, connex; //row-size col-size depth_size et connexité

	if ((argc != 8) && (argc != 10))
	{
	  fprintf(stderr, "usage: %s in.pgm xb yb [zb] xe ye [ze] connex out.pgm \n", argv[0]);
	  exit(0);
  	}

	if (argc == 8)
	  connex = atoi(argv[6]);
	else connex = atoi(argv[8]);

	if ((argc == 8) && ( (connex != 4) && (connex != 8))) {
	  fprintf(stderr, "%s: connexity for 2D images should be 4 or 8\n", argv[0]);
	  exit(0);
	}
	if ((argc == 10) && ( (connex != 6) && (connex != 18) && (connex != 26))) {
	  fprintf(stderr, "%s: connexity for 3D images should be 6, 18 or 26\n", argv[0]);
	  exit(0);
	}

	if (argc == 8) {
	  xb = atoi(argv[2]);
	  yb = atoi(argv[3]);
	  zb = 0;
	  xe = atoi(argv[4]);
	  ye = atoi(argv[5]);
	  ze = 0;
	} else {
	  xb = atoi(argv[2]);
	  yb = atoi(argv[3]);
	  zb = atoi(argv[4]);
	  xe = atoi(argv[5]);
	  ye = atoi(argv[6]);
	  ze = atoi(argv[7]);
	}

  	image = readimage(argv[1]); //lecture de l'image d'entrée  
  	if (image == NULL)
  	{
    		fprintf(stderr, "Lenoir: readimage failed\n");
    		exit(0);
  	}
	
	rs = rowsize(image);
	cs = colsize(image);
	ds = depth(image);
	x = xb + yb*rs + zb*rs*cs;
	y = xe + ye*rs + ze*rs*cs;
	out = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);

	Lenoir(image, x, y, connex, out);//Application de l'algorithme
	if (argc == 8)
	  writeascimage(out, argv[7]);
	else
	  writeascimage(out, argv[9]);
	freeimage(image);
	freeimage(out);
	
	return 1;
}/* Main */
