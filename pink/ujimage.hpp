 /*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2013-2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/**

 \brief This file contains the C++ image class declarations

 \ingroup development
 
 \file   ujimage.hpp

 \author UjoImro, 2009-2014

*/


// #ifdef __cplusplus // doxygen does not like it 
#ifndef __UJIMAGE_HPP
#define __UJIMAGE_HPP

#include "platform_specific.h"

#include <map>
#include <string>
#include <cstdint>
#include <fstream>
#include <climits>
#include <string.h>

#ifdef PINK_HAVE_NUMPY
//# define NPY_NO_DPRECATED_API NPY_1_7_API_VERSION
# include <Python.h>
# include <boost/cstdint.hpp>
# include <numpy/arrayobject.h>
# include <boost/smart_ptr.hpp>
# include <boost/python/object.hpp>
#endif /* PINK_HAVE_NUMPY */

#include <boost/smart_ptr.hpp>

#include "larith.h"
#include "mcimage.h"
#include "mccodimage.h"
#include "ui_pink_types.hpp"
#include "platform_specific.h"

#undef depth
#undef nbands

template <class T0>
index_t
depth( const T0 & t0 ) { return t0->depth_size; }

template <class T0>
index_t
nbands( const T0 & t0 ) { return t0->num_data_bands; }

#define _assert( cond ) {                       \
    if (!cond) pink_error( std::string("assertion failed:") + BOOST_PP_STRINGIZE(cond) ); \
  }



// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************

// COMPLEX OPERATORS in C++

// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************

bool operator==(const fcomplex & x, const fcomplex & y);
fcomplex & operator+=(fcomplex & x, const fcomplex & y);
fcomplex & operator-=(fcomplex & x, const fcomplex & y);
fcomplex & operator*=(fcomplex & x, const fcomplex & y);
fcomplex & operator*=(fcomplex & x, const float & y);
fcomplex & operator/=(fcomplex & x, const fcomplex & y);
bool operator!=(const fcomplex & x, const fcomplex & y);
fcomplex & operator+( const fcomplex & x, const fcomplex & y);
fcomplex & operator-( const fcomplex & x, const fcomplex & y);
fcomplex & operator*( const fcomplex & x, const fcomplex & y);
fcomplex & operator*( const fcomplex & x, const float & y);
fcomplex & operator*( const float & x, const fcomplex & y);
fcomplex & operator/( const fcomplex & x, const fcomplex & y);
bool operator>=( const fcomplex & x, const fcomplex & y);
bool operator<=( const fcomplex & x, const fcomplex & y);
bool operator>( const fcomplex & x, const fcomplex & y);
bool operator<( const fcomplex & x, const fcomplex & y);
std::string repr_fcomplex(const fcomplex & x);

bool operator==(const dcomplex & x, const dcomplex & y);
dcomplex & operator+=(dcomplex & x, const dcomplex & y);
dcomplex & operator-=(dcomplex & x, const dcomplex & y);
dcomplex & operator*=(dcomplex & x, const dcomplex & y);
dcomplex & operator*=(dcomplex & x, const double & y);
dcomplex & operator/=(dcomplex & x, const dcomplex & y);
bool operator!=(const dcomplex & x, const dcomplex & y);
dcomplex & operator+( const dcomplex & x, const dcomplex & y);
dcomplex & operator-( const dcomplex & x, const dcomplex & y);
dcomplex & operator*( const dcomplex & x, const dcomplex & y);
dcomplex & operator*( const dcomplex & x, const double & y);
dcomplex & operator*( const double & x, const dcomplex & y);
dcomplex & operator/( const dcomplex & x, const dcomplex & y);
bool operator>=( const dcomplex & x, const dcomplex & y);
bool operator<=( const dcomplex & x, const dcomplex & y);
bool operator>( const dcomplex & x, const dcomplex & y);
bool operator<( const dcomplex & x, const dcomplex & y);
std::string repr_dcomplex(const dcomplex & x);

namespace pink
{

  template <class T0>
  void
  do_not_delete( T0 * t0 ) {
  // std::cout << "NOT deleting (do_not_delete)" << std::endl;
    // intentionally left empty
  }

  template <class pixel_t> class image;

  namespace {
    /**
       \brief This function returns the pink type id of a pixel_type.
     
       description The input is a pixel type, like (float) and the
       output is the corresponding pink id (VFF_TYP_FLOAT)

       \param none
       \return The pink id of the type
    */
    template <class T0>
    struct pinktype {
      operator int32_t () {
        pink_error("Invalid pixel type in image type (9493)");
        return -1;
      }
    }; // struct type

    template <> struct pinktype<uint8_t> { operator int32_t () { return VFF_TYP_1_BYTE; } };
    template <> struct pinktype<image<uint8_t> > { operator int32_t () { return VFF_TYP_1_BYTE; } };
    template <> struct pinktype<int16_t> { operator int32_t () { return VFF_TYP_2_BYTE; } }; 
    template <> struct pinktype<image<int16_t> > { operator int32_t () { return VFF_TYP_2_BYTE; } }; 
    template <> struct pinktype<int32_t> { operator int32_t () { return VFF_TYP_4_BYTE; } }; 
    template <> struct pinktype<image<int32_t> > { operator int32_t () { return VFF_TYP_4_BYTE; } }; 
    template <> struct pinktype<float> { operator int32_t () { return VFF_TYP_FLOAT; } }; 
    template <> struct pinktype<image<float> > { operator int32_t () { return VFF_TYP_FLOAT; } }; 
    template <> struct pinktype<double> { operator int32_t () { return VFF_TYP_DOUBLE; } }; 
    template <> struct pinktype<image<double> > { operator int32_t () { return VFF_TYP_DOUBLE; } }; 
    template <> struct pinktype<fcomplex> { operator int32_t () { return VFF_TYP_COMPLEX; } }; 
    template <> struct pinktype<image<fcomplex> > { operator int32_t () { return VFF_TYP_COMPLEX; } }; 
    template <> struct pinktype<dcomplex> { operator int32_t () { return VFF_TYP_DCOMPLEX; } }; 
    template <> struct pinktype<image<dcomplex> > { operator int32_t () { return VFF_TYP_DCOMPLEX; } };
  } // unnamed namespace
  

  size_t
  typesize( const index_t & type );
  
  int32_t numpy2pink_type ( const int32_t & pink_type );
  
  int32_t pink2numpy_type ( const int32_t & numpy_type ); 

  template <class T0>
  void
  arraydelete( void * array ) {
    // std::cout << "deleting the array (arraydelete)" << std::endl;    
    delete [] reinterpret_cast<T0*>(array);
  }

  void cstylefree( void * ptr );
    
  class deleter_t {
  private:
    boost::shared_ptr<bool> m_enabled;
    typedef void (*m_deleter_t) (void*);
    m_deleter_t m_deleter;
            
  public:
    deleter_t( boost::shared_ptr<bool> & enabled,
               void (*deleter) (void*) )
      : m_enabled(enabled),
        m_deleter(deleter) { }

    void
    operator() ( void * ptr ) {
      if (*m_enabled) {
        // std::cout << "deleter enabled for (" << ptr << "); deleting" << std::endl;
        m_deleter(ptr);
      }
      else
      {        
        // std::cout << "deleter disabled for (" << ptr << "); NOT deleting" << std::endl;
      }      
      
      return;
    }

    m_deleter_t
    get_deleter() {
      return m_deleter;
    }
    
  }; // deleter_t
      
  class cxvimage {
  public:
    typedef std::vector<index_t> vint_t;

  protected:

    boost::shared_ptr<bool>    m_garbage_collection; // enable or disable garbage collection
    boost::shared_ptr<xvimage> m_xvimage; // the xvimage header
    boost::shared_ptr<char>    m_data;    // the image data
    vint_t                     m_size;    // the size of the image

    void disable_garbage_collection() {
      *m_garbage_collection = false;
      return;
    }
    
    void enable_garbage_collection() {
      *m_garbage_collection = true;
      return;
    }
    
  public:

    template <class pixel_t>
    pixel_t * pdata() {
      _assert(!this->isnull());      
      return reinterpret_cast<pixel_t*>(m_data.get());
    }

    template <class pixel_t>
    const pixel_t * pdata() const {
      _assert(!this->isnull());      
      return reinterpret_cast<const pixel_t*>(m_data.get());
    }

    const index_t & rows() const {
      _assert(!this->isnull());
      return m_xvimage->row_size;
    }

    const index_t & cols() const {
      _assert(!this->isnull());
      return m_xvimage->col_size;
    }

    const index_t & depth() const {
      _assert(!this->isnull());
      return m_xvimage->depth_size;
    }

    int32_t imtype() const {
      _assert(!this->isnull());
      return m_xvimage->data_storage_type;
    }

    const index_t & nbands() const {
      _assert(!this->isnull());
      return m_xvimage->num_data_bands;
    }

    const double & xdim () const {
      _assert(!this->isnull());
      return m_xvimage->xdim;
    }

    double & xdim () {
      _assert(!this->isnull());
      return m_xvimage->xdim;
    }

    const double & ydim () const {
      _assert(!this->isnull());
      return m_xvimage->ydim;
    }

    double & ydim () {
      _assert(!this->isnull());
      return m_xvimage->ydim;
    }

    const double & zdim () const {
      _assert(!this->isnull());
      return m_xvimage->zdim;
    }

    double & zdim () {
      _assert(!this->isnull());
      return m_xvimage->zdim;
    }

    const vint_t & size() const {
      _assert(!this->isnull());      
      return m_size;
    }

