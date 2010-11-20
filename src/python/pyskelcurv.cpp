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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyptcurve.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

// you should not define N as a macro
#undef N


namespace pink {
  namespace python {

    // ERROR this function changes the parameters
    // If somebody uses it again it'll have to be corrected
    int_image priority_image(
      char_image & image,
      int priovalue
      )
    {
      
      int32_t i, N;
      uint8_t *F;
      xvimage * prio;        
      prio = allocimage(NULL, rowsize(image.get_output()), colsize(image.get_output()), depth(image.get_output()), VFF_TYP_4_BYTE);
      if (prio == NULL)
      {   
        error("allocimage failed");
      }
      N = rowsize(image.get_output()) * colsize(image.get_output()) * depth(image.get_output());
      F = UCHARDATA(image.get_output());
      for (i = 0; i < N; i++) // inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
  
      if (priovalue == 0)
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
        if (priovalue == 1)
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
          if (priovalue == 2)
          {
            if (! lchamfrein(image.get_output(), prio))
            {
              error("lchamfrein failed");
            }
          }
          else
            if (priovalue == 3)
            {
              if (! lsedt_meijster(image.get_output(), prio))
              {
                error("lsedt_meijster failed");
              }
            }
            else
            {
              if (! ldist(image.get_output(), priovalue, prio))
              {
                error("ldist failed");
              }
            }
      for (i = 0; i < N; i++) // re-inverse l'image
        if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
        
      
      int_image result(*prio);
      freeimage(prio);
      
      return result;      
    } /* priority_image */
    
    template <class priority_image_type>    
    char_image general_skelcurv(
      const char_image & image,
      priority_image_type & prio,
      char_image & inhibit,
      int connex
      )
    {
      char_image result;
      result.copy(image);

      xvimage * xvinhibit;
      if (inhibit.isnull())
      {
        xvinhibit = NULL;        
      }
      else /* NOT inhibit.isnull() */
      {
        xvinhibit = inhibit.get_output();        
      } /* NOT inhibit.isnull() */
      
      
      if (image.get_size().size()==2)
      {
        if (! lskelcurv(result, prio, xvinhibit, connex))
        {
          error("lskelcurv failed");
        }        
      }
      else /* NOT size==2 */
      {
        if (image.get_size().size()==3)
        {
          if (! lskelcurv3d(result, prio, xvinhibit, connex))
          {
            error("lskelcurv3d failed");
          }
        }
        else /* NOT size==3 */
        {
          error("only 2D and 3D images are supported");          
        } /* NOT size==3 */
        
      } /* NOT size==2 */

      return result;      
    } /* general_skelcurv */
    
    
    template <class priority_image_type>
    char_image skelcurv(
      const char_image & image, 
      priority_image_type & prio,
      int connex,
      char_image inhibit=char_image()
      )
    {
      char_image result;
      result.copy(image);      

//      result=general_skelcurv(image, prio, inhibit, connex);

      return result;      
    } /* template skelcurv */

    char_image skelcurv2(
      const char_image & image, 
      int priovalue,
      int connex,
      char_image inhibit=char_image()
      )
    {
      char_image result;
      result.copy(image);      

      int_image prio;
//      prio = priority_image(result, priovalue);
      
//      result=general_skelcurv(image, prio, inhibit, connex);

      return result;      
    } /* NO TEMPLATE skelcurv */



  } /* namespace python */
} /* namespace pink */

UI_EXPORT_FUNCTION(
  skelcurv,
  pink::python::skelcurv,
  ( arg("image"),  arg("priority"), arg("connex"), arg("inhibit")="NULL" ),
  "WRITE ME!!!"
  );


void skelcurv2_export()
{
  def("skelcurv",
      &pink::python::skelcurv2,
      (arg("image"), arg("priority"),arg("connex"), arg("inhibit")="NULL"),
      "WRITE ME!!!"
    );  
} /* skelcurv2_export */

















































// LuM end of file
