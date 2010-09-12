/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, x2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <eigen2/Eigen/Core>
#include <eigen2/Eigen/LU>
#include <eigen2/Eigen/Geometry>

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiCutPlane.cpp: " << msg; call_error(fullmessage.str());}

USING_PART_OF_NAMESPACE_EIGEN

#define DVECT(x) cout << "----------------\n" << BOOST_PP_STRINGIZE(x) << " = \n" << x << "\n"


namespace pink {

  
  int tmp_round(float x)
  {
    int result=x;
    return x;
    
  } /* tmp_round */
  

  class v3d : public Vector3d 
  {
  public:
    v3d( double x, double y, double z)
      {
	(*this)[0]=x;
	(*this)[1]=y;
	(*this)[2]=z;
      } /* v3d */
    

    v3d( const VectorXd & other )      
      {
	FOR(q, 3)
	{
	  (*this)[q]=other[q];	  
	} /* FOR */
	
      } /* v3d */
    v3d( const boost::python::list & src )
      {
	if ( boost::python::len(src) < 3 )
	{
	  error("the list is too short for 3D vector");
	} /* if */
	FOR(q, 3)
	{
	  try
	  {
	    (*this)[q]=boost::python::extract<float>(src[q]);
	  } 
	  catch (...) 
	  {
	    error("the elements of the list must be convertible to doubles.");
	  }
	} /* FOR */
      } /* v3d */

        
  }; /* class v3d */

  // generates a simple rotation matrix. The input are the unit vectors and the output is
  // the rotation with z axe left unchanged
  MatrixXd simple_rotation( double alpha )
  {
    MatrixXd result(4,4);
    
    result << 
      cos(alpha), -sin(alpha), 0., 0.,
      sin(alpha),  cos(alpha), 0., 0.,
      0.        ,  0.        , 1., 0.,
      0.        ,  0.        , 0., 1.;
    
    return result;    
  } /* simple_rotation */
  


  MatrixXd ortho_normalize( MatrixXd simple )
  {

    //DVECT(simple);
    

    MatrixXd result(4,4);

    Vector3d reference;
    Vector3d orient;
    Vector3d axe;

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
  
    

  PTR<char_image> draw_plane( 
    const char_image & original, 
    float a, 
    float b, 
    float c, 
    float d )
  {
    // PTR<char_image> plane(new char_image(original.get_size()));
  
    PTR<char_image> result(new char_image(original));

    vint curr( result->get_size().size(), "curr"  );
    
    FOR(q, result->get_size().prod())
    {
      result->get_size().nextStep( q, curr );
      
//       if ( uiAbs(a*curr[0]+b*curr[1]+c*curr[2]+d) 
// 	   / sqrt( a*a + b*b + c*c ) <= 0.5 )
//       {
//     	(*plane)[q]=255;	
//       } /* if */

    if ( a*curr[0]+b*curr[1]+c*curr[2]+d  > 0 )
    {
      (*result)[q]=0;	
    } /* if */

      
    } /* FOR */
    
    // result = immap_min( original, *plane );

    return result;
  } /* draw_plane */

  
  PTR<char_image> project_plane( const char_image & src,
				 const boost::python::list & A,
				 const boost::python::list & B,
//				 const boost::python::list & shift,
				 double alpha
    )
  {

    vint size2D;
    size2D << src.get_size()[0], src.get_size()[1];
    
    PTR<char_image> result(new char_image(size2D));
    
    v3d pA(A);
    v3d pB(B);
    v3d pshift(A);

    //DVECT(pA);
    //DVECT(pB);
    //DVECT(pshift);
    
      

    v3d axe = v3d(pB - pA);

    //cout << "parameter"; DVECT(axe);
    

    MatrixXd P1(4,4), P2(4,4), P3(4,4);
    P1 << 
      1.,   0.,  0.,  1., /* orient */
      -1., -1., -1., -1., /* reference */
      axe.transpose(),            1., /* axe */
      0.,   0.,  0.,  1.;

    P2 = P1.transpose();
    
    
    //DVECT(P2);
    

    P3 = ortho_normalize(P2);

    //DVECT(P3);

    MatrixXd image(4,4);
    image << 
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      1,1,1,1;
    
    MatrixXd T1(4,4), T2(4,4);
    
    // this transformation takes the unity base to the 
    // beeds
   
    //DVECT(image.inverse());
    //DVECT(image);
 
    T1=P3*image.inverse();

    //DVECT(T1);
    

    MatrixXd transformation;
    T2 = T1.inverse();

    //DVECT(T2);
    //v3d(0,0,0,1);
    //VectorXd test(4), res(4);
    //test << 0,1,0,1;
    //DVECT(test);
    
    //DVECT(T2*test);


    
    MatrixXd R1(4,4);
    
    R1 = simple_rotation(alpha);

    //DVECT(R1);

    MatrixXd B1(4,4);

    
    // this projection (B1) rotates the base around the axe (pA pB) 
    B1 = T1 * R1 * T2;
    
    //DVECT(B1);

    MatrixXd new_base;
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
        

    MatrixXd TR = new_base * image.inverse();

//     DVECT(image.inverse());
    
    
    //DVECT(TR);
    
    
    //VectorXd test(4);
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


    VectorXd point(4);
    VectorXd inpoint(4);
    vint curr;
    vint pcurr;
    
    FOR(q, result->get_size()[0])
    {
      FOR(w, result->get_size()[1])
      {
	point << q, w, 0, 1;
	inpoint = TR*point;
	//DVECT(point);
	
	curr << 
	  tmp_round(inpoint[0]), 
	  tmp_round(inpoint[1]),
	  tmp_round(inpoint[2]);

	//_DEBUG(curr.repr());
	
	if (src.get_size().inside(curr))
	{
	  pcurr << q,w;
	  // _DEBUG(pcurr.repr());
	  
	  (*result)[pcurr]=src[curr];
	} /* src.inside(curr) */
	

      } /* FOR */
    } /* FOR */
    



    return result;
    
  } /* project_plane */
  
 






} /* namespace pink */
/* LuM end of file */

