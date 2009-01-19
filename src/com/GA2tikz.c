/* $Id: GA2tikz.c,v 1.1 2009-01-19 09:38:30 lnajman Exp $ */
/*! \file GA2tikz.c

\brief converts from GA to latex for illustrating small image configurations

<B>Usage:</B> GA2tikz in.ga [b] out.tikz

<B>Description:</B>
Produces a LaTeX file from a binary or grayscale GA image.
If <B>b</B> is present, the result is binary otherwise the edges are greyscale colored.

The result should be compiled with pdflatex to produce a pdf file.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Laurent Najman
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>

#define BASE_ASCII 'a'

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * ga;
  FILE *fp;
  int file;

  if ((argc != 3) && (argc != 4))
    {
      fprintf(stderr, "usage: %s GA.pgm [b] out.tikz \n", 
	      argv[0]);
      exit(1);
    }
  file = (argc == 3) ? 2 : 3;

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
  
  if ((fp = fopen(argv[file], "w"))==NULL) {
    fprintf(stderr, "%s: pb d'ouverture du fichier %s\n", argv[0], argv[file]);
    freeimage(ga);
    exit(1);
  }
  
  fprintf(fp, "\\documentclass{article}\n");

  fprintf(fp, "\\usepackage{tikz,nicefrac,amsmath,pifont}\n");
  fprintf(fp, "\\usetikzlibrary{arrows,snakes,backgrounds,patterns,matrix,shapes,fit,calc,shadows,plotmarks}\n");

  fprintf(fp, "\\usepackage[graphics,tightpage,active]{preview}\n");
  fprintf(fp, "\\PreviewEnvironment{tikzpicture}\n");
  fprintf(fp, "\\newlength{\\imagewidth}\n");
  fprintf(fp, "\\newlength{\\imagescale}\n");

  fprintf(fp, "\\begin{document}\n");

  fprintf(fp, "\\pgfdeclarelayer{background}\n");
  fprintf(fp, "\\pgfsetlayers{background,main}\n");

  fprintf(fp, "\\tikzstyle{vertex}=[circle,fill=black!25,minimum size=20pt,inner sep=0pt]\n");
  fprintf(fp, "\\tikzstyle{selected vertex} = [vertex, fill=black!25, thick]\n");
  fprintf(fp, "\\tikzstyle{edge} = [draw,thin,-]\n");
  fprintf(fp, "\\tikzstyle{weight} = [font=\\large]\n");
  fprintf(fp, "\\tikzstyle{selected edge} = [draw,very thick,-]\n");
  fprintf(fp, "\\tikzstyle{separating edge} = [draw,dashed,-]\n");

  if (argc == 3) { 
    fprintf(fp, "\\tikzstyle{color0} = [draw,line width=5pt,-,black!05]\n");
    fprintf(fp, "\\tikzstyle{color1} = [draw,line width=5pt,-,black!10]\n");
    fprintf(fp, "\\tikzstyle{color2} = [draw,line width=5pt,-,black!20]\n");
    fprintf(fp, "\\tikzstyle{color3} = [draw,line width=5pt,-,black!30]\n");
    fprintf(fp, "\\tikzstyle{color4} = [draw,line width=5pt,-,black!40]\n");
    fprintf(fp, "\\tikzstyle{color5} = [draw,line width=5pt,-,black!50]\n");
    fprintf(fp, "\\tikzstyle{color6} = [draw,line width=5pt,-,black!60]\n");
    fprintf(fp, "\\tikzstyle{color7} = [draw,line width=5pt,-,black!70]\n");
    fprintf(fp, "\\tikzstyle{color8} = [draw,line width=5pt,-,black!80]\n");
  }

  fprintf(fp, "\\begin{figure}\n");
  fprintf(fp, "\\begin{tikzpicture}[scale=2,auto,swap]\n");

  int32_t rs = rowsize(ga);                   /* taille ligne */ 
  int32_t cs = colsize(ga);                   /* taille colone */
  int32_t N = rs * cs;                        /* taille image */
  uint8_t *GA = UCHARDATA(ga);      
  int i,j,u,x,y;

  // Sommets
  N = 0;
  for(j = 0; j < cs; j++) {
    for (i=0; i < rs; i++) {
      fprintf(fp, "\\node[vertex] (%c) at (%d,%d) {%c};\n", 
	      BASE_ASCII+N, i, j, BASE_ASCII+N);
      N++; // A la fin N = rs*cs
    }
  }
  
  /* on explore d'abord les aretes horizontales */
  for(j = 0; j < cs; j++)
    for(i = 0; i < rs -1; i++){
      u = j * rs + i; x = Sommetx(u,N,rs); y = Sommety(u,N,rs);
      if (argc == 3) {
	fprintf(fp, "\\path[edge] (%c) -- node[weight] {$%d$} (%c);\n",
		BASE_ASCII+x, GA[u], BASE_ASCII+y);
      } else {
	if (GA[u] != 0)
	  fprintf(fp, "\\path[selected edge] (%c) --  (%c);\n",
		  BASE_ASCII+x,  BASE_ASCII+y);
	else 
	  fprintf(fp, "\\path[separating edge] (%c) --  (%c);\n",
		  BASE_ASCII+x,  BASE_ASCII+y);
      }
    }
  /* puis les aretes verticales */
  for(j = 0; j < cs -1; j++)
    for(i = 0; i < rs; i++)
      {
	u = N + j * rs + i; x = Sommetx(u,N,rs); y = Sommety(u,N,rs);
	if (argc == 3) {
	  fprintf(fp, "\\path[edge] (%c) -- node[weight] {$%d$} (%c);\n",
		  BASE_ASCII+x, GA[u], BASE_ASCII+y);
	} else {
	  if (GA[u] != 0)
	    fprintf(fp, "\\path[selected edge] (%c) --  (%c);\n",
		    BASE_ASCII+x,  BASE_ASCII+y);
	  else 
	    fprintf(fp, "\\path[separating edge] (%c) --  (%c);\n",
		    BASE_ASCII+x,  BASE_ASCII+y);
	}
      } 

  if (argc == 3) {
    // Colore les edges
    fprintf(fp,"\\begin{pgfonlayer}{background}\n");
    /* on explore d'abord les aretes horizontales */
    for(j = 0; j < cs; j++)
      for(i = 0; i < rs -1; i++){
	u = j * rs + i; x = Sommetx(u,N,rs); y = Sommety(u,N,rs);
	fprintf(fp, "\\path[color%d] (%c) --  (%c);\n",
		GA[u], BASE_ASCII+x,  BASE_ASCII+y);
      }
    /* puis les aretes verticales */
    for(j = 0; j < cs -1; j++)
      for(i = 0; i < rs; i++)
	{
	  u = N + j * rs + i; x = Sommetx(u,N,rs); y = Sommety(u,N,rs);
	  fprintf(fp, "\\path[color%d] (%c) -- (%c);\n",
		  GA[u], BASE_ASCII+x, BASE_ASCII+y);
	} 
    fprintf(fp, "\\end{pgfonlayer}\n");
  }

  fprintf(fp, "\\end{tikzpicture}\n");
  fprintf(fp, "\\end{figure}\n");
  fprintf(fp, "\\end{document}\n");
  fclose (fp);
  freeimage(ga); 
  return 0;
}
