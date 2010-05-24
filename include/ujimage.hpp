/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#ifdef __cplusplus
#ifndef __UJIMAGE_HPP
#define __UJIMAGE_HPP

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ujimage.hpp: " << msg; call_error(fullmessage.str());}

//#define UJIMAGE_DEBUG

namespace pink{

  class deep_xvimage;

//  string image_type( int im_type ); // returns the corresponding image types

  template <class im_type>
  class image_type_specific{
  public:
//#error: this image type is not defined
    string imtype(){         			      
      return "wrong type";			              
    };					              
    int int_im_type(){                                
      return -1;				      
    };                                                
  };
  

#define CREATE_IMAGE_TYPE( _class, name, int_type )			\
  template <>								\
  class image_type_specific< _class >{					\
  public:								\
  string imtype(){							\
    return name;							\
  };									\
  int int_im_type(){							\
    return int_type;							\
  };									\
  };									
  									
  									
  // helper function for reading and writing from and to xvimage
  PTR<vint> getDimensions( const int x, const int y, const int z, const int t );
  void setDimensions(const vint & dim, int & x, int & y, int & z, int & t);
  PTR<deep_xvimage> py_readimage( string filename );
  string image_type( int im_type );


  // this class will copy it's content when upcasted (constructed from xvimage)
  // it is used, when the data is needed (in readimage for example)
  // note: image name is fake
  class deep_xvimage: public xvimage {
  public:
    deep_xvimage(); // default constructor
    deep_xvimage( const xvimage & src ); // upcast constructor
    deep_xvimage( const deep_xvimage & src ); // copy constructor. RAISES AND ERROR!
                                              // it's not yet implemented, raises an
                                              // error if called implicitly
    ~deep_xvimage(); // default destructor
    string imtype(); // returns the image type
  };
  

  // this class will only copy the properties and set up a fake image name
  // it is used in ujoi, to generate fast an 'struct xvimage' for PInK's C functions
  class shallow_xvimage: public xvimage {
  public:
    shallow_xvimage(); // default constructor
    shallow_xvimage( const xvimage & src ); // upcast shallow copy constructor. It does not 
                                            // copy the data
    shallow_xvimage( const shallow_xvimage & src ); // copy constructor. RAISES AND ERROR!
                                                    // it's not yet implemented, raises an
                                                    // error if called implicitly
    shallow_xvimage( const vint & dim, int int_im_type );  // construct from dimension. The data 
    // type must be specified. See mcimage.h
    ~shallow_xvimage(); // default destructor
    string imtype(); // returns the image type
  }; /* xvImage: xvimage */

	

  template <class im_type> // im_type can be char, short, int, float and double
  // while int_im_type is the type's encoding into integer. See 'mccodimage.h'
  class ujoi: public image_type_specific<im_type> {
  public:
    typedef im_type pixel_type;
    

  private:

    PTR< shallow_xvimage > old_school;
    vint size;
    vint center;
    ARRAY<im_type> pixels;



    #ifdef UJIMAGE_DEBUG
    string debug; // representing the name of the object if debugged
    #endif /* UJIMAGE_DEBUG */
 
  public:

    ujoi( ); // creates an empty image, used in uiSqhool for determining the image type
    ujoi( string filename, string debug="" );
    ujoi( const xvimage & src, string debug="" ); // default constructor
    ujoi( const ujoi< im_type > & src, string debug="" ); // deep_copy_constructor. For conversion use convert2float
    ujoi( const boost::python::list & dim, string debug="" );
    ujoi( const vint & dim, string debug="" );
    ~ujoi(); // default destructor
    ujoi( const vint & dim, ARRAY<im_type> data, string debug="" ); // used to construct from ujif.

    im_type & operator[]( int pos ); // index acces to the elements
    const im_type & operator[]( int pos ) const; // const index acces to the elements

    im_type & operator[]( const vint & pos ); // vint acces to the elements
    const im_type & operator[]( const vint & pos ) const;  // const vint acces to the elements

    im_type & operator[]( const boost::python::list & pos ); // python list acces to the elements
    const im_type & operator[]( const boost::python::list & pos ) const; // const python list acces to the elements

    // these methods are the operators renamed so they could be wrapped by boost
    const im_type & get_operator_int( int pos ) const;
    void set_operator_int( int pos, const im_type & value );

    const im_type & get_operator_vint( const vint& pos ) const;
    void set_operator_vint( const vint & pos, const im_type & value );

