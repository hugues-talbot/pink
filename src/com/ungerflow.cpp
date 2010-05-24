/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This file is the bash front-end of the ungerflow function

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ungerflow.cpp: " << msg; call_error(fullmessage.str());}

using namespace std;
using namespace pink;


int main(int argc, char * argv[]){

  if ( argc != 8 + 1 )
  {
    //                        0               1           2    3      4       5          6               7            8
    cerr << "\n\nusage: ./ungerflow number_of_iterations tau theta G_image src_sink lambda/*_image*/ guidence_f stable_image\n\n";
    exit(1);
  }; /* if (argc!=8 + 1) */
  


  int iterations = lexical_cast<int>(argv[1]);
  float tau = lexical_cast<float>(argv[2]);
  float theta = lexical_cast<float>(argv[3]);
  
  PTR<float_image> gg( new float_image(argv[4]) );
  PTR<char_image> src_sink( new char_image(argv[5]) );
//  PTR<float_image> lambda_image( new float_image(argv[6]) );
  
// constant float image
  float lambda = lexical_cast<float>(argv[6]);
  PTR<float_image> lambda_image( new float_image(gg->get_size()) );
  FOR(q, lambda_image->get_size().prod())
  {
    (*lambda_image)[q]=lambda;
  } /* FOR */
  

  PTR<float_image> guidence_f( new float_image(argv[7]) );
  
  
  // adding the frames to the images
  PTR<char_image> framed_srcsink = frame_around( *src_sink, -1 );
  PTR<float_image> framed_gg = frame_around( *gg, 0. );
  PTR<float_image> framed_lambda = frame_around( *lambda_image, 0. );
  PTR<float_image> framed_guidence_f = frame_around( *guidence_f, 0. );

  // FOR(q, framed_srcsink->get_size().prod())
  // {
  //   if ( (*framed_srcsink)[q]==255 )
  //   {
  //     (*framed_srcsink)[q]=1;
  //   } /* if */
  // } /* FOR */


  PRINTIMAGE(framed_srcsink);
  PRINTIMAGE(framed_gg);
  PRINTIMAGE(framed_lambda);
  PRINTIMAGE(framed_guidence_f);

  PTR<float_image> result = lungerflow<float_image>( *framed_srcsink, 
						     *framed_gg, 
						     *framed_lambda,
						     *framed_guidence_f,
						     iterations, 
						     tau, 
						     theta );


  //removing the frame from the result
  PTR<float_image> unframed_result = remove_frame( *result );

  unframed_result->_writeimage(argv[8]);








  return 0;
  
} /* main */









































//LuM end of file
