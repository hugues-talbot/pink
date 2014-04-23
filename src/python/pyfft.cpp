/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "lfft.h"
#include "lcrop.h"
#include "ujimage.hpp"

namespace pink {
  namespace python {

    fcomplex_image
    py_fft(
      fcomplex_image image,
      int32_t dir
      )
    {
      if (image.size().size()!=2)
      {
        pink_error("Only 2D complex images are supported by FFT.");        
      } /* if */

      int32_t rs, cs, cs2, rs2;
      fcomplex_image result;
      
      cs = image.size()[0];            /* Number of rows */
      rs = image.size()[1];            /* Number of columns */
  
      rs2 = cs2 = 1;

      // We are looking for the smallest power of two greater or equal
      // then the image
      while (rs2 < rs) rs2 = rs2 << 1;
      while (cs2 < cs) cs2 = cs2 << 1;

      if ((rs2 != rs) || (cs2 != cs))
      {
        result.reset(rs2, cs2);

        result.xdim() = image.xdim();
        result.ydim() = image.ydim();
        result.zdim() = image.zdim();
        
        razimage(result);
        
        if (!linsert(image, result, 0, 0, 0))
          pink_error("function linsert failed");
        
        // NOTE: these commands are not necessary in C++
        ///freeimage(image);
        //image = image2;
      }
      // else /* NOT rs2!=rs or cs2!=cs */
      // {
      //   result = new fcomplex_image();
      //   (*result) = image.clone();
      // } /* NOT rs2!=rs or cs2!=cs */
      
  
      /* if (! lfft(image, dir)) */
      /* { */
      /*   fprintf(stderr, "%s: function lfft failed\n", argv[0]); */
      /*   exit(1); */
      /* } */
      lfft(result, dir);

      return result;      
    } /* py_fft */

  } /* namespace python */
} /* namespace pink */


// UI_EXPORT_ONE_FUNCTION( fft, 
//                         pink::python::py_fft, 
//                         (arg("image"), arg("direction")=0),
//                         doc__fft__c__
//   );

































// LuM end of file
