/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pydistc.cpp: " << msg; call_error(fullmessage.str());}

// you should not use one-letter macro names!
#undef N
#undef D

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    boost::python::object
    distc( const char_image & original_image,
           int mode )
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
        error("filein.pgm mode fileout.pgm"
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
      //   error("%s: allocimage failed");
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
            error("%s: ldisteuc failed");
          }
        }
        else
        {
          if (! ldisteuc3d(image, *result))
          {
            error("%s: ldisteuc3d failed");
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
            error("%s: ldistquad failed");
          }
        }
        else
        {
          if (! ldistquad3d(image, *result))
          {
            error("%s: ldistquad3d failed");
          }
        }
      }
      else if (mode == 2)
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        if (! lchamfrein(image, *result))
        {
          error("%s: lchamfrein failed");
        }
      }
      else if ((mode == 3) || (mode == 5))
      {
        if (! lsedt_meijster(image, *result))
        {
          error("%s: lsedt_meijster failed");
        }
        if (mode == 5)
        {
          float *D;
          convertfloat(&result);
          D = FLOATDATA(result);
          for (int i = 0; i < N; i++) D[i] = (float)sqrt(D[i]);
        }
      }
      else if (mode < 40)
      {
        for (int i = 0; i < N; i++) // inverse l'image
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        if (! ldist(image, mode, *result))
        {
          error("%s: ldist failed");
        }
      }
      else
      {
        if (! ldistbyte(image, mode, *result))
        {
          error("%s: ldist failed");
        }
      }


      boost::python::object * to_return;
      
      if (mode < 40)        
        //result = new int_image(image.get_size());
        to_return = new boost::python::object( *dynamic_cast<int_image*>(result) );      
      else
        //result = new char_image(image.get_size());
        to_return = new boost::python::object( *dynamic_cast<char_image*>(result) );
      
      
      
      return *to_return;
    } /* distc */
  
    
    


  } /* namespace python */
} /* namespace pink */

// UI_EXPORT_ONE_FUNCTION(
//   distc,
//   pink::python::distc,
//   ( arg("image"), arg("mode")),
//   "brief distance transform (internal)"
//   ""
//   "<B>Usage:</B> distc in.pgm mode out.pgm"
//   ""
//   "<B>Description:</B>"
//   "Distance to the complementary of the object X defined by the binary "
//   "image b in.pgm ."
//   "The result function DX(x) is defined by: DX(x) = min {d(x,y), y not in X}."
//   ""
//   "The distance d used depends on the parameter b mode :"
//   "li 0: approximate euclidean distance (truncated)"
//   "li 1: approximate quadratic euclidean distance"
//   "li 2: chamfer distance"
//   "li 3: exact quadratic euclidean distance"
//   "li 4: 4-distance in 2d"
//   "li 5: exact euclidean distance (float)"
//   "li 8: 8-distance in 2d"
//   "li 6: 6-distance in 3d"
//   "li 18: 18-distance in 3d"
//   "li 26: 26-distance in 3d"
//   "li 40: 4-distance in 2d (byte coded ouput)"
//   "li 80: 8-distance in 2d (byte coded ouput)"
//   "li 60: 6-distance in 3d (byte coded ouput)"
//   "li 180: 18-distance in 3d (byte coded ouput)"
//   "li 260: 26-distance in 3d (byte coded ouput)"
//   ""
//   "The output b out.pgm is of type int32_t for modes < 40, of type byte for other modes."
//   ""
//   "<B>Types supported:</B> byte 2d, byte 3d"
//   ""
//   "<B>Category:</B> morpho"
//   "ingroup  morpho"
//   ""
//   "author Michel Couprie, Xavier Daragon"
//   );














// LuM end of file
