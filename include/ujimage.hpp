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

//  string image_type( int pixel_type ); // returns the corresponding image types

  template <class pixel_type>
  class image_type_specific
  {
  public:
    
//#error: this image type is not defined
    string imtype() const 
      {         			      
	error("unimplemented image type specialization");
      }
    int int_pixel_type() const 
      {
        error("unimplemented image type specialization");
      }
    
    pixel_type sub( pixel_type a, pixel_type b )
      {
        error("unimplemented image type specialization");
      }
    
    pixel_type add( pixel_type a, pixel_type b)
      {
        error("unimplemented image type specialization");
      }
    
  }; /* class image_type specific */
  
  
// #define CREATE_IMAGE_TYPE( _class, name, int_type )			\
//   template <>								\
//   class image_type_specific< _class >{					\
//   public:								\
//   string imtype() const							\
//       {									\
// 	return name;							\
//       };								\
//   int int_pixel_type() const						\
//       {									\
// 	return int_type;						\
//       };								\
//   };									
  
  									
  // helper function for reading and writing from and to xvimage
  PTR<vint> getDimensions( const int x, const int y, const int z, const int t );
  void setDimensions(const vint & dim, int & x, int & y, int & z, int & t);
  PTR<deep_xvimage> py_readimage( string filename );
  string image_type_string( int pixel_type );


  // this class will copy it's content when upcasted (constructed from xvimage)
  // it is used, when the data is needed (in readimage for example)
  // note: image name is fake
  class deep_xvimage
  {
  private:
    xvimage * data;    
  public:
    deep_xvimage(); // default constructor
    deep_xvimage( const xvimage & src ); // upcast constructor
    deep_xvimage( const deep_xvimage & src ); // copy constructor. RAISES AND ERROR!
                                              // it's not yet implemented, raises an
                                              // error if called implicitly
    virtual ~deep_xvimage(); // default destructor
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
    shallow_xvimage( const vint & dim, int int_pixel_type );  // construct from dimension. The data 
    // type must be specified. See mcimage.h
    virtual ~shallow_xvimage(); // default destructor
    string imtype(); // returns the image type
  }; /* xvImage: xvimage */
 
  class pink_image
  {
  public:
    virtual operator xvimage*();    
  }; /* class pink_image */


  template<class image_type, class pixel_type>
  struct ujimage_operators
  {
    
    friend bool operator!=(const image_type & x, const image_type & y)
    {
      return !( x==y );      
    }

    friend image_type operator+( const image_type & x, const image_type & y)
      {
        return image_type().copy(x)+=y;        
      }

    friend image_type operator-( const image_type & x, const image_type & y)
      {
        return image_type().copy(x)-=y;        
      }

    friend image_type operator+( const image_type & x, pixel_type val)
      {
        return image_type().copy(x)+=val;        
      }

    friend image_type operator+( pixel_type val, const image_type & x)
      {
        return image_type().copy(x)+=val;        
      }

    friend image_type operator*( const image_type & x, pixel_type val)
      {
        return image_type().copy(x)*=val;        
      }
    
    friend image_type operator*( pixel_type val, const image_type & x)
      {
        return image_type().copy(x)*=val;        
      }


    friend image_type operator-( const image_type & x, pixel_type val)
      {
        return image_type().copy(x)-=val;        
      }

    friend image_type operator/( const image_type & x, pixel_type val)
      {
        return image_type().copy(x)/=val;        
      }

    
  }; /* struct ujimage_operators */

  // template<>
  // bool operator!=( )
  
  
  
  
  template <class pixel_type_> // pixel_type can be char, short, int, float and double
  // while int_pixel_type is the type's encoding into integer. See 'mccodimage.h'
  class ujoi:
    public pink_image,
    public image_type_specific<pixel_type_>,
    public ujimage_operators< ujoi<pixel_type_>, pixel_type_ >
  {
  public:
    typedef pixel_type_ pixel_type;
    typedef ujoi<pixel_type> image_type;
        
  private:

    PTR<shallow_xvimage> old_school;
    PTR<vint> size;
    PTR<vint> center;
    ARRAY<pixel_type> pixels;

    #ifdef UJIMAGE_DEBUG
    string debug; // representing the name of the object if debugged
    #endif /* UJIMAGE_DEBUG */
 
  public:

    ujoi( ); // creates an empty image, used in uiSqhool for determining the image type
    ujoi( string filename, string debug="" );
    ujoi( const xvimage & src, string debug="" ); // deep constructor takes xvimage and makes a copy
                                                  // it is not embeddable becouse readimage's using malloc/free, whereas boost's using new/delete 
    ujoi( const ujoi< pixel_type > & src, string debug="" ); // SHALLOW! copy_constructor. For conversion use convert2float
                                                             // for deep_copy use operator=
    image_type & operator=( const image_type & other );     // SHALLOW! copy constructor
    void reset( image_type & other );         // runtime shallow copy
    image_type & copy( const image_type & other ); // DEEP copy!!    
 
    

    ujoi( const boost::python::list & dim, string debug="" );
    ujoi( const vint & dim, string debug="" );
    virtual ~ujoi(); // default destructor
    ujoi( const vint & dim, ARRAY<pixel_type> data, string debug="" ); // used to construct from ujif.

    pixel_type & operator[]( int pos ); // index acces to the elements
    const pixel_type & operator[]( int pos ) const; // const index acces to the elements

    pixel_type & operator[]( const vint & pos ); // vint acces to the elements
    const pixel_type & operator[]( const vint & pos ) const;  // const vint acces to the elements

    pixel_type & operator[]( const boost::python::list & pos ); // python list acces to the elements
    const pixel_type & operator[]( const boost::python::list & pos ) const; // const python list acces to the elements

    // these methods are the operators renamed so they could be wrapped by boost
    const pixel_type & get_operator_int( int pos ) const;
    void set_operator_int( int pos, const pixel_type & value );

    const pixel_type & get_operator_vint( const vint& pos ) const;
    void set_operator_vint( const vint & pos, const pixel_type & value );

    const pixel_type & get_operator_list( const boost::python::list & pos ) const;
    void set_operator_list( const boost::python::list & pos, const pixel_type & value );

    void _writeimage( const string & filename ) const; // exports the image into a pgm file
    void _write_amira( const string & filename ) const; // exports the image into an amira mesh (.am) file

    // cast operators
    xvimage* get_output(); // this method is not exported to python. It could not even be, as boost doesn't support pointers.    
    operator xvimage*();
    operator const xvimage*() const;    
//    string imtype(); 
    const vint & get_size() const;
    const vint & get_center() const;
    vint & get_center();
    void set_center_vint( const vint & new_center );
    const vint & get_center_vint() const;
    void set_center_list( const boost::python::list & new_center );

    ARRAY<pixel_type> get_pixels();
    ARRAY<pixel_type> get_pixels() const;

    string repr() const;
    void fill( pixel_type value );
    image_type operator=( pixel_type value ); // equivalent with function fill


    bool operator==( const image_type & other ) const;
    image_type operator+=( const image_type & other );
    image_type operator-=( const image_type & other );
    
    image_type operator-=( const pixel_type & value );
    image_type operator+=( const pixel_type & value );
    image_type operator/=( const pixel_type & value );
    image_type operator*=( const pixel_type & value );
    
    int area();
    double average();
    bool isnull();
    double volume();
     
  }; /* class ujoi */


    /*
***************************************************************************************
***************************************************************************************
***************************************************************************************


IMAGE TYPE SPECIFICS


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

/*
***************************************************************************************
CHAR IMAGE
***************************************************************************************
 */

  template <>								
  class image_type_specific<unsigned char>{
    typedef unsigned char pixel_type;    
  public:								
    string imtype() const							
      {									
	return "uint8_t";
      }
    
    int int_pixel_type() const						
      {									
	return VFF_TYP_1_BYTE;						
      }

    pixel_type sub( pixel_type a, pixel_type b )
      {
        return a-b > 0 ? a-b : 0;        
      }

    pixel_type add( pixel_type a, pixel_type b)
      {
        return a+b < UCHAR_MAX ? a+b : UCHAR_MAX;        
      }
    
  }; /* image_type_specific char_image */								


  /*
***************************************************************************************
SHORT IMAGE
***************************************************************************************
 */
  
    template <>								
  class image_type_specific<unsigned short int>{
    typedef unsigned short int pixel_type;    
  public:								
    string imtype() const							
      {									
	return "uint16_t";
      }
    
    int int_pixel_type() const						
      {									
	return VFF_TYP_2_BYTE;						
      }

    pixel_type sub( pixel_type a, pixel_type b )
      {
        return a-b > 0 ? a-b : 0;        
      }

    pixel_type add( pixel_type a, pixel_type b)
      {
        return a+b < USHRT_MAX ? a+b : USHRT_MAX;        
      }
    
  }; /* image_type_specific short_image */								

  
  /*
***************************************************************************************
INT IMAGE
***************************************************************************************
 */

  
  template <>								
  class image_type_specific<int>{
    typedef int pixel_type;    
  public:								
    string imtype() const							
      {									
	return "int32_t";
      }
    
    int int_pixel_type() const						
      {									
	return VFF_TYP_4_BYTE;						
      }

    pixel_type sub(pixel_type a, pixel_type b )
      {
        return a-b > 0 ? a-b : 0;
      }

    pixel_type add(pixel_type a, pixel_type b)
      {
        return a+b < LONG_MAX ? a+b : LONG_MAX;
      }
    
  }; /* image_type_specific int_image */								

  
  /*
***************************************************************************************
FLOAT IMAGE
***************************************************************************************
 */

  
  template <>								
  class image_type_specific<float>{
    typedef float pixel_type;    
  public:								
    string imtype() const							
      {									
	return "float";
      }
    
    int int_pixel_type() const						
      {									
	return VFF_TYP_FLOAT;						
      }

    pixel_type sub( pixel_type a, pixel_type b )
      {
        return a-b;        
      }

    pixel_type add( pixel_type a, pixel_type b)
      {
        return a+b;
      }
    
  }; /* image_type_specific float_image */								

  
  /*
***************************************************************************************
double IMAGE
***************************************************************************************
 */

  template <>								
  class image_type_specific<double>{
    typedef double pixel_type;    
  public:								
    string imtype() const							
      {									
	return "double";
      }
    
    int int_pixel_type() const						
      {									
	return VFF_TYP_DOUBLE;						
      }

    pixel_type sub( pixel_type a, pixel_type b )
      {
        return a-b;        
      }

    pixel_type add( pixel_type a, pixel_type b)
      {
        return a+b;        
      }
    
  }; /* image_type_specific double_image */								

  
  // Valid image types
  // integer and float will be tested first.

  // CREATE_IMAGE_TYPE( unsigned char, "uint8_t", VFF_TYP_1_BYTE );
  // CREATE_IMAGE_TYPE( unsigned short int, "uint16_t", VFF_TYP_2_BYTE );
  // CREATE_IMAGE_TYPE( unsigned int, "int32_t", VFF_TYP_4_BYTE );
  // CREATE_IMAGE_TYPE( float, "float", VFF_TYP_FLOAT );
  // CREATE_IMAGE_TYPE( double, "double", VFF_TYP_DOUBLE );




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
  
  template <class pixel_type >
  ujoi<pixel_type >::ujoi( /* string debug="" */ ) {
    
    #if UJIMAGE_DEBUG >= 2
    cout << "creating an empty image (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

  } /* ujoi<pixel_type >::ujoi( const string & filename ) */


  template <class pixel_type>
  ujoi<pixel_type>::ujoi( string filename, string debug )
  {
    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    cout << "reading image '" << filename << "'\n";
    #endif /* UJIMAGE_DEBUG */

    xvimage * tmp;

    if (! (tmp = readimage(const_cast<char*>(filename.c_str()))) ) // readimage takes char* but hopefully does not change it
    {
      error("cannot read file '" + filename + "'");      
    } 
    
    writeimage(tmp, "tmp.pgm");
    
    if (tmp->data_storage_type != this->int_pixel_type() )
    {
      error("The image type of '" + filename + "' is '" + image_type_string(tmp->data_storage_type)
            + "', but expected '" + this->imtype() + "'." );
    } /* if */
      
    this->size.reset( new vint(
                        *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
                          tmp->row_size, 
                          tmp->col_size, 
                          tmp->depth_size, 
                          tmp->time_size
                          )
                        )
      );

    // setting up the center
    this->center.reset(new vint( size->size(), -1 ));
  
    this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating the array for the pixel types

    std::copy( &(static_cast<pixel_type*>(tmp->image_data)[0]),
               &(static_cast<pixel_type*>(tmp->image_data)[this->size->prod()]),
               this->pixels.get()
      );

    freeimage(tmp);
    
  } /* ujoi::ujoi */
  
  template <class pixel_type >
  ujoi<pixel_type >::ujoi( const struct xvimage & src, string debug ) {
    
    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    if (image_type_string(src.data_storage_type)!=this->imtype())
    {
      error("converting to ujoi from different pixel_type");
    } /* image_type(src.data_storage_type)!=this->imtype() */
    
    this->size.reset(
      new vint(
        *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
          src.row_size, 
          src.col_size, 
          src.depth_size, 
          src.time_size
          )
        )
      );

    // setting up the center
    this->center.reset( new vint( size->size(), -1 ));
  
    this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating the array for the pixel types

    std::copy( &(static_cast<pixel_type*>(src.image_data)[0]),
               &(static_cast<pixel_type*>(src.image_data)[this->size->prod()]),
               this->pixels.get()
      );

  } /* ujoi::ujoi */

  template <class im_type >
  ujoi<im_type>::ujoi( const ujoi< im_type > & src, string debug ) // SHALLOW_copy_constructor
    : size(src.size), center(src.center), old_school(src.old_school), pixels(src.pixels)
  {
    #if UJIMAGE_DEBUG >= 2
    cout << "WARNING!: copy constructors only constructs shallow copies!\n"
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */
  } /* ujoi::ujoi */


  template <class im_type>
  ujoi<im_type> & ujoi<im_type>::operator=( const image_type & other )     // SHALLOW_copy constructor
  {
    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    // note self assignment will be tested inside boost-smart-pointer
    this->size=other.size;
    this->center=other.center;
    this->old_school=other.old_school;   
    this->pixels=other.pixels; 
        
    return *this;
  } /* ujoi::operator= */
  

  
  template <class pixel_type >
  ujoi<pixel_type >::ujoi( const vint & dim, string debug ){

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */


    this->size.reset(new vint( dim )); // creating a copy of the size
    this->center.reset(new vint( size->size(), -1 ));
    this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

    // setting up elements zero
    FOR(q, size->prod())
      this->pixels[q]=0;

  } /* ujoi::ujoi */

  template <class pixel_type >
  ujoi<pixel_type >::ujoi( const boost::python::list & dim, string debug ){

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    this->size.reset( new vint(dim)); // creating a copy of the size
    this->center.reset( new vint( size->size(), -1 ));

    this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

    // setting up elements zero
    FOR(q, size->prod())
      this->pixels[q]=0;

  } /* ujoi::ujoi */


  template <class pixel_type >
  ujoi<pixel_type >::~ujoi( ){

    #if UJIMAGE_DEBUG >= 2
    cout << "deleting image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

// deletes 'xvImage' automaticly
    /////!!!!!!! cout<< "deleting " << old_school->imtype() << "_char"  << endl; 
  } /* ujoi::~ujoi */



  template <class pixel_type >
  ujoi<pixel_type >::ujoi( const vint & dim, ARRAY<pixel_type> data, string debug )
  {

    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image " << debug << endl;
    #endif /* UJIMAGE_DEBUG */

    size.reset(new vint( dim )); // creating a copy of the size
    center.reset(new vint( size->size(), -1 ));
    pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

    if (this->size->size()<=4){
      old_school.reset( new shallow_xvimage( *size, this->int_pixel_type() ) );
    };

    std::copy( (&data[0]), (&data[size->prod()]), pixels.get());

  } /* ujoi::ujoi */



  template <class pixel_type >
  void ujoi<pixel_type >::_writeimage( const string & filename ) const // exports the image into a pgm file
  {
    // writeimage takes 'char *', while 'c_str()' gives 'const char *'. 
    // I don't want to cast unnecesserily, so I'll just copy it.
 
    char * cstr = new char [ filename.size() + 1 ]; // converting the filename for 'writeimage'
    strcpy ( cstr, filename.c_str() );
    
    writeimage( const_cast<image_type*>(this)->get_output(), cstr );    

  } /* _writeimage */



  template<class pixel_type>
  void write_a_pixel( fstream & s, pixel_type & value ) 
  {
    error("write_a_pixel called with a wrong pixel type");
  } /* write_a_pixel default */

  template<> // implemented in 'ujimage.cpp'
  void write_a_pixel<float>( fstream & s, float & value ) ;

  template<> // implemented in 'ujimage.cpp'
  void write_a_pixel<unsigned char>( fstream & s, unsigned char & value ) ;


  template <class pixel_type >
  void ujoi<pixel_type >::_write_amira( const string & filename ) const // exports the image into an amira mesh (.am) file
  { 
  
    
    string typetext;
  
    switch (this->int_pixel_type())  { 
      
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
        
    if (this->size->size()!=3)
    {
      cout << "file: " << filename; 
      error("you can export only 3D images at this point");
    } 
    else /* NOT this->size.size()!=3 */
    {
      fstream s;
      s.open ( filename.c_str(), fstream::out );
      s << "# UjoImro PInK Amira export, 2009-2010\n\n"
	<< "# AmiraMesh BINARY-LITTLE-ENDIAN 2.1\n\n\n"
	<< "define Lattice " << " " << (*this->size)[0] << " " << (*this->size)[1] << " " << (*this->size)[2] << "\n\n"
	<< "Parameters {\n"
	<< "\tContent\"" << " " << (*this->size)[0] << "x" << (*this->size)[1] << "x" << (*this->size)[2] << " PInK pgm export \",\n"
        << "\tBoundingBox 0 " << (*this->size)[0]-1 << " 0 " << (*this->size)[1]-1 << " 0 " << (*this->size)[2]-1 << ",\n"
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
      
      FOR(q, (*size)[2]) {
	FOR(w, (*size)[1]) { 
	  FOR(e, (*size)[0]) {
	    
	    curr[0]=e;
	    curr[1]=w;
	    curr[2]=q;
	    
	    
	    s.write( 
	      reinterpret_cast<char*>(&pixels[ 
					size->position(curr)
					] 
		),
	      sizeof(pixel_type)
	      );
	    
	  }; /* FOR(i, size[2]) */
	}; /* FOR(j, size[1]) */
      }; /* FOR(k, size[0]) */
      
      
      s << "\n";
      s.close();
    } /* NOT this->size.size()!=3 */
    cout << "file '" << filename << "' exported in Amira format\n";
  } /* ujoi<pixel_type >::_write_amira */



  template <class pixel_type>
  ujoi<pixel_type>::operator xvimage* () // this method is not exported to python. It could not even be, as boost doesn't support pointers.
  {
    return this->get_output();    
  } /* ujoi::xvimage*  */

  template <class pixel_type>
  ujoi<pixel_type>::operator const xvimage* () const // this method is not exported to python. It could not even be, as boost doesn't support pointers.
  {
    return this->get_output();    
  } /* ujoi::xvimage*  */


  template <class pixel_type >
  xvimage* ujoi<pixel_type >::get_output(){
    if (this->size->size()>4){
      error("error: Images with more than four dimensions can not be extracted to 'xvimage'");
    }; /* this->size.size()>4 */

    this->old_school.reset( new shallow_xvimage( *size, this->int_pixel_type() ) );
    
    this->old_school->image_data=pixels.get();
    // setting up the data type. This is an 
    // unsafe action, and is only allowed 
    // for backward compatibility.
    
    return old_school.get(); // you must never 'free()' or 'delete' this pointer!
  } /* ujoi::get_output */



  template <class pixel_type >
  pixel_type & ujoi<pixel_type >::operator[](int pos){ // index acces to the elements

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

  } /* ujoi::operator[] */



  template <class pixel_type >
  const pixel_type & ujoi<pixel_type >::operator[](int pos) const { // const index acces to the elements

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

  } /* ujoi::operator[] */


  template <class pixel_type >
  pixel_type & ujoi<pixel_type >::operator[](const vint & pos){ // vint acces to the elements

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

    return pixels[size->position(pos)];

  } /* ujoi::operator[] */



  template <class pixel_type >
  const pixel_type & ujoi<pixel_type >::operator[](const vint & pos) const { // const vint acces to the elements

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

    return pixels[size->position(pos)];

  } /* ujoi::operator[] */



  template <class pixel_type >
  pixel_type & ujoi<pixel_type >::operator[]( const boost::python::list & pos ){ // python list acces to the elements

    vint vint_pos(pos);

    return (*this)[vint_pos];

  } /* ujoi::operator[] */



  template <class pixel_type >
  const pixel_type & ujoi<pixel_type >::operator[]( const boost::python::list & pos ) const { // const python list acces to the elements

    vint vint_pos(pos);

    return (*this)[vint_pos];

  } /* ujoi::operator[] */



  template <class pixel_type >
  const pixel_type & ujoi<pixel_type >::get_operator_int( int pos ) const {
    return (*this)[pos]; 
  };



  template <class pixel_type >
  void ujoi<pixel_type >::set_operator_int( int pos, const pixel_type & value ){
    (*this)[pos]=value;
  }



  template <class pixel_type >
  const pixel_type & ujoi<pixel_type >::get_operator_vint( const vint & pos ) const {
    return (*this)[pos];
  }



  template <class pixel_type >
  void ujoi<pixel_type >::set_operator_vint( const vint & pos, const pixel_type & value ){
    (*this)[pos]=value;
  }



  template <class pixel_type >
  const pixel_type & ujoi<pixel_type >::get_operator_list( const boost::python::list & pos ) const {
    return (*this)[pos];
  }



  template <class pixel_type >
  void ujoi<pixel_type >::set_operator_list( const boost::python::list & pos, const pixel_type & value ){
    (*this)[pos]=value;
  }



// template <class pixel_type >
// string ujoi<pixel_type >::imtype(){
//   switch (int_pixel_type){

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



  template <class pixel_type >
  const vint & ujoi<pixel_type >::get_size() const
  {
    return *size;
  } /* ujoi:: get_size */

  template <class pixel_type >
  const vint & ujoi<pixel_type >::get_center() const
  {
    return *center;
  } /* ujoi::get_center */

  template <class pixel_type >
  vint & ujoi<pixel_type >::get_center()
  {
    return *center;
  } /* ujoi::get_center */



  template <class pixel_type >
  void ujoi<pixel_type >::set_center_vint( const vint & new_center ){

    if ( new_center.size() != size->size() )
    {      
      error("set_center: the center coordinate has to have the same dimension as the image");      
    }
    else 
    {
      center.reset( new vint( new_center ));
    }
    
  } /* ujoi::set_center_vint */

  template <class pixel_type >
  const vint & ujoi<pixel_type >::get_center_vint() const
  {
    return *center;    
  } /* ujoi::get_center_vint */



  template <class pixel_type >
  void ujoi<pixel_type >::set_center_list( const boost::python::list & new_center ){


    vint vint_new_center = vint( new_center ); // converting 'boost::python::list' into 'vint'

    if ( vint_new_center.size() != size->size() )
    {      
      error("set_center: the center coordinate has to have the same dimension as the image");
    }
    else /* NOT vint_new_center.size() != size->size() */
    {      
      center.reset( new vint( vint_new_center ) );
    } /* NOT vint_new_center.size() != size->size() */
  } /* ujoi::set_center_list */



  template <class pixel_type >
  ARRAY<pixel_type> ujoi<pixel_type >::get_pixels(){
    return pixels;
  } /* ujoi::get_pixels */



  template <class pixel_type >
  ARRAY<pixel_type> ujoi<pixel_type >::get_pixels() const{
    return pixels;
  } /* ujoi::get_pixels */


  template <class pixel_type >
  string ujoi<pixel_type >::repr() const
  {
    stringstream ss;
    ss << this->imtype() << " image of size " << get_size().repr();
    
    string result;
    
    result = ss.str();
    
    return result;
  } /* ujoi::repr */

  template <class pixel_type >
  void ujoi<pixel_type >::fill( pixel_type value ) 
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]=value;      
    } /* FOR */
    
  } /* ujoi::fill */
  
  
  template <class pixel_type>
  ujoi<pixel_type> ujoi<pixel_type>::operator=( pixel_type value ) // equivalent with function fill
  {
    this->fill(value);    
  } /* ujoi::operator= */

  
  template <class pixel_type >
  bool ujoi<pixel_type >::operator==( const image_type & other ) const
  {
    FOR(q, get_size().prod())
    {
      if ((*this)[q]!=other[q])
      {
        return false;
      } /* (*this)[q]!=other[q] */      
    } /* FOR */

    return true;
  } /* ujoi::operator== */


  template <class pixel_type >
  ujoi<pixel_type> ujoi<pixel_type >::operator+=( const image_type & other )
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]=this->add((*this)[q],other[q]);
    } /* FOR */
    
    return *this;    
  } /* ujoi::operator+= */




  template <class pixel_type >
  ujoi<pixel_type> /*image_type*/
  ujoi<pixel_type >::operator-=( const image_type & other )
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]=this->sub((*this)[q],other[q]);
    } /* FOR */
    
    return *this;      
  } /* ujoi::operator-= */


  
  template <class pixel_type >
  ujoi<pixel_type> /*image_type*/
  ujoi<pixel_type >::operator-=( const pixel_type & value )
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]-=value;      
    } /* FOR */
    
    return *this;      
  } /* ujoi::operator-= */


  
  template <class pixel_type >
  ujoi<pixel_type> /*image_type*/
  ujoi<pixel_type >::operator+=( const pixel_type & value )
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]+=value;      
    } /* FOR */
    
    return *this;    
  } /* ujoi::operator+= */


  
  template <class pixel_type >
  ujoi<pixel_type> /*image_type*/
  ujoi<pixel_type >::operator/=( const pixel_type & value )
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]/=value;      
    } /* FOR */
    
    return *this;    
  } /* ujoi::operator/= */


  
  template <class pixel_type >
  ujoi<pixel_type> /*image_type*/
  ujoi<pixel_type >::operator*=( const pixel_type & value )
  {
    FOR(q, get_size().prod())
    {
      (*this)[q]*=value;      
    } /* FOR */
    
    return *this;    
  } /* ujoi::operator*= */
  

  
  template <class pixel_type >
  int ujoi<pixel_type >::area() // number of non zero pixels
  {
    int result=0;
    
    FOR(q, get_size().prod())
    {
      if ((*this)[q]!=0)
      {
        result++;        
      } /* (*this)[q]!=0 */
    } /* FOR */
  } /* ujoi::area */


  
  template <class pixel_type >
  double ujoi<pixel_type >::average() // average value
  {
    double result=0;
    result = this->volume()/this->get_size().prod();
    return result;    
  } /* ujoi::average */


  
  template <class pixel_type >
  bool ujoi<pixel_type >::isnull() 
  {
    FOR(q, get_size().prod())
    {
      if ((*this)[q]!=0)
      {
        return false;        
      } /* (*this)[q]!=0 */
    } /* FOR */

    return true;    
  } /* ujoi::isnull */


  
  template <class pixel_type >
  double ujoi<pixel_type >::volume() // sum of all the pixels
  {
    double result=0;
    
    FOR(q, get_size().prod())
    {
      result+=(*this)[q];
    } /* FOR */

    return result;
  } /* ujoi::volume */
  

  template <class pixel_type >
  void ujoi<pixel_type >::reset( image_type & other )
  {
    this->size=other.size;
    this->center=other.center;
    this->old_school=other.old_school;
    this->pixels=other.pixels;  
  } /* ujoi::reset */

  template <class pixel_type>
  ujoi<pixel_type> & ujoi<pixel_type >::copy( const image_type & other )
  {
    #if UJIMAGE_DEBUG >= 2
    this->debug=debug; // representing the name of the object if debugged
    cout << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << endl;
    #endif /* UJIMAGE_DEBUG */

    if (this==&other) // self-assignment test
    {
      return *this;      
    }
    
    this->size.reset( new vint( *other.size ));
    this->center.reset( new vint( *other.center ));
    this->old_school.reset(new shallow_xvimage( (*other.size), other.int_pixel_type() )); // creating a new xvimage for the 'pink::ujoi' object
    // the constructor will not copy the data
  
    this->pixels.reset(new pixel_type[size->prod()]); // allocating the array for the pixel types

    std::copy( &other[0], &other[size->prod()], this->pixels.get());
        
    return *this;
  } /* ujoi::copy */










































}; /* namespace pink */
#endif /* __UJIMAGE_HPP */
#endif /* __cplusplus */
// LuM end of file
