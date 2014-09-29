/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2014
  BME-Infokom
  ujoimro@gmail.com
*/

#include <stdio.h>

#include "larith.h"
#include "mcimage.h"
#include "mccodimage.h"

int32_t lsurimp( const xvimage * grayscale, 
                 const xvimage * binary, 
                 const char * filename ) {
# define F_NAME "lsurimp"
  ACCEPTED_TYPES1( grayscale, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1( binary, VFF_TYP_1_BYTE);

  struct xvimage * tmp;
  struct xvimage * tmp3;

  tmp = copyimage(binary);
  linverse(tmp);
  tmp3 = copyimage(grayscale);
  lmin(tmp3,tmp);
  copy2image(tmp,binary);
  ladd(tmp,tmp3);
  writergbimage(tmp, tmp3, tmp3, filename);

  freeimage(tmp);
  freeimage(tmp3);

  return 1;
# undef F_NAME
}




// LuM end of file
