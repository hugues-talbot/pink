/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
*/
// The Pink Image Reading and Writing Functions

#include "pyexport.hpp"

#include <Python.h>
#include <functional>

#include "mcimage.h"

/**
   This is a test function, which creates a 2D Numpy array with a
   given size.
 */
boost::python::object 
create_image( const index_t & rows, const index_t & cols ) {
  return pink::char_image(rows, cols).steel();
}

namespace {

  using boost::python::tuple;
  using boost::python::object;
  using boost::python::make_tuple;
  
  tuple
  w_readrgbimage( const std::string & filename ) {
    xvimage * p_red;
    xvimage * p_green;
    xvimage * p_blue;

    int32_t exception = readrgbimage( filename.c_str(), &p_red, &p_green, &p_blue );

    pink::cxvimage red  (p_red);
    pink::cxvimage green(p_green);
    pink::cxvimage blue (p_blue);

    tuple result = make_tuple( red.steel(), green.steel(), blue.steel() );

    return result;    
  } // cpp_readimage

  tuple
  w_readrgb( const std::string & filename ) {
    xvimage * p_red;
    xvimage * p_green;
    xvimage * p_blue;

    int32_t exception = readrgb( filename.c_str(), &p_red, &p_green, &p_blue );

    pink::cxvimage red  (p_red);
    pink::cxvimage green(p_green);
    pink::cxvimage blue (p_blue);

    tuple result = make_tuple( red.steel(), green.steel(), blue.steel() );

    return result;    
  } // cpp_readimage

  tuple
  w_readbmp( const std::string & filename ) {
    xvimage * p_red;
    xvimage * p_green;
    xvimage * p_blue;

    int32_t exception = readbmp( filename.c_str(), &p_red, &p_green, &p_blue );

    pink::cxvimage red  (p_red);
    pink::cxvimage green(p_green);
    pink::cxvimage blue (p_blue);

    tuple result = make_tuple( red.steel(), green.steel(), blue.steel() );

    return result;    
  } // cpp_readimage


} // unnamed namespace


void
pyio () {

  using pink::allocdef;
  using pink::exportdef;
  using boost::python::arg;
  using boost::python::def;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using std::placeholders::_1;
  using boost::python::borrowed;

  // we create the IO submodule
  // motivation:
  // http://bfroehle.com/2011/07/boost-python-submodules/
  // http://isolation-nation.blogspot.hu/2008/09/packages-in-python-extension-modules.html
  
  // make "from mypackage.IO import <whatever>" work
  object io_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.IO"))));
  
  // make "from mypackage import IO" work
  scope().attr("IO") = io_module;
  
  // set the current scope to the new sub-module
  scope io_scope = io_module;
  
  // export stuff in the IO namespace

  def( "create_image", create_image, (arg("rows"), arg("cols")), "This is a test function, which creates a 2D Numpy array with a given size." );

  exportdef( "writeimage", writeimage, (arg("image"), arg("filename")), "WRITE ME!!!" );

  exportdef( "writese", writese, (arg("image"), arg("filename"), arg("x"), arg("y"), arg("z") ), "Writes a structuring element. FINISH ME!!!" );

  exportdef( "writelongimage", writelongimage, (arg("image"), arg("filename")), "WRITE ME!!!" );
  
  exportdef( "writerawimage", writerawimage, (arg("image"), arg("filename")), "WRITE ME!!!" );
  
  exportdef( "writeascimage", writeascimage, (arg("image"), arg("filename")), "WRITE ME!!!" );
  
  exportdef( "printimage", printimage, (arg("image") ), "WRITE ME!!!" );
  
  exportdef( "writergbimage", writergbimage, (arg("red image"), arg("green image"), arg("blue image"), arg("filename")), "WRITE ME!!!" );
  
  exportdef( "writergbascimage", writergbascimage, (arg("red image"), arg("green image"), arg("blue image"), arg("filename")), "WRITE ME!!!" );

  def( "readrgbimage", w_readrgbimage, (arg("filename")), "WRITE ME!!!");

  def( "readrgb", w_readrgb, (arg("filename")), "WRITE ME!!!");
    
  def( "readbmp", w_readbmp, (arg("filename")), "WRITE ME!!!");

  allocdef( "readimage", readimage, (arg("filename")) );
    
  import_array();  // numpy initialization
  
  return;
} // pyio


// LuM end of file
