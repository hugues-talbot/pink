/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include "pyexport.hpp"

#include "ldist.h"
#include "pyasfbin.hpp"

using pink::char_image;


char_image pink::python::asfbin(
  const char_image & image,
  const index_t & max_radius,
  const index_t & min_radius
  )
{
  char_image result;
  result = image.clone();      

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
        pink_error("function ldilatball failed");
      }
      if (! lerosball(result, rayon, 0))
      {
        pink_error("function lerosball failed");
            
      }
      /* ouverture par l'element structurant */
      if (! lerosball(result, rayon, 0))
      {
        pink_error("function lerosball failed");
            
      }
      if (! ldilatball(result, rayon, 0))
      {
        pink_error("function ldilatball failed");
            
      }
    }
    else
    {
      /* ouverture par l'element structurant */
      if (! lerosball(result, rayon, 0))
      {
        pink_error("function lerosball failed");
            
      }
      if (! ldilatball(result, rayon, 0))
      {
        pink_error("function ldilatball failed");
            
      }
      /* fermeture par l'element structurant */
      if (! ldilatball(result, rayon, 0))
      {
        pink_error("function ldilatball failed");
            
      }
      if (! lerosball(result, rayon, 0))
      {
        pink_error("function lerosball failed");
            
      }
    }

  } /* for (rayon = 1; rayon <= rayonmax; rayon++) */

  return result;      
} /* asfbin */














































// LuM end of file