    cxvimage( const int32_t & type, const index_t & rs, const index_t & cs = 1, const index_t & ds = 1 )
      : m_garbage_collection( new bool(true) )  {
      // std::cout << "cxvimage (" << this << ") created (type, rs, cs, ds)" << std::endl;
      _assert(rs>0);
      _assert(cs>=1);
      _assert(ds>=1);
      xvimage * tmp = allocimage( NULL, rs, cs, ds, type );
      if (!tmp) pink_error("Couldn't allocate the image 012");
      m_xvimage.reset( tmp, freeheader );
      m_data.reset( reinterpret_cast<char*>(tmp->image_data), deleter_t( m_garbage_collection, cstylefree ));

      m_size = { rs, cs, ds };
      if (m_size[2]==1) m_size.pop_back();
      if (m_size[1]==1) m_size.pop_back();

      _assert(m_size[0]>0);
      
      return;
    }

    cxvimage( const int32_t & type, vint_t size, boost::shared_ptr<char> data )
      : m_garbage_collection( new bool(false) )  {
      // std::cout << "cxvimage (" << this << ") created (type, size, data)" << std::endl;

      _assert(size.size()>0);      
      _assert(size[0]>0);
      
      m_size = size;
      
      while (size.size()<3) size.push_back(1);
      xvimage * tmp = allocheader( NULL, size[1], size[0], size[2], type );

      if (!tmp) pink_error("Couldn't allocate the image 014");
      tmp->image_data = reinterpret_cast<void*>(data.get());      
      m_xvimage.reset( tmp, freeheader );
      m_data = data;
      m_xvimage->image_data = m_data.get();
      
      return;
    }

    cxvimage( const int32_t & type, vint_t size, char * data )
      : m_garbage_collection( new bool(false) ) {
      // std::cout << "cxvimage (" << this << ") created (type, size, data*)" << std::endl;
            
      _assert(size[0]>0);
      
      m_size = size;
      
      while (size.size()<3) size.push_back(1);
      xvimage * tmp = allocheader( NULL, size[1], size[0], size[2], type );
 
      if (!tmp) pink_error("Couldn't allocate the image 014");
      m_xvimage.reset( tmp, freeheader );
      m_data.reset( const_cast<char*>(data), do_not_delete<char> );
      *m_garbage_collection = false;
      m_xvimage->image_data = m_data.get();
      
      return;      
    }

    cxvimage( const int32_t & type, vint_t size )
      : m_garbage_collection( new bool(true) )  {
      // std::cout << "cxvimage (" << this << ") created (type, size)" << std::endl;
      _assert(size[0]>0);
      m_size = size;
      while (size.size()<3) size.push_back(1);
      xvimage * tmp = allocheader( NULL, size[1], size[0], size[2], type );

      if (!tmp) pink_error("Couldn't allocate the image 016");

      m_xvimage.reset( tmp, freeheader );
      char * data = new char[ pink::typesize(type) * pink::prod(m_size) ];
      _assert(data);

      m_data.reset( data, deleter_t(m_garbage_collection, arraydelete<char> ) );

      m_xvimage->image_data = m_data.get();
      return;
    }

    cxvimage( )
      : m_garbage_collection( new bool(false) )  {
      // std::cout << "cxvimage (" << this << ") created (empty)" << std::endl;
    }

    cxvimage & 
    operator = ( const cxvimage & other ) {
      // std::cout << "cxvimage::operator=(cxvimage&) called on " << this << std::endl;
      
      m_data               = other.m_data;
      m_size               = other.m_size;
      m_xvimage            = other.m_xvimage;
      m_garbage_collection = other.m_garbage_collection;

      return *this;
    } // operator = 

    cxvimage & 
    operator = ( cxvimage && other ) {
      // std::cout << "cxvimage::operator =(cxvimage&&, MOVE) called on " << this << std::endl;
      
      m_data               = std::move(other.m_data);
      m_size               = std::move(other.m_size);
      m_xvimage            = std::move(other.m_xvimage);
      m_garbage_collection = std::move(other.m_garbage_collection);

      return *this;
    } // operator = 
    
    
    cxvimage( const cxvimage & other )
      : m_xvimage(other.m_xvimage), m_data(other.m_data), m_size(other.m_size), m_garbage_collection(other.m_garbage_collection) {
      // std::cout << "cxvimage (" << this << ") copied ( const xvimage& )" << std::endl;

      return;      
    } // cxvimage &

    cxvimage( cxvimage && other )
      : m_xvimage(other.m_xvimage), m_data(other.m_data),  m_size(other.m_size), m_garbage_collection(other.m_garbage_collection) {
      // std::cout << "cxvimage (" << this << ") moved ( xvimage&& )" << std::endl;

      return;      
    } // cxvimage && 

    cxvimage( xvimage * other ) {
      // std::cout << "cxvimage (" << this << ") created ( xvimage* )" << std::endl;
      
      if (!other) pink_error("Attempted to create a new image from a NULL pointer. (934)");
      
      m_garbage_collection.reset( new bool(true) );        
      m_xvimage.reset( other, freeheader );        
      m_data.reset( reinterpret_cast<char*>(other->image_data), deleter_t(m_garbage_collection, cstylefree) );
      
      m_size = { other->col_size, other->row_size, other->depth_size, other->time_size };
      if (m_size[3]==1) m_size.pop_back();
      if (m_size[2]==1) m_size.pop_back();
      if (m_size[1]==1) m_size.pop_back();
      
      _assert(m_size[0]>0);
      
      return;
    } // cxvimage

    cxvimage( const boost::python::object & array )
      : m_garbage_collection( new bool(false) ) {
      // std::cout << "cxvimage (" << this << ") created ( const boost::python::object &  )" << std::endl;
      
      if (! PyArray_Check(array.ptr())) // verifying that the object is indeed a numpy array
        pink_error("This function can only convert numpy arrays");
      PyArrayObject* original_array = reinterpret_cast<PyArrayObject*>(array.ptr());
      PyArrayObject* tmparray = PyArray_GETCONTIGUOUS(original_array); // making sure that the object is continuous; // if it is continuous than no copy is done here
      if (!PyArray_ISCONTIGUOUS(tmparray)) // the array is not continuous
        pink_error("Internal error, the array should be continuous by now.");
      npy_intp nd = PyArray_NDIM(tmparray);

      npy_intp * dims = PyArray_DIMS(tmparray);
      m_size.resize(nd);
      if (nd>3) pink_error("More than 3D numpy array, not implemented!");
      for (index_t q=0; q<nd; q++) m_size[q]=dims[q];
      index_t type = PyArray_TYPE(original_array);
      index_t rs = 1;
      index_t cs = 1;
      index_t ds = 1;      
      if (nd>0) rs = m_size[1];
      if (nd>1) cs = m_size[0];
      if (nd>2) ds = m_size[2];
      
      xvimage * tmp = allocheader( NULL, rs, cs, ds, numpy2pink_type(type) );

      if (!tmp) pink_error("Couldn't allocate the image 015");

      char * data = PyArray_BYTES(tmparray);
      m_data.reset( data, do_not_delete<char> );
      *m_garbage_collection = false;
      m_xvimage.reset(tmp, freeheader);
      m_xvimage->image_data = PyArray_DATA(tmparray);
      return;
    }
    
    void
    reset(
      const int32_t & type,
      const index_t & rs,
      const index_t & cs = 1,
      const index_t & ds = 1
       ) {
      cxvimage tmp( type, rs, cs, ds );
      (*this)=tmp;

      return;      
    } // reset
    
    void
    reset( const int32_t & type, const vint_t & size ) {
      cxvimage tmp( type, size );
      (*this)=tmp;

      return;      
    } // reset

    cxvimage
    clone () const {
      // std::cout << "cxvimage::clone" << std::endl;      
      _assert(!this->isnull());

      cxvimage result( imtype(), size() );
        
      std::copy( this->m_data.get(),
                 this->m_data.get() + typesize(imtype()) * pink::prod(size()),
                 result.m_data.get()
        );
      result.m_xvimage->row_size          = this->m_xvimage->row_size;
      result.m_xvimage->col_size          = this->m_xvimage->col_size;
      result.m_xvimage->depth_size        = this->m_xvimage->depth_size;
      result.m_xvimage->time_size         = this->m_xvimage->time_size;
      result.m_xvimage->num_data_bands    = this->m_xvimage->num_data_bands;
      result.m_xvimage->data_storage_type = this->m_xvimage->data_storage_type;
      result.m_xvimage->xdim = this->m_xvimage->xdim;
      result.m_xvimage->ydim = this->m_xvimage->ydim;
      result.m_xvimage->zdim = this->m_xvimage->zdim;
      result.m_xvimage->xmin = this->m_xvimage->xmin;
      result.m_xvimage->xmax = this->m_xvimage->xmax;
      result.m_xvimage->ymin = this->m_xvimage->ymin;
      result.m_xvimage->ymax = this->m_xvimage->ymax;
      result.m_xvimage->zmin = this->m_xvimage->zmin;
      result.m_xvimage->zmax = this->m_xvimage->zmax;

      return result;
    };

    bool
    operator == ( const cxvimage & other ) const {
      return lequal( *this, other );
    } // ==
        
    virtual ~cxvimage () {
      // std::cout << "~cxvimage (" << this << ") called" << std::endl;
    }

    virtual operator xvimage* () {
      return m_xvimage.get();      
    }

    virtual operator const xvimage* () const {
      if (isnull()) pink_error("The image is uninitialized! (839)");
      return m_xvimage.get();      
    }
    
    bool isnull() const {
      return (!m_data);      
    }

    boost::python::object
    steel() {
      if (! *m_garbage_collection) pink_error("The image cannot be used in Python, because it is already exported! 399");

      disable_garbage_collection();

      index_t nd = size().size();
      _assert(nd>=2);
      
      boost::scoped_array<npy_intp> dims( new npy_intp[nd] );
      dims[0] = size()[1];
      dims[1] = size()[0];      
      
      for ( index_t q=2; q<nd; ++q ) dims[q] = size()[q];

      PyObject * numpy_array =
        PyArray_SimpleNewFromData (
          nd,
          dims.get(),
          pink2numpy_type(imtype()),
          pdata<void>()
          );

      deleter_t * deleter = boost::get_deleter<deleter_t, char>(m_data);
      _assert(deleter);

      PyObject * data = PyCObject_FromVoidPtr( m_data.get(), deleter->get_deleter() );
      _assert(data);

      reinterpret_cast<PyArrayObject*>(numpy_array)->base = data;
      // making boost handle the array as a smart pointer
      boost::python::handle<> handle(numpy_array);
      boost::python::object result(handle);
      return result;
    } // steel

  }; // class cxvimage
  
