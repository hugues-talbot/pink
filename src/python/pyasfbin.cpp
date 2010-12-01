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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyasfbin.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
  namespace python {

    char_image asfbin(
      const char_image & image,
      int max_radius,
      int min_radius = 1
      )
    {
      char_image result;
      result.copy(image);

      int32_t rayonmin=min_radius, rayonmax=max_radius, rayon;
      int32_t FERMETURE_EN_1 = 0;

      if (rayonmin == 0) 
      {
        rayonmin = 1;
        FERMETURE_EN_1 = 1;
      }
    
      for (rayon = rayonmin; rayon <= rayonmax; rayon++)
      {
        #ifdef VERBOSE
        fprintf(stderr, "%s: rayon = %d\n", argv[0], rayon);
        #endif

        if (FERMETURE_EN_1)
        {
          /* fermeture par l'element structurant */
          if (! ldilatball(result, rayon, 0))
          {
            error("function ldilatball failed");
          }
          if (! lerosball(result, rayon, 0))
          {
            error("function lerosball failed");
            
          }
          /* ouverture par l'element structurant */
          if (! lerosball(result, rayon, 0))
          {
            error("function lerosball failed");
            
          }
          if (! ldilatball(result, rayon, 0))
          {
            error("function ldilatball failed");
            
          }
        }
        else
        {
          /* ouverture par l'element structurant */
          if (! lerosball(result, rayon, 0))
          {
            error("function lerosball failed");
            
          }
          if (! ldilatball(result, rayon, 0))
          {
            error("function ldilatball failed");
            
          }
          /* fermeture par l'element structurant */
          if (! ldilatball(result, rayon, 0))
          {
            error("function ldilatball failed");
            
          }
          if (! lerosball(result, rayon, 0))
          {
            error("function lerosball failed");
            
          }
        }

      } /* for (rayon = 1; rayon <= rayonmax; rayon++) */

      return result;      
    } /* asfbin */
    


  } /* namespace python */
} /* namespace pink */

UI_EXPORT_ONE_FUNCTION(
  asfbin,
  pink::python::asfbin,
  ( arg("image"), arg("max radius"), arg("min radius")=1),
  "Zoom (shrink or engarle) an image. "
  "The three arguments are the zoom factors. "
  "More advanced parametrization through the 'pink.zoom' function."
  "Note: this function is too rigid (you can't choose the zoom algorithm) "
  " and should definitely be replaced later"
  );



















































// LuM end of file