    const im_type & get_operator_list( const boost::python::list & pos ) const;
    void set_operator_list( const boost::python::list & pos, const im_type & value );

    void _writeimage( const string & filename ); // exports the image into a pgm file
    void _write_amira( const string & filename ); // exports the image into an amira mesh (.am) file
    xvimage* operator&(); // this method is not exported to python. It could not even be, as boost doesn't support pointers.
    xvimage* get_output(); // this method is not exported to python. It could not even be, as boost doesn't support pointers.
//    string imtype(); 
    const vint & get_size() const;
    const vint & get_center() const;
    void set_center_vint( const vint & new_center );
    void set_center_list( const boost::python::list & new_center );

    ARRAY<im_type> get_pixels();
    ARRAY<im_type> get_pixels() const;

  }; /* class ujoi */

  // Valid image types
  // integer and float will be tested first.

  CREATE_IMAGE_TYPE( unsigned char, "uint8_t", VFF_TYP_1_BYTE );
  CREATE_IMAGE_TYPE( unsigned short int, "uint16_t", VFF_TYP_2_BYTE );
  CREATE_IMAGE_TYPE( unsigned int, "int32", VFF_TYP_4_BYTE );
  CREATE_IMAGE_TYPE( float, "float", VFF_TYP_FLOAT );
  CREATE_IMAGE_TYPE( double, "double", VFF_TYP_DOUBLE );




  typedef ujoi<unsigned char/*,                 VFF_TYP_1_BYTE*/ >   char_image;
  typedef ujoi<unsigned short int/*,            VFF_TYP_2_BYTE*/ >  short_image;
  typedef ujoi<unsigned/*long*/int/*,           VFF_TYP_4_BYTE*/ >    int_image; // LONG INT is 8 byte's on 64 bit system
  typedef ujoi<float/*,                         VFF_TYP_FLOAT*/  >  float_image;
  typedef ujoi<double/*,                        VFF_TYP_DOUBLE*/ > double_image;





/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


TEMPLATE IMPLEMENTATION


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/






  
/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


c++ class pink::ujoi (this is a template class, so it stays in the header)


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/
  
  template <class im_type >
  ujoi<im_type >::ujoi( /* string debug="" */ ) {
    
    #if UJIMAGE_DEBUG >= 2
    cout << "creating an empty image (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

  } /* ujoi<im_type >::ujoi( const string & filename ) */


  template <class im_type>
  ujoi<im_type>::ujoi( string filename, string debug )
  {
    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    cout << "reading image '" << filename << "'\n";
    #endif /* UJIMAGE_DEBUG */

    PTR<deep_xvimage> tmp;
      

    try
    {
      tmp = py_readimage( filename );
    }
    catch (...)
    {
      error("cannot read file '" + filename + "'");      
    };
    
    
    if (tmp->data_storage_type != this->int_im_type() )
    {
      error("The image type of '" + filename + "' is '" + tmp->imtype()
	    + "', but expected '" + this->imtype() + "'." );
    } /* if */
      
    this->size = vint(
      *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
	tmp->row_size, 
	tmp->col_size, 
	tmp->depth_size, 
	tmp->time_size
	)
      );
	
	

    // setting up the center
    this->center=vint( size.size(), -1 );
  
    this->old_school.reset( new shallow_xvimage( *tmp ) ); // creating a new xvimage for the 'pink::ujoi' object
    // the constructor will not copy the data
  
    this->pixels.reset( new im_type[ size.prod() ] ); // allocating the array for the pixel types
  

