#include "NonLocalFilter.h"

#include <math.h>
#include <iostream>
#include <vector>
#include <stdlib.h>

template <typename PixelType>
NonLocalFilter<PixelType> :: NonLocalFilter(PixelType *_image, int _patch_size, int _search_size, double _alpha, int _dimx, int _dimy, int _dimz):
    m_image(_image),
    m_patch_size(_patch_size),
    m_search_size(_search_size),
    m_alpha(_alpha),
    m_dimx(_dimx),
    m_dimy(_dimy),
    m_dimz(_dimz){}

template <typename PixelType>
inline int NonLocalFilter<PixelType> :: index2D(int x, int y)
{
    return (x+m_dimx*y);
}

template <typename PixelType>
inline int NonLocalFilter<PixelType> :: index3D(int x, int y , int z)
{
     return (x+m_dimx*y+m_dimx*m_dimy*z);
}

template <typename PixelType>
void NonLocalFilter<PixelType> :: Execute3D()
{

    int const image_size=m_dimx*m_dimy*m_dimz;
    int const r_patch=floor(static_cast<double>(m_patch_size/2));
    int const r_search=floor(static_cast<double>(m_search_size/2));


    std :: vector<PixelType> outputI(image_size);
    int x, y, z; // Current pixel coordinates
    int X, Y, Z;  // Coordinates of the patch central pixel
    int dx, dy, dz;
    int m, n, p; // Coordinates inside the patch
    double w=0, W, Wt=0, v, V=0;
    int diff;


    //copy of the input data
	std::memcpy(&outputI[0], &m_image[0],image_size*sizeof(PixelType));

   #pragma omp parallel for private(x,y,z,Z,Y,X,dx,dy,dz,W,Wt,V,w,v)
    for (z=0; z<m_dimz; z++)
    {
        for (y=0; y<m_dimy; y++)
        {
            for (x=0; x<m_dimx; x++)
            {
              //  std :: cout << "x :" << x << ", " << "y :" << y << std :: endl;
                V=0;
                Wt=0;
                for (Z=z-r_search; Z<=z+r_search; Z++)
                {
                    for (Y=y-r_search; Y<=y+r_search; Y++)
                    {
                        for (X=x-r_search; X<=x+r_search; X++)
                        {
                            w=0;
                            for (dz=-r_patch; dz<=r_patch; dz++)
                            {

                                for (dy=-r_patch; dy<=r_patch; dy++)
                                {
                                    for (dx=-r_patch; dx<=r_patch; dx++)
                                    {
                                        m=X+dx;
                                        n=Y+dy;
                                        p=Z+dz;
                                        //std :: cout << "m :" << m << ", " << "n :" << n << std :: endl;

                                        if ((m>=0) && (m<m_dimx) && (n>=0) && (n<m_dimy) && (p>=0) && (p<m_dimz)
                                            && ((x+dx)>=0) && ((x+dx)<=m_dimx) && ((y+dy)>=0) && ((y+dy)<=m_dimy) && ((z+dz)>=0) && ((z+dz)<m_dimz)
                                            && (X>=0) && (X<m_dimx) && (Y>=0) && (Y<m_dimy) && (Z>=0) && (Z<m_dimz))
                                        {
                                            diff=m_image[index3D(x+dx,y+dy,z+dz)]-m_image[index3D(m,n,p)];
                                            w=w+diff*diff; // sum of square difference
                                            //std :: cout << "m :" << m << ", " << "n :" << n << std :: endl;
                                            //std :: cout << "w :" << w <<  std :: endl;
                                        }
                                    }
                                }
                            }
                            // w : difference du patch (X,Y,Z) avec
                            if ((w>0) && (X>=0) && (X<m_dimx) && (Y>=0) && (Y<m_dimy) && (Z>=0) && (Z<m_dimz))
                            {
                                W=exp(-m_alpha*w); // Weight for the (X,Y,Z) patch
                                v=W*m_image[index3D(X,Y,Z)]; // New value of the (x,y,z) pixel just according to the (X,Y,Z) patch
                                Wt+=W;
                                V+=v; // New value of the (x,y,z) pixel according to all patchs

                            }
                        }
                    }
                }

                if (Wt>0.0)
                {
                            V=int(V/Wt+0.5); // Normalization and conversion to in
                            outputI[index3D(x,y,z)]=V;
                }
            }
        }
    }
        //copy of the output data
        std::memcpy(&m_image[0], &outputI[0],image_size*sizeof(PixelType));
}

template <typename PixelType>
void NonLocalFilter<PixelType> :: Execute2D()
{

    int const image_size=m_dimx*m_dimy;
    int const r_patch=floor(static_cast<double>(m_patch_size/2));
    int const r_search=floor(static_cast<double>(m_search_size/2));


    std :: vector<PixelType> outputI(image_size);
    int x, y; // Current pixel coordinates
    int X, Y;  // Coordinates of the patch central pixel
    int dx, dy;
    int m, n; // Coordinates inside the patch
    double w=0, W, Wt=0, v, V=0;
    int diff;


    //copy of the input data
	std::memcpy(&outputI[0], &m_image[0],image_size*sizeof(PixelType));

  		#pragma omp parallel for private(x,y,Y,X,dx,dy,W,Wt,V,w,v)
        for (y=0; y<m_dimy; y++)
        {
            for (x=0; x<m_dimx; x++)
            {
              //  std :: cout << "x :" << x << ", " << "y :" << y << std :: endl;
                V=0;
                Wt=0;

                for (Y=y-r_search; Y<=y+r_search; Y++)
                {
                    for (X=x-r_search; X<=x+r_search; X++)
                    {
                        w=0;

                        for (dy=-r_patch; dy<=r_patch; dy++)
                        {
                            for (dx=-r_patch; dx<=r_patch; dx++)
                            {
                                m=X+dx;
                                n=Y+dy;
                                //std :: cout << "m :" << m << ", " << "n :" << n << std :: endl;

                                if ((m>=0) && (m<m_dimx) && (n>=0) && (n<m_dimy)
                                    && ((x+dx)>=0) && ((x+dx)<=m_dimx) && ((y+dy)>=0) && ((y+dy)<=m_dimy)
                                    && (X>=0) && (X<m_dimx) && (Y>=0) && (Y<m_dimy))
                                {
                                    diff=m_image[index2D(x+dx,y+dy)]-m_image[index2D(m,n)];
                                    w=w+diff*diff; // sum of square difference
                                    //std :: cout << "m :" << m << ", " << "n :" << n << std :: endl;
                                    //std :: cout << "w :" << w <<  std :: endl;
                                }
                            }
                        }
                        // w : difference du patch (X,Y,Z) avec
                        if ((w>0) && (X>=0) && (X<m_dimx) && (Y>=0) && (Y<m_dimy))
                        {
                            W=exp(-m_alpha*w); // Weight for the (X,Y,Z) patch
                            v=W*m_image[index2D(X,Y)]; // New value of the (x,y,z) pixel just according to the (X,Y,Z) patch
                            Wt+=W;
                            V+=v; // New value of the (x,y,z) pixel according to all patchs

                        }
                    }
                }

		if (Wt>0.0)
		{
                    V=int(V/Wt+0.5); // Normalization and conversion to in
                    outputI[index2D(x,y)]=V;
		}
            }
        }
        //copy of the output data
        std::memcpy(&m_image[0], &outputI[0],image_size*sizeof(PixelType));
}

template <typename PixelType>
PixelType* NonLocalFilter<PixelType> :: GetResult()
{
	return m_image;
}
