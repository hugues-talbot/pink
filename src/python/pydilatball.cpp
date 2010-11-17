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
#define error(msg) {stringstream fullmessage; fullmessage << "in pydilatball.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    xvimage* main_dilatball( xvimage * image, int r, int mode)
    {
      xvimage * res;

      if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 4) && 
          (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
      {
        error("distance type must be one of [0|1|2|3|4|8|6|18|26]");
      } /* if mode */

      if (depth(image) == 1) // 2D
      {
        if (r >= 0)
        {
          if (! ldilatdisc(image, r, mode))
          {
            error("function ldilatdisc failed");
          }
        }
        else
        {
          if (mode == 3)
          {
            if (!convertlong(&image))
            {
              error("function convertlong failed");
            }
            if (! (res = lredt2d(image)))
            {
              error("function lredt2d failed");
            }
          }
          else
          {
            if (! (res = ldilatdiscloc(image, mode)))
            {
              error("function ldilatdiscloc failed");
            }
          }
        }
      }
      else // 3D
      {
        if (r >= 0)
        {
          if (! ldilatball(image, r, mode))
          {
            error("function ldilatball failed");
          }
        }
        else
        {
          if (! (res = ldilatballloc(image, mode)))
          {
            error("function ldilatballloc failed");        
          }
        }
      }

      if (r >= 0) 
      {   
        return image; 
      }
      else 
      {
        freeimage(image);
        return res;
      }
    } /* ldilatball */



    char_image dilatball(
      char_image & image, ///!!!!!!!!!!!!!!! const here
      int radius,
      int mode=0
      )
    {
      xvimage * expendable;
      xvimage * old_school_result;  
      expendable = copyimage(image); 

      old_school_result=main_dilatball(expendable, radius, mode);

      char_image result(*old_school_result); 

      freeimage(expendable);
      freeimage(old_school_result);
  
      return result;  
    } /* py_dilatball */


  } /* namespace python */
} /* namespace pink */







    
UI_EXPORT_ONE_FUNCTION(
  dilatball,
  pink::python::dilatball,
  ( arg("image"), arg("radius"), arg("mode")=0 ),
  "brief morphological binary dilation by a ball"
  ""
  "<B>Usage:</B> dilatball in.pgm r [dist] out.pgm"
  ""
  "<B>Description:</B>"
  "Morphological binary dilation by a metric ball."
  "If the parameter r is a positive integer, then"
  "the structuring element for this dilation is a ball (or a disc) of radius r."
  "In this case the result of the dilation is defined by Y = {x; d(x,X) <= r} where X"
  "represents the original point set (non-null pixels in the original image)."
  "If the parameter r is -1, then "
  "the structuring element for each point x is a ball (or a disc) of radius F(x),"
  "where F is the greyscale image  in.pgm ."
  ""
  "The dilation is computed by thresholding a distance map."
  "The distance used depends on the optional parameter dist (default is 0) :"
  "- 0: approximate euclidean distance"
  "- 1: approximate quadratic euclidean distance"
  "- 2: chamfer distance"
  "- 3: exact quadratic euclidean distance"
  "- 4: 4-distance in 2d"
  "- 8: 8-distance in 2d"
  "- 6: 6-distance in 3d"
  "- 18: 18-distance in 3d"
  "- 26: 26-distance in 3d"
  ""
  "warning The input image in.pgm must be a binary image. No test is done."
  ""
  "<B>Types supported:</B> byte 2d, byte 3d"
  ""
  "<B>Category:</B> morpho"
  "group  morpho"
  ""
  "author Michel Couprie 2002"
// end of the documenation
  );
















































// LuM end of file