    // memcpy should be parallelized with #pragma omp for
    memcpy( this->pixels.get(), tmp->image_data, sizeof( im_type ) * this->size.prod() ); // xvimage's type must be the same as im_type!

  } /* ujoi::ujoi */
  
  template <class im_type >
  ujoi<im_type >::ujoi( const struct xvimage & src, string debug ) {
    
    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    if (image_type(src.data_storage_type)!=this->imtype()){
      error("converting to ujoi from different pixel_type");
    }; /* image_type(src.data_storage_type)!=this->imtype() */
    
    this->size = vint(
      *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
	src.row_size, 
	src.col_size, 
	src.depth_size, 
	src.time_size
	)
      );
	
	

    // setting up the center
    this->center=vint( size.size(), -1 );
  
    this->old_school.reset( new shallow_xvimage( src ) ); // creating a new xvimage for the 'pink::ujoi' object
    // the constructor will not copy the data
  
    this->pixels.reset( new im_type[ size.prod() ] ); // allocating the array for the pixel types
  

    // memcpy should be parallelized with #pragma omp for
    memcpy( this->pixels.get(), src.image_data, sizeof( im_type ) * this->size.prod() ); // xvimage's type must be the same as im_type!
  };

  template <class im_type >
  ujoi< im_type >::ujoi( const ujoi< im_type > & src, string debug ){ // deep_copy_constructor

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    this->size = vint( src.size );
    this->center = vint( src.center );
    this->old_school.reset( new shallow_xvimage( (* src.old_school ) ) ); // creating a new xvimage for the 'pink::ujoi' object
    // the constructor will not copy the data
  
    this->pixels.reset( new im_type[size.prod()] ); // allocating the array for the pixel types
  
  
    // memcpy should be parallelized with #pragma omp for
    memcpy(this->pixels.get(), src.pixels.get(), sizeof(im_type)*size.prod()); // xvimage's type must be the same as im_type!
    
  };

  template <class im_type >
  ujoi<im_type >::ujoi( const vint & dim, string debug ){

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */


    this->size = vint( dim ); // creating a copy of the size
    this->center = vint( size.size(), -1 );
    this->pixels.reset( new im_type[ size.prod() ] ); // allocating memory for the pixels

    if (this->size.size()<=4){
      this->old_school.reset( new shallow_xvimage( size, this->int_im_type() ) );
    };

    // I think reset can be done in parallel with #pragma omp for
    // setting up elements zero
    FOR(q, size.prod())
      this->pixels[q]=0;

  };

  template <class im_type >
  ujoi<im_type >::ujoi( const boost::python::list & dim, string debug ){

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    this->size=vint(dim); // creating a copy of the size
    this->center = vint( size.size(), -1 );

    this->pixels.reset( new im_type[ size.prod() ] ); // allocating memory for the pixels

    if (this->size.size()<=4){
      this->old_school.reset( new shallow_xvimage( size, this->int_im_type() ) );
    };

    // I think reset can be done in parallel with #pragma omp for
    // setting up elements zero
    FOR(q, size.prod())
      this->pixels[q]=0;

  };


  template <class im_type >
  ujoi<im_type >::~ujoi( ){

    #if UJIMAGE_DEBUG >= 2
    cout << "deleting image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

// deletes 'xvImage' automaticly
    /////!!!!!!! cout<< "deleting " << old_school->imtype() << "_char"  << endl; 
  };



  template <class im_type >
  ujoi<im_type >::ujoi( const vint & dim, ARRAY<im_type> data, string debug ){

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image " << debug << endl;
    #endif /* UJIMAGE_DEBUG */

    size = vint( dim ); // creating a copy of the size
    center = vint( size.size(), -1 );

    pixels.reset( new im_type[ size.prod() ] ); // allocating memory for the pixels

    if (this->size.size()<=4){
      old_school.reset( new shallow_xvimage( size, this->int_im_type() ) );
    };

    // I think copying can be done in parallel with #pragma omp for
    memcpy( pixels.get(), data.get(), sizeof( im_type ) * size.prod( ) ); // copying the elements from the array
  };



  template <class im_type >
  void ujoi<im_type >::_writeimage( const string & filename ){ // exports the image into a pgm file
    // writeimage takes 'char *', while 'c_str()' gives 'const char *'. 
    // I don't want to cast unnecesserily, so I'll just copy it.
 
    char * cstr = new char [ filename.size() + 1 ]; // converting the filename for 'writeimage'
    strcpy ( cstr, filename.c_str() );
    
    writeimage( this->operator&(), cstr );    

  } /* _writeimage */



  template<class pixel_type>
  void write_a_pixel( fstream & s, pixel_type & value ) 
  {
    error("write_a_pixel called with a wrong pixel type");
  } /* write_a_pixel default */

  template<> // implemented in 'ujimage.cpp'
  void write_a_pixel<float>( fstream & s, float & value ) ;

  template<> // implemented in 'ujimage.cpp'
  void write_a_pixel<char>( fstream & s, char & value ) ;


  template <class im_type >
  void ujoi<im_type >::_write_amira( const string & filename ){ // exports the image into an amira mesh (.am) file
  
    
    string typetext;
  
    switch (this->int_im_type())  { 
      
    case VFF_TYP_1_BYTE:
      typetext.assign("byte");
      break;
      
    case VFF_TYP_FLOAT:
      typetext.assign("float");
      break;
      
    default:
      cout << "\nfile: " << filename; 
      error("you can export only 'char' and 'float' images at this point");	
    } /* switch */
    
    
    if (this->size.size()!=3) {
      cout << "file: " << filename; 
      error("you can export only 3D images at this point");
    } 
    else /* NOT this->size.size()!=3 */
    {
      fstream s;
      s.open ( filename.c_str(), fstream::out );
      s << "# UjoImro PInK Amira export, 2009-2010\n\n"
	<< "# AmiraMesh BINARY-LITTLE-ENDIAN 2.1\n\n\n"
	<< "define Lattice " << " " << this->size[0] << " " << this->size[1] << " " << this->size[2] << "\n\n"
	<< "Parameters {\n"
	<< "\tContent\"" << " " << this->size[0] << "x" << this->size[1] << "x" << this->size[2] << " PInK pgm export \",\n"
	<< "\tBoundingBox 0 " << this->size[0]-1 << " 0 " << this->size[1]-1 << " 0 " << this->size[2]-1 << ",\n"
	<< "\tCoordType \"uniform\"\n"
	<< "}\n\n"
	<< "Lattice { " << typetext << " Data } @1\n\n"
	<< "# Data section follows\n"
	<< "@1\n";
      
      
      // From avizo 6.0 manual:
      // ... the specified number of bytes is read in binary format. 
      // It is assumed that sizeof(short) is 2, sizeof(int) is 4, 
      // sizeof(float) is 4, sizeof(double) is 8, and sizeof(complex) is 8. 
      // Multidimensional arrays indexed via [k][j][i] are read with i running fastest. 
      
      
      vint curr(3);
      
      FOR(q, size[2]) {
	FOR(w, size[1]) { 
	  FOR(e, size[0]) {
	    
	    curr[0]=e;
	    curr[1]=w;
	    curr[2]=q;
	    
	    
	    write_a_pixel( s, pixels[size.position(curr)] );
	    
	  }; /* FOR(i, size[2]) */
	}; /* FOR(j, size[1]) */
      }; /* FOR(k, size[0]) */
      
      
      s << "\n";
      s.close();
    } /* NOT this->size.size()!=3 */
    cout << "file '" << filename << "' exported in Amira format\n";
  };



  template <class im_type >
  xvimage* ujoi<im_type >::operator&(){
    if (this->size.size()>4){
      error("error: Images with more than four dimensions can not be extracted to 'xvimage'");
    }; /* this->size.size()>4 */

    old_school->image_data=pixels.get(); // setting up the data type. This is an 
    // unsafe action, and is only allowed 
    // for backward compatibility.
    // the rest of the properties is already set.
    return old_school.get(); // you must never 'free()' or 'delete' this pointer!
  };



  template <class im_type >
  xvimage* ujoi<im_type >::get_output(){
    if (this->size.size()>4){
      error("error: Images with more than four dimensions can not be extracted to 'xvimage'");
    }; /* this->size.size()>4 */

    old_school->image_data=pixels.get(); // setting up the data type. This is an 
    // unsafe action, and is only allowed 
    // for backward compatibility.
    // the rest of the properties is already set.
    return old_school.get(); // you must never 'free()' or 'delete' this pointer!
  };



  template <class im_type >
  im_type & ujoi<im_type >::operator[](int pos){ // index acces to the elements

    // CAPABILITY TEST IN DEBUG MODE
    // this is a test which would be slow in an everyday situation,
    // but can save ages while debugging
    #ifdef UJIMAGE_DEBUG
    if ( ( pos < 0 ) or (pos >= this->size.prod()) )
    {
      cerr << "error: number of elements is " << this->size.prod() << " while pos = " << pos << endl;
      error("You are trying to access elements otside of the image\n");
    } /* if */
    #endif /* UJIMAGE_DEBUG */

    
    return pixels[pos];

  };



  template <class im_type >
  const im_type & ujoi<im_type >::operator[](int pos) const { // const index acces to the elements

    // CAPABILITY TEST IN DEBUG MODE
    // this is a test which would be slow in an everyday situation,
    // but can save ages while debugging
    #ifdef UJIMAGE_DEBUG
    if ( ( pos < 0 ) or (pos >= this->size.prod()) )
    {
      cerr << "error: number of elements is " << this->size.prod() << " while pos = " << pos << endl;
      error("You are trying to access elements otside of the image\n");
    } /* if */
    #endif /* UJIMAGE_DEBUG */

    return pixels[pos];

  };


  template <class im_type >
  im_type & ujoi<im_type >::operator[](const vint & pos){ // vint acces to the elements

    // CAPABILITY TEST IN DEBUG MODE
    // this is a test which would be slow in an everyday situation,
    // but can save ages while debugging
    #ifdef UJIMAGE_DEBUG
    if ( not size.inside(pos) )
    {
      cerr << "error: image dimensions are " << this->size.repr() << " while pos = " << pos.repr() << "\n";
      error("You are trying to access elements otside of the image\n");
    } /* if */
    #endif /* UJIMAGE_DEBUG */

    return pixels[size.position(pos)];

  };



  template <class im_type >
  const im_type & ujoi<im_type >::operator[](const vint & pos) const { // const vint acces to the elements

    // CAPABILITY TEST IN DEBUG MODE
    // this is a test which would be slow in an everyday situation,
    // but can save ages while debugging
    #ifdef UJIMAGE_DEBUG
    if ( not size.inside(pos) )
    {
      cerr << "error: image dimensions are " << this->size.repr() << " while pos = " << pos.repr() << "\n";
      error("You are trying to access elements otside of the image\n");
    } /* if */
    #endif /* UJIMAGE_DEBUG */


    return pixels[size.position(pos)];

  };



  template <class im_type >
  im_type & ujoi<im_type >::operator[]( const boost::python::list & pos ){ // python list acces to the elements

    vint vint_pos(pos);

    return (*this)[vint_pos];

  };



  template <class im_type >
  const im_type & ujoi<im_type >::operator[]( const boost::python::list & pos ) const { // const python list acces to the elements

    vint vint_pos(pos);

    return (*this)[vint_pos];

  };



  template <class im_type >
  const im_type & ujoi<im_type >::get_operator_int( int pos ) const {
    return (*this)[pos]; 
  };



  template <class im_type >
  void ujoi<im_type >::set_operator_int( int pos, const im_type & value ){
    (*this)[pos]=value;
  };



  template <class im_type >
  const im_type & ujoi<im_type >::get_operator_vint( const vint & pos ) const {
    return (*this)[pos];
  };



  template <class im_type >
  void ujoi<im_type >::set_operator_vint( const vint & pos, const im_type & value ){
    (*this)[pos]=value;
  };



  template <class im_type >
  const im_type & ujoi<im_type >::get_operator_list( const boost::python::list & pos ) const {
    return (*this)[pos];
  };



  template <class im_type >
  void ujoi<im_type >::set_operator_list( const boost::python::list & pos, const im_type & value ){
    (*this)[pos]=value;
  };



