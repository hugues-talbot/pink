/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2014
  ProCarPlan s.r.o.
  ujoimro@gmail.com
  
  (C) UjoImro, 2007-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/** \file uiGradient.cpp
 * \brief This is a unit for calculating the gradient of the image. It uses the capabilites of GNU's gsl.
 *
 * The Unit calculates the gradient of an image. It is used, to calculate the measure field for integration.
 * \author UjoImro, 2007-2011, 2014
 */

#include <vector>
#include <gsl/gsl_interp.h>
#include <boost/smart_ptr.hpp>

#include "ujimage.hpp"
#include "ui_pink_types.hpp"


// general gradient ---------------------------------------------------------------------------------

namespace pink
{ 

/**
  This function is used to iterate over a fields sides. That is to say you give it the "step" and the "coord"inate 
  to ignore, and the result is point by point a variation of the remaining coordinates. there are prodExcept(coord) 
  coordinates on the lower side.
*/
  std::vector<index_t> 
  uiStep( const std::vector<index_t> & dim, index_t step, index_t coord )
  {
    index_t d = dim.size();
  
    std::vector<index_t> result( d,0);
    std::vector<index_t> slist  (d-1,0);
    std::vector<index_t> partres(d-1,0);

    FOR( q, coord ) slist[q]=dim[q];

    for ( index_t q = coord + 1; q <= d - 1; q++ ) slist[q-1] = dim[q];

    pink::next_step( slist, step, partres ) ;
  
    FOR(q, coord) result[q]=partres[q];

    result[coord]=0;

    for ( index_t q = coord + 1; q <= d - 1; q++ ) result[q] = partres[q - 1];
    
    return result;
  } /* uiStep */


/**
   \brief Calculates the derivate square of the image.

   It takes the vector and replaces the values with the derivate of an
   interpolating spline.
   
   \param values input vector
   
   \param result result vector
   
   \note if the size of the vector is odd for some strange reason I
   don't know gnu gsl does not want to derivate it, so if odd lenght is
   detected, one additional element is adjusted, with a value of the
   original last element, afterward the last derivate result is just
   ignored.  for large images it should not cause any algorithmical
   (formal) problems.
*/
  std::vector<double>
  uiDerivateVect( const std::vector<double> & values )
  {
  
    index_t d = values.size();
    std::vector<double> result(d);  
    std::vector<double> x(d);

    FOR(q, d) x[q] = q;
      
    gsl_interp * interp = gsl_interp_alloc( gsl_interp_cspline, d );//gsl_interp_akima, d );//gsl_interp_cspline, d );
    gsl_interp_init( interp, x.data(), values.data(), d );
    gsl_interp_accel * accel = gsl_interp_accel_alloc();
    FOR(q,d) result[q]=/*pow(*/ gsl_interp_eval_deriv(interp, x.data(), values.data(), q, accel /* ),2));*/);
  
    gsl_interp_free( interp );
    gsl_interp_accel_free( accel );
    return result;
  } /* uiDerivateVect */

/**
   \brief Calculates the absolute value of the directional derivates.
   \param f input image
   \return a new scalar field with the derivates.
*/
  float_image
  uiGradientAbs( const float_image & f )
  {
#   ifdef UJIMAGE_DEBUG
    std::cout << "Calculating general gradient\n";
#   endif /* UJIMAGE_DEBUG */
    index_t d = f.size().size();
    float_image result(f.size());
  
    FOR( q, d )
    {
      FOR(w, pink::prod_except(f.size(), q))
      {
        std::vector<index_t> coord(uiStep(f.size(), w, q ));
        std::vector<double>  vect(f.size()[q]);

        FOR( e, f.size()[q] )
        {
          coord[q] = e;
          vect[e]  = f(coord);
        }
        std::vector<double> der = uiDerivateVect(vect);
        FOR( e, f.size()[q] )
        {          
          coord[q]=e;
          result(coord) += (der[e] * der[e]); // needed to be added because square is not square any more
        } /* FOR( e, f ) */;
      } /* FOR(w, f.size().prodExcept(q) ) */;
    } /*   FOR( q, d ) */

    return result;
  } /* uiGradientAbs */

  boost::python::object
  pyuiGradientAbs( boost::python::object pyimage ) {
    float_image image(pyimage);

    float_image result = uiGradientAbs(image);

    return result.steel();    
  }
  

  
// directional gradient ------------------------------------------------------------------------------------------------

  const double Pi = 3.141592653589793;

