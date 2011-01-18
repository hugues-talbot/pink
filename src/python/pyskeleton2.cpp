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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyskeleton2.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


// based on skeleton.c

// ERROR N is not supposed to be used as a define macro
#undef N

namespace pink {
  namespace python {


    // the content of this functon is copycat from
    // skeleton.c
    int_image skeleton_distance(
      char_image & image,
      int priocode
      )
    {
      int_image prio(image.get_size());

      int32_t i, N;
      uint8_t *F;
      N = rowsize(image.get_output()) * colsize(image.get_output()) * depth(image.get_output());
      F = UCHARDATA(image.get_output());
      for (i = 0; i < N; i++) // inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
  
      if (priocode == 0)
      {
        if (depth(image.get_output()) == 1)
        {
          if (! ldisteuc(image.get_output(), prio))
          {
            error("ldisteuc failed");
          }
        }
        else
        {
          if (! ldisteuc3d(image.get_output(), prio))
          {
            error("ldisteuc3d failed");
          }
        }
      }
      else
        if (priocode == 1)
        {
          if (depth(image.get_output()) == 1)
          {
            if (! ldistquad(image.get_output(), prio))
            {
              error("ldistquad failed");
            }
          }
          else
          {
            if (! ldistquad3d(image.get_output(), prio))
            {
              error("ldistquad3d failed");
            }
          }
        }
        else
          if (priocode == 2)
          {
            if (! lchamfrein(image.get_output(), prio))
            {
              error("lchamfrein failed");
            }
          }
          else
            if (priocode == 3)
            {
              if (! lsedt_meijster(image.get_output(), prio))
              {
                error("lsedt_meijster failed");
              }
            }
            else
            {
              if (! ldist(image.get_output(), priocode, prio))
              {
                error("ldist failed");
              }
            }
      for (i = 0; i < N; i++) // re-inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
      
      return prio;
    } /* skeleton_distance */


    template <class image_type>
    char_image
    skeleton_im_prioim_connex_inhibval (
      const char_image & image,
      image_type prioimage,
      int connex,
      int inhibval=-1
      )
    {
      if (inhibval==-1)
      {
        char_image result;        
        result = skeleton_im_prioim_connex_inhibimage(image, prioimage, connex, char_image());
        return result;        
      } /* inhibval == -1 */

      char_image result;
      result.copy(image);

      if (depth(result.get_output()) == 1)  // the image is 2D
      {
        if (! lskelubp(result.get_output(), prioimage, connex, inhibval))
        {
          error("lskelubp failed");
        }
      }
      else  // the image is 3D
      {
        if (! lskelubp3d(result.get_output(), prioimage, connex, inhibval))
        {
          error("lskelubp3d failed");
        }
      }  // the image is 3D

      return result;
    } /* skeleton_im_prioim_connex_inhibimage */

    
    char_image
    skeleton_im_prioint_connex_inhibimage(
      const char_image & image,
      int priocode,
      int connex,
      /*CAN_BE_NULL*/ char_image inhibimage
      )
    {
      char_image result;
      result.copy(image);

      int_image prio;
      prio = skeleton_distance(result, priocode);

      if (depth(result.get_output()) == 1) // the image is 2D
      {
        if (! lskelubp2(result.get_output(), prio, connex, can_be_null(inhibimage))) { error("lskelubp2 failed"); }
      }
      else // the image is 3D
      {
        if (! lskelubp3d2(result.get_output(), prio, connex, can_be_null(inhibimage))) { error("lskelubp3d2 failed"); }
      } // the image is 3D

      return result;
    } /* skeleton_im_prioint_connex_inhibimage */

    char_image
    skeleton_im_prioint_connex_inhibval(
      const char_image & image,
      int priocode,
      int connex,
      /*CAN_BE_NULL*/ int inhibval = -1
      )
    {
      char_image result;
      result.copy(image);

      int_image prio;
      prio = skeleton_distance(result, priocode);

      result = skeleton_im_prioim_connex_inhibval(image, prio, connex, inhibval);      
      
      return result;
    } /* skeleton_im_prioint_connex_inhibimage */


    template <class image_type>
    char_image
    skeleton_im_prioim_connex_inhibimage (
      const char_image & image,
      image_type prioimage,
      int connex,
      char_image inhibimage
      )
    {
      char_image result;
      result.copy(image);
      
      if (depth(result.get_output()) == 1) // the image is 2D
      {
        if (! lskelubp2(result, prioimage, connex, can_be_null(inhibimage))) { error("lskelubp2 failed"); }
      }
      else // the image is 3D
      {
        if (! lskelubp3d2(result, prioimage, connex, can_be_null(inhibimage))) { error("lskelubp3d2 failed"); }
      } // the image is 3D

      return result;
    } /* skeleton_im_prioim_connex_inhibimage */


  } /* namespace python */
} /* namespace pink */

void skeleton2_export()
{
  def( "skeleton",
       &pink::python::skeleton_im_prioint_connex_inhibimage,
       ( arg("image"), arg("prio"), arg("connexity"), arg("inhibit") ),
       doc__skeleton__c__
    );

  def( "skeleton",
       &pink::python::skeleton_im_prioint_connex_inhibval,
       ( arg("image"), arg("prio"), arg("connexity"), arg("inhibit")=-1 ),
       doc__skeleton__c__
    );
  
  UI_DEFINE_FUNCTION(
    "skeleton",
    pink::python::skeleton_im_prioim_connex_inhibimage,
    ( arg("image"), arg("prio"), arg("connexity"), arg("inhibit") ),
    doc__skeleton__c__
    );

  UI_DEFINE_FUNCTION(
    "skeleton",
    pink::python::skeleton_im_prioim_connex_inhibval,
    ( arg("image"), arg("prio"), arg("connexity"), arg("inhibit")=-1 ),
    doc__skeleton__c__
    );

  
} /* skeleton_export */









































// LuM end of file
