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



namespace pink {
  namespace python {


    void generate_rgb_image(
      const char_image & R,
      const char_image & G,
      const char_image & B,
      string filename 
      )
    {

      ARRAY<char> c_str_filename(new char[filename.size()+1]);
      strcpy(c_str_filename.get(), filename.c_str());
  

      char_image copy_R;
      copy_R = R;
      char_image copy_G;
      copy_G = G;
      char_image copy_B;
      copy_B = B;
  
      writergbimage( copy_R.get_output(),
                     copy_G.get_output(),
                     copy_B.get_output(),
                     c_str_filename.get()
        );
  
///!!!  cout << "ppm image written to '" << filename << "'\n";
  
  
      /* return void */
    } /* surimp */

    void surimp(
      const char_image & BW,
      const char_image & red,
      string filename
      )
    {

      ARRAY<char> c_str_filename(new char[filename.size()]);
      strcpy(c_str_filename.get(), filename.c_str());
  

      char_image image1;
      image1=BW;
      char_image image2;
      image2=red;
      struct xvimage * tmp;
      struct xvimage * tmp3;


      tmp = copyimage(image2.get_output());
      linverse(tmp);
      tmp3 = copyimage(image1.get_output());
      lmin(tmp3,tmp);
      copy2image(tmp,image2.get_output());
      ladd(tmp,tmp3);
      writergbimage(tmp, tmp3, tmp3, c_str_filename.get());
      //freeimage(image1); this image is freed automatically by boost 
      //freeimage(image2); this image is freed automatically by boost 
      freeimage(tmp);
      freeimage(tmp3);

/////!!!  cout << "ppm image written to '" << filename << "'\n";


      /*  return void;*/  
    } /* surimp */
  } /* namespace python */
} /* namespace pink */




UI_EXPORT_ONE_FUNCTION( surimp, 
			pink::python::surimp, 
			( arg("black and white image"), arg("the binary red marker image"), arg("the ppm filename")),
			"WRITE ME!!!\n"
  );



UI_EXPORT_ONE_FUNCTION( generate_rgb_image, 
			pink::python::generate_rgb_image, 
			(arg("red image"), arg("green image"), arg("blue image"), arg("the ppm filename")),
			"WRITE ME!!!\n"
  );







// LuM end of file