  template <class pixel_t>  class image_iterator;
  template <class pixel_t>  class const_image_iterator;
  template <class pixel_t>  class noconst_image_iterator;
  
  template <class pixel_t>
  class image : public cxvimage {

  public:

    typedef pixel_t pixel_type;    
    
    image( ) { }
        
    image( const index_t & rs, const index_t & cs=1, const index_t & ds=1 )
      : cxvimage( pinktype<pixel_t>(), rs, cs, ds ) { }

    image( const std::vector<index_t> & dim ) : cxvimage( pinktype<pixel_t>(), dim ) { }

    image( const std::vector<index_t> & dim, boost::shared_ptr<pixel_t> data ) { pink_error("IMPLEMENT ME!!!"); }

    image( const std::vector<index_t> & dim, pixel_t * data )
      :  cxvimage( pinktype<pixel_t>(), dim, reinterpret_cast<char*>(data)  ) {
      // std::cout << "image( vector, pixel_t* ) called" << std::endl;      
    }

    image( const cxvimage & other ) {
      if ( pinktype<pixel_t>() != other.imtype() )
        pink_error("invalid conversion (7338)");
      
      this->operator=(other);
    }

    image( const image<pixel_t> & other ) : cxvimage(other) {
      // here comes the cloning of not-inherited variables
      // intentionally left empty
    }

    image( boost::python::object object ) : cxvimage(object) {
      // here comes the cloning of not-inherited variables
      // intentionally left empty
    }
        
    pixel_t &
    operator() ( const index_t & x ) {
      _assert(!this->isnull());
      return this->pdata<pixel_t>()[x];
    }

    const pixel_t &
    operator() ( const index_t & x ) const {
      _assert(!this->isnull());
      return this->pdata<pixel_t>()[x];
    }

    pixel_t &
    operator() ( const index_t & x, const index_t & y ) {
      _assert(size().size()==2);
      return this->pdata<pixel_t>()[y*rows() + x];
    }

    const pixel_t &
    operator() ( const index_t & x, const index_t & y ) const {
      _assert(size().size()==2);      
      return this->pdata<pixel_t>()[y*rows() + x];
    }

    pixel_t &
    operator() ( const index_t & x, const index_t & y, const index_t & z ) {
      _assert(size().size()==3);
      return this->pdata<pixel_t>()[((z)*cols()+(y))*rows()+(x)];
    }

    const pixel_t &
    operator() ( const index_t & x, const index_t & y, const index_t & z ) const {
      _assert(size().size()==3);
      return this->pdata<pixel_t>()[((z)*cols()+(y))*rows()+(x)];
    }

    const pixel_t &
    operator() ( std::vector<index_t> & pos ) const {
      return this->pdata<pixel_t>()[position( size(), pos )];
    }

    pixel_t &
    operator() ( std::vector<index_t> & pos ) {
      return this->pdata<pixel_t>()[position( size(), pos )];
    }

    noconst_image_iterator<pixel_t> begin() { return noconst_image_iterator<pixel_t>( 0, this );  }
    noconst_image_iterator<pixel_t> end() { return noconst_image_iterator<pixel_t>( prod(size()), this ); }
    const_image_iterator<pixel_t> begin() const { return const_image_iterator<pixel_t>( 0, this ); }
    const_image_iterator<pixel_t> end() const { return const_image_iterator<pixel_t>( prod(size()), this ); }

    image<pixel_t> clone () const {
      // std::cout << "image::<pixel_t>::clone start" << std::endl;      
      image<pixel_t> result;

      // clone the variables of the base class
      result = cxvimage::clone();

      // clone the variables of this (image) class
      // no variables yet

      return result;
    }

    pixel_t * get() { return this->pdata<pixel_t>(); }

    const pixel_t * get() const { return this->pdata<pixel_t>(); }

    operator xvimage* () override {
      if (isnull()) pink_error("The image is uninitialized! (840)");
      return m_xvimage.get();      
    }

    image<pixel_t> & operator = ( const cxvimage & other ) {
      // std::cout << "operator image<pixel_t>::operator= called" << std::endl;      
      // get the references for the variables of the base
      _assert( pinktype<pixel_t>() == other.imtype());      
      cxvimage::operator=(other);

      // get the references for our (image) variables ()
      // intentionally left empty

      return *this;      
    }
    
    image<pixel_t> & operator = ( const image<pixel_t> & other ) {
      // get the references for the variables of the base
      cxvimage::operator=(other);

      // get the references for our (image) variables ()
      // intentionally left empty

      return *this;            
    }
    
    image<pixel_t> & operator = ( image<pixel_t> && other ) {
      // std::cout << "image<pixel_t>::operator= image<pixel_t> && other" << std::endl;

      // moving the variables of the base class
      cxvimage::operator=(other);

      // moving the variables of the image class
      // intentionally left empty
      
      return *this;            
    }
        
    operator const xvimage* () const override {
      if (isnull()) pink_error("The image is uninitialized! (841)");
      return m_xvimage.get();      
    }

    void
    reset( const vint_t & size ) {
      image<pixel_t> tmp(size);
      (*this)=tmp;
      
      return;      
    } // reset

    void
    reset( const index_t & rs, const index_t & cs=0, const index_t & ds=0 ) {
      image<pixel_t> tmp(rs, cs, ds);
      (*this)=tmp;
      
      return;      
    } // reset

    void swap( image<pixel_t> & other ) { pink_error("IMPLEMENT ME!!!"); }
    
  }; // class image

  
  template <class pixel_t>
  class noconst_image_iterator
    : public std::iterator<std::bidirectional_iterator_tag, index_t >{

  private:
    index_t m_pos;
    image<pixel_t> * pm_object;

  public:

    noconst_image_iterator() : m_pos(-1), pm_object(NULL) { }    

    noconst_image_iterator( const index_t & pos, image<pixel_t> * p_object )
      : m_pos(pos), pm_object(p_object)  { }
    
    pixel_t & operator * () { return (*pm_object)(m_pos); } // operator *

    noconst_image_iterator<pixel_t> & operator ++ () { ++m_pos; return *this; } // ++I

    noconst_image_iterator<pixel_t> operator ++ (int) { noconst_image_iterator result(*this); m_pos++; return result; } // I++

    noconst_image_iterator<pixel_t> & operator -- () { --m_pos; return *this; } // --I

    noconst_image_iterator<pixel_t> operator -- (int) { noconst_image_iterator result(*this); m_pos--; return result;  } // I--

    bool operator == ( noconst_image_iterator const& rhs ) const { return (rhs.m_pos == m_pos); }
        
    bool operator!=( noconst_image_iterator const& rhs) const { return !(*this==rhs); }

    const index_t & row () const { pink_error("IMPLEMENT ME!!!"); }
    const index_t & col () const { pink_error("IMPLEMENT ME!!!"); }
    const index_t & depth () const { pink_error("IMPLEMENT ME!!!"); }
    const index_t & pos () const { pink_error("IMPLEMENT ME!!!"); }
    const std::vector<index_t> & vpos () const { pink_error("IMPLEMENT ME!!!"); }
    
  }; // class const_image_iterator

  template <class pixel_t>
  class const_image_iterator
    : public std::iterator<std::bidirectional_iterator_tag, index_t >{

  private:
    index_t m_pos;
    const image<pixel_t> * pm_object;

  public:

    const_image_iterator() : m_pos(-1), pm_object(NULL) { }    

    const_image_iterator( const index_t & pos, const image<pixel_t> * p_object )
      : m_pos(pos), pm_object(p_object)  { }
    
    const pixel_t & operator * () { return (*pm_object)(m_pos); } // operator *

    const_image_iterator<pixel_t> & operator ++ () { ++m_pos; return *this; } // ++I

    const_image_iterator<pixel_t> operator ++ (int) { const_image_iterator result(*this); m_pos++; return result; } // I++

    const_image_iterator<pixel_t> & operator -- () { --m_pos; return *this; } // --I

    const_image_iterator<pixel_t> operator -- (int) { const_image_iterator result(*this); m_pos--; return result;  } // I--

    bool operator == ( const_image_iterator<pixel_t> const& rhs ) const { return (rhs.m_pos == m_pos); }
        
    bool operator!=( const_image_iterator<pixel_t> const& rhs) const { return !(*this==rhs); }

    index_t row () const { pink_error("IMPLEMENT ME!!!"); }
    index_t col () const { pink_error("IMPLEMENT ME!!!"); }
    index_t depth () const { pink_error("IMPLEMENT ME!!!"); }
    
  }; // class const_image_iterator

    
  
  /**
     \brief The char_image specialization.
  */
  typedef image<uint8_t>   char_image;

  /**
  \brief The short_image specialization.
  */
  typedef image<int16_t>  short_image;

  /**
  \brief The int_image specialization.
  */
  typedef image<int32_t> int_image;

  /**
  \brief The float_image specialization.
  */
  typedef image<float> float_image;

  /**
  \brief The double_image specialization.
  */
  typedef image<double> double_image;

    /**
  \brief The double_image specialization.
  */
  typedef image<fcomplex> fcomplex_image;

