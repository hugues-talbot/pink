// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pyopening.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_opening( const char_image & src, 
			    const char_image & elem
  ){

  if (not ((elem.get_center())[0]>=0) and (((elem.get_center())[1]>=0)))
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

    if (! ldilateros_leros(&(* result ), &(* elem_const_away ), x, y))
      // &(* result ) == result->operator&()
      // &(* elem_const_away ) == elem_const_away->operator&()
    {
      error("function leros failed");
    }      
    
    if (! lsym(&(* elem_const_away ), 'c'))
      // &(* elem_const_away ) == elem_const_away->operator&()
    {
      error("function lsym failed");
    }
    
    if (! ldilateros_ldilat(&(* result ), &(* elem_const_away ), rs - 1 - x, cs - 1 - y)) 
    // &(* result ) == result->operator&()
    // &(* elem_const_away ) == elem_const_away->operator&()
    {
      error("function ldilat failed");
    }

  } 
  else /* NOT result->get_size()==2 */ 
  {
    if (result->get_size()==3)
    {

      if (not ((elem.get_center())[3]>=0))
      {
	error("The center of 'elem' must be specified.");
      };

      int rs = result->get_size()[0];
      int cs = result->get_size()[1];
      int ds = result->get_size()[2];
      int x = elem_const_away->get_center()[0];
      int y = elem_const_away->get_center()[1];
      int z = elem_const_away->get_center()[2];

      if (! leros3d(&(* result ), &(* elem_const_away ), rs - 1 - x, cs - 1 - y, ds - 1 - z))
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
      
      if (! ldilat3d(&(* result ), &(* elem_const_away ), x, y, z))
	// &(* result ) == result->operator&()
	// &(* elem_const_away ) == elem_const_away->operator&()
      {
	error("function leros3d failed");
      }

    } 
    else /* NOT result->get_size()==3 */
    {
      error("pyopening: the image must be 2D or 3D");
    }; /* NOT result->get_size()==3 */
    
  } /* NOT result->get_size()==2 */ 
  
  
  return result;    
};



void py_opening_export(){

  def( "opening", &py_opening,
       args("src", "elem"),
       " \n"
       "morphological opening by a plane structuring element \n"
       " \n"
       "Description: \n"
       "Morphological opening by a plane structuring element. \n"
       "The (plane) structuring element is given by the non-null values in 'elem',  \n"
       "its origin is the 'center' of the 'elem' \n"
       " \n"
       "Types supported: byte 2d, byte 3d \n"
       " \n"
       "Category: morpho \n"
       " \n"
       "author Michel Couprie 2002 \n"
       " \n"
       // end of the documenation
    );


};


















































// LuM end of file

















































// LuM end of file
