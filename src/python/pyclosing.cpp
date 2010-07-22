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
#define error(msg) {stringstream fullmessage; fullmessage << "in pyclosing.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_closing( const char_image & src, 
			    const char_image & elem
  ){

  if (not (elem.get_center()[0]>=0) and ((elem.get_center()[1]>=0)))
  {
    error("The center of 'elem' must be specified.");
  };


  if (src.get_size().size()!=elem.get_size().size()){
    error("error: the dimensions of 'src' and 'elem' must be equal");
  };

  PTR<char_image> result( new char_image(src) );
  PTR<char_image> elem_const_away( new char_image(elem) );


  if (result->get_size()==2)
  {
    int rs = result->get_size()[0];
    int cs = result->get_size()[1];
    int x = elem_const_away->get_center()[0];
    int y = elem_const_away->get_center()[1];
      
    
    if (! ldilateros_ldilat(&(* result ), &(* elem_const_away ), rs - 1 - x, cs - 1 - y)) 
    // &(* result ) == result->operator&()
    // &(* elem_const_away ) == elem_const_away->operator&()
    {
      error("function ldilat failed");
    }
    
    if (! lsym(&(* elem_const_away ), 'c'))
      // &(* elem_const_away ) == elem_const_away->operator&()
    {
      error("function lsym failed");
    }
    
    if (! ldilateros_leros(&(* result ), &(* elem_const_away ), x, y))
      // &(* result ) == result->operator&()
      // &(* elem_const_away ) == elem_const_away->operator&()
    {
      error("function leros failed");
    }

  } 
  else /* NOT result->get_size()==2 */ 
  {
    if (result->get_size()==3)
    {

      // the first two center coordinates of elem have already been tested
      if (not (elem_const_away->get_center()[2]>=0))
      {
	error("The center of 'elem' must be specified.");
      };
      
      int rs = result->get_size()[0];
      int cs = result->get_size()[1];
      int ds = result->get_size()[2];
      int x = elem_const_away->get_center()[0];
      int y = elem_const_away->get_center()[1];
      int z = elem_const_away->get_center()[2];
      
      if (! ldilat3d(&(* result ), &(* elem_const_away ), x, y, z))
	// &(* result ) == result->operator&()
	// &(* elem_const_away ) == elem_const_away->operator&()
      {
	error("function leros3d failed");
      }
      
      if (! lsym(&(* elem_const_away ), 'c'))
	// &(* elem_const_away ) == elem_const_away->operator&()
      {
	error("function lsym failed");
      }
      
      if (! leros3d(&(* result ), &(* elem_const_away ), rs - 1 - x, cs - 1 - y, ds - 1 - z))
	// &(* result ) == result->operator&()
	// &(* elem_const_away ) == elem_const_away->operator&()
      {
	error("function leros3d failed");
      }

    } 
    else /* NOT result->get_size()==3 */
    {
      error("pyclosing: the image must be 2D or 3D");
    }; /* NOT result->get_size()==3 */
    
  } /* NOT result->get_size()==2 */ 
  
  
  return result;    
};


PTR<char_image> py_closeball( const char_image & src, 
			      int r, int mode=2
  )
{
  if ((mode != 0) && (mode != 2) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    error("dist = [0|2|4|8|6|18|26]");
  } /* if */

  PTR<char_image> result( new char_image(src) );
  

  if (result->get_size().size()==2)
  {
    if (! ldilatdisc(result->get_output(), r, mode))
    {
      error("function ldilatdisc failed");
    }
    if (! lerosdisc(result->get_output(), r, mode))
    {
      error("function lerosdisc failed");
    }
  }
  else /* NOT result->get_size().size()==2 */
  {
    if (! ldilatball(result->get_output(), r, mode))
    {
      error("function ldilatball failed");
    }
    if (! lerosball(result->get_output(), r, mode))
    {
      error("function lerosball failed");
    }
  } /* NOT result->get_size().size()==2 */

  return result;
} /* py_closeball */



UI_EXPORT_ONE_FUNCTION( cpp_closeball, 
			py_closeball, 
			args("image", "the ray of the ball", "distance function (defaults to 2)"),
			"WRITE ME!!!\n"
  )






void py_closing_export(){

  def( "closing", &py_closing,
       args("src", "elem"),
       " \n"
       "morphological closing by a plane structuring element \n"
       " \n"
       "Description: \n"
       "Morphological closing by a plane structuring element. \n"
       "The (plane) structuring element is given by the non-null values in 'elem',  \n"
       "its origin is the 'center' of the 'elem'  \n"
       " \n"
       "Types supported: byte 2d, byte 3d \n"
       " \n"
       "Category: morpho \n"
       " \n"
       "\author Michel Couprie 2002 \n"
       " \n"
       // end of the documenation
    );


};


















































// LuM end of file
