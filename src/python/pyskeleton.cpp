// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyskeleton.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


// based on skeleton.c

// ERROR N is not supposed to be used as a define macro
#undef N

namespace pink {
  namespace python {


//prototype
    char_image skeleton_ultimate(
      char_image * uj_image, 
      int_image * uj_prio,
      int priocode,
      int connex, 
      char_image * uj_inhibit,
      int inhibitcode,
      bool inhibit_image, // true if there is an inhibit image
      bool inhibit_value, // true if there is an inhibit value
      bool prio_image, // if true, than the priority is an image, 
      // else it's a number, which is important
      bool prio_value  
      );



/* nc == non_const */

    char_image skeleton_im_int_int_void
    (
      const char_image & uj_image, 
      int priocode,
      int connex
      )
    {

      char_image res;
      res.copy(uj_image);
      char_image result;
      result =  skeleton_ultimate( &res,    // char_image & uj_image, 
                                      NULL,           // char_image & uj_prio,
                                      priocode,  // int priocode,
                                      connex,    // int connexity  
                                      NULL,      // char_image & uj_inhibit,
                                      -1,        // int inhibitcode,
                                      false,     // bool inhibit_image, // true if there is an inhibit image
                                      false,     // bool inhibit_value, // true if there is an inhibit value
                                      false,     // bool prio_image, // if true, than the priority is an image, 
                                      //     else it's a number, which is important
                                      true       // bool prio_value
        );

      return result;
    } /* skeleton_im_int_int_void */


    char_image skeleton_im_im_int_void(
      const char_image & uj_image, 
      const int_image & uj_prio,
      int connex
      ) 
    {

      char_image res;
      res.copy(uj_image);
      int_image nc_prio;
      nc_prio.copy(uj_prio);
  
      char_image result;
      result =
        skeleton_ultimate( &res,   // char_image & uj_image, 
                              &nc_prio,  //     char_image & uj_prio,
                              -1,       // int priocode,
                              connex,   // int connexity  
                              NULL,     //     char_image & uj_inhibit,
                              -1,       // int inhibitcode,
                              false,    //     bool inhibit_image, // true if there is an inhibit image
                              false,    // bool inhibit_value, // true if there is an inhibit value
                              true,     //     bool prio_image, // if true, than the priority is an image, 
                              //     else it's a number, which is important
                              false     // bool prio_value
          );
  
      return result;
    } /* skeleton_im_im_int_void */





    char_image skeleton_im_int_int_im( const char_image & uj_image, 
                                          int prio,
                                          int connex,			     
                                          const char_image & inhibit
      ) 
    {

      char_image res;
      res.copy(uj_image);

      char_image nc_inhibit;
      nc_inhibit.copy(inhibit);

      char_image result =  
        skeleton_ultimate( &res,    // char_image & uj_image, 
                              NULL,      //     char_image & uj_prio,
                              prio,      // int priocode,
                              connex,    // int connexity  
                              &nc_inhibit,//     char_image & uj_inhibit,
                              -1,        // int inhibitcode,
                              true,      //     bool inhibit_image, // true if there is an inhibit image
                              false,     // bool inhibit_value, // true if there is an inhibit value
                              false,     //     bool prio_image, // if true, than the priority is an image, 
                              //     else it's a number, which is important
                              true       // bool prio_value
          );
  
      return result;

    };



    char_image skeleton_im_im_int_im( const char_image & uj_image, 
                                         const int_image & uj_prio,
                                         int connex,			     
                                         const char_image & inhibit
      ) 
    {
  
      char_image res;
      res.copy(uj_image);
      int_image nc_prio;
      nc_prio.copy(uj_prio);
      char_image nc_inhibit;
      nc_inhibit.copy(inhibit);
  
      char_image result;
      result =
        skeleton_ultimate( &res,    // char_image & uj_image, 
                              &nc_prio,   //     char_image & uj_prio,
                              -1,        // int priocode,
                              connex,    // int connexity  
                              &nc_inhibit,//     char_image & uj_inhibit,
                              -1,        // int inhibitcode,
                              true,      //     bool inhibit_image, // true if there is an inhibit image
                              false,     // bool inhibit_value, // true if there is an inhibit value
                              true,      //     bool prio_image, // if true, than the priority is an image, 
                              //     else it's a number, which is important
                              false      // bool prio_value
          );
  
      return result;

    };







    char_image skeleton_im_int_int_int( const char_image & uj_image, 
					   int prio,
					   int connex,			     
					   int inhibit_value
      ) 
    {

      char_image res;
      res = uj_image;
  
      char_image result;
      result =
        skeleton_ultimate( &res,    // char_image & uj_image, 
                              NULL,      //     char_image & uj_prio,
                              prio,      // int priocode,
                              connex,    // int connexity  
                              NULL,      //     char_image & uj_inhibit,
                              inhibit_value, // int inhibitcode,
                              false,      //     bool inhibit_image, // true if there is an inhibit image
                              true,     // bool inhibit_value, // true if there is an inhibit value
                              false,      //     bool prio_image, // if true, than the priority is an image, 
                              //     else it's a number, which is important
                              true      // bool prio_value
          );
  
      return result;

    };