    /**
  \brief The double_image specialization.
  */
  typedef image<dcomplex> dcomplex_image;

    
//   class deep_xvimage;  
  
// //  string image_type( int pixel_type ); // returns the corresponding image types

//   template <class pixel_type>
//   class image_type_specific
//   {
//   public:
    
// //#error: this image type is not defined
//     std::string imtype() const 
//       {         			      
// 	pink_error("unimplemented image type specialization");
//       }
//     index_t int_pixel_type() const 
//       {
//         pink_error("unimplemented image type specialization");
//       }
    
//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         pink_error("unimplemented image type specialization");
//       }
    
//     pixel_type add( pixel_type a, pixel_type b)
//       {
//         pink_error("unimplemented image type specialization");
//       }

//     void init_pixel( pixel_type & a )
//       {
//         pink_error("unimplemented image type specialization");
//       }
  
    
//   }; /* class image_type specific */
  
  
  
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
  
  									
  // /**
  //    helper function for reading and writing from and to xvimage
  // */
  // boost::shared_ptr<types::vint> getDimensions( const index_t x, const index_t y, const index_t z, const index_t t );
  
  // void setDimensions(const types::vint & dim, index_t & x, index_t & y, index_t & z, index_t & t);

  // /**
  //    This function is a wrap around readimage. It returns a C++ class
  //    which is then converted into ujoi<>
  //  */
  // boost::shared_ptr<deep_xvimage> py_readimage( std::string filename );

  // std::string image_type_string( index_t pixel_type );


  // /**
  //   This class will copy it's content when upcasted (constructed from xvimage).
  //   It is used, when the data is needed (in readimage for example)
  //   \note image name is fake
  // */
  // class deep_xvimage
  // {
  // private:
  //   xvimage * data;    
  // public:
  //   deep_xvimage(); // default constructor
  //   deep_xvimage( const xvimage & src ); // upcast constructor
  //   deep_xvimage( const deep_xvimage & src ); // copy constructor. RAISES AND ERROR!
  //                                             // it's not yet implemented, raises an
  //                                             // error if called implicitly
  //   virtual ~deep_xvimage(); // default destructor
  //   std::string imtype(); // returns the image type
  // };
  

  // // this class will only copy the properties and set up a fake image name
  // // it is used in ujoi, to generate fast an 'struct xvimage' for PInK's C functions
  // class shallow_xvimage: public xvimage
  // {
  // public:
  //   shallow_xvimage(); // default constructor
  //   shallow_xvimage( const xvimage & src ); // upcast shallow copy constructor. It does not 
  //                                           // copy the data
  //   shallow_xvimage( const shallow_xvimage & src ); // copy constructor. RAISES AND ERROR!
  //                                                   // it's not yet implemented, raises an
  //                                                   // error if called implicitly
  //   shallow_xvimage( const types::vint & dim, index_t int_pixel_type );  // construct from dimension. The data 
  //   // type must be specified. See mcimage.h
  //   virtual ~shallow_xvimage(); // default destructor
  //   std::string imtype(); // returns the image type
  // }; /* xvImage: xvimage */
 


//   /**
//   \brief This structure frees the images, that have been allocated by
//   readimage.

//   description As readimage is a C function, it uses malloc to allocate
//   it's images. The images can not be pre-allocated as their type and
//   size is not known at the time readimage is called. This object is
//   used to 'free' the memory when ujoi is created from an xvimage. Note
//   that the data is not copyed, so the xvimage structure should not be
//   automatically freed.
//   */
//   struct liberator_t
//   {
    
//     template<class PT0>
//     void operator()( PT0* & p )
//       {
// #       if UJIMAGE_DEBUG >= 2
//         std::cout << "liberator_t called on " << p << std::endl;
// #       endif /* UJIMAGE_DEBUG >= 2 */
//         free(reinterpret_cast<void*>(p));
//         p = NULL;        
//       }
  
//   }; /* struct liberator_t */



  // /**
  // \brief This class forms the base of all image classes.
  
  // It is used in template metaprogramming. As all the image types are
  // derived from this type, you can test for an arbitrary type if it's
  // an image. See \ref pink::python::convert_if .
  // */
  // class pink_image
  // {
  // public:
  //   virtual operator xvimage*()=0;
  //   //virtual operator const xvimage*() const = 0;
  // }; /* class pink_image */


  // template<class image_type, class pixel_type>
  // struct ujimage_operators
  // {
    
  //   friend bool operator!=(const image_type & x, const image_type & y)
  //   {
  //     return !( x==y );      
  //   }

  //   friend image_type operator+( const image_type & x, const image_type & y)
  //     {
  //       return x.clone()+=y;        
  //     }

  //   friend image_type operator-( const image_type & x, const image_type & y)
  //     {
  //       return x.clone()-=y;        
  //     }

  //   friend image_type operator+( const image_type & x, pixel_type val)
  //     {
  //       return x.clone()+=val;        
  //     }

  //   friend image_type operator+( pixel_type val, const image_type & x)
  //     {
  //       return x.clone()+=val;        
  //     }

  //   friend image_type operator*( const image_type & x, pixel_type val)
  //     {
  //       return x.clone()*=val;        
  //     }
    
  //   friend image_type operator*( pixel_type val, const image_type & x)
  //     {
  //       return x.clone()*=val;        
  //     }


  //   friend image_type operator-( const image_type & x, pixel_type val)
  //     {
  //       return x.clone()-=val;        
  //     }

  //   friend image_type operator/( const image_type & x, pixel_type val)
  //     {
  //       return x.clone()/=val;        
  //     }

    
  // }; /* struct ujimage_operators */

