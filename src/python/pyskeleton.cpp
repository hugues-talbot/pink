/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "pyexport.hpp"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include "ldist.h"
#include "lskeletons.h"
#include "lsquelbin.h"
#include "python_doc.h"

// based on skeleton.c

// ERROR N is not supposed to be used as a define macro
#undef N

namespace pink {
  namespace python {

//prototype
    char_image skeleton_ultimate(
      const char_image image, 
      const int_image  prio,
      index_t          priocode,
      index_t          connex, 
      const char_image inhibit,
      index_t          inhibitcode
      );



/* nc == non_const */

    char_image
    skeleton_im_int_int_void ( const char_image & image, index_t priocode, index_t connex ) {
      return skeleton_ultimate( image, int_image(), priocode, connex, char_image(), -1 );
    } /* skeleton_im_int_int_void */


    char_image
    skeleton_im_im_int_void ( const char_image & image, const int_image & prio, index_t connex ) {
      return skeleton_ultimate( image, prio, -1, connex, char_image(), -1 );
    } /* skeleton_im_im_int_void */

    char_image
    skeleton_im_int_int_im( const char_image & image, index_t prio, index_t connex, const char_image & inhibit ) {
      return skeleton_ultimate( image, int_image(), prio, connex, inhibit, -1 );
    };

    char_image
    skeleton_im_im_int_im( const char_image & image, const int_image & prio, index_t connex, const char_image & inhibit ) {
      return skeleton_ultimate( image, prio, -1, connex, inhibit, -1 );
    };

    char_image
    skeleton_im_int_int_int( const char_image & image, index_t prio, index_t connex, index_t inhibit_value ) {
      return skeleton_ultimate( image, int_image(), prio, connex, char_image(), inhibit_value );
    };

    char_image
    skeleton_im_im_int_int( const char_image & image, const int_image & prio, index_t connex, index_t inhibit_value ) {
      return skeleton_ultimate( image, prio, -1, connex, char_image(), inhibit_value );
    };

  
    char_image
    skeleton_ultimate(
      const char_image image, 
      const int_image  prio,
      const index_t    priocode,
      const index_t    connex, 
      const char_image inhibit,
      const index_t    inhibitvalue
      )
    {

      char_image result = image.clone();
      int_image m_prio;
      
      index_t dim = image.size().size();
      if ((dim<2) || (dim>3)) pink_error("only 2D and 3D images are supported.");
            
      if (!prio.isnull())
      {
        m_prio = prio;        
      }
      else
      { 
        // the distance map has to be calculated
        index_t N;
        uint8_t *F;
        m_prio.reset(result.size());
    
        N = pink::prod(result.size());
        F = result.get();

        // we inverse the result
        for (index_t i = 0; i < N; i++) 
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
    
        // calculating the appropriate distance map
        switch ( priocode )
        {
      
        case 0:
          if ( result.depth() == 1 )
          { if (! ldisteuc(result, m_prio ) ) pink_error("ldisteuc failed") }
          else
          { if (! ldisteuc3d( result, m_prio ) ) pink_error("ldisteuc3d failed"); }
          break;
    
        case 1:      
          if ( result.depth() == 1 )
          { if (! ldistquad( result, m_prio ) ) pink_error("ldistquad failed") }
          else
          { if (! ldistquad3d(result, m_prio ) ) pink_error("ldistquad3d failed") }
          break;

        case 2:
          if (! lchamfrein( result, m_prio ) ) pink_error("lchamfrein failed");
          break;

        case 3:
          if (! lsedt_meijster( result, m_prio ) ) pink_error("lsedt_meijster failed");
          break;

        default:
          if (! ldist( result, priocode, m_prio ) ) pink_error("ldist failed");
          break;

        } /* switch (priocode) */


        // re-inversing the result
        for (index_t i = 0; i < N; i++) 
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

      } /* not if (prio_image) */
    
      if (result.depth() == 1)
      {
        if (inhibit.isnull())
        { if (! lskelubp2( result, m_prio, connex, inhibit ) ) pink_error("lskelubp2 failed"); }
        else  /* not (inhibit) */
        {
          assert(inhibitvalue != -1);
          if (! lskelubp( result, m_prio, connex, inhibitvalue ) ) pink_error("lskelubp failed");
        }
      } 
      else /* not  (depth(image) == 1) */ 
      {
        if (inhibit.isnull())
        { if (! lskelubp3d2( result, m_prio, connex, inhibit ) ) pink_error("lskelubp3d2 failed"); }
        else /* not (inhibit) */
        {
          assert(inhibitvalue != -1);          
          if (! lskelubp3d( result, m_prio, connex, inhibitvalue ) ) pink_error("lskelubp3d failed");
        }
      } /* not (depth(image) == 1) */


      return result;
    };




  } /* namespace python */
} /* namespace pink */


/*
***********************************************************************************************
***********************************************************************************************
***********************************************************************************************

   exported functions

***********************************************************************************************
***********************************************************************************************
***********************************************************************************************
 */

void skeleton_export() 
{
  // def( "skeleton_im_int_int_void", &pink::python::skeleton_im_int_int_void,
  //      args("image", "priority value", "connexity"),
  //      "the help is in 'help(pink.skeleton)'"
  //   );

  // def( "skeleton_im_im_int_void", &pink::python::skeleton_im_im_int_void,
  //      args("image", "priority image", "connexity"),
  //      "the help is in 'help(pink.skeleton)'"
  //   );

  // def( "skeleton_im_int_int_im", &pink::python::skeleton_im_int_int_im,
  //      args("image", "priority value", "connexity", "inhibited pixels"),
  //      "the help is in 'help(pink.skeleton)'"
  //   );

  // def( "skeleton_im_im_int_im", &pink::python::skeleton_im_im_int_im,
  //      args("image", "priority image", "connexity", "inhibited pixels"),
  //      "the help is in 'help(pink.skeleton)'"
  //   );

  // def( "skeleton_im_int_int_int", &pink::python::skeleton_im_int_int_int,
  //      args("image", "priority value", "connexity", "inhibited value"),
  //      "the help is in 'help(pink.skeleton)'"
  //   );

  // def( "skeleton_im_im_int_int", &pink::python::skeleton_im_im_int_int,
  //      args("image", "priority_image", "connexity", "inhibited value"),
  //      "the help is in 'help(pink.skeleton)'"
  //   );  

 
} /* py_skeleton_export */









































// LuM end of file
