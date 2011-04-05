/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#include "pink_python.h"

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    

    fcomplex_image
    py_fft(
      const fcomplex_image & image,
      int32_t dir
      )
    {
      if (image.get_size().size()!=2)
      {
        pink_error("Only 2D complex images are supported by FFT.");        
      } /* if */

      int32_t rs, cs, cs2, rs2;
      float_image * result;
      
      cs = image.get_size()[0];            /* Number of rows */
      rs = image.get_size()[1];            /* Number of columns */
  
      rs2 = cs2 = 1;

      // We are looking for the smallest power of two greater or equal
      // then the image
      while (rs2 <= rs) rs2 = rs2 << 1;
      while (cs2 <= cs) cs2 = cs2 << 1;

      if ((rs2 != rs) || (cs2 != cs))
      {
        vint new_size(2);
        new_size << rs2, cs2;
        
        result = new float_image(new_size);
        
        result->get_output()->xdim = image->get_output()->xdim;
        result->get_output()->ydim = image->get_output()->ydim;
        result->get_output()->zdim = image->get_output()->zdim;
        
        razimage(image2);
        if (!linsert(image, image2, 0, 0, 0))
        {
          fprintf(stderr, "%s: function linsert failed\n", argv[0]);
          exit(1);
        }
        freeimage(image);
        image = image2;
      } /* if rs2!=rs or cs2!=cs */
  
      /* if (! lfft(image, dir)) */
      /* { */
      /*   fprintf(stderr, "%s: function lfft failed\n", argv[0]); */
      /*   exit(1); */
      /* } */
    
      return lfft(image, dir);
    } /* py_fft */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION( minmax, 
		    pink::python::minmax, 
		    (arg("image")),
                    "Returns the minimum and maximum values of the image.\n"
                    "It's using less comparisons then just looking for the maximum and"
                    "then the minimum."
  );

































// LuM end of file
