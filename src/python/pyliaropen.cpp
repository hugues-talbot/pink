/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
#include "liar_fseries.h"
#include "pink_python.h"
#include "RPO.hpp"
#include "BilateralFilter.h"
#include "NonLocalFilter.h"
#include "NonLocalFilterSioux.h"

using namespace boost::python;
using namespace pink;


namespace pink {
  namespace python {
    template <class image_t>
    image_t liaropenrect
    (
      const image_t & src,
      const int SEnx,
      const int SEny,
      const int SEnz
    )
    {

      image_t result;
      result = src.clone();

     // The low-level function imfopen_rect etc return 0 to indicate success
    // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
         struct xvimage *myxvimage = result.get_output();
         if (imfopen_rect( myxvimage, SEnx, SEny, myxvimage) )
         {
           pink_error("function imfopen_rect failed");
         }
       }
       else  // NOT the image is 2D
       {
         struct xvimage *myxvimage = result.get_output();
         if (imfopen3D_rect( myxvimage, SEnx, SEny, SEnz, myxvimage) )
         {
           pink_error("function imfopen3D_rect failed");
         }
       } // NOT the image is 2D

      return result;
    } /* liaropen */

    template   <class image_t>
    image_t liaropenpoly
    (
      const image_t & src,
      const int radius,
      const int type,
      const int sides
    )
    {
        int errorcode = 0;
        image_t result;
        result = src.clone();

        // The low-level function imfopen_rect etc return 0 to indicate success
        // HT 20120227

       if ( src.get_size().size()==2) // the image is 2D
       {
           // It's OK, imfdilate_poly does not modify the input
           struct xvimage *myoutput = result.get_output();
         if ( (errorcode = imfopen_poly(myoutput, radius, type, sides, myoutput)) != 0)
         {
           pink_error("function imfopen_poly failed with error = ");
         } /* (! ldilateros_ldilat( src, elem_const_away, x, y)) */
       }
       else  // NOT the image is 2D
       {
           pink_error("function liaropenpoly not available for 3D images");
       } // NOT the image is 2D

      return result;
    } /* liaropenpoly */

    template   <class image_t>
    image_t liarRPO
    (
      const image_t & input_image,
      const int orientationx,
      const int orientationy,
      const int orientationz,
      const int L,
      const int K,
      const int reconstruct
    )
    {
        int errorcode = 0;
        image_t result_image = input_image.clone();

        // The low-level function seems to always succeed
	//

	// user-specified orientation
	std::vector<int> orientation(3);
	orientation[0] = orientationx;
	orientation[1] = orientationy;
	orientation[2] = orientationz;

	// image structure
	struct xvimage *outputxvimage = result_image.get_output();

	// dimensions
	int nx = outputxvimage->row_size;
        int ny = outputxvimage->col_size;
        int nz = outputxvimage->depth_size;

	// buffers
	// this looks weird, but input_buffer is copied immediately inside RPO
	PixelType *output_buffer = (PixelType*) (outputxvimage->image_data);
	// at this stage the output buffer contains the input image because of the clone() above


	// create the RPO object
        RPO RPO1(orientation, L, K, reconstruct, output_buffer, output_buffer, nx, ny, nz);

	// Execute
	RPO1.Execute();

	// get result
	return (result_image);

    } /* liarRPO*/


     template   <class image_t>
    image_t liarBilateralFilter
    (
      const image_t & input_image,
      const int window_size,
      const double alpha,
      const double beta
    )
    {
        int errorcode = 0;
        image_t result_image = input_image.clone();

        // The low-level function seems to always succeed
	//

	// image structure
	struct xvimage *outputxvimage = result_image.get_output();

	// dimensions
	int nx = outputxvimage->row_size;
        int ny = outputxvimage->col_size;
        int nz = outputxvimage->depth_size;


	// 2 Dimensions

	if (nz==1)
	{
   	    // buffers
       	    PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);

	    // create the RPO object
            BilateralFilter BF1(input_buffer, window_size, alpha, beta, nx, ny,1);

   	    // Execute
  	    BF1.Execute2D();
	}

	else
	{
   	    // buffers
       	    PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);

	    // create the RPO object
            BilateralFilter BF1(input_buffer, window_size, alpha, beta, nx, ny,nz);
	    		
