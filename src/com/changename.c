/* $Id: changename.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file changename.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>changename</B> - sets a new name for an image

SYNOPSIS

<B>changename</B> in.pgm name [nameaux]

DESCRIPTION

The image <B>in.pgm</B> receives a new name: <B>name</B> or <B>name</B>(<B>nameaux</B>).    

Types supported: all types.

CLASS 

gene

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

#define BUFFERSIZE 1024

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  char buf[4096];  

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s file.pgm newname [nameaux] \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  sprintf(buf, "%s", argv[2]);
  if (argc == 4)
  {
    strcat(buf, "(");
    strcat(buf, argv[3]);
    strcat(buf, ")");
  }
  image->name = buf;
  writeimage(image, argv[1]);
  image->name = NULL;        /* necessaire pour ne pas faire de free(buf) */
  freeimage(image);
  return 0;
} /* main */
