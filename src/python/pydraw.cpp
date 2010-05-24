// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#define error(msg) {stringstream fullmessage; fullmessage << "in pydraw.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;



PTR<char_image> py_drawcurve2D( const vint & size,
				const vint & coordX,
				const vint & coordY
  ) {

  if (size.size()!=2){
    error("only 2D images are supported. Nag the developer.");
  }; /* size.size()!=2 */

  if (coordX.size() != coordY.size()){
    error("error: The number of X and Y coordinates must be equal.");
  }; /* coordX.size() != coordy.size() */
  

  PTR<char_image> result( new char_image(size) );


  FOR( q, coordX.size()-1 ){
    ldrawline( &(*result), // &(*result) == result->operator& and it returns an "xvimage*"
	       coordX[q],
	       coordY[q],
	       coordX[q+1],
	       coordY[q+1]
      ); 
  }; /* FOR coordX.size()-1 */

  return result;
}; /* py_drawcurve2D */




/*
***********************************************************************************************
***********************************************************************************************
***********************************************************************************************

   exported functions

***********************************************************************************************
***********************************************************************************************
***********************************************************************************************
 */


void py_drawcurve_export(){
  def( "cpp_drawcurve2D", &py_drawcurve2D,
       args("image size", "X coordinates", "Y coordinates"),
       "the help is in 'help(pink.drawcurve)'"
    );
};











































// LuM end of file