   	    // Execute
  	    BF1.Execute3D();
	}

	// get result
	return (result_image);

    } /* liarBilateralFilter */

     template   <class image_t>
    image_t liarNonLocalFilter
    (
      const image_t & input_image,
      const int patch_size,
      const int search_size,
      const double alpha
    )
    {
        int errorcode = 0;
        image_t result_image = input_image.clone();

        // The low-level function seems to always succeed
	//

	// image structure
	struct xvimage *outputxvimage = result_image.get_output();

	// dimensions
	int nx = outputxvimage->row_size;
        int ny = outputxvimage->col_size;
        int nz = outputxvimage->depth_size;


	// 2 Dimensions

	if (nz==1)
	{
   	    //buffers
       	    PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);

	    //create the BilateralFilter object
            NonLocalFilter NL1(input_buffer, patch_size, search_size, alpha, nx, ny,1);

   	    // Execute
  	    NL1.Execute2D();
	}

	else
	{
   	    // buffers
       	    PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);

	    // create the RPO object
            NonLocalFilter NL1(input_buffer, patch_size, search_size, alpha, nx, ny,nz);

   	    // Execute
  	    NL1.Execute3D();
	}

	// get result
	return (result_image);

    } /* liarNonLocalFilter */

     template   <class image_t>
    image_t liarNonLocalFilterSioux
    (
      const image_t & input_image,
      const int patch_size,
      const int search_size,
      const double alpha
    )
    {
        int errorcode = 0;
        image_t result_image = input_image.clone();

        // The low-level function seems to always succeed
	//

	// image structure
	struct xvimage *outputxvimage = result_image.get_output();

	// dimensions
	int nx = outputxvimage->row_size;
        int ny = outputxvimage->col_size;
        int nz = outputxvimage->depth_size;


	// 2 Dimensions

	if (nz==1)
	{
   	    //buffers
       	    PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);

	    //create the NonLocalFilterSioux object
            NonLocalFilterSioux NLS1(input_buffer, patch_size, search_size, alpha, nx, ny,1);

   	    // Execute
  	    NLS1.Execute2D();
	}

	else
	{
   	    // buffers
       	    PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);

	    // create the NonLocalFilterSioux object
            NonLocalFilterSioux NLS1(input_buffer, patch_size, search_size, alpha, nx, ny,nz);

   	    // Execute
  	    NLS1.Execute3D();
	}

	// get result
	return (result_image);

    } /* liarNonLocalFilterSioux */

  } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
  fopenrect,
  pink::python::liaropenrect,
  ( arg("src"), arg("SEnx"),arg("SEny"), arg("SEnz") ),
  "Fast opening by a flat 2D rectangle or 3D rectangle parallelepiped"
  // end of the documenation
  );

UI_EXPORT_FUNCTION(
  fopenpoly,
  pink::python::liaropenpoly,
  ( arg("src"), arg("Radius"),arg("Type"), arg("Sides") ),
  "Fast 2D opening  by a flat polygon, given a radius, a type of line (0=periodic or 1=Bresenham) and a number of sides (can be zero)"
  );


UI_EXPORT_FUNCTION(
  RPO,
  pink::python::liarRPO,
  ( arg("input_image"), arg("orientationX"),arg("orientationY"), arg("orientationZ"), arg("L"), arg("K"),arg("reconstruction") ),
  "Robust 3D path opening, given an orientation (x,y,z); a length L, a noise robustness factor K, and optional reconstruction\n"
  "the following orientations are legal:\n"
  "   0  0  1  : depth direction\n"
  "   0  1  0  : vertical\n"
  "   1  0  0  : horizontal\n"
  "   1  1  1  : diagonal NE/SW+depth\n"
  "   1  1 -1  : diagonal NE/SW-depth\n"
  "  -1  1  1  : diagonal NW/SE+depth\n"
  "  -1  1 -1  : diagonal NW/SE-depth\n"
  "\n"
  "For 2D images, directions (0 1), (1 0), (1 1) and (-1 1) are sufficient\n"
  "reconstruction parameter is 0 or 1\n"
  );


UI_EXPORT_FUNCTION(
  BilateralFilter,
  pink::python::liarBilateralFilter,
  ( arg("input_image"), arg("window_size"), arg("alpha"), arg("beta")),
  "Bilateral Filter, given a window size and weights alpha beta. \n"
  " alpha is the weight of the distance factor \n"
  " beta is the weight of the intensity factor \n"
  " Works in 2 and 3 dimensions \n"
  );

UI_EXPORT_FUNCTION(
  NonLocalFilter,
  pink::python::liarNonLocalFilter,
  ( arg("input_image"), arg("patch_size"), arg("search_size"), arg("alpha")),
  "Non Local Filter, given the patch size, the search window size and the weight alpha. \n"
  " alpha is the weight of the sum of squared differences \n"
  " Works in 2 and 3 dimensions \n"
  );

UI_EXPORT_FUNCTION(
  NonLocalFilterSioux,
  pink::python::liarNonLocalFilterSioux,
  ( arg("input_image"), arg("patch_size"), arg("search_size"), arg("alpha")),
  "\n Non Local Filter, given the patch size, the search window size and the weight alpha. \n"
  " alpha is the weight of the sum of squared differences \n"
  " Works in 2 and 3 dimensions \n" 
  " This function uses the algorithm of Darbon &al in ""Fast NonLocal Filtering Applied to Electron Cryomicroscopy"" (2008) \n"
  );
















































// LuM end of file