    char_image skeleton_im_im_int_int(
      const char_image & uj_image, 
      const int_image & uj_prio,
      int connex,			     
      int inhibit_value
      ) 
    {

      char_image res;
      res.copy(uj_image);
      int_image nc_prio;
      nc_prio.copy(uj_prio);
  
      char_image result;  
      result = skeleton_ultimate( &res,    // char_image & uj_image, 
                                     &nc_prio,   //     char_image & uj_prio,
                                     -1,        // int priocode,
                                     connex,    // int connexity  
                                     NULL,      //     char_image & uj_inhibit,
                                     inhibit_value, // int inhibitcode,
                                     false,      //     bool inhibit_image, // true if there is an inhibit image
                                     true,     // bool inhibit_value, // true if there is an inhibit value
                                     true,      //     bool prio_image, // if true, than the priority is an image, 
                                     //     else it's a number, which is important
                                     false      // bool prio_value
        );
  
      return result;

    };



  
    char_image skeleton_ultimate( char_image * uj_image, 
                                     int_image * uj_prio,
                                     int priocode,
                                     int connex, 
                                     char_image * uj_inhibit,
                                     int inhibitcode,
                                     bool inhibit_image, // true if there is an inhibit image
                                     bool inhibit_value, // true if there is an inhibit value
                                     bool prio_image, // if true, than the priority is an image, 
                                     // else it's a number, which is important
                                     bool prio_value
                                 
      )
    {

      char_image result;

      struct xvimage * image;
      struct xvimage * prio;
      struct xvimage * inhibimage = NULL;
      int32_t inhibvalue;

      image = uj_image->get_output(); // xvimage* uj_image -> operator&()
      if ( inhibit_image )
      {
        inhibimage = uj_inhibit->get_output();
      } /* inhibit_image */

      if ( prio_image )
      {
        prio = uj_prio->get_output();
      }
      else /* not ( prio_image ) */
      { 

        // the distance map has to be calculated
        int32_t i, N;
        uint8_t *F;
        prio = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
        if (prio == NULL) error("allocimage failed");
    
        N = rowsize(image) * colsize(image) * depth(image);
    
        F = UCHARDATA(image);

        // we inverse the image
        for (i = 0; i < N; i++) 
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
    

        // calculating the appropriate distance map
        switch ( priocode ){
      
        case 0:
          if ( depth( image ) == 1 )
            if (! ldisteuc(image, prio ) ) error("ldisteuc failed")
              else
                if (! ldisteuc3d( image, prio ) ) error("ldisteuc3d failed");
          break;
    
        case 1:      
          if ( depth( image ) == 1 )
            if (! ldistquad( image, prio ) ) error("ldistquad failed")
              else
                if (! ldistquad3d(image, prio ) ) error("ldistquad3d failed")
                                                    break;

        case 2:
          if (! lchamfrein( image, prio ) ) error("lchamfrein failed");
          break;

        case 3:
          if (! lsedt_meijster( image, prio ) ) error("lsedt_meijster failed");
          break;

        default:
          if (! ldist( image, priocode, prio ) ) error("ldist failed");
          break;

        } /* switch (priocode) */


        // re-inversing the image
        FOR(i, N) 
          if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

      } /* not if (prio_image) */
  

      if ( not inhibit_value )
      {
        inhibvalue = -1;
      } /* not not inhibit_value */
      else
      {
        inhibvalue=inhibitcode;
      }/* not not inhibit_value */
  

      if (depth(image) == 1)
      {
        if (inhibit_image)
        {
          if (! lskelubp2( image, prio, connex, inhibimage ) )
          {
            error("lskelubp2 failed");
          }

          result = *image;
        }
        else  /* not (inhibit_image) */
        {
          if (! lskelubp( image, prio, connex, inhibvalue ) )
            error("lskelubp failed");
      
          result = *image;
        }
      } 
      else /* not  (depth(image) == 1) */ 
      {
        if (inhibit_image) 
        {
          if (! lskelubp3d2( image, prio, connex, inhibimage ) ) 
          {
            error("lskelubp3d2 failed");
          }

          result = *image;
        }
        else /* not (inhibit_image) */
        {
          if (! lskelubp3d( image, prio, connex, inhibvalue ) )
          {
            error("lskelubp3d failed");
          }
      
          result = *image;
        }
      } /* not (depth(image) == 1) */


      if ( not prio_image ) freeimage(prio);
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
  def( "skeleton_im_int_int_void", &pink::python::skeleton_im_int_int_void,
       args("image", "priority value", "connexity"),
       "the help is in 'help(pink.skeleton)'"
    );

  def( "skeleton_im_im_int_void", &pink::python::skeleton_im_im_int_void,
       args("image", "priority image", "connexity"),
       "the help is in 'help(pink.skeleton)'"
    );

  def( "skeleton_im_int_int_im", &pink::python::skeleton_im_int_int_im,
       args("image", "priority value", "connexity", "inhibited pixels"),
       "the help is in 'help(pink.skeleton)'"
    );

  def( "skeleton_im_im_int_im", &pink::python::skeleton_im_im_int_im,
       args("image", "priority image", "connexity", "inhibited pixels"),
       "the help is in 'help(pink.skeleton)'"
    );

  def( "skeleton_im_int_int_int", &pink::python::skeleton_im_int_int_int,
       args("image", "priority value", "connexity", "inhibited value"),
       "the help is in 'help(pink.skeleton)'"
    );

  def( "skeleton_im_im_int_int", &pink::python::skeleton_im_im_int_int,
       args("image", "priority_image", "connexity", "inhibited value"),
       "the help is in 'help(pink.skeleton)'"
    );  
} /* py_skeleton_export */









































// LuM end of file

