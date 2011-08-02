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
/** \file uiFibreTypes.h
 * \brief Header file for uiFibreTypes.cpp
 * 
 * Declaration of special types. The purpose of the type declaration is the universality. I want the program to be size- and dimension independent.
 */

/* Line 12+1 */
/* 
uiFibreTypes.h
declaration of special types. The purpose of the type 
declaration is the universality. I want the program to 
be size-independent
*/
//#define PTR boost::shared_ptr
//#define ARRAY boost::shared_array

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/python.hpp>
#include <boost/smart_ptr.hpp>

#include "mcimage.h"
#include "sqlite3.h"

// using boost::lexical_cast;
// using std::string;
// using std::cout;
// using std::stringstream;
// using std::endl;
// using std::vector;
// using std::fstream;
// using std::pair;
// using std::ofstream;
// using std::ios_base;
// using std::ifstream;
// using std::stringstream;





#ifdef UJIMAGE_DEBUG
  #define DEBUG(x) std::cout << "debug: " << BOOST_PP_STRINGIZE(x) << " = " << x << "\n"
  #define PRINTIMAGE(x)							\
    std::cout << "writing image: " << BOOST_PP_STRINGIZE(x) << " as '" << BOOST_PP_STRINGIZE(x) << ".pgm'\n"; \
    x->_writeimage(BOOST_PP_STRINGIZE(x)".pgm")

#else
  #define DEBUG(x)
  #define PRINTIMAGE(x)
#endif /* UJIMAGE_DEBUG */

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

#define ClassDef(Type)                          \
  class T##Type;                                \
  typedef T##Type * P##Type

#define StructDef(Type)                         \
  typedef struct T##Type;                       \
  typedef T##Type * P##Type

#define EPSILON 0.01

#define round(z) ((z-(double)((int)z))<0.5?((int)z):((int)z+1))

typedef char *   pchar;
typedef char ** ppchar;

typedef sqlite3 * psqlite3;

typedef sqlite3_blob *   psqlite3_blob;
typedef sqlite3_blob ** ppsqlite3_blob;

typedef sqlite3_stmt *   psqlite3_stmt;
typedef sqlite3_stmt ** ppsqlite3_stmt;

//typedef float uiVal_type; /* !!! Still assumed to be a simple type e.g. automatically copyes the value*/
#define uiVal_type float
//#define uiVal_type double


// The coresponding image for maxflow calculations
#define UVT_image float_image
//#define UVT_image double_image


template <class T0, class T1> 
inline T0 _max( T0 t0, T1 t1 )
{
  return t0 > t1 ? t0 : t1;
} /* _max */

template <class T0, class T1> 
inline T0 _min( T0 t0, T1 t1 )
{
  return t0 < t1 ? t0 : t1;
} /* _min */



class vval_type: public std::vector<uiVal_type> {
public:
  vval_type();

  vval_type( const vval_type & src ); //copy constructor
  vval_type( index_t size );
  vval_type( index_t size, index_t defvalue );
  ~vval_type();
  void normate(); //sets it up the same direction but 1. length.
};

class vint: public std::vector<index_t>
{
#ifdef UJIMAGE_DEBUG
private:
  std::string debug;
#endif /* UJIMAGE_DEBUG */

public:
  vint();
  vint( const vint & src ); //copy constructor
  vint(const vint & src, std::string debug); // copy constructor with debugging
  vint( index_t size, std::string debug="" );
  vint( index_t size, index_t defvalue );
  vint( const boost::python::list & src );
  ~vint();
  void reset();
  index_t prod() const;
  index_t prodExcept( index_t p ) const;
  uiVal_type fabs() const;
  std::string repr() const;
  void nextStep( index_t step, vint & result ) const;
  bool on_side( const vint & point ) const;
  bool inside( const vint & ) const;
  //bool inside( const boost::python::list & point ) const;
  bool nextStep_it( vint & result ) const;
  bool operator==( const vint & other ) const;
  bool operator!=( const vint & other ) const;
  index_t position( const vint & elem ) const;
  bool addSet( const vint & other );
  vint & operator << ( const index_t & initiator );
  vint & operator,   ( const index_t & next );
}; /* vint */

template <class im_type>
class uiVector
{
public:
  boost::shared_array<im_type> values;
  index_t length;
  uiVector(index_t length);
  uiVector(const std::vector<im_type> & src); // copy constructor
  ~uiVector();
}; /* uiVector */



template <class im_type>
uiVector<im_type>::uiVector(index_t len){
  values.reset(new im_type[len]);
};



template <class im_type>
uiVector<im_type>::~uiVector(){
//  free(values); //will be freed automaticly by boost:shared_array
  length=0; // setting 0 for no confusion at errorneus read
};



template <class im_type>
uiVector<im_type>::uiVector(const std::vector<im_type> & src){
  length = src.size();
  values.reset(new im_type[length]);
  FOR(q, length)
    values[q]=src[q];
};


typedef struct {
  index_t start;
  index_t end;
} uiDibble;

// jump is for reducing the number of allocation in the dibbling. about to be tuned
#define jump 10000
// maxdibble is the size allocation for the constrain. It should never depass L1-data-10%
// on core2 duo it is about 7000 for v + g + 3 * dimension * f
#define MaxDibble 500
// maximal dimension supposed in compile-time
#define compileDim 4

//not yet used
#define MinDibble 100

class progressBar {
private:
  index_t max, min, pos;
  bool measure;
  bool started;
  time_t begin, finish;
  time_t last_report;
  std::string time2string(time_t seconds);
//  start_time
public:
  progressBar();
  ~progressBar();
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
  
};

class uiDibbles {
private:
  index_t size;
  index_t length;
public:
  boost::shared_array<uiDibble> values;
  uiDibbles(index_t n);
  uiDibbles();
  ~uiDibbles();
  void addElement(index_t start, index_t end);
  index_t get_length();
};

void call_error(const std::string message);

#define pink_error(message)                                             \
  {                                                                     \
  std::cerr << std::endl << "Pink error!" << std::endl << "in '" << __FILE__<< "' (line " << __LINE__ << "): " << message << std::endl << std::endl; \
  throw "Pink error!\n";                                                \
  }


uiVal_type uiAbs( uiVal_type x );

//uiVal_type vAbs(vector<uiVal_type> & v);
//void vTimes(vector<uiVal_type> & v, uiVal_type x); //// it multiplies every element of the vector
//int vProdExcept(vint * v, int p); //// gives the product of all but p-th element

uiVal_type uiSqr( uiVal_type x );
//void vReset(vint & v);

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
