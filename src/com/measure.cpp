/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file is the bash front-end of the lmeasure function

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in measure.cpp: " << msg; call_error(fullmessage.str());}

using namespace std;
using namespace pink;

#define epsilon 0.00001

int main(int argc, char * argv[]){

  if ( argc!=2 + 1 )
  {
    cerr << "\n\nNormalized absolute measure calculated from the image.\n";
 //                        0              1                  2 
    cerr << "usage: ./gradientabs src(float_image) result(float_image)\n\n";
    exit(1);
  } /* if (argc!=2 + 1) */
  

  


  PTR<float_image> src( new float_image(argv[1]) );

  float min, max;

  lminmaxval( *src, max, min );

  if ( min == max )
  {
    error("All pixels are equal in the image.");
  } /* if */

  src = lnormalize( *src );

  PTR<float_image> result = uiGradientAbs( *src );


  PRINTIMAGE(result);


  lminmaxval( *result, max, min );

  if ( min == max )
  {
    error("All pixels are equal in the image.");
  } /* if */


  FOR(q, result->get_size().prod())
  {
    (*result)[q] = 1. / ( epsilon + (*result)[q] );
  } /* FOR */

  result = lnormalize( *result );

  result->_writeimage(argv[2]);

} /* main */









































//LuM end of file
