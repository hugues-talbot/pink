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

#include <stdio.h>

#include "lsym.h"
#include "ldilateros.h"
#include "ldilateros3d.h"


int32_t 
lopening( struct xvimage * image, struct xvimage * elem, index_t center_x, index_t center_y, index_t center_z ) {
#undef F_NAME
#define F_NAME "lopening"

  index_t rs, cs, ds;

  rs = rowsize(elem);
  cs = colsize(elem);
  ds = depth(elem);

  if (depth(image) == 1)
  {
    if (! ldilateros_leros(image, elem, center_x, center_y))
    {
      fprintf(stderr, "%s: function leros failed\n", F_NAME);
      return 0;
    }

    if (! lsym(elem, 'c'))
    {
      fprintf(stderr, "%s: function lsym failed\n", F_NAME);
      return 0;
    }

    if (! ldilateros_ldilat(image, elem, rs - 1 - center_x, cs - 1 - center_y))
    {
      fprintf(stderr, "%s: function ldilat failed\n", F_NAME);
      return 0;
    }
  }
  else
  {
/*
    fprintf(stderr, "%s: opening 3d Not Yet Implemented\n", F_NAME);
    return 0;
*/
    if (! leros3d(image, elem, center_x, center_y, center_z))
    {
      fprintf(stderr, "%s: function leros3d failed\n", F_NAME);
      return 0;
    }

    if (! lsym(elem, 'c'))
    {
      fprintf(stderr, "%s: function lsym failed\n", F_NAME);
      return 0;
    }

    if (! ldilat3d(image, elem, rs - 1 - center_x, cs - 1 - center_y, ds - 1 - center_z))
    {
      fprintf(stderr, "%s: function ldilat3d failed\n", F_NAME);
      return 0;
    }

  }
  return 1;
#undef F_NAME
}
