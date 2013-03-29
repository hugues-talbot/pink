#include "BilateralFilter.h"

# include <iostream>
# include <vector>

// Constructor
BilateralFilter::BilateralFilter(int *_image, int _window_size, int _alpha, int _beta, int _dimx, int _dimy, int _dimz) :
    image(_image),
    window_size(_window_size),
    alpha(_alpha),
    beta(_beta),
    dimx(_dimx),
    dimy(_dimz),
    dimz(_dimz){}


void BilateralFilter::Execute()
{

    int image_size=dimx*dimy*dimz;
    std::vector<PixelType> originalI(image_size);

  	//copy input data
	std::memcpy(&originalI[0], &image[0],image_size*sizeof(PixelType));
}
