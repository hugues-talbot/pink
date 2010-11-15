/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2007-2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/** \file uiGradient.cpp
 * \brief This is a unit for calculating the gradient of the image. It uses the capabilites of GNU's gsl.
 *
 * The Unit calculates the gradient of an image. It is used, to calculate the measure field for integration.
 * \author UjoImro, 2007.
 */


#include <pink.h>


#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiGradient.cpp: " << msg; call_error(fullmessage.str());}
// general gradient ---------------------------------------------------------------------------------
namespace pink { 

// this function is used to iterate over a fields sides. That is to say you give it the "step" and the "coord"inate 
// to ignore, and the result is point by point a variation of the remaining coordinates. there are prodExcept(coord) 
// coordinates on the lower side.
PTR<vint> uiStep( const vint & dim, int step, int coord ){
  int d = dim.size();
  
  PTR<vint> result (new vint(d,0));
  vint slist  (d-1,0);
  vint partres(d-1,0);

  FOR( q, coord ){ 
    slist[q]=dim[q];
  } /* FOR */

  for ( int q = coord + 1; q <= d - 1; q++ ) {
    slist[q-1] = dim[q];
  } /* for */
  
  slist.nextStep( step, partres ) ;
  
  FOR(q, coord) {
    (*result)[q]=partres[q];
  } /* FOR */

  (*result)[coord]=0;

  for ( int q = coord + 1; q <= d - 1; q++ ) {
    (*result)[q] = partres[q - 1];
  } /* FOR */
  
  return result;
}; /* uiStep */


/**
 * Calculates the derivate square of the image. It takes the vector and replaces the values with the derivate of an interpolating spline.
 * @param values input vector
 * @param result result vector
 */
PTR<vval_type> uiDerivateVect( vval_type & values ){
//solved
// if the size of the vector is odd for some strange reason I don't know
// gnu gsl does not want to derivate it, so if odd lenght is detected, one additional
// 	element is adjusted, with a value of the original last element, 
// 	afterward the last derivate result is just ignored.
// 	for large images it should not cause any algorithmical (formal) problems.

  
  int d = values.size();
  PTR<vval_type> result(new vval_type(d));
  
  ARRAY<double> x(new double[ d ]);
  ARRAY<double> y(new double[ d ]);
  FOR(q, d){
    x[q] = q;
    y[q] = values[q];
  };
  
  gsl_interp * interp = gsl_interp_alloc( gsl_interp_cspline, d );//gsl_interp_akima, d );//gsl_interp_cspline, d );
  gsl_interp_init( interp, x.get(), y.get(), d );
  gsl_interp_accel * accel = gsl_interp_accel_alloc();
  result->clear();
  FOR(q,d) {
    (*result)[q]=/*pow(*/ gsl_interp_eval_deriv(interp, x.get(), y.get(), q, accel /* ),2));*/);
  } /* FOR */
  gsl_interp_free( interp );
  gsl_interp_accel_free( accel );
  return result;
} /* uiDerivateVect */

/**
 * Calculates the absolute value of the directional derivates.
 * @param f input image
 * @return a new scalar field with the derivates.
 */
PTR<float_image> uiGradientAbs( float_image & f ){
#ifdef UJIMAGE_DEBUG
  cout << "Calculating general gradient\n";
#endif /* UJIMAGE_DEBUG */
  int d = f.get_size().size();  
  
  PTR<float_image> result(new float_image( f.get_size() ));
  
  FOR( q, d ){
    FOR(w, f.get_size().prodExcept(q) ) {
      
      PTR<vint> coord( uiStep( f.get_size(), w, q ) );
      
      PTR<vval_type> vect( new vval_type( f.get_size()[q] ) );
      
      FOR( e, f.get_size()[q] ){
	(*coord)[q] = e;
	(*vect)[e]  = f[*coord];
      };
      //	printf("q=%d\n",q);
      PTR<vval_type> der = uiDerivateVect( *vect );
      //		der=vect; // just for testing
      //vect->clear(); //deleted later
      FOR( e, f.get_size()[q] ){
	(*coord)[q]=e;
	(*result)[*coord] += uiSqr((*der)[e]); // needed to be added because square is not square any more
      } /* FOR( e, f.get_size()[q] ) */;
    } /* FOR(w, f.get_size().prodExcept(q) ) */;
  }; /*   FOR( q, d ) */
//  result->_sqrt();
  return result;
} /* uiGradientAbs */

// // directional gradient ------------------------------------------------------------------------------------------------

// const uiVal_type Pi = 3.141592653589793;

// const uiVal_type c1 = 1./(sqrt(2.*Pi));

// /**
//  * Simulates the gauss (normal) density function
//  * @param x point
//  * @param m expected value
//  * @param sigma humidity
//  * @return N(m,sigma)(x)
//  */
// inline uiVal_type gauss( uiVal_type x, uiVal_type m, uiVal_type sigma ){
//   return c1/sigma*exp(-uiSqr(x-m)/(2.*uiSqr(sigma)));
// };


// /**
//  * Returns the distance between a plane and a point. The plane is defined with it's normal, and assumed to contain the origo [0,0,0].
//  * @param n the normal vector
//  * @param p the point
//  * @param dim the dimension
//  * @return the distance between the plane and the point
//  * \note The dimension must always be 3. (others are not implemented)
//  * \note the distance is directional
//  */
// inline uiVal_type uiDPointPlane(const vector<uiVal_type> n/*ormal*/, const vector<uiVal_type> p/*oint*/, const int dim){
//   return (n[0]*p[0]+n[1]*p[1]+n[2]*p[2])/sqrt(uiSqr(n[0])+uiSqr(n[1])+uiSqr(n[2]));
// };

// /**
//  * Returns the distance between a line and a point. The line is defined by its direction vector and assumed to contain the origo [0,0,0]
//  * @param d direction vector
//  * @param p the point
//  * @param dim the dimension
//  * @return the distance between the vector and the point
//  * \note The dimension must always be 3. (others are not implemented)
//  * \note the distance is dirctional
//  */
// inline uiVal_type uiDPointLine( const vector<uiVal_type> d/*irection*/, const vector<uiVal_type> p/*oint*/, const int dim ){
//   // dim=3!
//   uiVal_type t = (d[0]*p[0]+d[1]*p[1]+d[2]*p[2])/(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
//   return sqrt(uiSqr(d[0]*t-p[0])+uiSqr(d[1]*t-p[1])+uiSqr(d[2]*t-p[2]));
// };

// /**
//  * Returns the sign of the number
//  * @param x the number
//  * @return -1 if x<0 1 elseway
//  */
// inline uiVal_type uiSign(const uiVal_type x){
//   if (x<0) { return -1.;} else {return 1.; };
// };

// /**
//  * Creates a mask for directional derivate. The derivate can not be calculated directly because of the noise, so I've figured, that we take a neighborhood and weight it with gauss. The direction will have big sigma, the non direction will have small. Then everything is weighted to be one.
//  * @param direction The direction of the derivate.
//  * @param size The size of the mask to be created.
//  * @param mask The result mask. Must be a cube and well prepared for "size".
//  * @param d represents the dimension of the space for further use, because the function is only prepared for three dimensional space.
//  * \note the function only works in 3D.
//  */
// inline void uiCreateMask( vval_type & direction, int size, uiScalarField & mask, int d ){
//   // d=3!;
//   const uiVal_type sigma_dir = 2.;
//   const uiVal_type sigma_neigh = 1.;
//   vint curr(3,0);
//   vval_type point(3,0);
  
//   for ( int q=-size; q<=size-1; q++ ){
//     for ( int w=-size; w<=size-1; w++){
//       for ( int e=-size; e<=size-1; e++){
// 	curr[0]=q+size; point[0]=float(q);
// 	curr[1]=w+size; point[1]=float(w);
// 	curr[2]=e+size; point[2]=float(e);
// 	uiVal_type dplane = uiDPointPlane( direction, point, d);
// 	uiVal_type dline = uiDPointLine( direction, point, d);
// 	mask[curr]= uiSign(dplane) * gauss(dplane, 0., sigma_dir) * gauss(dline, 0., sigma_neigh);
//       };
//     };
//   };
// };

// /**
//  * Multiplies the mask with the image at a given place, point by point. 
//  * @param center the coordinate, we're interested in
//  * @param image the original image
//  * @param mask the mask
//  * @return The value of the multiplication
//  * \note only in 3D
//  */
// inline uiVal_type uiCalcPiece( vint center, uiScalarField & image, uiScalarField & mask ) {
//   int size = mask.size[0]/2; // assumed to be all equal
//   uiVal_type sum_up = 0.;			
//   uiVal_type sum_down = 0.;			
//   vint coord_mask(3);
//   vint coord_image(3);
//   uiVal_type c1 = 0.;
//   for ( int q = -size; q<=size-1; q++ ){
//     for ( int w = -size; w<=size-1; w++ ){
//       for ( int e = -size; e<=size-1; e++ ){
// 	coord_mask[0]=q+size; 
// 	coord_mask[1]=w+size; 
// 	coord_mask[2]=e+size;
// 	coord_image[0]=center[0]+q; 
// 	coord_image[1]=center[1]+w; 
// 	coord_image[2]=center[2]+e;
// 	c1 = mask[coord_mask];
// 	if (c1<0){
// 	  sum_down+=c1*image[coord_image];
// 	}
// 	else
// 	  sum_up+=c1*image[coord_image];
//       };
//     };
//   };
//   return sum_up+sum_down;//fabs(fabs(sum_up)-fabs(sum_down));	
// };

// uiVal_type epsilon_x=0.1;
// uiVal_type epsilon_y=0.12;
// uiVal_type epsilon_z=0.08;

// /**
//  * Calculates the directional gradient of an image. The center of the direction is the middle point.
//  * @param f The input image
//  * @return The gradient image
//  */
// PTR<uiScalarField> uiDirectionalGrad( uiScalarField & image ){
//   cout << "Calculating directional gradient\n";
//   // creating a local copy;
//   uiScalarField f(image);
//   int d = f.size.size();
//   if (d != 3) { 
//     error("prepared only for 3D, exiting...\n");
//   };
//   vint dim(f.size);
//   if (uiSqr(dim[0]-dim[1])+uiSqr(dim[0]-dim[2])!=0){
//     error("prepared only for cubes (not rectangloids), exiting...\n");
//   };
//   PTR<uiScalarField> presult( new uiScalarField( dim ));
//   int dif=5;
//   vint mask_dim(3);
//   vint center(3);
//   center[0]=f.size[0]/2;
//   center[1]=f.size[1]/2;
//   center[2]=f.size[2]/2;
//   mask_dim[0] = mask_dim[1] = mask_dim[2] = 2*dif;
  
//   uiScalarField mask(mask_dim);
//   vval_type direction (3,0.);
//   vint curr (3,0);
  
//   for ( int q=dif; q<=/*15; q++){//*/dim[0]-dif-1; q++ ){
//     printf("I am at line %d \n", q);
//     for ( int w=dif; w<=/*15; w++){//*/dim[1]-dif-1; w++){
//       for ( int e=dif; e<=/*15; e++){//*/dim[2]-dif-1; e++){
// // 				printf("I am at slice %d\n", e);
// 	curr[0]=q; curr[1]=w; curr[2]=e;
// 	direction[0]=fabs(q-center[0])+epsilon_x; direction[1]=fabs(w-center[1])+epsilon_y; direction[2]=fabs(e-center[2])+epsilon_z;
// 	//if (not( (direction[0]==0) and (direction[1]==0) and (direction[2]==0))){
// 	uiCreateMask( direction, dif, mask, d );
// 	(*presult)[curr] = uiCalcPiece( curr, f, mask );
// 	//}
// 	//else {
// 	//	result[curr] = 0;
// 	//};
//       };
//     };
//   };

//   return presult;
// };


// // uiDirGradSpecial --------------------------------------------------------------------------------------------------------------------


// PTR<uiScalarField> uiDirGradSpecial( uiScalarField & f ) {
//   cout << "Calculating special directional gradient\n";
//   int d = f.size.size();
//   if (d!=3) {printf("Special purpose function, prepared only for 3D.\n exiting...\n"); exit(1);};
//   vint dim (f.size);
//   vint coord;
// // 	uiScalarField result;
//   PTR<uiScalarField> presult(new uiScalarField(dim));
//   uiScalarField result_x(dim);
//   uiScalarField result_y(dim);
//   uiScalarField result_z(dim);
//   int q;
  
//   // direction x --------------------------------------------------------------
//   cout << "epoch direction x (1/4)\n";
//   q = 0;
//   FOR( w, dim.prodExcept(q)){
//     PTR<vint> coord = uiStep( dim, w, q );
//     vval_type vect(dim[q]);
//     FOR(e, dim[q]){
//       (*coord)[q]=e;
//       vect[e]=f[coord];
//     };
//     PTR<vval_type> der = uiDerivateVect(vect);

//     FOR(e, dim[q]){
//       (*coord)[q]=e;
//       result_x[coord]=(*der)[e];
//     };
//   };
  
//   // direction y --------------------------------------------------------------
//   cout << "epoch direction y (2/4)\n";
//   q = 1;
//   FOR( w, dim.prodExcept(q)){
//     PTR<vint> coord = uiStep( dim, w, q );
//     vval_type vect(dim[q]);
//     FOR(e, dim[q]){
//       (*coord)[q]=e;
//       vect[e]=f[coord];
//     };
//     PTR<vval_type> der = uiDerivateVect(vect);

//     FOR(e, dim[q]){
//       (*coord)[q]=e;
//       result_y[coord]=(*der)[e];
//     };
//   };

  
//   // direction z --------------------------------------------------------------
//   cout << "epoch direction z (3/4)\n";
//   q = 2;

//   FOR( w, dim.prodExcept(q)) {
//     PTR<vint> coord = uiStep( dim, w, q );
//     vval_type vect(dim[q]);
//     FOR(e, dim[q]){
//       (*coord)[q]=e;
//       vect[e]=f[coord];
//     };
//     PTR<vval_type> der = uiDerivateVect(vect);

//     FOR(e, dim[q]){
//       (*coord)[q]=e;
//       result_z[coord]=(*der)[e];
//     };
//   };


//   // final step, scalar product -----------------------------------------------
  
//   const uiVal_type epsilon_x=0.1;
//   const uiVal_type epsilon_y=0.12;
//   const uiVal_type epsilon_z=0.08;
  
//   vint center(3);
  
//   vval_type direction (3);
//   vint curr (3);
  
//   center[0]=f.size[0]/2;
//   center[1]=f.size[1]/2;
//   center[2]=f.size[2]/2;
  
  
//   cout << "epoch final (4/4)\n";
//   FOR(q, dim[0]){
//     FOR(w, dim[1]){
//       FOR(e, dim[2]){
// 	direction[0]=fabs(q-center[0])+epsilon_x; 
// 	direction[1]=fabs(w-center[1])+epsilon_y; 
// 	direction[2]=fabs(e-center[2])+epsilon_z;
// 	curr[0]=q; 
// 	curr[1]=w; 
// 	curr[2]=e;
// 	//normalizing the vector
// 	direction.normate();
// 	//vTimes( direction, 1. / vAbs( direction ));
// 	(*presult)[curr]=
// 	  pow(
// 	    result_x[curr]*direction[0]+
// 	    result_y[curr]*direction[1]+
// 	    result_z[curr]*direction[2],
// 	    2
// 	    );
//       };
//     };
//   };
  
//   return presult;
// };


} /* namespace pink */



/* LuM end of file */
