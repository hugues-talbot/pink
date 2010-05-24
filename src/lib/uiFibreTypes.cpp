/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2006-2010
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

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiFibreTypes.cpp: " << msg; call_error(fullmessage.str());}

//Exception handling

void call_error(const string message){
  cout << endl << "error: " << message << endl << endl;
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

//converting a time in seconds to string. It converts up to days
string progressBar::time2string(time_t seconds){
  time_t day = seconds / 86400;
  seconds = seconds % 86400;
  time_t hour = seconds / 3600;
  hour = hour % 3600;
  time_t minute = seconds / 60;
  minute = minute % 60;
  seconds = seconds % 60;

  stringstream ss;
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


void progressBar::maxPos(int maxPos){
  max = maxPos;
}


void progressBar::minPos(int minPos){
  min = minPos;
}


string progressBar::elapsedTime(){
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

string progressBar::remainingTime(){
  if (started) {
    if (measure) {
      int currtime = time(NULL);

      if (pos==min) return "undetermined";
	else {
	  int est_rem = int( 
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

string progressBar::operator << (int currPos){
  pos = currPos;
  last_report = time(NULL);
  return remainingTime();
}

void progressBar::setPos(int currPos){
  pos = currPos;
}

string progressBar::percent(){
  if ((max - min) == 0){
    return "undeterminable max==min";
  } else {
    stringstream ss;
    ss << int(100.*float(pos-min)/float(max - min));
    ss << "%";
    return ss.str();
  }
}

// vval_type object ---------------------------------------------------------------

vval_type::vval_type(){
//intentionally empty
}

vval_type::vval_type( const vval_type & src ): vector<uiVal_type>(src) {  //copy constructor
  /////!!!!!!! cout<< "copying vval_type object unnecessarily" << endl;
}

vval_type::vval_type( int size ): vector<uiVal_type>(size) {
//intentionally empty
}


vval_type::vval_type( int size, int defvalue ): vector<uiVal_type>(size, defvalue){
//intentionally empty
}

vval_type::~vval_type(){
//intentionally empty
}

void vval_type::normate(){ //sets it up the same direction but 1. length.
  int d = size();
  uiVal_type sum = 0.;
  FOR(q,d) sum+=uiSqr( (*this)[q] );
  sum = sqrt(sum);
  FOR(q,d) (*this)[q] /= sum;
}



// vint object---------------------------------------------------------------

vint::vint():vector<int>()  //default constructor
{
  //intentionally left empty
} /* vint::vint */

vint::vint(const vint & src):vector<int>(src)  //copy constructor
{
  //intentionally left empty
} /* vint::vint */

vint::vint(const vint & src, string debug ):vector<int>(src)  //copy constructor
{
  #ifdef UJIMAGE_DEBUG
  this->debug=debug; // representing the name of the object if debugged
  cout << "creating vint '" << debug << "' with address " << static_cast<void*>(this) <<"; with the copy constructor"<< endl;
  #endif /* UJIMAGE_DEBUG */
} /* vint::vint */


// vint::vint(int size):vector<int>(size)
// {
//   //intentionally left empty
// } /* vint::vint */

vint::vint( int size, string debug /*=""*/ ):vector<int>(size)
{
  #if UJIMAGE_DEBUG >= 3
  this->debug=debug; // representing the name of the object if debugged
  cout << "creating vint '" << debug << "' with address " << static_cast<void*>(this) << endl;
  #endif /* UJIMAGE_DEBUG */
} /* vint::vint */

vint::vint(int size, int defvalue):vector<int>(size, defvalue)
{
  // intentionally left empty
} /* vint::vint */

vint::vint( const boost::python::list & src ): vector<int>(boost::python::len(src),0) 
{
  int length=boost::python::len(src);
//  boost::python::object A = src[1];
//  int a = boost::python::extract<int>(A);
  FOR(q, length)
    try
    {
      (*this)[q]=boost::python::extract<int>(src[q]);
    } 
    catch (...) 
    {
      error("the elements of the list must be convertible to integers.");
    }
} /* vint::vint python list */

vint::~vint()
{
  #if UJIMAGE_DEBUG >= 3
  cout << "destroying vint '" << debug << "' with address " << static_cast<void*>(this) << endl;
  #endif /* UJIMAGE_DEBUG >= 3 */
} /* vint::~vint */

void vint::reset()
{
  int q = size()-1;
  do 
  { 
    (*this)[q--]=0; 
  } 
  while (q>=0);
} /* vint::reset */

int vint::prod() const
{
  int prod = 1;
  int q = size()-1;
  do 
  {
    prod*=(*this)[q--];
  } 
  while (q>=0);
  return prod;
} /* vint::prod */

int vint::prodExcept(int p) const
{
  int prod = 1;
  int q = size()-1;
  do { prod*=(*this)[q--]; } while (q>=0);
  return prod/(*this)[p];
} /* vint::prodExcept */

uiVal_type vint::fabs() const
{
  uiVal_type sum = 0;
  int q = this->size()-1;
  do 
  { 
    sum+=uiSqr((*this)[q--]); 
  } 
  while (q>=0);
  return sqrt(sum);
} /* vint::abs */

void vint::nextStep( int step, vint & result ) const { 
  //tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  int d = this->size();
  int prod = step;
  int q = 0;
    
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

  int d = this->size();
  int q = 0;
    
  while ( ( result[q] >= (*this)[q] - 1) and ( q < d-1 ) ) 
  {
    q++;
  }
  
  if ( (q == d-1) and (result[q] >= (*this)[q] - 1) )
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
    if ( (point[q]==0) or (point[q]==(*this)[q]-1) )
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
    if ( (point[q]<0) or (point[q]>=(*this)[q]) )
    {
      return false;	
    } /* if */	
  } /* FOR */
  
  return true;
} /* vint::inside */

bool vint::operator==( const vint & other ) const 
{
  bool result=true;
  int n=size();
  FOR(q, n)
    if ((*this)[q]!=other[q]){
      result=false;
      break;
    }
  return result;  
} /* vint::operator== */

bool vint::operator!=(const vint & other) const 
{
  return (not ((*this)==other));  
} /* vint::operator!= */


int vint::position( const vint & elem ) const 
{
  // tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  int d = this->size();
  int pos = 0;
  int t = 1;
  int q = 0;

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

  FOR(q, int(other.size())) 
  {
    bool is_in = false;

    FOR(w, int(this->size()))
      if ((*this)[w]==other[q]) is_in=true;

    if (not is_in) {
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

//   cout  << "a=" << a.print() << endl;

//   vint b(6);
//   b[0]=5;
//   b[1]=6;
//   b[2]=3;
//   b[3]=6;
//   b[4]=1;
//   b[5]=3;

//   cout << "b=" << b.print() << endl;

//   a.addSet(b);

//   cout  << "a.addSet(b)=" << a.print() << endl;

} /* vint::addSet */


// does the exact same thing as print, but in python it's called "__repr__"
string vint::repr() const
{
  if (size()==0)
  {
    #if UJIMAGE_DEBUG >= 1
    return "'" + this->debug + "' = []";
    #else /* NOT UJIMAGE_DEBUG >= 1 */
    return "[]";
    #endif /* NOT UJIMAGE_DEBUG >= 1 */    
  }
  else /* NOT size()==0 */
  {
    stringstream ss;
    #ifdef UJIMAGE_DEBUG
    ss << "'" << debug << "' = ";
    #endif /* UJIMAGE_DEBUG */
    ss << "[ ";
    
    FOR(q, int(size()-1))
      ss << (*this)[q] << ", ";
    
    ss << (*this)[size()-1] << " ]";
    
    return ss.str();
  } /* NOT size()==0 */
} /* vint::repr */


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

uiDibbles::uiDibbles(int n){
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



int uiDibbles::get_length(void){
  return length;
}



void uiDibbles::addElement(int start, int end){
  
  if (length==size) {
    size += jump;
    ARRAY<uiDibble> tmp(new uiDibble[size]);
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
