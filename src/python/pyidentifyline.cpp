/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper
// this file opens a file with raw image data as an image of given type

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pyidentifyline.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


boost::python::list
py_identifyline( const boost::python::list & X,
			       const boost::python::list & Y 
  )
{
  boost::python::list result;
  

  int length=boost::python::len(X);
  
  #if UJIMAGE_DEBUG >= 1

  int check=boost::python::len(Y);

  if (length != check)
  {    
    error("identifyline: the sizes of the vectors must be equal");
  } /* length != check */
  
  #endif /* UJIMAGE_DEBUG */


  ARRAY<double> cX( new double[length] );
  ARRAY<double> cY( new double[length] );

  FOR(q, length)
    try
    {
      cX[q]=boost::python::extract<double>(X[q]);
      cY[q]=boost::python::extract<double>(Y[q]);
    } 
    catch (...) 
    {
      error("identifyline: the elements of the list must be convertible to doubles.");
    }

  double a,b;
  
  if ( !lidentifyline( cX.get(), cY.get(), length, &a, &b) )
  {
    error("lidentifyline failed");
  } /* if */
  
  
  result.append(a);
  result.append(b);

  return result;
} /* py_border */



UI_EXPORT_ONE_FUNCTION( cpp_identifyline, 
			py_identifyline, 
			args("list of X coordinates", "list of Y coordinates"),
			"\brief identification of a best matching line from a set of 2D points\n"
			"\n"
			"<B>Usage:</B> identifyline in.list out.list\n"
			"\n"
			"<B>Description:</B>\n"
			"Identifies the parameters (a,b) of the equation of the 2D line:\n"
			"ax+b=y that minimizes the least square error between this line \n"
			"and the given points. Method: basic linear regression.\n"
			"\n"
			"<B>Types supported:</B> list 1D, list 2D\n"
			"\n"
			"<B>Category:</B> geo\n"
			"ingroup  geo\n"
			"\n"
			"\author Michel Couprie\n"
  )





// LuM end of file
