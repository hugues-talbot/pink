#ifndef UIGRADIENT_H_
#define UIGRADIENT_H_
/*
This software is ment to be free.
(C) UjoImro, 2006
ujoimro@gmail.com
*/
/** \file uiGradient.h
 * \brief Header file for uiGradient.cpp
 *
 * Using interpolation capabilities of GSL. It calculates derivates of the diskret function in every point where the function is defined the step is considered to be 1 so far.
 */

/* using interpolation capabilities of GSL */
/* Line 12+1 */
/* returns derivates of the diskret function 
	 in every point where the function is defined
	 the step is considered to be 1 so far*/

 //PTR<vector<uiVal_type> > uiDerivate( vector<uiVal_type> & values );
 //PTR<uiScalarField> uiGradientAbs( PTR<uiScalarField> & f );
 //PTR<uiScalarField> uiDirectionalGrad( PTR<uiScalarField> & f );
 //PTR<uiScalarField> uiDirGradSpecial( PTR<uiScalarField> & f );

namespace pink {

  PTR<float_image> uiGradientAbs( float_image & f );

};


#endif /*UIGRADIENT_H_*/
/* LuM end of file */
