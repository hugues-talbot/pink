/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2007-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


/** \file uiFibreTypes.cpp
 * \brief The Unit for special types used in the program.
 * 
 * The two main classes are uiScalarField and uiDiffEqField. It also provides some supplementary functions. See detailed description below.
 * \author UjoImro, 2007.
 */
// not my includes
/* Line 12+1 */

// my includes

#include "uiFibreTypes.h"

#undef error
#define error(msg) {std::stringstream fullmessage; fullmessage << "in uiFibreTypes.cpp: " << msg; call_error(fullmessage.str());}

//Exception handling

void call_error(const std::string message){
  std::cout << std::endl << "error: " << message << std::endl << std::endl;
  throw message;
};


// progressBar ---------------------------------------------------------------

progressBar::progressBar(){ 
  max=100;
  min=0; 
  pos=0; 
  started=false;
  measure=false;
} /* progressBar */


progressBar::~progressBar(){
  // intentionally left empty
} /* ~progressBar */

bool progressBar::timeToReport(){
  return ( time(NULL) - last_report ) >= PBAR_REPORT_INTERVALL;
} /* timeToReport */

//converting a time in seconds to std::string. It converts up to days
std::string progressBar::time2string(time_t seconds){
  time_t day = seconds / 86400;
  seconds = seconds % 86400;
  time_t hour = seconds / 3600;
  hour = hour % 3600;
  time_t minute = seconds / 60;
  minute = minute % 60;
  seconds = seconds % 60;

  std::stringstream ss;
  if (day == 1){
    ss << "1 day ";
  } else if (day>1){
    ss << day << " days ";
  }
  ss.fill('0');
  ss.width(2);
  ss << hour << "h:" ;
  
  ss.fill('0');
  ss.width(2);
  ss << minute << "m:";
  
  ss.fill('0');
  ss.width(2);
  ss << seconds << "s";
    
  return ss.str();
}

void progressBar::start() {
  measure = true;
  started = true;
  begin   = time(NULL);
  last_report = begin;
}


void progressBar::stop(){
  finish  = time(NULL);
  measure = false;
}


void progressBar::maxPos(index_t maxPos){
  max = maxPos;
}


void progressBar::minPos(index_t minPos){
  min = minPos;
}


std::string progressBar::elapsedTime(){
  if (started) {
    if (measure){
      return time2string(time(NULL) - begin);
    } else {
      return time2string(finish - begin);
    }
  } else {
    return "progressBar hasn't been started";
  }  
}

index_t progressBar::elapsedSeconds(){
  if (started) {
    if (measure){
      return time(NULL) - begin;
    } else {
      return finish - begin;
    }
  } else {
    return -1;
  }  
}

std::string progressBar::remainingTime(){
  if (started) {
    if (measure) {
      index_t currtime = time(NULL);

      if (pos==min) return "undetermined";
	else {
	  index_t est_rem = index_t( 
	    float( max - pos ) * float( currtime - begin ) / float( pos - min )
	    );
	  return time2string(est_rem);
	}
    } else {
      return time2string(0);
    }
  } else {
    return "progressBar hasn't been started";    
  }
}

std::string progressBar::operator << (index_t currPos){
  pos = currPos;
  last_report = time(NULL);
  return remainingTime();
}

void progressBar::setPos(index_t currPos){
  pos = currPos;
}

std::string progressBar::percent(){
  if ((max - min) == 0){
    return "undeterminable max==min";
  } else {
    std::stringstream ss;
    ss << index_t(100.*float(pos-min)/float(max - min));
    ss << "%";
    return ss.str();
  }
}

// vval_type object ---------------------------------------------------------------

vval_type::vval_type(){
//intentionally empty
}

vval_type::vval_type( const vval_type & src ): std::vector<uiVal_type>(src) {  //copy constructor
  /////!!!!!!! std::cout<< "copying vval_type object unnecessarily" << endl;
}

vval_type::vval_type( index_t size ): std::vector<uiVal_type>(size) {
//intentionally empty
}


vval_type::vval_type( index_t size, index_t defvalue ): std::vector<uiVal_type>(size, defvalue){
//intentionally empty
}

vval_type::~vval_type(){
//intentionally empty
}

void vval_type::normate(){ //sets it up the same direction but 1. length.
  index_t d = size();
  uiVal_type sum = 0.;
  FOR(q,d) sum+=uiSqr( (*this)[q] );
  sum = sqrt(sum);
  FOR(q,d) (*this)[q] /= sum;
}



// vindex_t object---------------------------------------------------------------

vint::vint():std::vector<index_t>()  //default constructor
{
  //intentionally left empty
} /* vint::vint */

vint::vint(const vint & src):std::vector<index_t>(src)  //copy constructor
{
  //intentionally left empty
} /* vint::vint */

