/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2007-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

/** \file uiImage.cpp
 * \brief Unit for reading and writing images to files.
 *
 * It is capable to handle 2D and 3D images. It is not fully fledged yet. I plan to use already written units instead. However it is capable now of reading 2D text \b pgm image files generated by The GIMP. And special binary 3D pgm files. It writes to standart 2D pgm text files or standart 3D CSIROS binary files.
 * \author UjoImro, 2007.
 */
// not my include
/* Line 12+1 */

#include <fstream>
#include <iostream>


//#include "pink.h"
#include "uiImage.h"
#include "lminmax.hpp"
#include "ujimage.hpp"

namespace pink
{

void uiWriteFloatImage3D( float_image& image, const std::string& filename )
{
  std::ofstream s;
  pink::types::vint dim;
  int x, y, z, currX, currY, currZ;
  ////!!!!!!!!!!!!!1unsigned char pix;
  int pix;
  double piq, min, max;
  std::stringstream ss;

  s.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc ); // create a new file, or empty existing one

  dim = pink::types::vint(3);
  x = image.get_size()[0];
  y = image.get_size()[1];
  z = image.get_size()[2];

  ss << "P8\n";
  ss << "# UjoImro Design Group, 2007\n";
  ss << x << " " << y << " " << z << std::endl;
  ss << "4294967295\n";

  s << ss.str();// writing the header into the file

  max = lmaxval<float_image>(image);
  min = lminval<float_image>(image);
  printf( "%f < Image < %f\n", min,max );
  FOR(q,  x * y * z - 1) {
    currX=(q % ( y * x )) / y;
    currY=(q % ( y * x )) % y;
    currZ=q / ( y * x );
    dim[0]=currX;
    dim[1]=currY;
    dim[2]=currZ;
    piq = image[dim];
    pix = int((piq-min)*2000000/* *4294967295.*/ / (max-min));
    s.write( (char*)&pix, 4 );
  };
  s.close();
};


/**
 * uiWriteImage's helper function. It is writing 3D CSIROS images
 * @param image the image's uiScalarField representation
 * @param filename the target filename
 */
void uiWriteImage3D( float_image& image, const std::string& filename )
{
  std::cout << "warning: uiWriteImage3D has been tuned to the CELL PROJECT\n";
  std::cout << "Writing image to file '" << filename << "'" << std::endl;
  std::ofstream s;
  pink::types::vint dim;
  int x, y, z, currX, currY, currZ;
  unsigned char pix;
  double piq;
  double min;
  double max;

  s.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc);

  dim = pink::types::vint( 3 );
  x = image.get_size()[0];
  y = image.get_size()[1];
  z = image.get_size()[2];

  max = lmaxval<float_image>(image);
  min = lminval<float_image>(image);
  printf( "%f < Image < %f\n", min,max );
  FOR(q, x * y * z - 1) {
    currY=(q % ( y * x )) / x;
    currX=(q % ( y * x )) % x;
    currZ=q / ( y * x );

    dim[0]=currX;
    dim[1]=currY;
    dim[2]=currZ;

    piq = image[dim];
    pix = int ( ( piq - min ) * 255 / ( max - min ) );
    s.write( (char*)&pix, 1 );
  };

  s.close();
};

/**
 * uiWriteImage's helper function. It is writing 2D pgm images.
 * @param image the image's uiScalarField representation
 * @param filename the target filename
 */
void uiWriteImage2D( float_image& image, const std::string& filename )
{
  std::ofstream s;
  std::stringstream ss;
  pink::types::vint dim;
  int x, y, pix;
  double piq, min, max;
  std::string tmp;

  s.open( filename.c_str(), std::ios_base::out | std::ios_base::trunc );

  ss << "P2\n";
  ss << "# UjoImro Design Group, 2006\n";

  dim = pink::types::vint(2);
  x = image.get_size()[0];
  y = image.get_size()[1];

  std::cout << "x=" << x << ", y=" << y << std::endl;

  ss << x << " " << y << std::endl;

  ss << "255\n";

  max = lmaxval<float_image>(image);
  min = lminval<float_image>(image);

  std::cout << "Image.max = " << max  << std::endl;
  std::cout << "Image.min = " << min  << std::endl;

  if (max==min)
  {
    pink_error("uiWarning: every element equals in the image")
  };

  s << ss.str(); //writing image header

  FOR( q, x * y ) {
    //if (q==1300){
    //	printf("itten vagyok.\n");
    //};
    dim[0]=q % x;
    dim[1]=q / x;
    piq = image[dim];

    if (max==min) {
      pix=0;

    } else {
      pix = int ( ( piq - min ) * 255 / ( max - min ) );
    }

    if ((pix<0) || (pix>255)) {
      std::cout << "uiWarning: garbage at the image "
                << filename << "[" << dim[0] << "," << dim[1] << "](" << q << ")=" << pix << std::endl;
      pix=150;
    };

    s << pix << std::endl;
  };

  s.close();
}; /* uiWriteImage2D */

/**
 * Writing image into a file. It's writing 2D pgm standart images. And 3D CSIROS images. The format might change later on.
 * @param image the image's uiScalarField representation.
 * @param filename the char* name of the file
 */
void uiWriteImage ( float_image& image, const std::string& filename )
{
  if ( image.get_size().size() == 2 ) {
    uiWriteImage2D( image, filename );

  } else
    if ( image.get_size().size() == 3 ) {
      uiWriteImage3D( image, filename );

    }
    else
    {
      pink_error("error uiWriteImage: image of this dimension can not be saved (saving not implemented)\n");
    };
}; /*  uiWriteImage  */






}; // namespace pink













/* LuM end of file */
