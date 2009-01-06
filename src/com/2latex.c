/* $Id: 2latex.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file pgm2latex.c

\brief converts a grayscale image into a latex file

<B>Usage:</B> pgm2latex image.pgm [bold.pgm [underl.pgm]] out.tex

<B>Description:</B>
Converts a grayscale image into a latex file. The value of each pixel is printed.

<B>Types supported:</B> byte 2d

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#define HEAD1 "\\documentstyle[A4]{article}"
#define HEAD2 "\\unitlength=1cm"
#define HEAD3 "\\parskip=0cm"
#define HEAD4 "\\begin{document}"
#define M1 "\\newcommand{\\za}[1]{\\makebox[0.8cm]{\\rule[-0.1cm]{0cm}{0.8cm}\\bf #1}}"
#define M2 "\\newcommand{\\zb}[1]{\\makebox[0.8cm]{\\rule[-0.1cm]{0cm}{0.8cm}#1}}"
#define M3 "\\newcommand{\\zc}[1]{\\makebox[0.8cm]{\\rule[-0.1cm]{0cm}{0.8cm}\\underline{\\bf #1}}}"
#define M4 "\\newcommand{\\zd}[1]{\\makebox[0.8cm]{\\rule[-0.1cm]{0cm}{0.8cm}\\underline{#1}}}"
#define BEGIMAGE "{\\tabcolsep=0cm\\begin{tabular}{"
#define ENDIMAGE "\\end{tabular}}"
#define ENDDOC "\\end{document}"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * bold = NULL;
  struct xvimage * underl = NULL;
  FILE *fd = NULL;
  int32_t rs, cs, x, y;
  char *macro;

  if ((argc < 3) || (argc > 5))
  {
    fprintf(stderr, "usage: %s image.pgm [bold.pgm [underl.pgm]] out.tex \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "2latex: readimage failed\n");
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;
  if ((rs > 20) || (cs > 20))
  {
    fprintf(stderr, "2latex: image too big\n");
    exit(1);
  }

  if (argc > 3)
  {
     bold = readimage(argv[2]);
     if (bold == NULL)
     {
       fprintf(stderr, "2latex: readimage failed\n");
       exit(1);
     }
     if ((bold->row_size != rs) || (bold->col_size != cs))
     {
       fprintf(stderr, "2latex: incompatible image sizes\n");
       exit(1);
     }
  }

  if (argc > 4)
  {
     underl = readimage(argv[3]);
     if (underl == NULL)
     {
       fprintf(stderr, "2latex: readimage failed\n");
       exit(1);
     }
     if ((underl->row_size != rs) || (underl->col_size != cs))
     {
       fprintf(stderr, "2latex: incompatible image sizes\n");
       exit(1);
     }
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "2latex: cannot open file: %s\n", argv[argc - 1]);
    exit(1);
  }


  fprintf(fd, "%s\n%s\n%s\n%s\n", HEAD1, HEAD2, HEAD3, HEAD4);
  fprintf(fd, "%s\n%s\n%s\n%s\n", M1, M2, M3, M4);

  fprintf(fd, "%s\n\n", image->name);    

  fprintf(fd, "%s", BEGIMAGE);
  for (x = 0; x < rs; x++) fprintf(fd, "l");
  fprintf(fd, "}\n");

  for (y = 0; y < cs; y++)
  {
    for (x = 0; x < rs; x++)
    {
      if (bold && *(UCHARDATA(bold) + y * rs + x))
      {
        if (underl && *(UCHARDATA(underl) + y * rs + x))
          macro = (char *)"\\zc";
        else
          macro = (char *)"\\za";
      }
      else
      {
        if (underl && *(UCHARDATA(underl) + y * rs + x))
          macro = (char *)"\\zd";
        else
          macro = (char *)"\\zb";
      }
      fprintf(fd, "%s{%3d}", macro, *(UCHARDATA(image) + y * rs + x));
      if (x != rs - 1) fprintf(fd, "&");
    } /* for x */
    fprintf(fd, "%s\n", "\\\\");
  } /* for y */

  fprintf(fd, "%s\n", ENDIMAGE);
  fprintf(fd, "%s\n", ENDDOC);
  fclose(fd);
  freeimage(image);
  if (argc > 4) freeimage(underl);
  if (argc > 3) freeimage(bold);
  return 0;
}