  const double c1 = 1./(sqrt(2.*Pi));

/**
 * Simulates the gauss (normal) density function
 * @param x point
 * @param m expected value
 * @param sigma humidity
 * @return N(m,sigma)(x)
 */
  inline double
  gauss( const double & x, const double & m, const double & sigma ){
    return c1/sigma*exp(-sqr(x-m)/(2.*sqr(sigma)));
  };


/**
 * Returns the distance between a plane and a point. The plane is defined with it's normal, and assumed to contain the origo [0,0,0].
 * @param n the normal vector
 * @param p the point
 * @param dim the dimension
 * @return the distance between the plane and the point
 * \note The dimension must always be 3. (others are not implemented)
 * \note the distance is directional
 */
  inline double
  uiDPointPlane( const std::vector<double> & n/*ormal*/, const std::vector<double> & p/*oint*/, const index_t & dim ) {
    return (n[0]*p[0]+n[1]*p[1]+n[2]*p[2])/sqrt(sqr(n[0])+sqr(n[1])+sqr(n[2]));
  };

/**
 * Returns the distance between a line and a point. The line is defined by its direction vector and assumed to contain the origo [0,0,0]
 * @param d direction vector
 * @param p the point
 * @param dim the dimension
 * @return the distance between the vector and the point
 * \note The dimension must always be 3. (others are not implemented)
 * \note the distance is dirctional
 */
  inline double
  uiDPointLine( const std::vector<double> & d/*irection*/, const std::vector<double> & p/*oint*/, const index_t & dim ){
    // dim=3!
    double t = (d[0]*p[0]+d[1]*p[1]+d[2]*p[2])/(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
    return sqrt(sqr(d[0]*t-p[0])+sqr(d[1]*t-p[1])+sqr(d[2]*t-p[2]));
  };

/**
 * Returns the sign of the number
 * @param x the number
 * @return -1 if x<0 1 elseway
 */
  inline double
  uiSign(const double x){
    if (x<0) { return -1.;} else {return 1.; };
  };

/**
 * Creates a mask for directional derivate. The derivate can not be calculated directly because of the noise, so I've figured, that we take a neighborhood and weight it with gauss. The direction will have big sigma, the non direction will have small. Then everything is weighted to be one.
 * @param direction The direction of the derivate.
 * @param size The size of the mask to be created.
 * @param mask The result mask. Must be a cube and well prepared for "size".
 * @param d represents the dimension of the space for further use, because the function is only prepared for three dimensional space.
 * \note the function only works in 3D.
 */
  float_image
  uiCreateMask( const std::vector<double> & direction, const index_t & size, const index_t & d ){
    const double sigma_dir   = 2.;
    const double sigma_neigh = 1.;
    std::vector<index_t> curr(3,0);
    std::vector<double> point(3,0);
    std::vector<index_t> vsize = {2*size, 2*size, 2*size};
    float_image  mask(vsize);
          
    for ( index_t q=-size; q<=size-1; q++ ){
      for ( index_t w=-size; w<=size-1; w++){
        for ( index_t e=-size; e<=size-1; e++){
          double dplane = uiDPointPlane( direction, {q, w, e}, d);
          double dline = uiDPointLine( direction, {q, w, e}, d);
          mask(q+size, w+size, e+size) = uiSign(dplane) * gauss(dplane, 0., sigma_dir) * gauss(dline, 0., sigma_neigh);
        } // e
      } // q
    } // w

    return mask;
  } // uiCreateMask

/**
 * Multiplies the mask with the image at a given place, point by point. 
 * @param center the coordinate, we're interested in
 * @param image the original image
 * @param mask the mask
 * @return The value of the multiplication
 * \note only in 3D
 */
  inline double
  uiCalcPiece( const std::vector<index_t> & center, float_image & image, float_image & mask ) {
    index_t size = mask.size()[0]/2; // assumed to be all equal
    double sum_up = 0.;			
    double sum_down = 0.;			
    std::vector<index_t> coord_mask(3);
    std::vector<index_t> coord_image(3);
    double c1 = 0.;
    for ( index_t q = -size; q<=size-1; q++ ){
      for ( index_t w = -size; w<=size-1; w++ ){
        for ( index_t e = -size; e<=size-1; e++ ){
          coord_mask[0]=q+size; 
          coord_mask[1]=w+size; 
          coord_mask[2]=e+size;
          coord_image[0]=center[0]+q; 
          coord_image[1]=center[1]+w; 
          coord_image[2]=center[2]+e;
          c1 = mask(coord_mask);
          if (c1<0){
            sum_down+=c1*image(coord_image);
          }
          else
            sum_up+=c1*image(coord_image);
        };
      };
    };
    return sum_up+sum_down;//fabs(fabs(sum_up)-fabs(sum_down));	
  } // uiCalcPiece

  double epsilon_x=0.1;
  double epsilon_y=0.12;
  double epsilon_z=0.08;

/**
 * Calculates the directional gradient of an image. The center of the direction is the middle point.
 * @param f The input image
 * @return The gradient image
 */
  float_image
  uiDirectionalGrad( const float_image & image ){
    std::cout << "Calculating directional gradient\n";
    // creating a local copy;
    float_image f(image);
    index_t d = f.size().size();
    if (d != 3) pink_error("prepared only for 3D, exiting...\n");
    std::vector<index_t> dim(f.size());
    if (sqr(dim[0]-dim[1])+sqr(dim[0]-dim[2])!=0) pink_error("prepared only for cubes (not rectangloids), exiting...\n");
    float_image result(dim);
    index_t dif=5;
    std::vector<index_t> mask_dim(3);
    std::vector<index_t> center(3);
    center[0]=f.size()[0]/2;
    center[1]=f.size()[1]/2;
    center[2]=f.size()[2]/2;
    mask_dim[0] = mask_dim[1] = mask_dim[2] = 2*dif;
  
    float_image mask(mask_dim);
    std::vector<double> direction (3,0.);
    std::vector<index_t> curr (3,0);
  
    for ( index_t q=dif; q<=/*15; q++){//*/dim[0]-dif-1; q++ ){
      std::cout << "I am at line " << d << std::endl;
      for ( index_t w=dif; w<=/*15; w++){//*/dim[1]-dif-1; w++){
        for ( index_t e=dif; e<=/*15; e++){//*/dim[2]-dif-1; e++){
// 				printf("I am at slice %d\n", e);
          curr[0]=q; curr[1]=w; curr[2]=e;
          direction[0]=fabs(q-center[0])+epsilon_x; direction[1]=fabs(w-center[1])+epsilon_y; direction[2]=fabs(e-center[2])+epsilon_z;
          //if (not( (direction[0]==0) and (direction[1]==0) and (direction[2]==0))){
          mask = uiCreateMask( direction, dif, d );
          result(curr) = uiCalcPiece( curr, f, mask );
          //}
          //else {
          //	result[curr] = 0;
          //};
        };
      };
    };

    return result;
  };


// uiDirGradSpecial --------------------------------------------------------------------------------------------------------------------


  float_image uiDirGradSpecial( const float_image & f ) {
    std::cout << "Calculating special directional gradient\n";
    index_t d = f.size().size();
    if (d!=3) pink_error("Special purpose function, prepared only for 3D.\n exiting...\n");
    std::vector<index_t> dim (f.size());
    std::vector<index_t> coord;
    float_image result(dim);
    float_image result_x(dim);
    float_image result_y(dim);
    float_image result_z(dim);
    index_t q;
  
    // direction x --------------------------------------------------------------
    std::cout << "epoch direction x (1/4)\n";
    q = 0;
    FOR( w, prod_except(dim, q)){
      std::vector<index_t> coord = uiStep( dim, w, q );
      std::vector<double>  vect(dim[q]);
      FOR(e, dim[q]){
        coord[q]=e;
        vect[e]=f(coord);
      };

      std::vector<double> der = uiDerivateVect(vect);

      FOR(e, dim[q]){
        coord[q]=e;
        result_x(coord)=der[e];
      };
    };
  
    // direction y --------------------------------------------------------------
    std::cout << "epoch direction y (2/4)\n";
    q = 1;
    FOR( w, prod_except( dim, q )){
      std::vector<index_t> coord = uiStep( dim, w, q );
      std::vector<double> vect(dim[q]);
      FOR(e, dim[q]){
        coord[q]=e;
        vect[e]=f(coord);
      };
      std::vector<double> der = uiDerivateVect(vect);

      FOR(e, dim[q]){
        coord[q]=e;
        result_y(coord)=der[e];
      };
    };

  
    // direction z --------------------------------------------------------------
    std::cout << "epoch direction z (3/4)\n";
    q = 2;

    FOR( w, prod_except( dim, q )) {
      std::vector<index_t> coord = uiStep( dim, w, q );
      std::vector<double> vect(dim[q]);
      FOR(e, dim[q]){
        coord[q]=e;
        vect[e]=f(coord);
      }
      std::vector<double> der = uiDerivateVect(vect);

      FOR(e, dim[q]){
        coord[q]=e;
        result_z(coord)=der[e];
      };
    };


    // final step, scalar product -----------------------------------------------
  
    const double epsilon_x=0.1;
    const double epsilon_y=0.12;
    const double epsilon_z=0.08;
  
    std::vector<index_t> center(3);
  
    std::vector<double> direction (3);
    std::vector<index_t> curr (3);
  
    center[0]=f.size()[0]/2;
    center[1]=f.size()[1]/2;
    center[2]=f.size()[2]/2;
  
  
    std::cout << "epoch final (4/4)\n";
    FOR(q, dim[0]){
      FOR(w, dim[1]){
        FOR(e, dim[2]){
          direction[0]=fabs(q-center[0])+epsilon_x; 
          direction[1]=fabs(w-center[1])+epsilon_y; 
          direction[2]=fabs(e-center[2])+epsilon_z;
          curr[0]=q; 
          curr[1]=w; 
          curr[2]=e;
          //normalizing the vector
          {
            double sum = 0;
            for ( auto & val : direction ) sum += sqr(val);
            double norm = sqrt(sum);

            if (norm > 0.000001)
              for ( auto & val : direction ) val /= norm;
          }
          
          //vTimes( direction, 1. / vAbs( direction ));
          result(curr)=
            pow(
              result_x(curr)*direction[0]+
              result_y(curr)*direction[1]+
              result_z(curr)*direction[2],
              2
              );
        };
      };
    };
  
    return result;
  };


} /* namespace pink */



/* LuM end of file */
