/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiCutPlane.cpp: " << msg; call_error(fullmessage.str());}


using namespace pink;

PTR<char_image> py_skeleton_im_int_int_im( 
  const char_image & uj_image, 
  int prio,
  int connex,			     
  const char_image & inhibit
  ) ;

namespace pink {
  
  typedef pair < PTR<vint>, bool > int_res;

  class plane
  {
  public:
    float a,b,c,d;
    plane(float a, float b, float c, float d)
      {
	this->a=a;
	this->b=b;
	this->c=c;
	this->d=d;
      } /* plane */

    string repr() const
      {
	stringstream ss;
	ss << a << "x + " << b << "y + " << c << "z + " << d << " == 0";	
	string result = ss.str();
	return result;	
      } /* repr */
        
  }; /* plane */

  class parallel_segment : public vint
  {
  private: 

    int max;
    PTR<vint> start;
    int dir;
    PTR<plane> cut;

    float distance( int pos )
      {
	if ( (pos<0) or (pos>=max) )
	{
	  error("the point is outside of the segment.");	  
	} /* max */
	
	int p[3];

	p[0]=(*start)[0];
	p[1]=(*start)[1];
	p[2]=(*start)[2];
	p[dir]+=pos;
	
	return cut->a * p[0] + cut->b * p[1] + cut->c * p[2] + cut->d;	

      } /* distance */    

    void set_plane( const plane & new_plane )
      {
	cut.reset( new plane(new_plane) );
      } /* set_point */
    
  public:
    parallel_segment( const vint & start, int dir, int max )
      {
	this->start.reset( new vint(start));
	this->dir=dir;
	this->max=max;
	
      } /* parallel_segment */

    int get_max()
      {
	return max;	
      } /* get_max */
    
    PTR<int_res> intersection( const plane & p )
      {
	
	PTR<int_res> result ( new int_res(PTR<vint> (new vint(3,0)), true ));
	
	// insterting the plane into the segment
	set_plane(p);
		
	// testing if the segment is on the same side of the plane
	
	if ( distance(0) * distance(max-1) > 0 )
	{
	  result->second=false;
	  return result;
	  
	} /* if */
	
	int low=0, high=max-1;
	
	// binary search
	while ( ( low - high > 1 ) or ( low - high < -1) )
	{

	  if (uiAbs(distance(low)) < uiAbs(distance(high)))
	  {
	    high = (low+high)/2;
	  } 
	  else /* NOT fabs(s.distance(low)) < fabs(s.distance(high)) */
	  {
	    low = (low+high)/2;
	  } /* NOT fabs(s.distance(low)) < fabs(s.distance(high)) */      
	} /* while */
	
	result->first.reset(new vint(*start));
	(*result->first)[dir]=low;
	result->second=true;

	return result;
	
      } /* intersection */
    
  }; /* parallel_segment */

  class point3D : public vint 
  {
  public:
    point3D( int x, int y, int z ) : vint(3,-1)
      {
	(*this)[0]=x;
	(*this)[1]=y;
	(*this)[2]=z;	
      } /* point3D */
    
  }; /* class point3D */

  class cube
  {
  private:
    int size_x, size_y, size_z;

    void test_segment( PTR<vector< PTR<vint> > > result, PTR<plane> pl, int p0, int p1, int p2, int dir, int max)
      {
	PTR<int_res> curr;
	PTR<parallel_segment> segment;
	PTR<point3D> curr_start;
	
	curr_start.reset( new point3D(p0,p1,p2) );
	segment.reset( new parallel_segment( *curr_start, dir, max ) ) ;
	curr = segment->intersection(*pl);

	if (curr->second)
	{
	  result->push_back(curr->first);
	} /* if */
	
      } /* test_segment */
    
  public:
    cube(int x, int y, int z)
      {
	size_x=x;
	size_y=y;
	size_z=z;
      } /* cube */

    PTR<vector<PTR<vint> > > intersections( PTR<plane> pl )
      {
	PTR<vector< PTR<vint> > > result( new vector<PTR<vint> >());
	
	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ 0, /* p_z*/ 0, /*dir = */ 0, size_x );
	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ 0, /* p_z*/ 0, /*dir = */ 1, size_y );
	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ 0, /* p_z*/ 0, /*dir = */ 2, size_z );
	
	test_segment(result, /*plane*/ pl,  /*p_x*/ size_x-1, /*p_y*/ 0, /* p_z*/ 0, /*dir = */ 1, size_y );
	test_segment(result, /*plane*/ pl,  /*p_x*/ size_x-1, /*p_y*/ 0, /* p_z*/ 0, /*dir = */ 2, size_z );

	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ 0, /* p_z*/ size_z-1, /*dir = */ 0, size_x );
	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ 0, /* p_z*/ size_z-1, /*dir = */ 1, size_y );
	
	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ size_y-1, /* p_z*/ 0, /*dir = */ 0, size_x );
	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ size_y-1, /* p_z*/ 0, /*dir = */ 2, size_z );
	
	test_segment(result, /*plane*/ pl,  /*p_x*/ size_x-1, /*p_y*/ size_y-1, /* p_z*/ 0, /*dir = */ 2, size_z );
	test_segment(result, /*plane*/ pl,  /*p_x*/ size_x-1, /*p_y*/ 0, /* p_z*/ size_z-1, /*dir = */ 1, size_y );

	test_segment(result, /*plane*/ pl,  /*p_x*/ 0, /*p_y*/ size_y-1, /* p_z*/ size_z-1, /*dir = */ 0, size_x );
	
	return result;	
      }
    
  }; /* cube */
    
  PTR<char_image> draw_plane( const char_image & original, 
			      float a, 
			      float b, 
			      float c, 
			      float d )
  {
    PTR<char_image> inhibit(new char_image(original));
    PTR<plane> pl( new plane(a,b,c,d) );
    
    cube cu( original.get_size()[0],
	     original.get_size()[1],
	     original.get_size()[2] );

    PTR<vector<PTR<vint> > > points;
    
    points = cu.intersections(pl);
    
    FOR(q, points->size())
    {
      DEBUG((*points)[q]->repr());
      
      (*inhibit)[(*(*points)[q])]=255;
    } /* FOR */
    
    // generating blank image
    PTR<char_image> blank( new char_image(original.get_size()) );
    FOR(q, blank->get_size().prod())
    {
      (*blank)[q]=255;
    } /* FOR */

    PTR<char_image> blank_around;
    blank_around = frame_around( *blank, 0 );
    PTR<char_image> inhibit_around;
    inhibit_around = frame_around( *inhibit, 0 );
    

    PTR<char_image> result;
    result = py_skeleton_im_int_int_im ( 
      *blank_around, 
      3 /* exact eucledian distance */, 
      6 /* connexity */, 
      *inhibit_around
      );
    
    return result;
  } /* draw_plane */
  
  
  

 






} /* namespace pink */
/* LuM end of file */