  // template<>
  // bool operator!=( )
  
  
//   /**
//      \brief This class represents the image types used in Pink-C++ and Pink-Python.

//      \tparam pixel_type_ can be 'char', 'short', 'int', 'float' and 'double'
//      while int_pixel_type is the type's encoding into integer. See 'mccodimage.h'     
//   */
//   template <class pixel_type_> 
//   class ujoi:
//     public pink_image,
//     public image_type_specific<pixel_type_>,
//     public ujimage_operators< ujoi<pixel_type_>, pixel_type_ >
//   {
//   public:
//     typedef pixel_type_ pixel_type;
//     typedef ujoi<pixel_type> image_type;
        
//   private:

//     boost::shared_ptr<types::vint> size;
//     boost::shared_ptr<types::vint> center;
//     boost::shared_array<pixel_type> pixels;
//     boost::shared_ptr<shallow_xvimage> old_school;

// #   ifdef UJIMAGE_DEBUG
//     std::string debug; // representing the name of the object if debugged
// #   endif /* UJIMAGE_DEBUG */
 
//   public:

//     /**
//       Creates an empty image, used in uiSqhool for determining the image type.
//     */
//     ujoi( ); 

//     /**
//     \brief Reads an empty image from a file
//     \note You have to know the image type prior to reading.

//     \param filename The image file
//     \param debug Holds an optional name for debug purposes. Never
//     really catched on.
//     */    
//     ujoi( std::string filename, std::string debug="" );

//     /**
//     \brief Embedding constructor. 
//     Takes 'xvimage' DOES NOT make a copy. It is embedded 
//     supposing that xvimage was created with malloc/free.
//     \param src Source object.
//     \param debug Holds an optional name for debug purposes. Never
//     really catched on.
//     */    
//     ujoi( xvimage * src, std::string debug="" ); 

//     /**
//     \brief Deep constructor. 

//     Takes 'xvimage' and makes a copy.
    
//     \param debug Holds an optional name for debug purposes. Never
//     really catched on.
//     */    
//     ujoi( const xvimage & src, std::string debug="" ); 

//     /**
//     \brief SHALLOW! copy_constructor. 
//     \note For conversion use convert2float
//     \param src Source object
//     \param debug Holds an optional name for debug purposes. Never
//     really catched on.
//     */    
//     ujoi( const ujoi< pixel_type > & src, std::string debug="" ); 

//     /**
//     \brief SHALLOW! copy constructor

//     This operator takes an object, and copies it's data pointers. If
//     you create an object in a function it assures the return of the
//     object without copying it's data. 

//     Example:
//     \code
//     char_image function( ... )
//     {
//       types::vint size(3);
//       size << 1000, 1000, 1000;
//       char_image result(size);
      
//       return result;
//     }
//     \endcode

//     Normally at the end of this function the compiler calls the copy
//     constructor. Here, however as the data is held in smart pointers,
//     only the pointers will be copied. For this reason the above
//     example returns the 'result' in constant time.

//     \param other The object to copy
//     \return It supports multiple attribution
//     */    
//     image_type operator=( const image_type & other );     

//     /**
//       runtime shallow copy
//     */
//     void reset( image_type & other );         

    
//     /**
//     \brief DEEP copy
//     This method is used, if you need a runtime deep-copy of the
//     object. The object creates a new object which is a deep copy of itself.
//     \return The cloned image
//     */    
//     image_type clone(void) const;


//     /**
//        \brief Content swap
//        This method swaps the content of two images. The swap happens
//        in constant time. The swap is achieved by exchanging the
//        pointers guarding the content of the image. Used for
//        iterations.
//     */
//     image_type swap( image_type other );
    
    
//     /**
//     \brief Creates an empty image of the given size. 
//     This constructor is mostly used from python, as it takes a python
//     vector. 

//     \param dim The dimensions of the image
//     \param debug
//     */
// #   ifdef PINK_HAVE_PYTHON
//     ujoi( const boost::python::list & dim, std::string debug="" );
// #   endif /* PINK_HAVE_PYTHON */



//     /**
//     \brief Creates an empty image of the given size. 

//     \param dim The dimensions of the image
//     \param debug
//     */        
//     ujoi( const types::vint & dim, std::string debug="" );


//     /**
//     The default destructor
//     */
//     virtual ~ujoi(); // default destructor

//     /**
//       Used to construct from ujif.
//     */
//     ujoi( const types::vint & dim, boost::shared_array<pixel_type> data, std::string debug="" );


//     /**
//        Creates a deep copy from a BLAS-style array. The pointer, and the
//        dimensions are specified.
//      */
//     ujoi( const types::vint & dim, pixel_type * data, std::string debug="" );
    
    
//     /**
//     \brief Index access to the pixels
//     With this operator you can access the pixels, as the image was a
//     vector; regardless of its dimensions.
//     \param pos The number of the pixel in the linear array.
//     \return Reference to the pixel.

//     Example:
//     \code
//     types::vint size(3);
//     size << 10,20,30;
//     char_image I(size);
//     std::cout << I[100];
//     I[200]=3;
//     \endcode
//     */    
//     pixel_type & operator[]( index_t pos );
//     const pixel_type & operator[]( index_t pos ) const;

//     /**
//     \brief Same as operator[]. Included to avoid ambiguity.
//     */    
//     pixel_type & operator()( index_t pos );
//     const pixel_type & operator()( index_t pos ) const;

//     /**
//     \brief 'types::vint' access to the pixels
//     With this operator you can access the pixels, by specifying the
//     position of the pixel with a 'types::vint' vector. 
//     \param pos A vector pointing somewhere in the image. 
//     \return Reference to the pixel.
    
//     \note The method tests if the vector is inside the image in DEBUG
//     mode. In RELEASE mode, for performance reasons it accepts
//     anything. If the code fail in the line where you access the image
//     elements, you should consider recompiling in debug mode.

//     Example:
//     \code
//     types::vint size(3);
//     size << 10,20,30;

//     types::vint position(3);
//     position << 3,4,2;

//     char_image I(size);
//     std::cout << I[position];
//     position << 4,4,4;
//     I[position]=3;
//     \endcode
//     */    
//     pixel_type & operator[]( const types::vint & pos ); // types::vint acces to the elements
//     const pixel_type & operator[]( const types::vint & pos ) const;  // const types::vint acces to the elements

//     /**
//     \brief Python vector access to the pixels.

//     This method let's you access the pixels using a Python vector. It
//     is mainly used from python 
//     Example (in the Python interpreter):
//     \code
//     I=pink.cpp.char_image([10,20,30])
//     print I[[0,3,2]]
//     \endcode

//     \param pos The vector with the position. 

//     \note The position is always checked, so the vector must point
//     somewhere in the image. 

//     \return A reference to the pixel
//     */
// #   ifdef PINK_HAVE_PYTHON
//     pixel_type & operator[]( const boost::python::list & pos ); // python list acces to the elements
//     const pixel_type & operator[]( const boost::python::list & pos ) const; // const python list acces to the elements
// #   endif /* PINK_HAVE_PYTHON */

//     // these methods are the operators renamed so they could be wrapped by boost
//     const pixel_type & get_operator_int( index_t pos ) const;
//     void set_operator_int( index_t pos, const pixel_type & value );

//     const pixel_type & get_operator_vint( const types::vint& pos ) const;
//     void set_operator_vint( const types::vint & pos, const pixel_type & value );

// #   ifdef PINK_HAVE_PYTHON
//     const pixel_type & get_operator_list( const boost::python::list & pos ) const;
//     void set_operator_list( const boost::python::list & pos, const pixel_type & value );
// #   endif /* PINK_HAVE_PYTHON */
    
//     void _writeimage( const std::string & filename ) const; // exports the image into a pgm file
//     void _write_amira( const std::string & filename ) const; // exports the image into an amira mesh (.am) file

// #   ifdef PINK_HAVE_PYTHON
//     PyObject * get_pixels_python();
// #   endif /* PINK_HAVE_PYTHON */    
//     // cast operators

//     /**
//     \brief Cast operator. 
//     Casts the ujoi<> object to 'xvimage*' in constant time.
//     */    
//     xvimage* get_output(); // this method is not exported to python. It could not even be, as boost doesn't support pointers.    

//     /**
//     \brief Cast operator. 
//     Casts the ujoi<> object to 'xvimage*' in constant time.
//     */    
//     operator xvimage*();

//     /**
//     \brief Cast operator. 
//     Casts the ujoi<> object to 'xvimage*' in constant time.
//     */    
//     operator const xvimage*() const;    

// //    std::string imtype(); 

//     /**
//     \brief Returns a vector with the size of the image object
//     */
//     const types::vint & get_size() const;

//     /**
//     \brief Returns a vector with the center of the image object
//     */
//     const types::vint & get_center() const;
//     types::vint & get_center();
//     const types::vint & get_center_vint() const;

//     /**
//     \brief Sets the center of the image. 

//     The image center is mainly used in masks. For example the opening
//     operator applies the image relative to it's center. 
//     */    
//     void set_center_vint( const types::vint & new_center );

//     /**
//     \brief Sets the center of the image. This method is called from Python

//     The image center is mainly used in masks. For example the opening
//     operator applies the image relative to it's center. 
//     */
// #   ifdef PINK_HAVE_PYTHON
//     void set_center_list( const boost::python::list & new_center );
// #   endif /* PINK_HAVE_PYTHON */

//     /**
//     \brief Returns a smart pointer to the array of the pixels. 
//     */    
//     boost::shared_array<pixel_type> get_pixels();
//     boost::shared_array<pixel_type> get_pixels() const;

//       /**
//          \brief Takes a pointer in and copy the data it points to onto the image buffer
//       */
//       void set_pixels(long ptr, long nbytes);
      
//     /**
//     \brief Returns a string with some informations about the
//     image. Generally it's size and pixel type.
//     */
//     std::string repr() const;

//     /**
//     \brief Fills the image with the given value. That is sets every
//     element of the image to the given value. 
//     */
//     image_type fill( pixel_type value );

//     /**
//     \brief Fills the image with the given value. That is sets every
//     element of the image to the given value. Same as fill, but in
//     operator form.
//     */
//     image_type operator=( pixel_type value ); // equivalent with function fill

//     bool operator==( const image_type & other ) const;
//     image_type operator+=( const image_type & other );
//     image_type operator-=( const image_type & other );
    
//     image_type operator-=( const pixel_type & value );
//     image_type operator+=( const pixel_type & value );
//     image_type operator/=( const pixel_type & value );
//     image_type operator*=( const pixel_type & value );
    
//     /**
//     \brief Counts the non zero pixels.
//     */
//     index_t area();

//     /**
//     \brief Counts the image average value. 
//     */
//     double average();

//     /**
//     \brief Tests if every pixel is null in the image.
//     */
//     bool isnull();

//     /**
//     \brief Calculates the volume of the image (the sum of every pixel). 
//     */
//     double volume();     
//   }; /* class ujoi */

 
//     /*
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************


// IMAGE TYPE SPECIFICS


// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// */

// /*
// ***************************************************************************************
// CHAR IMAGE
// ***************************************************************************************
//  */

//   template <>								
//   class image_type_specific<unsigned char>{
//     typedef unsigned char pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "uint8_t";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_1_BYTE;						
//       }

//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         return a-b > 0 ? a-b : 0;        
//       }

//     pixel_type add( pixel_type a, pixel_type b)
//       {
//         return a+b < UCHAR_MAX ? a+b : UCHAR_MAX;        
//       }

//     void init_pixel( pixel_type & a )
//       {
//         a=0;
//       }

//   }; /* image_type_specific char_image */								


//   /*
// ***************************************************************************************
// SHORT IMAGE
// ***************************************************************************************
//  */
  
//   template <>								
//   class image_type_specific<unsigned short int>{
//     typedef unsigned short int pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "uint16_t";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_2_BYTE;						
//       }
    
//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         return a-b > 0 ? a-b : 0;        
//       }
    
//     pixel_type add( pixel_type a, pixel_type b)
//       {
//         return a+b < USHRT_MAX ? a+b : USHRT_MAX;        
//       }
    
//     void init_pixel( pixel_type & a )
//       {
//         a=0;
//       }
      
//   }; /* image_type_specific short_image */								

  
//   /*
// ***************************************************************************************
// INT IMAGE
// ***************************************************************************************
//  */

  
//   template <>								
//   class image_type_specific<int>{
//     typedef int pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "int32_t";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_4_BYTE;						
//       }

//     pixel_type sub(pixel_type a, pixel_type b )
//           {
//               // HT, why clipping substractions ? 
//         return a-b > 0 ? a-b : 0;
//       }

//     pixel_type add(pixel_type a, pixel_type b)
//           {
//         return a+b < INT_MAX ? a+b : INT_MAX;
//       }

//     void init_pixel( pixel_type & a )
//       {
//         a=0;
//       }
    
//   }; /* image_type_specific int_image */								

  
//   /*
// ***************************************************************************************
// FLOAT IMAGE
// ***************************************************************************************
//  */

  
//   template <>								
//   class image_type_specific<float>{
//     typedef float pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "float";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_FLOAT;						
//       }

//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         return a-b;        
//       }

//     pixel_type add( pixel_type a, pixel_type b)
//       {
//         return a+b;
//       }

//     void init_pixel( pixel_type & a )
//       {
//         a=0.;
//       }

//   }; /* image_type_specific float_image */								

  
//   /*
// ***************************************************************************************
// double IMAGE
// ***************************************************************************************
//  */

//   template <>								
//   class image_type_specific<double>{
//     typedef double pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "double";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_DOUBLE;						
//       }

//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         return a-b;        
//       }

//     pixel_type add( pixel_type a, pixel_type b)
//       {
//         return a+b;        
//       }

//     void init_pixel( pixel_type & a )
//       {
//         a=0.;
//       }

//   }; /* image_type_specific double_image */								

//   /*
// ***************************************************************************************
// fcomplex IMAGE
// ***************************************************************************************
//  */

//   template <>								
//   class image_type_specific<fcomplex>{
//     typedef fcomplex pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "fcomplex";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_COMPLEX;						
//       }

//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         return a-b;        
//       }

//     pixel_type add( pixel_type a, pixel_type b )
//       {
//         return a+b;        
//       }

//     void init_pixel( pixel_type & a )
//       {
//         a.re=0.;
//         a.im=0.;        
//       }    
//   }; /* image_type_specific fcomplex_image */
  
//     /*
// ***************************************************************************************
// dcomplex IMAGE
// ***************************************************************************************
//  */

//   template <>								
//   class image_type_specific<dcomplex>{
//     typedef dcomplex pixel_type;    
//   public:								
//     std::string imtype() const							
//       {									
// 	return "dcomplex";
//       }
    
//     index_t int_pixel_type() const						
//       {									
// 	return VFF_TYP_DCOMPLEX;						
//       }

//     pixel_type sub( pixel_type a, pixel_type b )
//       {
//         return a-b;        
//       }

//     pixel_type add( pixel_type a, pixel_type b )
//       {
//         return a+b;        
//       }

//     void init_pixel( pixel_type & a )
//       {
//         a.re=0.;
//         a.im=0.;        
//       }
    
//   }; /* image_type_specific dcomplex_image */
  
  
  // Valid image types
  // integer and float will be tested first.

