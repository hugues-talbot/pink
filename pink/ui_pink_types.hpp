/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2007-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus
#ifndef UIFIBRETYPES_H_
#define UIFIBRETYPES_H_
/** \file ui_pink_types.hpp
 * \brief Header file for uiFibreTypes.cpp
 * 
 * Declaration of special types. The purpose of the type declaration is the universality. I want the program to be size- and dimension independent.
 */

/* Line 12+1 */
/* 
   ui_pink_types.hpp
   declaration of special types. The purpose of the type 
   declaration is the universality. I want the program to 
   be size-independent
*/
//#define PTR boost::shared_ptr
//#define ARRAY boost::shared_array

#include <vector>
#include <string>
#include <sstream>
#include <assert.h>
#include <iostream>

#include "mcimage.h"

#ifdef UJIMAGE_DEBUG
# define DEBUG(x) std::cout << "debug: " << BOOST_PP_STRINGIZE(x) << " = " << x << "\n"
# define PRINTIMAGE(x)							\
    std::cout << "writing image: " << BOOST_PP_STRINGIZE(x) << " as '" << BOOST_PP_STRINGIZE(x) << ".pgm'\n"; \
    x->_writeimage(BOOST_PP_STRINGIZE(x)".pgm")
#else /* NOT UJIMAGE_DEBUG */    
# define DEBUG(x)
# define PRINTIMAGE(x)    
#endif /* NOT UJIMAGE_DEBUG */

#define _DEBUG(x) std::cout << "_debug: " << BOOST_PP_STRINGIZE(x) << " = " << x << "\n"

#define _PRINTIMAGE(x)							\
    std::cout << "_writing image: " << BOOST_PP_STRINGIZE(x) << " as '" << BOOST_PP_STRINGIZE(x) << ".pgm'\n"; \
    x._writeimage(BOOST_PP_STRINGIZE(x)".pgm")

#define _PRINTAMIRA(x)							\
    std::cout << "_writing image: " << BOOST_PP_STRINGIZE(x) << " as '" << BOOST_PP_STRINGIZE(x) << ".am'\n"; \
    x._write_amira(BOOST_PP_STRINGIZE(x)".am")

// eigen2 debug facility
#define DVECT(x) std::cout << "----------------\n" << BOOST_PP_STRINGIZE(x) << " = \n" << x << "\n"

    // this macro generates from 'IMAGE_TYPE(potencial)' -> 'image_type(potencial), "potencial"' 
#define IMAGE_TYPE(x)				\
    image_type( x, BOOST_PP_STRINGIZE(x) )

// does not throw !OB
#define pink_warning(message) \
{ \
std::cerr << "Pink warning: " << message << std::endl; \
}

#define pink_error(message)                                             \
    {                                                                   \
      std::cerr << std::endl << "Pink error!" << std::endl << "in '" << __FILE__<< "' (line " << __LINE__ << "): " << message << std::endl << std::endl; \
      throw "Pink error!\n";                                            \
    }
    
// Limit of a blob for sqlite. A little under 900MB
#define BLOB_SIZE 900000000

#define OPEN_BLOB_RO 0
#define OPEN_BLOB_RW 1

// Sentinel constant
#define PBAR_REPORT_INTERVALL 1

// General Macros
// if st is an unsigned int and 0, than st-1 will result an enormous number
#define FOR(i,st) for (index_t i = 0; i<st; i++)
#define FORR(i,st) for (i = 0; i<st; i++)

// #define round(z) ((z-(double)((int)z))<0.5?((int)z):((int)z+1))


namespace pink
{

  template <class T0>
  T0
  round_up( T0 global_size, T0 group_size )
  {
    T0 r = global_size % group_size;
    if( r == 0 )
      return global_size;
    return global_size + group_size - r;
  } /* round_up */


  template <class T0>
  T0
  prod( const std::vector<T0> & vector ) {
    T0 result(1);
    
    for ( auto & elem : vector ) result *= elem;
    
    return result;
  } // prod


  template <class T0>
  bool
  on_side( const T0 & image_size, const T0 & point )
  {
    if (point.size() != image_size.size()) pink_error("The 'point' must have same dimension (length) than the 'vector'.");
  
    FOR(q, image_size.size())
      if ( (point[q]==0) || (point[q]==image_size[q]-1) ) return true;	
  
    return false;
  } /* vint::on_side */

