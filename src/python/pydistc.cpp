/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include "ldist.h"
#include "pink_python.h"
#include "ui_convert.hpp"

#include <boost/cast.hpp>

// you should not use one-letter macro names!
#undef N
#undef D

using namespace boost::python;
using namespace pink;
using boost::polymorphic_cast;


namespace pink {
  namespace python {

    boost::python::object
    distc(
      const char_image & original_image,
      int mode
      )
    {
      pink_image * result;
      char_image image;
      image.copy(original_image);
      int N;
      uint8_t *F;

      if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 5) &&
          (mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26) &&
          (mode != 40) && (mode != 80) && (mode != 60) && (mode != 180) && (mode != 260))
      {
        pink_error("filein.pgm mode fileout.pgm"
              "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n"
              "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n"
              "                40, 80 (2D), 60, 180, 260 (3D)\n");
      } /* if mode */

      if (mode < 40)        
        // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
        result = new int_image(image.get_size());      
      else
        // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
        result = new char_image(image.get_size());
      
      // if (result == NULL)
      // {   
      //   pink_error("%s: allocimage failed");
      // }

      N = image.get_size().prod();//rowsize(image) * colsize(image) * depth(image);
      F = UCHARDATA(image.get_output());

      if (mode == 0)
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        if (depth(image.get_output()) == 1)
        {
          if (! ldisteuc(image, *result))
          {
            pink_error("%s: ldisteuc failed");
          }
        }
        else
        {
          if (! ldisteuc3d(image, *result))
          {
            pink_error("%s: ldisteuc3d failed");
          }
        }
      }
      else if (mode == 1)
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        if (depth(image.get_output()) == 1)
        {
          if (! ldistquad(image, *result))
          {
            pink_error("%s: ldistquad failed");
          }
        }
        else
        {
          if (! ldistquad3d(image, *result))
          {
            pink_error("%s: ldistquad3d failed");
          }
        }
      }
      else if (mode == 2)
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        if (! lchamfrein(image, *result))
        {
          pink_error("%s: lchamfrein failed");
        }
      }
      else if ((mode == 3) || (mode == 5))
      {
        if (! lsedt_meijster(image, *result))
        {
          pink_error("%s: lsedt_meijster failed");
        }
        if (mode == 5)
        {
          float *D;
          pink_image * tmp = result;
          result = new float_image();
          polymorphic_cast<float_image*>(result)->copy(pink::convert2float(*polymorphic_cast<int_image*>(tmp)));
          delete tmp;          
          //convertfloat(&result);
          D = FLOATDATA(polymorphic_cast<float_image*>(result)->get_output());
          for (int i = 0; i < N; i++) D[i] = (float)sqrt(D[i]);
        }
      }
      else if (mode < 40)
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        if (! ldist(image, mode, *result))
        {
          pink_error("%s: ldist failed");
        }
      }
      else
      {
        if (! ldistbyte(image, mode, *result))
        {
          pink_error("%s: ldist failed");
        }
      }


      boost::python::object * to_return;

      if (mode == 5)
      {
        to_return = new boost::python::object( *polymorphic_cast<float_image*>(result) );
      }
      else  /* NOT mode == 5 */
      {
        
        if (mode < 40)
        {        
          //result = new int_image(image.get_size());
          to_return = new boost::python::object( *polymorphic_cast<int_image*>(result) );
        }        
        else /* NOT mode < 40 */
        {        
          //result = new char_image(image.get_size());
          to_return = new boost::python::object( *polymorphic_cast<char_image*>(result) );
        }      
      } /* NOT mode == 5 */      
      
      return *to_return;
    } /* distc */
  
    

    boost::python::object
    dist(
      const char_image & original_image,
      int mode
      )
    {
      pink_image * result;
      char_image image;
      image.copy(original_image);
      int N;
      uint8_t *F;

      if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 5) &&
          (mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26) &&
          (mode != 40) && (mode != 80) && (mode != 60) && (mode != 180) && (mode != 260))
      {
        pink_error("filein.pgm mode fileout.pgm"
              "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n"
              "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n"
              "                40, 80 (2D), 60, 180, 260 (3D)\n");
      } /* if mode */

      if (mode < 40)        
        // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
        result = new int_image(image.get_size());      
      else
        // result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
        result = new char_image(image.get_size());
      
      // if (result == NULL)
      // {   
      //   pink_error("%s: allocimage failed");
      // }

      N = image.get_size().prod();//rowsize(image) * colsize(image) * depth(image);
      F = UCHARDATA(image.get_output());

      if (mode == 0)
      {
        if (depth(image.get_output()) == 1)
        {
          if (! ldisteuc(image, *result))
          {
            pink_error("%s: ldisteuc failed");
          }
        }
        else
        {
          if (! ldisteuc3d(image, *result))
          {
            pink_error("%s: ldisteuc3d failed");
          }
        }
      }
      else if (mode == 1)
      {
        if (depth(image.get_output()) == 1)
        {
          if (! ldistquad(image, *result))
          {
            pink_error("%s: ldistquad failed");
          }
        }
        else
        {
          if (! ldistquad3d(image, *result))
          {
            pink_error("%s: ldistquad3d failed");
          }
        }
      }
      else if (mode == 2)
      {
        if (! lchamfrein(image, *result))
        {
          pink_error("%s: lchamfrein failed");
        }
      }
      else if ((mode == 3) || (mode == 5))
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

        if (! lsedt_meijster(image, *result))
        {
          pink_error("%s: lsedt_meijster failed");
        }
        if (mode == 5)
        {
          float *D;
          pink_image * tmp = result;
          result = new float_image();
          polymorphic_cast<float_image*>(result)->copy(pink::convert2float(*polymorphic_cast<int_image*>(tmp)));
          delete tmp;          
          //convertfloat(&result);
          D = FLOATDATA(polymorphic_cast<float_image*>(result)->get_output());
          for (int i = 0; i < N; i++) D[i] = (float)sqrt(D[i]);
        }
      }
      else if (mode < 40)
      {
        if (! ldist(image, mode, *result))
        {
          pink_error("%s: ldist failed");
        }
      }
      else
      {
        if (! ldistbyte(image, mode, *result))
        {
          pink_error("%s: ldist failed");
        }
      }


      boost::python::object * to_return;

      if (mode == 5)
      {
        to_return = new boost::python::object( *polymorphic_cast<float_image*>(result) );
      }
      else  /* NOT mode == 5 */
      {
        
        if (mode < 40)
        {        
          //result = new int_image(image.get_size());
          to_return = new boost::python::object( *polymorphic_cast<int_image*>(result) );
        }        
        else /* NOT mode < 40 */
        {        
          //result = new char_image(image.get_size());
          to_return = new boost::python::object( *polymorphic_cast<char_image*>(result) );
        }      
      } /* NOT mode == 5 */      
      
      return *to_return;
    } /* dist */
  
        
    


  } /* namespace python */
} /* namespace pink */

void distc_export()
{

  UI_DEFINE_ONE_FUNCTION(
    dist,
    pink::python::dist,
    ( arg("image"), arg("mode")),
    doc__dist__c__
    // end of the documenation    
    );
  
  
  UI_DEFINE_ONE_FUNCTION(
    distc,
    pink::python::distc,
    ( arg("image"), arg("mode")),
    doc__distc__c__
    // end of the documenation
    );
  
  
} /* distc_export */











// LuM end of file