  // CREATE_IMAGE_TYPE( unsigned char, "uint8_t", VFF_TYP_1_BYTE );
  // CREATE_IMAGE_TYPE( unsigned short int, "uint16_t", VFF_TYP_2_BYTE );
  // CREATE_IMAGE_TYPE( unsigned int, "int32_t", VFF_TYP_4_BYTE );
  // CREATE_IMAGE_TYPE( float, "float", VFF_TYP_FLOAT );
  // CREATE_IMAGE_TYPE( double, "double", VFF_TYP_DOUBLE );



  // /**
  // \brief The char_image specialization.
  // */
  // typedef ujoi<unsigned char/*,                 VFF_TYP_1_BYTE*/ >   char_image;

  // /**
  // \brief The short_image specialization.
  // */
  // typedef ujoi<unsigned short int/*,            VFF_TYP_2_BYTE*/ >  short_image;

  // /**
  // \brief The int_image specialization.
  // */
  // typedef ujoi</*long*/int32_t/*,               VFF_TYP_4_BYTE*/ >    int_image; // LONG INT is 8 byte's on 64 bit system

  // /**
  // \brief The float_image specialization.
  // */
  // typedef ujoi<float/*,                         VFF_TYP_FLOAT*/  >  float_image;

  // /**
  // \brief The double_image specialization.
  // */
  // typedef ujoi<double/*,                        VFF_TYP_DOUBLE*/ > double_image;

  //   /**
  // \brief The double_image specialization.
  // */
  // typedef ujoi<fcomplex/*,                        VFF_TYP_COMPLEX*/ > fcomplex_image;

