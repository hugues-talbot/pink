/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/




#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ujimage.cpp: " << msg; call_error(fullmessage.str());}

namespace pink{

PTR<pink::deep_xvimage> py_readimage( string filename ){
  // readimage takes 'char *', while 'c_str()' gives 'const char *'. 
  // I don't want to cast unnecesserily, so I'll just copy it.

  char * cstr = new char [filename.size()+1]; // converting the filename for 'readimage'
  strcpy ( cstr, filename.c_str() );
  
  xvimage * tmp = readimage( cstr ); // reading the image

  if (tmp==NULL)
  {
    error("file not found: " + filename );
  }
  
  PTR< pink::deep_xvimage > result( new deep_xvimage( *tmp ) ); // upcast deep copying the xvimage

  freeimage( tmp ); // freeing xvimage
  
  return result;
};


PTR<vint> getDimensions( const int x, const int y, const int z, const int t ){
	PTR<vint> presult;
	if (t>1) {
		/////!!!!!!! cout<< "I've desided for 4D." << endl;
		presult.reset(new vint(4,-1));
		(*presult)[0]=x;
		(*presult)[1]=y;
		(*presult)[2]=z;
		(*presult)[3]=t;
	} else if (z>1){
		/////!!!!!!! cout<< "I've desided for 3D." << endl;
		presult.reset(new vint(3,-1));
		(*presult)[0]=x;
		(*presult)[1]=y;
		(*presult)[2]=z;
	} else if (y>1){
		/////!!!!!!! cout<< "I've desided for 2D." << endl;
		presult.reset(new vint(2,-1));
		(*presult)[0]=x;
		(*presult)[1]=y;
	} else if (x>1){
		/////!!!!!!! cout<< "I've desided for 1D or less." << endl;
		error("an image should have at least 2 dimensions");
	};
	
	if ((t>1) and (z==1)){
		error("two dimensional time series are probably not handled well");
	};
	return presult;
}


void setDimensions(const vint & dim, int & x, int & y, int & z, int & t){
  int d = dim.size();
  bool result = true;
  x=y=z=t=1;
  switch (d){
  case 1:
    x=dim[0];
    y=1;
    z=1;
    t=1;
    break;
  case 2:
    x=dim[0];
    y=dim[1];
    z=1;
    t=1;
    break;
  case 3:
    x=dim[0];
    y=dim[1];
    z=dim[2];
    t=1;
    break;
  case 4:
    x=dim[0];
    y=dim[1];
    z=dim[2];
    t=dim[3];
    break;
  default:
    result = false;
    error("error: the dimension is wrong or unsupported");
    break;
  } /* switch */
};

string image_type( int im_type ){
  switch (im_type){
    
  case VFF_TYP_1_BYTE:
    return "uint8_t"; /* pixels are byte (uint8_t) */
    break;
    
  case VFF_TYP_2_BYTE:
    return "int16_t"; /* pixels are two byte (int16_t) */
    break;
    
  case VFF_TYP_4_BYTE:
    return "int32_t"; /* pixels are four byte (integer) */
    break;
    
  case VFF_TYP_FLOAT:
    return "float"; /* pixels are float (single precision)*/
    break;
    
  case VFF_TYP_DOUBLE:
    return "double"; /* pixels are float (double precision)*/
    break;

  default:
    error("bad image type or not specified.");
  };
  
};

/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


c++ class shallow_xvimage


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

shallow_xvimage::shallow_xvimage(){ 
  name = new char[1];
  strcpy(name, "");
};


shallow_xvimage::shallow_xvimage( const shallow_xvimage & src ){ 
                                         // Carefully, the image data is not copied. 
                                         // It will be created by 'ujoi::operato&'
                                         // for functions which use xvimage from python
  name=new char[1];  // I allocate it if anything wanted to use it. It will be empty anyway.
  strcpy(name, "");
  image_data=NULL;   // the image data is never allocated and therefore destroyed !
                     // it is only used for 'ujoi::operator&'
  this->row_size=src.row_size;
  this->col_size=src.col_size;
  this->depth_size=src.depth_size;
  this->time_size=src.time_size;
  this->num_data_bands=src.num_data_bands;
  this->data_storage_type=src.data_storage_type;
  this->xdim=src.xdim;
  this->ydim=src.ydim;
  this->zdim=src.zdim;
  this->xmin=src.xmin;
  this->xmax=src.xmax;
  this->ymin=src.ymin;
  this->ymax=src.ymax;
  this->zmin=src.zmin;
  this->zmax=src.zmax;
};


shallow_xvimage::shallow_xvimage( const xvimage & src ){ 
                                         // Carefully, the image data is not copied. 
                                         // It will be created by 'ujoi::operato&'
                                         // for functions which use xvimage from python
  name=new char[1];  // I allocate it if anything wanted to use it. It will be empty anyway.
  strcpy(name, "");
  image_data=NULL;   // the image data is never allocated and therefore destroyed !
                     // it is only used for 'ujoi::operator&'
  this->row_size=src.row_size;
  this->col_size=src.col_size;
  this->depth_size=src.depth_size;
  this->time_size=src.time_size;
  this->num_data_bands=src.num_data_bands;
  this->data_storage_type=src.data_storage_type;
  this->xdim=src.xdim;
  this->ydim=src.ydim;
  this->zdim=src.zdim;
  this->xmin=src.xmin;
  this->xmax=src.xmax;
  this->ymin=src.ymin;
  this->ymax=src.ymax;
  this->zmin=src.zmin;
  this->zmax=src.zmax;
};

shallow_xvimage::shallow_xvimage( const vint & dim, int int_im_type ){ 
                                         // // Carefully, the image data  
                                         // will be created by 'ujoi::operator&'
                                         // for functions which use xvimage from python
  name=new char[1];  // I allocate it if anything wanted to use it. It will be empty anyway.
  strcpy(name, "");
  image_data=NULL;   // the image data is never allocated and therefore destroyed !
                     // it is only used for 'ujoi::operator&'

  int x,y,z,t;
  setDimensions( dim, x, y, z, t );

  
  this->row_size   = x;
  this->col_size   = y;
  this->depth_size = z;
  this->time_size  = t;

  this->num_data_bands    = 1;

  this->data_storage_type = int_im_type; /// !!!!!!!!

  this->xdim = -1;
  this->ydim = -1;
  this->zdim = -1;
  this->xmin = -1;
  this->xmax = -1;
  this->ymin = -1;
  this->ymax = -1;
  this->zmin = -1;
  this->zmax = -1;
};


shallow_xvimage::~shallow_xvimage(){
  delete name;
  ////// /// !!!!!!!!!! delete image->image_data; // this is a shallow container 
                                                  // image data is not to be deleted!
  /////!!!!!!! cout<< "deleting shallow_xvimage" << endl;
  // the rest of the properties are destroyed automaticly
};

string shallow_xvimage::imtype(){
  return image_type(this->data_storage_type);
};


/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


c++ class pink::deep_xvimage


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

deep_xvimage::deep_xvimage(){ 
  name = new char[1];
  strcpy(name, "");
  image_data=reinterpret_cast<void*>( new char[1] );
};


deep_xvimage::deep_xvimage( const deep_xvimage & src ){ 
  error(
    "The 'deep_xvimage' copy constructor is not implemented. If you want to use it, "
    "you have to implement it. If it's called implicitly (that is to say not by you) "
    "then it's a bug and has to be corrected,"
    );
};

deep_xvimage::deep_xvimage( const xvimage & src ){ 
                                         // Carefully, the image data is not copied. 
                                         // It will be created by 'ujoi::operator&'
                                         // for functions which use xvimage from python
  name=new char[1];  // I allocate it if anything wanted to use it. It will be empty anyway.
  strcpy(name, "");

  PTR<vint> size = getDimensions( src.row_size, // detecting the dimensions according to row-, col-, depth- and time_size.
				  src.col_size, 
				  src.depth_size, 
				  src.time_size );

  int pixel_size;
  switch ( src.data_storage_type )
  {
//   case VFF_TYP_BIT:      
//     ts = 1; 
//     break;
  case VFF_TYP_1_BYTE:   
    pixel_size = sizeof(char); 
    break;
  case VFF_TYP_2_BYTE:   
    pixel_size = sizeof(short int); 
    break;
  case VFF_TYP_4_BYTE:   
    pixel_size = sizeof(int); 
    break;
  case VFF_TYP_FLOAT:    
    pixel_size = sizeof(float); 
    break;
  case VFF_TYP_DOUBLE:   
    pixel_size = sizeof(double); 
    break;
  default: 
    error("bad data type");
  }; /* switch ( src.data_storage_type ) */
  

  this->row_size=src.row_size;
  this->col_size=src.col_size;
  this->depth_size=src.depth_size;
  this->time_size=src.time_size;
  this->num_data_bands=src.num_data_bands;
  this->data_storage_type=src.data_storage_type;
  this->xdim=src.xdim;
  this->ydim=src.ydim;
  this->zdim=src.zdim;
  this->xmin=src.xmin;
  this->xmax=src.xmax;
  this->ymin=src.ymin;
  this->ymax=src.ymax;
  this->zmin=src.zmin;
  this->zmax=src.zmax;

  int alloc_size = pixel_size * size->prod();
  
  DEBUG(alloc_size);
  DEBUG(pixel_size);
  DEBUG(size->prod());
  DEBUG(size->size());

  this->image_data=reinterpret_cast<void*>(new char[ alloc_size ]);   
                     
  memcpy( this->image_data, src.image_data, alloc_size );
};

// deep_xvimage::deep_xvimage( const vint & dim, int int_im_type ){ 
//                                          // // Carefully, the image data  
//                                          // will be created by 'ujoi::operator&'
//                                          // for functions which use xvimage from python
//   name=new char[1];  // I allocate it if anything wanted to use it. It will be empty anyway.
//   strcpy(name, "");
//   image_data=NULL;   // the image data is never allocated and therefore destroyed !
//                      // it is only used for 'ujoi::operator&'

//   int x,y,z,t;
//   setDimensions( dim, x, y, z, t );

  
//   this->row_size   = x;
//   this->col_size   = y;
//   this->depth_size = z;
//   this->time_size  = t;

//   this->num_data_bands    = 1;

//   this->data_storage_type = int_im_type; /// !!!!!!!!

//   this->xdim = -1;
//   this->ydim = -1;
//   this->zdim = -1;
//   this->xmin = -1;
//   this->xmax = -1;
//   this->ymin = -1;
//   this->ymax = -1;
//   this->zmin = -1;
//   this->zmax = -1;
// };


deep_xvimage::~deep_xvimage(){
  delete name;
  free(image_data);                           
  /////!!!!!!! cout<< "deleting deep_xvimage" << endl;
  // the rest of the properties are destroyed automaticly
};

string deep_xvimage::imtype(){
  return image_type(this->data_storage_type);
};






template<>
void write_a_pixel<float>
(
  fstream & s, 
  float & value
  ) 
{
  s.write( reinterpret_cast<char*>(&value), 4 );
} /* write_a_pixel float */

  

template<>
void write_a_pixel<unsigned char>
(
  fstream & s, 
  unsigned char & value
  ) 
{
  s << value;
} /* write_a_pixel char */








} /* namespace pink */












































// LuM end of file
