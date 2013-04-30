#include "BilateralFilter.h"

#include <math.h>
#include <iostream>
#include <vector>
#include <stdlib.h>


// Constructor
template <typename PixelType>
BilateralFilter<PixelType>::BilateralFilter(PixelType *_image, int _window_size, double _alpha, double _beta, int _dimx, int _dimy, int _dimz) :
    image(_image),
    window_size(_window_size),
    alpha(_alpha),
    beta(_beta),
    dimx(_dimx),
    dimy(_dimy),
    dimz(_dimz){}

template <typename PixelType>
inline int BilateralFilter<PixelType> :: index3D(int x, int y, int z)
{
    return (x+dimx*y+dimx*dimy*z);
}

template <typename PixelType>
inline int BilateralFilter<PixelType> :: index2D(int x, int y)
{
    return (x+dimx*y);
}

template <typename PixelType>
void BilateralFilter<PixelType>::Execute3D()
{
    int const image_size=dimx*dimy*dimz;
    std::vector<PixelType> outputI(image_size);

  	//copy of the input data
	std::memcpy(&outputI[0], &image[0],image_size*sizeof(PixelType));

    int const r_window=floor(window_size/2);


    int z, y ,x, dz, dy, dx;
    double w=0, v=0;
    double W=0, V=0;

    #pragma omp parallel for private(x,y,z,dx,dy,dz,W,V,w,v)
    for (z=0; z<dimz; z++)
    {
        for (y=0; y<dimy; y++)
        {
            for (x=0; x<dimx; x++)
            {
                W=0;
                V=0;

                for (dz=-r_window; dz<=r_window; dz++)
                {
                    for (dy=-r_window; dy<=r_window; dy++)
                    {
                        for (dx=-r_window; dx<=r_window; dx++)
                        {
                            // (X,Y) : Coordinates of the current pixel in the window centered on the (x,y,z) pixel
                            int X=x+dx;
                            int Y=y+dy;
                            int Z=z+dz;

                            if ((X>=0) && (X<dimx) && (Y>=0) && (Y<dimy) && (Z>=0) && (Z<dimz)) // If the current pixel is inside the image
                            {
                                double d=sqrt(dx*dx+dy*dy+dz*dz); // Distance between the central pixel and the current pixel
                                int d1=(image[index3D(X,Y,Z)]-image[index3D(x,y,z)]);
                                int dI=d1*d1; // Difference of intensity between the central pixel and the current pixel

                                w=exp(-alpha*d)*exp(-beta*dI); // Weight of the (X,Y) pixel.
                                v=w*image[index3D(X,Y,Z)]; // New intensity of the central pixel depending only on the current pixel.

                                W+=w; // Sum of the weight
                                V+=v; // Sum of the intensity
                            }
                        }
                    }
              	}
		V=V/W;
                outputI[index3D(x,y,z)]=static_cast<int>(V+0.5);
            }
        }
    }

	// Copy output data
	std::memcpy( &image[0], &outputI[0],image_size*sizeof(PixelType));
}

template <typename PixelType>
void BilateralFilter<PixelType>::Execute2D()
{
    int const image_size=dimx*dimy;
    std::vector<PixelType> outputI(image_size);

  	//copy of the input data
	std::memcpy(&outputI[0], &image[0],image_size*sizeof(PixelType));

    int const r_window=floor(window_size/2);


    int y ,x, dy, dx;
    double w=0, v=0;
    double W=0, V=0;

    #pragma omp parallel for private(x,y,dx,dy,W,V,w,v)
    for (y=0; y<dimy; y++)
    {
        for (x=0; x<dimx; x++)
        {
            W=0;
            V=0;
            for (dy=-r_window; dy<=r_window; dy++)
            {
                for (dx=-r_window; dx<=r_window; dx++)
                {
                    // (X,Y) : Coordinates of the current pixel in the window centered on the (x,y,z) pixel
                    int X=x+dx;
                    int Y=y+dy;

                    if ((X>=0) && (X<dimx) && (Y>=0) && (Y<dimy)) // If the current pixel is inside the image
                    {
                        double d=sqrt(dx*dx+dy*dy); // Distance between the central pixel and the current pixel
                        int d1=(image[index2D(X,Y)]-image[index2D(x,y)]);
                        int dI=d1*d1; // Difference of intensity between the central pixel and the current pixel

                        w=exp(-alpha*d)*exp(-beta*dI); // Weight of the (X,Y) pixel.
                        v=w*image[index2D(X,Y)]; // New intensity of the central pixel depending only on the current pixel.

                        W+=w; // Sum of the weight
                        V+=v; // Sum of the intensity
                    }
                }
            }
            V=int(V/W+0.5); // Normalisation and conversion to int
            outputI[index2D(x,y)]=V;

        }
    }

	// Copy output data
	std::memcpy( &image[0], &outputI[0],image_size*sizeof(PixelType));
}

template <typename PixelType>
PixelType* BilateralFilter<PixelType>::GetResult()
{
	return image;
}