  //   /**
  // \brief The double_image specialization.
  // */
  // typedef ujoi<dcomplex/*,                        VFF_TYP_DCOMPLEX*/ > dcomplex_image;





/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


TEMPLATE IMPLEMENTATION


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/






  
// /*
// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************


// c++ class pink::ujoi (this is a template class, so it stays in the header)


// ***************************************************************************************
// ***************************************************************************************
// ***************************************************************************************
// */
  
//   template <class pixel_type >
//   ujoi<pixel_type >::ujoi( /* std::string debug="" */ ) {
    
// #   if UJIMAGE_DEBUG >= 2
//     std::cerr << "creating an empty image (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     // defining the size for the isnull function
//     this->size.reset(new types::vint(1,0));    
    
//   } /* ujoi<pixel_type >::ujoi( const std::string & filename ) */


  
//   template <class pixel_type>
//   ujoi<pixel_type>::ujoi( std::string filename, std::string debug )
//   {
// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
//     std::cerr << "reading image '" << filename << "'\n";
// #   endif /* UJIMAGE_DEBUG */

//     xvimage * tmp;

//     if (! (tmp = readimage(const_cast<char*>(filename.c_str()))) ) // readimage takes char* but hopefully does not change it
//     {
//       pink_error("cannot read file '" + filename + "'");      
//     } 
    
//     writeimage(tmp, "tmp.pgm");
    
//     if (tmp->data_storage_type != this->int_pixel_type() )
//     {
//       pink_error("The image type of '" + filename + "' is '" + image_type_string(tmp->data_storage_type)
//             + "', but expected '" + this->imtype() + "'." );
//     } /* if */
      
//     this->size.reset( new types::vint(
//                         *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
//                           tmp->row_size, 
//                           tmp->col_size, 
//                           tmp->depth_size, 
//                           tmp->time_size
//                           )
//                         )
//       );

//     // setting up the center
//     this->center.reset(new types::vint( size->size(), -1 ));
  
//     this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating the array for the pixel types

//     std::copy( &(static_cast<pixel_type*>(tmp->image_data)[0]),
//                &(static_cast<pixel_type*>(tmp->image_data)[this->size->prod()]),
//                this->pixels.get()
//       );

//     freeimage(tmp);
    
//   } /* ujoi::ujoi */


  
//   template <class pixel_type >
//   ujoi<pixel_type >::ujoi( xvimage * src, std::string debug )
//     : pixels( reinterpret_cast<pixel_type*>(src->image_data), liberator_t() )
//   {
    
// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     if (image_type_string(src->data_storage_type)!=this->imtype())
//     {
//       pink_error("The image type is '" + image_type_string(src->data_storage_type)
//             + "', but expected '" + this->imtype() + "'." );
//     } /* image_type(src.data_storage_type)!=this->imtype() */
    
//     this->size.reset(
//       new types::vint(
//         *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
//           src->row_size, 
//           src->col_size, 
//           src->depth_size, 
//           src->time_size
//           )
//         )
//       );

//     // setting up the center
//     this->center.reset( new types::vint( size->size(), -1 ));

//     // the pixels are fetched at the constructor.
//   } /* ujoi::ujoi */


  
//   template <class pixel_type >
//   ujoi<pixel_type >::ujoi( const xvimage & src, std::string debug )
//   {
    
// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     if (image_type_string(src.data_storage_type)!=this->imtype())
//     {
//       pink_error("The image type is '" + image_type_string(src.data_storage_type)
//             + "', but expected '" + this->imtype() + "'." );
//     } /* image_type(src.data_storage_type)!=this->imtype() */
    
//     this->size.reset(
//       new types::vint(
//         *getDimensions( // detecting the dimensions according to row-, col-, depth- and time_size.
//           src.row_size, 
//           src.col_size, 
//           src.depth_size, 
//           src.time_size
//           )
//         )
//       );

//     // setting up the center
//     this->center.reset( new types::vint( size->size(), -1 ));

//     this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating the array for the pixel types
    
//     std::copy(
//       &(static_cast<pixel_type*>(src.image_data)[0]),
//       &(static_cast<pixel_type*>(src.image_data)[this->size->prod()]),
//       this->pixels.get()
//       );

//   } /* ujoi::ujoi */


  
//   template <class im_type >
//   ujoi<im_type>::ujoi( const ujoi< im_type > & src, std::string debug ) // SHALLOW_copy_constructor
//     : size(src.size), center(src.center), old_school(src.old_school), pixels(src.pixels)
//   {
// #   if UJIMAGE_DEBUG >= 2
//     std::cerr << "WARNING!: copy constructors only constructs shallow copies!\n";    
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */
//   } /* ujoi::ujoi */


  
//   template <class im_type>
//   ujoi<im_type> ujoi<im_type>::operator=( const image_type & other )     // SHALLOW_copy constructor
//   {
// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     // note self assignment will be tested inside boost-smart-pointer
//     this->size=other.size;
//     this->center=other.center;
//     this->old_school=other.old_school;   
//     this->pixels=other.pixels; 
        
//     return *this;
//   } /* ujoi::operator= */
  

  
//   template <class pixel_type >
//   ujoi<pixel_type >::ujoi( const types::vint & dim, std::string debug ){

// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */


//     this->size.reset(new types::vint( dim )); // creating a copy of the size
//     this->center.reset(new types::vint( size->size(), -1 ));
//     this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

//     // setting up elements zero
//     FOR(q, size->prod())
//     {      
//       this->init_pixel(this->pixels[q]);      
//     };  
//   } /* ujoi::ujoi */


// # ifdef PINK_HAVE_PYTHON
//   template <class pixel_type >
//   ujoi<pixel_type >::ujoi( const boost::python::list & dim, std::string debug ){

// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     this->size.reset( new types::vint(dim)); // creating a copy of the size
//     this->center.reset( new types::vint( size->size(), -1 ));

//     this->pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

//     // setting up elements zero
//     FOR(q, size->prod())
//     {      
//       this->init_pixel(this->pixels[q]);      
//     };
//   } /* ujoi::ujoi */
// # endif /* PINK_HAVE_PYTHON */

  
  
//   template <class pixel_type >
//   ujoi<pixel_type >::~ujoi( ){

// #   if UJIMAGE_DEBUG >= 2
//     std::cerr << "deleting image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

// // deletes 'xvImage' automaticly
//     /////!!!!!!! std::cout<< "deleting " << old_school->imtype() << "_char"  << std::endl; 
//   } /* ujoi::~ujoi */



//   template <class pixel_type >
//   ujoi<pixel_type >::ujoi( const types::vint & dim, boost::shared_array<pixel_type> data, std::string debug )
//   {

// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image " << debug << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     size.reset(new types::vint( dim )); // creating a copy of the size
//     center.reset(new types::vint( size->size(), -1 ));
//     pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

//     if (this->size->size()<=4){
//       old_school.reset( new shallow_xvimage( *size, this->int_pixel_type() ) );
//     };

//     std::copy( (&data[0]), (&data[size->prod()]), pixels.get());

//   } /* ujoi::ujoi */

  
//   template <class pixel_type>
//   ujoi<pixel_type>::ujoi (
//     const types::vint & dim,
//     pixel_type * data,
//     std::string debug
//     )
//   {
// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image " << debug << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     size.reset(new types::vint( dim )); // creating a copy of the size
//     center.reset(new types::vint( size->size(), -1 ));
//     pixels.reset( new pixel_type[ size->prod() ] ); // allocating memory for the pixels

//     if (this->size->size()<=4){
//       old_school.reset( new shallow_xvimage( *size, this->int_pixel_type() ) );
//     };

//     // making a copy of the elements
//     std::copy( (&data[0]), (&data[size->prod()]), pixels.get());
//   } // ujoi::ujoi
  

  

//   template <class pixel_type >
//   void ujoi<pixel_type >::_writeimage( const std::string & filename ) const // exports the image into a pgm file
//   {
//     // writeimage takes 'char *', while 'c_str()' gives 'const char *'. 
//     // I don't want to cast unnecesserily, so I'll just copy it.
 
//     char * cstr = new char [ filename.size() + 1 ]; // converting the filename for 'writeimage'
//     strcpy ( cstr, filename.c_str() );
    
//     writeimage( const_cast<image_type*>(this)->get_output(), cstr );    

//   } /* _writeimage */



//   template<class pixel_type>
//   void write_a_pixel( std::fstream & s, pixel_type & value ) 
//   {
//     pink_error("write_a_pixel called with a wrong pixel type");
//   } /* write_a_pixel default */

//   template<> // implemented in 'ujimage.cpp'
//   void write_a_pixel<float>( std::fstream & s, float & value ) ;

//   template<> // implemented in 'ujimage.cpp'
//   void write_a_pixel<unsigned char>( std::fstream & s, unsigned char & value ) ;


//   template <class pixel_type >
//   void ujoi<pixel_type >::_write_amira( const std::string & filename ) const // exports the image into an amira mesh (.am) file
//   {   
//     std::string typetext;
  
//     switch (this->int_pixel_type())  { 
      
//     case VFF_TYP_1_BYTE:
//       typetext.assign("byte");
//       break;

//     case VFF_TYP_4_BYTE:
//       typetext.assign("int");
//       break;
      
//     case VFF_TYP_FLOAT:
//       typetext.assign("float");
//       break;
      
//     default:
//       std::cerr << "\nfile: " << filename; 
//       pink_error("you can export only 'char', 'int'(32bit) and 'float' images at this point");	
//     } /* switch */
        
//     if (this->size->size()!=3)
//     {
//       std::cerr << "file: " << filename; 
//       pink_error("you can export only 3D images at this point");
//     } 
//     else /* NOT this->size.size()!=3 */
//     {
//       std::fstream s;
//       s.open ( filename.c_str(), std::fstream::out );
//       s << "# UjoImro PInK Amira export, 2009-2010\n\n"
// 	<< "# AmiraMesh BINARY-LITTLE-ENDIAN 2.1\n\n\n"
// 	<< "define Lattice " << " " << (*this->size)[0] << " " << (*this->size)[1] << " " << (*this->size)[2] << "\n\n"
// 	<< "Parameters {\n"
// 	<< "\tContent\"" << " " << (*this->size)[0] << "x" << (*this->size)[1] << "x" << (*this->size)[2] << " PInK pgm export \",\n"
//         << "\tBoundingBox 0 " << (*this->size)[0]-1 << " 0 " << (*this->size)[1]-1 << " 0 " << (*this->size)[2]-1 << ",\n"
// 	<< "\tCoordType \"uniform\"\n"
// 	<< "}\n\n"
// 	<< "Lattice { " << typetext << " Data } @1\n\n"
// 	<< "# Data section follows\n"
// 	<< "@1\n";
      
      
//       // From avizo 6.0 manual:
//       // ... the specified number of bytes is read in binary format. 
//       // It is assumed that sizeof(short) is 2, sizeof(int) is 4, 
//       // sizeof(float) is 4, sizeof(double) is 8, and sizeof(complex) is 8. 
//       // Multidimensional arrays indexed via [k][j][i] are read with i running fastest. 
      
      
//       types::vint curr(3);
      
//       FOR(q, (*size)[2]) {
// 	FOR(w, (*size)[1]) { 
// 	  FOR(e, (*size)[0]) {
	    
// 	    curr[0]=e;
// 	    curr[1]=w;
// 	    curr[2]=q;
	 	    
// 	    s.write( 
// 	      reinterpret_cast<char*>(&pixels[ 
// 					size->position(curr)
// 					] 
// 		),
// 	      sizeof(pixel_type)
// 	      );
	    
// 	  }; /* FOR(i, size[2]) */
// 	}; /* FOR(j, size[1]) */
//       }; /* FOR(k, size[0]) */
      
      
//       s << "\n";
//       s.close();
//     } /* NOT this->size.size()!=3 */
//     std::cerr << "file '" << filename << "' exported in Amira format\n";
//   } /* ujoi<pixel_type >::_write_amira */



//   template <class pixel_type>
//   ujoi<pixel_type>::operator xvimage* () // this method is not exported to python. It could not even be, as boost doesn't support pointers.
//   {
//     return this->get_output();    
//   } /* ujoi::xvimage*  */


  
//   template <class pixel_type>
//   ujoi<pixel_type>::operator const xvimage* () const // this method is not exported to python. It could not even be, as boost doesn't support pointers.
//   {
//     return this->get_output();    
//   } /* ujoi::xvimage*  */


  
//   template <class pixel_type >
//   xvimage* ujoi<pixel_type >::get_output(){
//     if (this->size->size()>4){
//       pink_error("error: Images with more than four dimensions can not be extracted to 'xvimage'");
//     }; /* this->size.size()>4 */

//     this->old_school.reset( new shallow_xvimage( *size, this->int_pixel_type() ) );
    
//     this->old_school->image_data = pixels.get();
//     // setting up the data type. This is an 
//     // unsafe action, and is only allowed 
//     // for backward compatibility.
    
//     return old_school.get(); // you must never 'free()' or 'delete' this pointer!
//   } /* ujoi::get_output */



//   template <class pixel_type >
//   pixel_type & ujoi<pixel_type >::operator[](index_t pos){ // index acces to the elements

//     // CAPABILITY TEST IN DEBUG MODE
//     // this is a test which would be slow in an everyday situation,
//     // but can save ages while debugging
// #   ifdef UJIMAGE_DEBUG
//     if ( ( pos < 0 ) or (pos > this->size->prod()) ) // note pos == size->prod intentionally left in for std::copy
//     {
//       std::cerr << "error: number of elements is " << this->size->prod() << " while pos = " << pos << std::endl;
//       pink_error("You are trying to access elements outside of the image\n");
//     } /* if */
// #   endif /* UJIMAGE_DEBUG */

    
//     return pixels[pos];

//   } /* ujoi::operator[] */



//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::operator[](index_t pos) const { // const index acces to the elements

//     // CAPABILITY TEST IN DEBUG MODE
//     // this is a test which would be slow in an everyday situation,
//     // but can save ages while debugging
// #   ifdef UJIMAGE_DEBUG
//     if ( ( pos < 0 ) or (pos > this->size->prod()) ) // note pos == size->prod intentionally left in for std::copy
//     {
//       std::cerr << "error: number of elements is " << this->size->prod() << " while pos = " << pos << std::endl;
//       pink_error("You are trying to access elements outside of the image\n");
//     } /* if */
// #   endif /* UJIMAGE_DEBUG */

//     return pixels[pos];

//   } /* ujoi::operator[] */



//   template <class pixel_type >
//   pixel_type & ujoi<pixel_type >::operator()( index_t pos )
//   {
//     return pixels[pos];    
//   }
  


//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::operator()( index_t pos ) const
//   {
//     return pixels[pos];    
//   }
  


//   template <class pixel_type >
//   pixel_type & ujoi<pixel_type >::operator[](const types::vint & pos){ // vint acces to the elements

//     // CAPABILITY TEST IN DEBUG MODE
//     // this is a test which would be slow in an everyday situation,
//     // but can save ages while debugging
// #   ifdef UJIMAGE_DEBUG
//     if ( not size->inside(pos) )
//     {
//       std::cerr << "error: image dimensions are " << this->size->repr() << " while pos = " << pos.repr() << "\n";
//       pink_error("You are trying to access elements outside of the image\n");
//     } /* if */
// #   endif /* UJIMAGE_DEBUG */

//     return pixels[size->position(pos)];

//   } /* ujoi::operator[] */



//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::operator[](const types::vint & pos) const { // const types::vint acces to the elements

//     // CAPABILITY TEST IN DEBUG MODE
//     // this is a test which would be slow in an everyday situation,
//     // but can save ages while debugging
// #   ifdef UJIMAGE_DEBUG
//     if ( not size->inside(pos) )
//     {
//       std::cerr << "error: image dimensions are " << this->size->repr() << " while pos = " << pos.repr() << "\n";
//       pink_error("You are trying to access elements outside of the image\n");
//     } /* if */
// #   endif /* UJIMAGE_DEBUG */

//     return pixels[size->position(pos)];

//   } /* ujoi::operator[] */


// # ifdef PINK_HAVE_PYTHON
//   template <class pixel_type >
//   pixel_type & ujoi<pixel_type >::operator[]( const boost::python::list & pos ){ // python list acces to the elements

//     types::vint vint_pos(pos);

//     return (*this)[vint_pos];

//   } /* ujoi::operator[] */



//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::operator[]( const boost::python::list & pos ) const { // const python list acces to the elements

//     types::vint vint_pos(pos);

//     return (*this)[vint_pos];

//   } /* ujoi::operator[] */
// # endif /* PINK_HAVE_PYTHON */



//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::get_operator_int( index_t pos ) const {
//     return (*this)[pos]; 
//   };



//   template <class pixel_type >
//   void ujoi<pixel_type >::set_operator_int( index_t pos, const pixel_type & value ){
//     (*this)[pos]=value;
//   }



//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::get_operator_vint( const types::vint & pos ) const {
//     return (*this)[pos];
//   }



//   template <class pixel_type >
//   void ujoi<pixel_type >::set_operator_vint( const types::vint & pos, const pixel_type & value ){
//     (*this)[pos]=value;
//   }


// # ifdef PINK_HAVE_PYTHON
//   template <class pixel_type >
//   const pixel_type & ujoi<pixel_type >::get_operator_list( const boost::python::list & pos ) const {
//     return (*this)[pos];
//   }



//   template <class pixel_type >
//   void ujoi<pixel_type >::set_operator_list( const boost::python::list & pos, const pixel_type & value ){
//     (*this)[pos]=value;
//   }
// # endif /* PINK_HAVE_PYTHON */



// template <class pixel_type >
// std::string ujoi<pixel_type >::imtype(){
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



//   template <class pixel_type >
//   const types::vint & ujoi<pixel_type >::get_size() const
//   {
//     return *size;
//   } /* ujoi:: get_size */


  
//   template <class pixel_type >
//   const types::vint & ujoi<pixel_type >::get_center() const
//   {
//     return *center;
//   } /* ujoi::get_center */


  
//   template <class pixel_type >
//   types::vint & ujoi<pixel_type >::get_center()
//   {
//     return *center;
//   } /* ujoi::get_center */



//   template <class pixel_type >
//   void ujoi<pixel_type >::set_center_vint( const types::vint & new_center ){

//     if ( new_center.size() != size->size() )
//     {      
//       pink_error("set_center: the center coordinate has to have the same dimension as the image");      
//     }
//     else 
//     {
//       center.reset( new types::vint( new_center ));
//     }
    
//   } /* ujoi::set_center_vint */


  
//   template <class pixel_type >
//   const types::vint & ujoi<pixel_type >::get_center_vint() const
//   {
//     return *center;    
//   } /* ujoi::get_center_vint */


// # ifdef PINK_HAVE_PYTHON
//   template <class pixel_type >
//   void ujoi<pixel_type >::set_center_list( const boost::python::list & new_center ){


//     types::vint vint_new_center = types::vint( new_center ); // converting 'boost::python::list' into 'vint'

//     if ( vint_new_center.size() != size->size() )
//     {      
//       pink_error("set_center: the center coordinate has to have the same dimension as the image");
//     }
//     else /* NOT vint_new_center.size() != size->size() */
//     {      
//       center.reset( new types::vint( vint_new_center ) );
//     } /* NOT vint_new_center.size() != size->size() */
//   } /* ujoi::set_center_list */


  
//   template <class pixel_type >
//   PyObject* ujoi<pixel_type >::get_pixels_python()
//   {
// #   if (PY_MAJOR_VERSION == 3) && (PY_MINOR_VERSION >= 3)
//       return PyMemoryView_FromMemory (
//           reinterpret_cast<char*>(this->pixels.get()),
//           this->size->prod()*sizeof(pixel_type),
//           PyBUF_READ
//           );
// #   else // NOT (PY_MAJOR_VERSION > 3) and (PY_MINOR_VERSION >= 3)
//       return PyBuffer_FromMemory(
//           reinterpret_cast<void*>(this->pixels.get()),
//           this->size->prod()*sizeof(pixel_type)
//           );      
// #   endif // NOT (PY_MAJOR_VERSION > 3) and (PY_MINOR_VERSION >= 3)
// //    return pixels.get();
//   } /* ujoi::get_pixels */
// # endif /* PINK_HAVE_PYTHON */

  

//   template <class pixel_type >
//   boost::shared_array<pixel_type> ujoi<pixel_type >::get_pixels(){
//     return pixels;
//   } /* ujoi::get_pixels */



//   template <class pixel_type >
//   boost::shared_array<pixel_type> ujoi<pixel_type >::get_pixels() const{
//     return pixels;
//   } /* ujoi::get_pixels */


//   template <class pixel_type >
//   void ujoi<pixel_type >::set_pixels(long ptr, long nbytes) {
//       // brutal
//       if (nbytes == this->size->prod()*sizeof(pixel_type))
//           memcpy(reinterpret_cast<void*>(this->pixels.get()), (void*)ptr, nbytes);
//       else
//           std::cerr << "Incorrect size for buffer, nothing done" << std::endl;
//   } /* ujoi::set_pixels */


    
//   template <class pixel_type >
//   std::string ujoi<pixel_type >::repr() const
//   {
//     std::stringstream ss;
//     ss << this->imtype() << " image of size " << get_size().repr();
    
//     std::string result;
    
//     result = ss.str();
    
//     return result;
//   } /* ujoi::repr */


  
//   template <class pixel_type >
//   ujoi<pixel_type> ujoi<pixel_type >::fill( pixel_type value ) 
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)=value;      
//     } /* FOR */