  template <class T0>
  bool
  inside( const T0 & image_size, const T0 & point ) 
  {
    assert(image_size.size() == point.size());    
    FOR( q, image_size.size() )
      if ( (point[q]<0) || (point[q]>=image_size[q]) ) return false;	
  
    return true;
  } /* vint::inside */


  template <class T0>
  void
  reset( T0 & t0 )
  {
    index_t q = t0.size()-1;
    do 
    { 
      t0[q--]=0; 
    } 
    while (q>=0);
  } /* vint::reset */


  template <class T0>
  index_t
  position( const T0 & size, const T0 & elem ) 
  {
    // tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
    // ATTENITON !!!!!!!! New version, now the X is the fastest
    // also note, that the size should be the parameter and the object should change
    // to comply with the no parameter change paradigm

    index_t d = size.size();
    index_t pos = 0;
    index_t t = 1;
    index_t q = 0;

    while ( q <= d - 1 ) {
      pos += elem[q] * t;
      t *= size[q];
      q+=1;
    }
    
    return pos;
  } /* vint::position */


  
  template <class T0, class T1>
  void
  fill( T0 & t0, const T1 & t1 ) {
    for ( auto & pixel : t0 ) pixel = t1;
    return;
  }

  template <class T0>
  std::string
  repr( const T0 & t0 ) {
    std::stringstream ss;
    ss << "[ ";
    for ( auto & coord : t0 ) ss << coord << ", ";
    ss << "]";

    return ss.str();    
  } // repr
  

  /**
     \note tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
     
     \warning  New version, now the X is the fastest also note, that the size should be the parameter and the object should change to comply with the no parameter change paradigm
   */
  template <class T0>
  void
  next_step( const T0 & dim, const index_t & step, T0 & result ) {

    index_t d    = dim.size();
    index_t prod = step;
    index_t q    = 0;
    
    while ( q <= d - 1 ) {      
      if ( dim[q] > 0 ) result[q] = prod % dim[q];
      
      if ( prod > 0 ) prod = prod / dim[q];

      q++;
    }

    return;
  } // next_step

  template <class T0>
  bool
  next_step_it( const T0 & size, T0 & pos ) {
    index_t d = size.size();
    index_t q = 0;
    
    while ( ( pos[q] >= size[q] - 1) && ( q < d-1 ) ) 
    {
      q++;
    }
  
    if ( (q == d-1) && (pos[q] >= size[q] - 1) )
    {
      return false;
    }
  
    pos[q] += 1;
    
    while (q > 0) 
    {
      q--;
      pos[q]=0;    
    }
  
    return true;

  } // next_step_it
  

  template <class T0>
  inline T0
  sqr( const T0 & t0 ) {
    return t0 * t0;
  } // sqr

  template <class T0>
  inline T0
  prod_except( const std::vector<T0> & input, const index_t & skip ) {
    T0 prod = 1;

    for (index_t q = 0; q<input.size(); q++ ) 
      if (q!=skip) prod *= input[q];      
    
    return prod;
  } // prod_except

  template <class Fn1, class Fn2, class...ARGS>
  void
  pick( char * message, bool cond, Fn1 fn1, Fn2 fn2, ARGS...args ) {
    if (cond)
    {
      if (!fn1(args...)) pink_error( message + std::string(" function failed (001).") );
    }
    else
    { if (!fn2(args...)) pink_error( message + std::string(" function failed (002).") ); }
    
    return;  
  } // pick


  
  namespace types
  {
    

//     class vint: public std::vector<index_t>
//     {
// #   ifdef UJIMAGE_DEBUG
//     private:
//       std::string debug;
// #   endif /* UJIMAGE_DEBUG */

//     public:

//       vint();
//       vint( const vint & src ); //copy constructor
//       vint( const vint & src, std::string debug ); // copy constructor with debugging
//       vint( index_t size, std::string debug=""  );
//       vint( index_t size, index_t defvalue );
// #     ifdef PINK_HAVE_PYTHON
//       vint( const boost::python::list & src );
// #     endif /* PINK_HAVE_PYTHON */
//       ~vint();
//       void reset();
//       index_t prod() const;
//       index_t prod_except( index_t p ) const;
//       double fabs() const;
//       std::string repr() const;
//       void next_step( index_t step, vint & result ) const;
//       bool on_side( const vint & point ) const;
//       bool inside( const vint & ) const;
//       //bool inside( const boost::python::list & point ) const;
//       bool next_step_it( vint & result ) const;
//       bool operator==( const vint & other ) const;
//       bool operator!=( const vint & other ) const;
//       index_t position( const vint & elem ) const;
//       bool add_set( const vint & other );
//       vint & operator << ( const index_t & initiator );
//       vint & operator,   ( const index_t & next );
//     }; /* class vint */


    
    class progress_bar
    {
      
