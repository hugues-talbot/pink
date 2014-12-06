/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, x2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include "ui_pink_types.hpp"

#include <gsl/gsl_interp.h>
#ifdef PINK_HAVE_PYTHON
# include <gsl/gsl_multifit_nlin.h>
#endif /* PINK_HAVE_PYTHON */

#include <Eigen/LU>
#include <Eigen/Core>
#include <Eigen/Geometry>
#ifdef PINK_HAVE_NUMPY
# include <boost/python/list.hpp>
# include <boost/python/extract.hpp>
#endif /* PINK_HAVE_NUMPY */

#include "ujimage.hpp"

namespace pink {
  
  index_t tmp_round(float x)
  {
    index_t result=x;
    return x;
    
  } /* tmp_round */
  

  class v3d : public Eigen::Vector3d 
  {
  public:
    v3d( double x, double y, double z) {
      (*this)[0]=x;
      (*this)[1]=y;
      (*this)[2]=z;
    } /* v3d */
    

    v3d( const Eigen::VectorXd & other ) {
      for (index_t q=0; q<3; q++) (*this)[q] = other[q];
    } /* v3d */

#   ifdef PINK_HAVE_PYTHON
    v3d( const boost::python::list & src )
      {
	if ( boost::python::len(src) < 3 )
	{
	  pink_error("the list is too short for 3D vector");
	} /* if */
	FOR(q, 3)
	{
	  try
	  {
	    (*this)[q]=boost::python::extract<float>(src[q]);
	  } 
	  catch (...) 
	  {
	    pink_error("the elements of the list must be convertible to doubles.");
	  }
	} /* FOR */
      } /* v3d */
#   endif /* PINK_HAVE_PYTHON */

        
  }; /* class v3d */

  // generates a simple rotation matrix. The input are the unit vectors and the output is
  // the rotation with z axe left unchanged
  Eigen::MatrixXd simple_rotation( double alpha )
  {
    Eigen::MatrixXd result(4,4);
    
    result << 
      cos(alpha), -sin(alpha), 0., 0.,
      sin(alpha),  cos(alpha), 0., 0.,
      0.        ,  0.        , 1., 0.,
      0.        ,  0.        , 0., 1.;
    
    return result;    
  } /* simple_rotation */
  


  Eigen::MatrixXd ortho_normalize( Eigen::MatrixXd simple )
  {

    //DVECT(simple);
    

    Eigen::MatrixXd result(4,4);

    Eigen::Vector3d reference;
    Eigen::Vector3d orient;
    Eigen::Vector3d axe;

    orient << simple(0,0), simple(1,0), simple(2,0);
    //DVECT(orient);
    
    // reference is autogenerated
    axe << simple(0,2), simple(1,2), simple(2,2);
    //DVECT(axe);
    
    
    v3d new_reference = v3d(orient.cross(axe));
    v3d new_orient = v3d(axe.cross(new_reference));
    
    new_orient.normalize();
    new_reference.normalize();
    axe.normalize();
    

    result << 
      new_orient.transpose(), 1,
      new_reference.transpose(), 1, 
      axe.transpose(), 1,
      simple(0,3), simple(1,3), simple(2,3), simple(3,3);
    


    //DVECT(result);
    
    
    return result.transpose();
    
  } /* normalize */
  
    

  // !!!!!!!!!! CHECK MEE!!!
  