//     return *this;    
//   } /* ujoi::fill */
  

  
//   template <class pixel_type>
//   ujoi<pixel_type> ujoi<pixel_type>::operator=( pixel_type value ) // equivalent with function fill
//   {
//     this->fill(value);    
//   } /* ujoi::operator= */


  
//   template <class pixel_type >
//   bool ujoi<pixel_type >::operator==( const image_type & other ) const
//   {
//     FOR(q, get_size().prod())
//     {
//       if ((*this)(q)!=other(q))
//       {
//         return false;
//       } /* (*this)[q]!=other[q] */      
//     } /* FOR */

//     return true;
//   } /* ujoi::operator== */

  

//   template <class pixel_type >
//   ujoi<pixel_type> ujoi<pixel_type >::operator+=( const image_type & other )
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)=this->add((*this)(q),other(q));
//     } /* FOR */
    
//     return *this;    
//   } /* ujoi::operator+= */



//   template <class pixel_type >
//   ujoi<pixel_type> /*image_type*/
//   ujoi<pixel_type >::operator-=( const image_type & other )
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)=this->sub((*this)(q),other(q));
//     } /* FOR */
    
//     return *this;      
//   } /* ujoi::operator-= */


  
//   template <class pixel_type >
//   ujoi<pixel_type> /*image_type*/
//   ujoi<pixel_type >::operator-=( const pixel_type & value )
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)-=value;      
//     } /* FOR */
    
//     return *this;      
//   } /* ujoi::operator-= */


  
//   template <class pixel_type >
//   ujoi<pixel_type> /*image_type*/
//   ujoi<pixel_type >::operator+=( const pixel_type & value )
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)+=value;      
//     } /* FOR */
    
//     return *this;    
//   } /* ujoi::operator+= */


  
//   template <class pixel_type >
//   ujoi<pixel_type> /*image_type*/
//   ujoi<pixel_type >::operator/=( const pixel_type & value )
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)/=value;      
//     } /* FOR */
    
//     return *this;    
//   } /* ujoi::operator/= */


  
//   template <class pixel_type >
//   ujoi<pixel_type> /*image_type*/
//   ujoi<pixel_type >::operator*=( const pixel_type & value )
//   {
//     FOR(q, get_size().prod())
//     {
//       (*this)(q)*=value;      
//     } /* FOR */
    
//     return *this;    
//   } /* ujoi::operator*= */
  

  
//   template <class pixel_type >
//   index_t ujoi<pixel_type >::area() // number of non zero pixels
//   {
//     index_t result=0;
    
//     FOR(q, get_size().prod())
//     {
//       if ((*this)(q)!=0)
//       {
//         result++;        
//       } /* (*this)(q)!=0 */
//     } /* FOR */
//   } /* ujoi::area */


  
//   template <class pixel_type >
//   double ujoi<pixel_type >::average() // average value
//   {
//     double result=0;
//     result = this->volume()/this->get_size().prod();
//     return result;    
//   } /* ujoi::average */


  
//   template <class pixel_type >
//   bool ujoi<pixel_type >::isnull() 
//   {
//     FOR(q, get_size().prod())
//     {
//       if ((*this)(q)!=0)
//       {
//         return false;        
//       } /* (*this)(q)!=0 */
//     } /* FOR */

//     return true;    
//   } /* ujoi::isnull */


  
//   template <class pixel_type >
//   double ujoi<pixel_type >::volume() // sum of all the pixels
//   {
//     double result=0;
    
//     FOR(q, get_size().prod())
//     {
//       result+=(*this)(q);
//     } /* FOR */

//     return result;
//   } /* ujoi::volume */
  

  
//   template <class pixel_type >
//   void ujoi<pixel_type >::reset( image_type & other )
//   {
//     this->size=other.size;
//     this->center=other.center;
//     this->old_school=other.old_school;
//     this->pixels=other.pixels;  
//   } /* ujoi::reset */


  
//   template <class pixel_type>
//   ujoi<pixel_type> ujoi<pixel_type >::clone(void) const
//   {
//     image_type result;      
// #   if UJIMAGE_DEBUG >= 2
//     result.debug = debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */

//     result.size.reset( new types::vint(*size) );
//     result.center.reset( new types::vint(*center) );
//     result.old_school.reset( new shallow_xvimage( *this->size, this->int_pixel_type() )); // creating a new xvimage for the 'pink::ujoi' object
//     // the constructor will not copy the data
  
//     result.pixels.reset(new pixel_type[size->prod()]); // allocating the array for the pixel types

//     std::copy( &(*this)(0), &(*this)(size->prod()), result.pixels.get());
        
//     return result;
//   } /* ujoi::clone */


  
//   template <class pixel_type>
//   ujoi<pixel_type> ujoi<pixel_type >::swap( image_type other )
//   {
// #   if UJIMAGE_DEBUG >= 2
//     this->debug=debug; // representing the name of the object if debugged
//     std::cerr << "creating image '" << debug << "' (" << static_cast<void*>(this) << ")" << std::endl;
// #   endif /* UJIMAGE_DEBUG */
    
//     if (this==&other) // self-assignment test
//     {
//       return *this;      
//     }

//     // swapping every variable
//     this->size.swap(other.size);
//     this->center.swap(other.center);
//     this->old_school.swap(other.old_school); 
//     this->pixels.swap(other.pixels); 
    
//     return *this;
//   } /* ujoi::swap */

  
} /* namespace pink */

#endif /* __UJIMAGE_HPP */
// #endif /* __cplusplus */
// LuM end of file