vint::vint(const vint & src, std::string debug ):std::vector<index_t>(src)  //copy constructor
{
# if UJIMAGE_DEBUG >= 6
  this->debug=debug; // representing the name of the object if debugged
  std::cout << "creating vint '" << debug << "' with address " << static_cast<void*>(this) <<"; with the copy constructor"<< std::endl;
# endif /* UJIMAGE_DEBUG */
} /* vint::vint */


// vint::vint(int size):std::vector<int>(size)
// {
//   //intentionally left empty
// } /* vint::vint */

vint::vint( index_t size, std::string debug /*=""*/ ):std::vector<index_t>(size)
{
# if UJIMAGE_DEBUG >= 6
  this->debug=debug; // representing the name of the object if debugged
  std::cout << "creating vint '" << debug << "' with address " << static_cast<void*>(this) << std::endl;
# endif /* UJIMAGE_DEBUG */
} /* vint::vint */

vint::vint(index_t size, index_t defvalue):std::vector<index_t>(size, defvalue)
{
  // intentionally left empty
} /* vint::vint */

vint::vint( const boost::python::list & src ): std::vector<index_t>(boost::python::len(src),0) 
{
  index_t length=boost::python::len(src);
//  boost::python::object A = src[1];
//  int a = boost::python::extract<int>(A);
  FOR(q, length)
    try
    {
      (*this)[q]=boost::python::extract<index_t>(src[q]);
    } 
    catch (...) 
    {
      error("the elements of the list must be convertible to integers.");
    }
} /* vint::vint python list */

vint::~vint()
{
# if UJIMAGE_DEBUG >= 6
  std::cout << "destroying vint '" << debug << "' with address " << static_cast<void*>(this) << std::endl;
# endif /* UJIMAGE_DEBUG >= 3 */
} /* vint::~vint */

void vint::reset()
{
  index_t q = size()-1;
  do 
  { 
    (*this)[q--]=0; 
  } 
  while (q>=0);
} /* vint::reset */

index_t vint::prod() const
{
  index_t prod = 1;
  index_t q = size()-1;
  do 
  {
    prod*=(*this)[q--];
  } 
  while (q>=0);
  return prod;
} /* vint::prod */

index_t vint::prodExcept(index_t p) const
{
  index_t prod = 1;
  index_t q = size()-1;
  do { prod*=(*this)[q--]; } while (q>=0);
  return prod/(*this)[p];
} /* vint::prodExcept */

uiVal_type vint::fabs() const
{
  uiVal_type sum = 0;
  index_t q = this->size()-1;
  do 
  { 
    sum+=uiSqr((*this)[q--]); 
  } 
  while (q>=0);
  return sqrt(sum);
} /* vint::abs */

void vint::nextStep( index_t step, vint & result ) const { 
  //tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  index_t d = this->size();
  index_t prod = step;
  index_t q = 0;
    
  while ( q <= d - 1 ){
            
    if ( (*this)[q] > 0 )
    {
      result[q] = prod % (*this)[q];
    }
                
    if ( prod > 0 )
    {
      prod = prod / (*this)[q];
    }

    q++;
  }
} /* vint::nextStep */

bool vint::nextStep_it( vint & result ) const {
  // tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  index_t d = this->size();
  index_t q = 0;
    
  while ( ( result[q] >= (*this)[q] - 1) && ( q < d-1 ) ) 
  {
    q++;
  }
  
  if ( (q == d-1) && (result[q] >= (*this)[q] - 1) )
  {
    return false;
  }
  
  result[q] += 1;
  
  while (q > 0) 
  {
    q--;
    result[q]=0;    
  }
  
  return true;
} /* vint::nextStep_it */

bool vint::on_side( const vint & point ) const
{
  if (point.size() != this->size())
  {
    error("The 'point' must have same dimension (length) than the 'vector'.");
  } /* if */
  
  FOR(q, this->size())
  {
    if ( (point[q]==0) || (point[q]==(*this)[q]-1) )
    {
      return true;	
    } /* if */	
  } /* FOR */
  
  return false;
} /* vint::on_side */

bool vint::inside( const vint & point ) const 
{
  FOR(q, this->size())
  {
    if ( (point[q]<0) || (point[q]>=(*this)[q]) )
    {
      return false;	
    } /* if */	
  } /* FOR */
  
  return true;
} /* vint::inside */

bool vint::operator==( const vint & other ) const 
{
  bool result=true;
  index_t n=size();
  FOR(q, n)
    if ((*this)[q]!=other[q]){
      result=false;
      break;
    }
  return result;  
} /* vint::operator== */

bool vint::operator!=(const vint & other) const 
{
  return (!((*this)==other));  
} /* vint::operator!= */