  char_image draw_plane( 
    const char_image & original, 
    float a, 
    float b, 
    float c, 
    float d )
  {
    
    char_image result;
    result = original.clone();

    for ( auto pixel = result.begin(); pixel != result.end(); ++pixel ) 
      if ( a * pixel.row() + b * pixel.col() + c * pixel.depth() + d < 1 ) *pixel = 255;
      
    return result;    
  } /* draw_plane */


# ifdef PINK_HAVE_PYTHON
  char_image project_plane( const char_image & src,
                            const boost::python::list & A,
                            const boost::python::list & B,
//			    const boost::python::list & shift,
                            double alpha
    )
  {
    
    char_image result(src.rows(), src.cols());
    
    v3d pA(A);
    v3d pB(B);
    v3d pshift(A);

    //DVECT(pA);
    //DVECT(pB);
    //DVECT(pshift);
    
      

    v3d axe = v3d(pB - pA);

    //cout << "parameter"; DVECT(axe);
    

    Eigen::MatrixXd P1(4,4), P2(4,4), P3(4,4);
    P1 << 
      1.,   0.,  0.,  1., /* orient */
      -1., -1., -1., -1., /* reference */
      axe.transpose(),            1., /* axe */
      0.,   0.,  0.,  1.;

    P2 = P1.transpose();
    
    
    //DVECT(P2);
    

    P3 = ortho_normalize(P2);

    //DVECT(P3);

    Eigen::MatrixXd image(4,4);
    image << 
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      1,1,1,1;
    
    Eigen::MatrixXd T1(4,4), T2(4,4);
    
    // this transformation takes the unity base to the 
    // beeds
   
    //DVECT(image.inverse());
    //DVECT(image);
 
    T1=P3*image.inverse();

    //DVECT(T1);
    

    Eigen::MatrixXd transformation;
    T2 = T1.inverse();

    //DVECT(T2);
    //v3d(0,0,0,1);
    //Eigen::VectorXd test(4), res(4);
    //test << 0,1,0,1;
    //DVECT(test);
    
    //DVECT(T2*test);


    
    Eigen::MatrixXd R1(4,4);
    
    R1 = simple_rotation(alpha);

    //DVECT(R1);

    Eigen::MatrixXd B1(4,4);

    
    // this projection (B1) rotates the base around the axe (pA pB) 
    B1 = T1 * R1 * T2;
    
    //DVECT(B1);

    Eigen::MatrixXd new_base;
    new_base = B1*P3;
    // at this point projection holds the new base of the cut
    //DVECT(new_base);

    new_base.col(2).swap(new_base.col(1));

    //DVECT(new_base);

    new_base(0,0)+=pA(0);
    new_base(1,0)+=pA(1);
    new_base(2,0)+=pA(2);
    
    new_base(0,1)+=pA(0);
    new_base(1,1)+=pA(1);
    new_base(2,1)+=pA(2);
    
    new_base(0,2)+=pA(0);
    new_base(1,2)+=pA(1);
    new_base(2,2)+=pA(2);
    

    new_base(0,3)+=pA(0);
    new_base(1,3)+=pA(1);
    new_base(2,3)+=pA(2);
    
    //DVECT(new_base);
        

    Eigen::MatrixXd TR = new_base * image.inverse();

//     DVECT(image.inverse());
    
    
    //DVECT(TR);
    
    
    //Eigen::VectorXd test(4);
    //test << 1,0,0,1;
    //DVECT(test);
    //DVECT(TR*test);

    //test << 0,1,0,1;
    //DVECT(test);
    //DVECT(TR*test);

    //test << 0,0,1,1;
    //DVECT(test);
    //DVECT(TR*test);

    //test << 0,0,0,1;
    //DVECT(test);
    //DVECT(TR*test);

    //test << 100,120,0,1;
    //DVECT(test);
    //DVECT(TR*test);


    Eigen::VectorXd point(4);
    Eigen::VectorXd inpoint(4);
    std::vector<index_t> curr(3);
    std::vector<index_t> pcurr(2);
    
    FOR(q, result.rows())
    {
      FOR(w, result.cols())
      {
	point << q, w, 0, 1;
	inpoint = TR*point;
	//DVECT(point);
	
	curr = { tmp_round(inpoint[0]), tmp_round(inpoint[1]), tmp_round(inpoint[2]) };

	//_DEBUG(curr.repr());
	
	if (pink::inside(src.size(), curr))
	{
	  pcurr = { q, w };
	  // _DEBUG(pcurr.repr());
	  
	  result(pcurr)=src(curr);
	} /* src.inside(curr) */
	

      } /* FOR */
    } /* FOR */
    



    return result;
    
  } /* project_plane */
# endif /* PINK_HAVE_PYTHON */
  
 






} /* namespace pink */
/* LuM end of file */

