// UjoImro, 2009
// This software is meant to be free
// this file contains the prototype for wrapping the ujoi object using different pixel types
// I think it's doable with templates, but the way is not quite clear, 
// so in the mean time I'll just use c++'s define directive
// and here is how it's done


/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


Python class pink.image / c++ pink::ujoi


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/


template <class object_type, class pixel_type>
void ujoi_object_export( const char* object_name, const char* object_description ){



  class_< object_type , PTR< object_type > > (
    object_name,
    object_description,
    init<vint>(args("self","size"), "Default constructor. Creates an empty image of size 'dim'.")
    )
    
    
    
    .def( init< xvimage > (
	    args("self", "src"), 
	    "This constructor creates an 'image' object from a pink 'xvimage' structure. "
	    "It is used mainly by pink's readimage function and as the image object of "
	    "pink's C functions. "
	    )
      )



    .def( init< object_type > (
	    args("self", "src"), 
	    "This is the deep copy constructor of the 'ujoi' objects. It'll copy all the data "
	    "and all the parameters including the 'xvimage' properties "
	    )
      )
    

    
    .def( init< vint, ARRAY< pixel_type > > (
	    args("self", "dim", "data"),
	    "This constructor creates an 'image' object from a data 'ARRAY' and the "
	    "corresponding 'dim' dimension. It is used by the 'uiSqhool' objects "
	    "which extracts the images from 'ujif' files in this format."
	    ) 
      )
    
    
    
    .def( init < boost::python::list >(
	    args("self", "dim"),
	    "This constructor creates an empty image of dimension 'dim'. "
	    "It uses a python list, so it can be called directly from python. "
	    )
      )
    
    

    .def( "imtype", &object_type::imtype, 
	  args("self"),
	  "Returns a string with the type held in the object. Current types " 
	  "can be: uint8_t, int16_t, int32, float, double"
      )



    .def( "writeimage", &object_type::_writeimage, 
	  args("self", "filename"),
	  "writes the image from the object into a 'pgm' file 'filename'"
      )



    .def( "writeamira", &object_type::_write_amira,
	  args("self", "filename"),
	  "writes the image from the object into amira binary mesh file 'filename'"
      )

    .def( "__repr__", &object_type::repr,
	  args("self"),
	  "writes a short string of information about the image"
      )

    .def( "fill", &object_type::fill,
	  args("self"),
	  "fill's the image with the given value"
      )



/*
***********************************************************************************************
  the 'size' property (read only)
***********************************************************************************************
*/



    .add_property( "size", 
		   make_function( 
		     &object_type::get_size, 
		     return_value_policy<copy_const_reference>()
		     ),
		   "Returns a 'vint' vector with the dimensions of the image. "
		   "The 'vint' is read-only, but it would be no point in changing it "
		   "as it wouldn't invoke the reallocation of the data"
      )
   




/*
***********************************************************************************************
  the 'center' property
***********************************************************************************************
*/


    .add_property( "center", 
		   make_function( 
		     &object_type::get_center, return_value_policy<copy_const_reference>()
		     ),
		   &object_type::set_center_list,
		   "Reads and sets the center point. If the coordinates are -1, then "
		   "the center is undefined. When changing the center, the new center point "
		   "must have the same dimensions as the image."
		   
      )

    .def("set_center", &object_type::set_center_vint, 
	 args("self", "new_center"),
	 "sets up 'new_center' as the center point of the image"
	 "the new center point "
	 "must have the same dimensions as the image."
      )

    
/*
***********************************************************************************************
operator[int]
***********************************************************************************************
*/


    
    .def( "__getitem__", &object_type::get_operator_int, return_value_policy<copy_const_reference>(),
	  args("self", "pos"),
	  "This function accesses the pixels of the image. It is used for the 'a=image[pos]' like access model."
	  
      )
    
    
    
    .def( "__setitem__", &object_type::set_operator_int,
	  args("self", "pos", "value"),
	  "This function accesses the pixels of the image. It is used for the 'image[pos]=a' like access model."
      )

/*
***********************************************************************************************
  operator[pink::vint]
***********************************************************************************************
 */

    
    .def( "__getitem__", &object_type::get_operator_vint, return_value_policy<copy_const_reference>(),
	  args("self", "pos"),
	  "This function accesses the pixels of the image. It is used for the 'a=image[pos]' like access model."
	  
      )



    .def( "__setitem__", &object_type::set_operator_vint,
	  args("self", "pos", "value"),
	  "This function accesses the pixels of the image. It is used for the 'image[pos]=a' like access model."
      )
    
/*
***********************************************************************************************
  operator[boost::python::list]
***********************************************************************************************
 */


    .def( "__getitem__", &object_type::get_operator_list, return_value_policy<copy_const_reference>(),
	  args("self", "pos"),
	  "This function accesses the pixels of the image. It is used for the 'a=image[pos]' like access model."

      )



    .def( "__setitem__", &object_type::set_operator_list,
	  args("self", "pos", "value"),
	  "This function accesses the pixels of the image. It is used for the 'image[pos]=a' like access model."
      )
    
    ;


}; /* ujoi_object_export */











































































//LuM end of file
