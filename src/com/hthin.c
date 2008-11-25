/* $Id: hthin.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file hthin.c

\brief grayscale homotopic thinning

<B>Usage:</B> hthin in.pgm <imcond.pgm|null> connex niter out.pgm

<B>Description:</B>
Grayscale homotopic thinning (refs. [BEC97, CBB01]).
The parameter \b connex gives the connectivity used for the minima;
possible choices are 4, 8 in 2D and 6, 26 in 3D.
Let F be the function corresponding to the input image \b in.pgm.
Let G be the function corresponding to the input image 
\b imcond.pgm, or the null function if the keyword \b null is used.
The algorithm is the following:

\verbatim
Repeat niter times:
    L = {(p,d) such that p is destructible for F and d = delta-(p,F)}
    While L not empty
       extract a couple (p,d) from L
       F(p) = max{ d, delta-(p,F), G(p) }    
Result: F
\endverbatim

If \b niter is set to -1, then the operator iterates until stability.

References:

[BEC97] G. Bertrand, J. C. Everat and M. Couprie: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag97.ps.gz">"Image segmentation through operators based upon topology"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.

[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.

<B>Types supported:</B> byte 2D, byte 3D.

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie

<B>Example:</B>

hthin ur1 null 4 10 ur1_hthin<br>
minima ur1_hthin 4 ur1_hthin_m

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/ur1.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/ur1_hthin.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/ur1_hthin_m.gif"></td>
  </tr>
  <tr><td align="center">ur1</td>
      <td align="center">ur1_hthin</td>
      <td align="center">ur1_hthin_m</td>
  </tr>
</table>
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t nitermax;
  int32_t connex;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s im.pgm <imcond.pgm|null> connex niter fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8) && (connex != 6) && (connex != 26))
  {
    fprintf(stderr, "%s : connex = <4|8|6|26>\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[4]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if ((connex == 4) || (connex == 8))
  {
    if (! lhthindelta(image, imagecond, nitermax, connex))
    {
      fprintf(stderr, "%s: lhthindelta failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lhthindelta3d(image, imagecond, nitermax, connex))
    {
      fprintf(stderr, "%s: lhthindelta3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
