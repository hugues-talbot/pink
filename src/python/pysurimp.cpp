/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pysurimp.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;


void py_generate_rgb_image( const char_image & R,
			    const char_image & G,
			    const char_image & B,
			    string filename 
  )
{

  ARRAY<char> c_str_filename(new char[filename.size()+1]);
  strcpy(c_str_filename.get(), filename.c_str());
  

  PTR<char_image> copy_R(new char_image(R));
  PTR<char_image> copy_G(new char_image(G));
  PTR<char_image> copy_B(new char_image(B));
  
  writergbimage( copy_R->get_output(),
		 copy_G->get_output(),
		 copy_B->get_output(),
		 c_str_filename.get()
    );
  
  cout << "ppm image written to '" << filename << "'\n";
  
  
  /* return void */
} /* py_surimp */

void py_surimp( const char_image & BW,
		const char_image & red,
		string filename
  )
{

  ARRAY<char> c_str_filename(new char[filename.size()]);
  strcpy(c_str_filename.get(), filename.c_str());
  

  PTR<char_image> image1(new char_image(BW));
  PTR<char_image> image2(new char_image(red));
  struct xvimage * tmp;
  struct xvimage * tmp3;


  tmp = copyimage(image2->get_output());
  linverse(tmp);
  tmp3 = copyimage(image1->get_output());
  lmin(tmp3,tmp);
  copy2image(tmp,image2->get_output());
  ladd(tmp,tmp3);
  writergbimage(tmp, tmp3, tmp3, c_str_filename.get());
  //freeimage(image1); this image is freed automatically by boost 
  //freeimage(image2); this image is freed automatically by boost 
  freeimage(tmp);
  freeimage(tmp3);

  cout << "ppm image written to '" << filename << "'\n";


  /*  return void;*/  
} /* py_surimp */





UI_EXPORT_ONE_FUNCTION( cpp_surimp, 
			py_surimp, 
			args("black and white image", "the binary red marker image", "the ppm filename"),
			"WRITE ME!!!\n"
  )


UI_EXPORT_ONE_FUNCTION( cpp_generate_rgb_image, 
			py_generate_rgb_image, 
			args("red image", "green image", "blue image", "the ppm filename"),
			"WRITE ME!!!\n"
  )






// LuM end of file