// template <class im_type >
// string ujoi<im_type >::imtype(){
//   switch (int_im_type){

//   case VFF_TYP_1_BYTE:
//     return "uint8_t"; /* pixels are byte (uint8_t) */
//     break;

//   case VFF_TYP_2_BYTE:
//     return "int16_t"; /* pixels are two byte (int16_t) */
//     break;

//   case VFF_TYP_4_BYTE:
//     return "int32"; /* pixels are four byte (integer) */
//     break;

//   case VFF_TYP_FLOAT:
//     return "float"; /* pixels are float (single precision)*/
//     break;

//   case VFF_TYP_DOUBLE:
//     return "double"; /* pixels are float (double precision)*/
//     break;

//   default:
//     error("bad image type or not specified.");

//   };

// };



  template <class im_type >
  const vint & ujoi<im_type >::get_size() const{
    return size;
  };



  template <class im_type >
  const vint & ujoi<im_type >::get_center() const{
    return center;
  };



  template <class im_type >
  void ujoi<im_type >::set_center_vint( const vint & new_center ){

    if ( new_center.size() != size.size() )

      error("set_center: the center coordinate has to have the same dimension as the image")

      else 

	center = vint( new_center );
  };



  template <class im_type >
  void ujoi<im_type >::set_center_list( const boost::python::list & new_center ){


    vint vint_new_center = vint( new_center ); // converting 'boost::python::list' into 'vint'

    if ( vint_new_center.size() != size.size() )

      error("set_center: the center coordinate has to have the same dimension as the image")

      else 

	center = vint( vint_new_center );
  };



  template <class im_type >
  ARRAY<im_type> ujoi<im_type >::get_pixels(){
    return pixels;
  };



  template <class im_type >
  ARRAY<im_type> ujoi<im_type >::get_pixels() const{
    return pixels;
  }


























































}; /* namespace pink */
#endif /* __UJIMAGE_HPP */
#endif /* __cplusplus */
// LuM end of file