    private:
      index_t max, min, pos;
      bool measure;
      bool started;
      time_t begin, finish;
      time_t last_report;
      std::string time2string(time_t seconds);
//  start_time
    public:
      progress_bar();
      ~progress_bar();
      bool timeToReport();
      void start();
      void stop();
      void maxPos(index_t maxPos);
      void minPos(index_t minPos);
      void setPos(index_t currPos);
      std::string operator << (index_t currPos);
      std::string percent();
      std::string elapsedTime();
      index_t elapsedSeconds();  
  
      std::string remainingTime();
  
    }; /* class progress_bar */

  } // namespace types
} // namespace pink


//int vProd(const vint & v);

/* class uiScalarField{ */
/* public: */
/*   vint size; */
/*   boost::shared_ptr< uiVector<uiVal_type> > values; */
/* private: */
/*   int position ( const vint & elem ); */
/*   uiVal_type r( int a, int b, int c, uiVal_type x, uiVal_type y, uiVal_type z); */
/*   uiVal_type r( int a, int b, uiVal_type x, uiVal_type y ); */
/* public: */
/*   uiScalarField( boost::shared_ptr<vint> pdim ); */
/*   uiScalarField( const vint & dim ); */
/*   uiScalarField( const vint & dim, boost::shared_array<float> data ); */
/*   uiScalarField( const vint & dim, boost::shared_array<unsigned char> data ); */
/*   uiScalarField( const vint & dim, uiScalarField & src, int xmin, int ymin );////just 2D */
/*   ~uiScalarField( void ); */
/*   //void clear(void); */
/*   void copy( uiScalarField & src ); */
/*   void copy( uiScalarField & src, vint & coord ); //// just 2D */
/*   vint dimensionVect( void ); */
/* //  uiVal_type & operator [] ( vint * n ); */
/*   uiVal_type & operator [] ( const vint & n ); */
/*   uiVal_type & operator [] ( const boost::shared_ptr<vint> & n ); */
/*   uiVal_type & operator [] ( int n ); */
/*   uiVal_type max( void ); */
/*   uiVal_type min( void ); */
/*   void add( uiVal_type v ); */
/*   void _sqrt( void ); */
/*   void measure( void ); /\* should be parametrized later*\/ */
/*   void circles( void ); /\* Should be parametrized later*\/ */
/*   void circles(int x, int y, int z); */
/*   void circles(int x, int y, int z, int a, int b, int c);//min distance from two points */
/*   void circles(int x, int y ); */
/*   void times( uiVal_type q ); // multiplies the field with a constant */
/*   void div( uiScalarField & image ); // divides the image with an other image pixelvise */
/*   void setDefaultSink( void );		 */
/*   vint giveSrcPoint( void ); */
/*   void setSrc( uiScalarField & image ); */
/*   void setSink( uiScalarField & image ); */
/*   boost::shared_array<unsigned char> getCharRepresentation(); // I use pointers here because sqlite3, the pointers must be deleted after */
/* //  boost::shared_array<unsigned char> getCharRepresentation( const vint & pos, const vint & size ); */
/*   boost::shared_array<float> getFloatRepresentation(); // I use pointers here because sqlite3, the pointers must be deleted after */
/* //  boost::shared_array<float> getFloatRepresentation( const vint & pos, const vint & size ); */
/*   void apply( uiScalarField & src, const vint & pos);   */
/* }; */

/* typedef boost::shared_ptr<uiScalarField> puiScalarField; */































































#endif /*UIFIBRETYPES_H_*/
#endif /*__cplusplus*/
/* LuM end of file */
