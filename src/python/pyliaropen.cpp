/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <cassert>
#include <iostream>

#include "liar_fseries.h"
#include "pink_python.h"
#include "RPO_maj_float.hpp"

#include "BilateralFilter.h"
#include "BilateralFilter.hpp"

#include "NonLocalFilter.h"
#include "NonLocalFilter.hpp"

#include "NonLocalFilterSioux.h"

#include "liarp.h"


#include "rotate3d.h"
#include "rotate3d_generic.h"


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


        if (outputxvimage->data_storage_type == VFF_TYP_4_BYTE) {
            // call the RPO function
            RPO3D(output_buffer, output_buffer,orientation, L, K, reconstruct, nx, ny, nz);

        } else {
            pink_error("Pixel type not yet supported\n");
        } 
        
        return result_image;

    } /* liarRPO*/
    
    
    
      template   <class image_t>
    image_t liarUnionRPO
    (
      const image_t & input_image,
      const int L,
      const int K,
      const int reconstruct
    )
    {
        int errorcode = 0;
        image_t result_image = input_image.clone();

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


        if (outputxvimage->data_storage_type == VFF_TYP_4_BYTE) {
            // call the RPO function
            UNION_RPO3D(output_buffer, output_buffer,L, K, reconstruct, nx, ny, nz);

        } else {
            pink_error("Pixel type not yet supported\n");
        } 
        
        return result_image;

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


        if (outputxvimage->data_storage_type == VFF_TYP_4_BYTE) {
            // 2 Dimensions

            if (nz==1)
            {
                // buffers
                PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);
                
                // create the RPO object
                BilateralFilter<PixelType> BF1(input_buffer, window_size, alpha, beta, nx, ny,1);
                
                // Execute
                BF1.Execute2D();
            }
            
            else
            {
                // buffers
                PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);
                
                // create the RPO object
                BilateralFilter<PixelType> BF1(input_buffer, window_size, alpha, beta, nx, ny,nz);
                
                // Execute
                BF1.Execute3D();
            }
        } else {
            pink_error("Pixel type not yet supported\n");
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


	
        if ((outputxvimage->data_storage_type == VFF_TYP_1_BYTE) ||
            (outputxvimage->data_storage_type == VFF_TYP_4_BYTE)) {
            if (nz==1) { // 2D image
                PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);
                
                //create the NonLocalFilter object
                NonLocalFilter<PixelType> NL1(input_buffer, patch_size, search_size, alpha, nx, ny,1);
                
                // Execute
                NL1.Execute2D();
                
            } else { // 3D image
                PixelType *input_buffer = (PixelType*) (outputxvimage->image_data);
                
                // create the NonLocalFilter object
                NonLocalFilter<PixelType> NL1(input_buffer, patch_size, search_size, alpha, nx, ny,nz);
                
                // Execute
                NL1.Execute3D();
            }
        } else {
            pink_error("Pixel type not yet supported\n");
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


    template   <class image_t>
    image_t liarRotation3D
    (
      const image_t & input_image,
      const double alpha,
      const double beta,
      const double gamma,
      const int interpolate,
      const int value,
      const int rmbdr           
    )
    
    {
        int errorcode = 0;
        image_t copy = input_image.clone();
        image_t output_image;
        
        // definition of local templated pixel_type
        typedef typename image_t::pixel_type pixel_type;

        // The low-level function seems to always succeed
	//

	// image structure
	struct xvimage *inputxvimage = copy.get_output();

	// dimensions
        int nx = inputxvimage->row_size;
        int ny = inputxvimage->col_size;
        int nz = inputxvimage->depth_size;
        int image_size = nx*ny*nz;

        int fnx;
        int fny;
        int fnz;
        int row0=round(nx/2);
        int col0=round(ny/2);
        int slice0=round(nz/2);


        if ((inputxvimage->data_storage_type == VFF_TYP_1_BYTE) ||
            (inputxvimage->data_storage_type == VFF_TYP_4_BYTE))   {
            // buffers
            pixel_type *input_buffer = (pixel_type*) (inputxvimage->image_data);
            pixel_type *output_buffer = NULL;
            
            int res=lrotate3d<pixel_type>
                (input_buffer, &output_buffer, nx, ny, nz, &fnx, &fny, &fnz,			
                 alpha, beta, gamma, interpolate, value, 
                 1, // we force border removal
                 row0, col0, slice0);   
            //std::assert(output_buffer != NULL);
            
            
            boost::python::list dimlist;
            
            dimlist.append(fnx);
            dimlist.append(fny);
            dimlist.append(fnz);

            pink::types::vint dim(dimlist);

            
            long newimage_size = dim.prod();

            std::cerr << "new dimx = " << fnx
                      << ", new dimy = " << fny
                      << ", new dimz = " << fnz
                      << ", prod = " << newimage_size << std::endl;
            
 
            boost::shared_array<pixel_type> data(new pixel_type[newimage_size]);
            
            std::memcpy(&data[0], output_buffer,newimage_size*sizeof(pixel_type));
            free(output_buffer); // not freeimage !
            
            image_t tmp_image( dim, data );
            output_image = tmp_image;	     
        } else {
            pink_error("Pixel type not yet supported\n");
        }


	// get result
	return (output_image);

    } /* liarRotation3D */
    
    

    template   <class image_t>
    image_t liaropenline3d
    (
      const image_t & input_image,
      const int dx,
      const int dy,
      const int dz,
      const int k
    )
    {
        int errorcode = 0;
        image_t result_image = input_image.clone();

		// definition of local templated pixel_type
        // typedef typename image_t::pixel_type pixel_type;

        // The low-level function seems to always succeed
	//

	// image structure
	struct xvimage *outputxvimage = result_image.get_output();

	// dimensions
	//	int nx = outputxvimage->row_size;
    //    int ny = outputxvimage->col_size;
    //    int nz = outputxvimage->depth_size;


    // buffers
   	//   char *input_buffer = (char *) (outputxvimage->image_data);

    // Call the Cpp-function
    
    	imfopen3D_line(outputxvimage, dx, dy, dz, k, outputxvimage); 
     	 ///glineminmax3d(input_buffer,nx,ny,nz,k,dx,dy,dz,operation,lineop);

	// get result
	return (result_image);

    } /* liarglineminmax3d */





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
  "Robust path opening, given an orientation (x,y,z); a length L, a noise robustness factor K, and optional reconstruction\n"
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
  UnionRPO,
  pink::python::liarUnionRPO,
  ( arg("input_image"), arg("L"), arg("K"),arg("reconstruction") ),
  "Compute the union of the Robust path opening in each orientation (see RPO), given a length L, a noise robustness factor K, and optional reconstruction\n"
  "Works in 2 and 3 dimensions\n"
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


UI_EXPORT_FUNCTION(
    Rotation3D,
    pink::python::liarRotation3D,
  ( arg("input_image"), arg("alpha"), arg("beta"), arg("gamma"), arg("interpolate"), arg("value"), arg("rmbdr")),
  "\n Rotation of a 3D image \n"
  " alpha, beta, gamma : Euler angle in degrees \n"
  " interpolate : 0 means Nearest neighbor interpolation, 1 means linear interpolation \n"
  " value : value of the pixel added during the rotation (usually set to 0) \n"
  " rmbdr : mode for the border ? \n"
  );


UI_EXPORT_FUNCTION(
    fopenline3d,
    pink::python::liaropenline3d,
  ( arg("input_image"), arg("dx"), arg("dy"), arg("dz"), arg("k")),
	"\n This function performs erosion or dilation along a line parallel to a given direction vector (dx,dy,dz). \n"
	"Whether an erosion or a dilation is performed, specified by the operation argument, this being either genfmin (set operation to 1) for erosion or genfmax (set operation to 0) for dilation.\n"
	"The lineop argument is then used to specify whether this operation uses a periodic  (set lineop to 0) or a bresenham line (set lineop to 1) by passing either periodic3d or bresenham3d.\n"
	"As with the two dimensional case, cascading this function can be used to generate other erosions and dilations.\n"

 	"param input_image: input image \n"
	"param k: length of structuring element \n"
	"param dx: x dimension of SE direction vector \n"
	"param dy: y dimension of SE direction vector \n"
	"param dz: z dimension of SE direction vector \n"
	"param operation : min (set to 1) or max (set to 0) operation \n"
	"param lineop : line operation to use bresenham3d (set to 1) or periodic3d (set to 0) \n"
	);


























// LuM end of file
