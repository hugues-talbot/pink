// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pydilat.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_dilat( const char_image & src, 
			  const char_image & elem
  )
{
  
  if ((src.get_size().size()!=2) and (elem.get_size().size()!=2))
  {
    error("only 2D images are supported, both 'src' and 'elem'");
  };
  
  if (not (elem.get_center()[0]>=0) and (elem.get_center()[1]>=0))
  {
    error("The center of 'elem' must be specified.");
  };

  PTR<char_image> result( new char_image(src) );
  PTR<char_image> elem_const_away( new char_image(elem) );

  if (! ldilateros_ldilat( &(* result ),  // &(* src ) == src->operator&()
			   &(* elem_const_away ), // &(* elem_const_away ) == elem_const_away->operator&()
			   elem_const_away->get_center()[0],
			   elem_const_away->get_center()[0]
	)
    )
  {
    error("function ldilat failed");
  } /* NOT (! ldilateros_ldilat( src, elem_const_away, x, y)) */

  return result;    
}; /* py_dilat */



void py_dilat_export(){

  def( "dilat", &py_dilat,
       args("src", "elem"),
" \n"
"<B>dilat</B> - morphological dilation by a plane structuring element \n"
" \n"
"SYNOPSIS \n"
" \n"
"DESCRIPTION \n"
" \n"
"The (plane) structuring element is given by the non-null values in 'elem',  \n"
"its origin is given by the 'center' of 'elem'  \n"
" \n"
"Types supported: byte 2D. \n"
" \n"
"category  \n"
"morph \n"
"author Michel Couprie \n"
" \n"
// end of the documenation
    );


}; /* py_dilat_export */
















































// LuM end of file
