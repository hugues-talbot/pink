/* $Id: showheader.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file showheader.c

\brief prints image information

<B>Usage:</B> showheader in.pgm

<B>Description:</B>
Prints all informations contained in the image file header: type, header, size and comments.

<B>Types supported:</B> all pgm and ppm types

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - septembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <mcimage.h>

#define BUFFERSIZE 1024

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s file.pgm \n", argv[0]);
    exit(1);
  }

  if (!showheader(argv[1]))
  {
    fprintf(stderr, "%s: function showheader failed \n", argv[0]);
    exit(1);
  }

  return 0;
} /* main */
