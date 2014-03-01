/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus

#include "ujimage.hpp"

// you shouldn't use one character macros
#undef N

namespace pink {

  image<uint8_t>
  genball( double radius, int dim )
  {    
    image<uint8_t> result;
    
    if (dim==2) 
    {
      result.reset( 2*radius+1, 2*radius+1 );
      result.center() = {radius, radius};

      FOR(q, result.cols())
        FOR(w, result.rows())
        if ( sqrt(q*q + w*w) <= radius )
          result(q, w) = 255;
    }
    else /* NOT dim == 2 */
      if (dim==3)
      {               
        result.reset( 2*radius+1, 2*radius+1, 2*radius+1 );
        result.center() = { radius, radius, radius };

        FOR(q, result.depth())
          FOR(w, result.cols())
          FOR(e, result.rows())
          if ( sqrt( q*q + w*w + e*e )<=radius)
            result(e,w,q)=255;
      }
      else /* NOT dim == 3 */
        pink_error("only 2D and 3D supported in this moment.");           

    
    return result;  
    
  } /* genball */
  




  
} /* namespace pink */








#endif /*__cplusplus*/
/* LuM end of file */