index_t vint::position( const vint & elem ) const 
{
  // tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  index_t d = this->size();
  index_t pos = 0;
  index_t t = 1;
  index_t q = 0;

  while ( q <= d - 1 ) {
    pos += elem[q] * t;
    t *= (*this)[q];
    q+=1;
  }
    
  return pos;
} /* vint::position */

bool vint::addSet( const vint & other )
{

  bool result=false;

  FOR(q, index_t(other.size())) 
  {
    bool is_in = false;

    FOR(w, index_t(this->size()))
      if ((*this)[w]==other[q]) is_in=true;

    if (!is_in) {
      this->push_back(other[q]);
      result=true;
    }

  }

  return result;

// EXAMPLE:
//   vint a(4);
//   a[0]=1;
//   a[1]=2;
//   a[2]=3;
//   a[3]=4;

//   std::cout  << "a=" << a.print() << endl;

//   vint b(6);
//   b[0]=5;
//   b[1]=6;
//   b[2]=3;
//   b[3]=6;
//   b[4]=1;
//   b[5]=3;

//   std::cout << "b=" << b.print() << endl;

//   a.addSet(b);

//   std::cout  << "a.addSet(b)=" << a.print() << endl;

} /* vint::addSet */


// does the exact same thing as print, but in python it's called "__repr__"
std::string vint::repr() const
{
  if (size()==0)
  {
#   if UJIMAGE_DEBUG >= 1
    return "'" + this->debug + "' = []";
#   else /* NOT UJIMAGE_DEBUG >= 1 */
    return "[]";
#   endif /* NOT UJIMAGE_DEBUG >= 1 */    
  }
  else /* NOT size()==0 */
  {
    std::stringstream ss;
#   ifdef UJIMAGE_DEBUG
    ss << "'" << debug << "' = ";
#   endif /* UJIMAGE_DEBUG */
    ss << "[ ";
    
    FOR(q, index_t(size()-1))
      ss << (*this)[q] << ", ";
    
    ss << (*this)[size()-1] << " ]";
    
    return ss.str();
  } /* NOT size()==0 */
} /* vint::repr */


vint & vint::operator<< ( const index_t & initiator )
{
//   #if UJIMAGE_DEBUG >=1
//   if ( this->size()!=initiator.size())
//   {
//     error("you are trying to assign a vector of different size ");
//   } /* this->size()!=initiator.size() */
//   #endif /* UJIMAGE_DEBUG */

  this->clear();
  this->push_back(initiator);
  return *this;  
} /* vint::operator<< */

vint & vint::operator, ( const index_t & next )
{
  this->push_back(next);
  return *this;
} /* vint::operator, */


/// ------------------------------------------------
/// end of vint
/// ------------------------------------------------


/**
 * Calculates the absolute value of x.
 * @param x uiVal_type (float or double)
 * @return absolute value
 */
uiVal_type uiAbs( uiVal_type x )
{
  if ( x < 0 ) 
  { 
    return -x; 
  }
  else /* NOT x < 0 */
  { 
    return x; 
  } /* NOT x < 0 */
} /* uiAbs */




/**
 * Calculates the square of x.
 * @param x uiVal_type (float or double)
 * @return square of x
 */
uiVal_type uiSqr( uiVal_type x )
{
  return x*x;
} /* uiSqr */

// ---------------------- uiDibble -----------------------------------

uiDibbles::uiDibbles(index_t n){
  length=0;
  if (n<jump) size=jump; else size=n;
  values.reset(new uiDibble[size]);
//   length=0;
//   if (n<jump) size=jump; else size=n;
//   values=(uiDibble*)calloc(size,sizeof(uiDibble));
}

uiDibbles::uiDibbles(void){
  length=0;
  size=jump;
  values.reset(new uiDibble[size]);

//   length=0;
//   size=jump;
//   values=(uiDibble*)calloc(size,sizeof(uiDibble));
}

uiDibbles::~uiDibbles(void){
  length=0; // setting 0 for no confusion at errorneus read
//  free(values); //will be freed automaticly by boost::shared_array
}



index_t uiDibbles::get_length(void){
  return length;
}



void uiDibbles::addElement(index_t start, index_t end){
  
  if (length==size) {
    size += jump;
    boost::shared_array<uiDibble> tmp(new uiDibble[size]);
    FOR(q, length) tmp[q]=values[q];
    values.swap(tmp); // tmp will be destroyed at the end of the "if"
  };
  length++;
  values[length-1].start=start;
  values[length-1].end=end;








//   if (length==size) {
//     size += jump;
//     ARRAY<uiDibble> * newvalues = (uiDibble*)malloc(size*sizeof(uiDibble));
//     for (int q = 0; q<=length - 1; q++ ) newvalues[q]=values[q];
//     free(values);
//     values=newvalues;
//   };
//   length++;
//   values[length-1].start=start;
//   values[length-1].end=end;
}

// ---------------------- end of uiVector ----------------------------

/* LuM end of file */
