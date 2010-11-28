/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyclosing.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {

    char_image closing(
      const char_image & image, 
      const char_image & elem
      )
    {
      char_image result;
      result.copy(image);
      char_image elem_ce; // this image is usually small so it's ok to copy it
      elem_ce.copy(elem);
      

      if (not (elem.get_size().inside(elem.get_center())))
      {
        error("The center of the structuring element must be defined");        
      }

      if (not ((image.get_size().size()==2) or (image.get_size().size()==3)) )
      {
        error("Only 2D and 3D images are supported.");        
      }
      

      if (image.get_size().size() == 2)
      {

        int x = elem.get_center()[0];        
        int y = elem.get_center()[1];
        // int z = elem.get_center()[2];
        int rs = elem.get_size()[0];        
        int cs = elem.get_size()[1];        
        //int ds = elem.get_size()[2];

        if (! ldilateros_ldilat(result, elem_ce, rs - 1 - x, cs - 1 - y))
        {
          error("function ldilat failed");
        }

        if (! lsym(elem_ce, 'c'))
        {
          error("function lsym failed");
        }

        if (!ldilateros_leros(result, elem_ce, x, y))
        {
          error("function leros failed");
        }
      }
      else /* NOT image.get_size().size() == 2 */
      {
        int x = elem.get_center()[0];        
        int y = elem.get_center()[1];
        int z = elem.get_center()[2];
        int rs = elem.get_size()[0];        
        int cs = elem.get_size()[1];        
        int ds = elem.get_size()[2];

        
        if (! ldilat3d(result, elem_ce, x, y, z))
        {
          error("function leros3d failed");
        }

        if (! lsym(elem_ce, 'c'))
        {
          error("function lsym failed");
        }

        if (! leros3d(result, elem_ce, rs - 1 - x, cs - 1 - y, ds - 1 - z))
        {
          error("function leros3d failed");
        }
      }  /* NOT image.get_size().size() == 2 */

  
      return result;    
    } /* py_closing */


    char_image closeball(
      const char_image & src, 
      int r,
      int mode=0
      )
    {
      if ((mode != 0) && (mode != 2) && (mode != 4) && 
          (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
      {
        error("dist = [0|2|4|8|6|18|26]");
      } /* if */

      char_image result;
      result.copy(src);
  

      if (result.get_size().size()==2)
      {
        if (! ldilatdisc(result.get_output(), r, mode))
        {
          error("function ldilatdisc failed");
        }
        if (! lerosdisc(result.get_output(), r, mode))
        {
          error("function lerosdisc failed");
        }
      }
      else /* NOT result.get_size().size()==2 */
      {
        if (! ldilatball(result.get_output(), r, mode))
        {
          error("function ldilatball failed");
        }
        if (! lerosball(result.get_output(), r, mode))
        {
          error("function lerosball failed");
        }
      } /* NOT result.get_size().size()==2 */

      return result;
    } /* py_closeball */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_ONE_FUNCTION( closeball, 
			pink::python::closeball, 
			( arg("image"), arg("the ray of the ball"), arg("distance function")=0 ),
			"WRITE ME!!!\n"
  );



UI_EXPORT_ONE_FUNCTION( closing, 
			pink::python::closing,
                        ( arg("src"), arg("elem") ),
                        " \n"
                        "morphological closing by a plane structuring element \n"
                        " \n"
                        "Description: \n"
                        "Morphological closing by a plane structuring element. \n"
                        "The (plane) structuring element is given by the non-null values in 'elem',  \n"
                        "its origin is the 'center' of the 'elem'  \n"
                        " \n"
                        "Types supported: byte 2d, byte 3d \n"
                        " \n"
                        "Category: morpho \n"
                        " \n"
                        "\author Michel Couprie 2002 \n"
                        " \n"
                        // end of the documenation
  );























































// LuM end of file
