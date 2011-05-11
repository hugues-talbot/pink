/*
Copyright ESIEE (2009)

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file fits2pgm.c

\brief converts from fits format into pgm format

<B>Usage:</B> fits2pgm in.fits out.pgm

<B>Description:</B> Converts from fits format into pgm format.

Parameters:
\li \b in.fits : source file in fits format

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2000 */

#  include <stdio.h>

#ifdef HAVE_FITS_LIB

#  include <stdint.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <assert.h>
#  include <mcimage.h>
#  include <mccodimage.h>
#  include <mcutil.h>
#  include "fitsio.h"
#  include "lfits2pgm.h"


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */

{

    struct xvimage *image;
    char *filename;

    if (argc != 3) {
        fprintf(stderr, "usage: %s in.fits out.pgm \n", argv[0]);
        exit(1);
    }

    filename=argv[1];
    if (filename == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }


    image=lfits2pgm(filename);

    if (! image)
  {
    fprintf(stderr, "%s: function lfits2pgm failed\n", argv[0]);
    exit(1);
  }

    writeimage(image, argv[argc-1]);
  	freeimage(image);


    return 0;
} // main

#else // HAVE_FITS_LIB

int main()
{
    fprintf(stderr, "The FITSIO library was not available when this program was compiled\n");
    return (1);
}

#endif HAVE_FITS_LIB



