#include "../include/NonLocalFilterSioux.h"

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

NonLocalFilterSioux::NonLocalFilterSioux(PixelType *_image, int _patch_size, int _search_size, double _alpha, int _dimx, int _dimy, int _dimz):
        m_image(_image),
        m_patch_size(_patch_size),
        m_search_size(_search_size),
        m_alpha(_alpha),
        m_dimx(_dimx),
        m_dimy(_dimy),
        m_dimz(_dimz){}

inline int NonLocalFilterSioux :: index2D(int _x, int _y)
{
    return (_x+m_dimx*_y);
}

inline int NonLocalFilterSioux :: index3D(int _x, int _y , int _z)
{
     return (_x+m_dimx*_y+m_dimx*m_dimy*_z);
}

void NonLocalFilterSioux :: Execute2D()
{
    const int image_size=m_dimx*m_dimy;
    std::vector<PixelType> outputI(image_size);

	std::memcpy(&outputI[0], &m_image[0],image_size*sizeof(PixelType));

    std::vector<double> Sdxy(image_size);
    std::vector<PixelType> M(image_size);
    std::vector<PixelType> Z(image_size);
    PixelType V,w;

    for (int dx=-m_search_size; dx<=m_search_size; dx++)
    {
        for (int dy=-m_search_size; dy<=-m_search_size; dy++)
        {
            V=0;
            for (int y=0; y<image_size; y++)
            {
                for (int x=0; x<image_size; x++)
                {

                    for (int Y=0; Y<=y; Y++)
                    {
                        for (int X=0; X<=x; X++)
                        {
                            if (((X+dx)>=0) && ((X+dx)<image_size) && ((Y+dy)>=0) && ((Y+dy)<image_size))
                            {
                                V+=(m_image[index2D(X,Y)]-m_image[index2D(X+dx,Y+dy)])*(m_image[index2D(X,Y)]-m_image[index2D(X+dx,Y+dy)]);
                            }
                        }
                    }
                    Sdxy[index2D(x,y)]=V;
                }
            }

            for (int sx=0; sx<image_size; sx++)
            {
                for (int sy=0; sy<image_size; sy++)
                {
                    w=0;
                    if (((sx+m_patch_size)<image_size) && ((sy+m_patch_size<image_size)))
                    {
                        w+=Sdxy[index2D(sx+m_patch_size,sy+m_patch_size)];
                    }
                    if (((sx+m_patch_size)<image_size) && ((sy-m_patch_size>=0)))
                    {
                        w-=Sdxy[index2D(sx+m_patch_size,sy-m_patch_size)];
                    }
                    if (((sx-m_patch_size)>=0) && ((sy+m_patch_size<image_size)))
                    {
                       w-=Sdxy[index2D(sx-m_patch_size,sy+m_patch_size)];
                    }
                    if (((sx-m_patch_size)>=0) && ((sy-m_patch_size>=0)))
                    {
                        w+=Sdxy[index2D(sx-m_patch_size,sy-m_patch_size)];
                    }

                    outputI[index2D(sx,sy)]=outputI[index2D(sx,sy)]+w*outputI[index2D(sx+dx,sy+dy)];
                    M[index2D(sx,sy)]=std::max(M[index2D(sx,sy)],w);
                    Z[index2D(sx,sy)]+=w;
                }
            }
        }
    }
    for (int sx=0; sx<image_size; sx++)
    {
        for (int sy=0; sy<image_size; sy++)
        {
            outputI[index2D(sx,sy)]+=M[index2D(sx,sy)]*m_image[index2D(sx,sy)];

            if ((M[index2D(sx,sy)]+Z[index2D(sx,sy)])==0)
            {
                std::cout << "Division par 0" << std::endl;
            }
            outputI[index2D(sx,sy)]=outputI[index2D(sx,sy)]/(M[index2D(sx,sy)]+Z[index2D(sx,sy)]);
        }
    }

	// Copy output data
	std::memcpy( &m_image[0], &outputI[0],image_size*sizeof(PixelType));

}

void NonLocalFilterSioux :: Execute3D()
{
    const int image_size=m_dimx*m_dimy*m_dimz;
    std::vector<PixelType> outputI(image_size);

	std::memcpy(&outputI[0], &m_image[0],image_size*sizeof(PixelType));

    std::vector<double> Sdxyz(image_size);
    std::vector<PixelType> M(image_size);
    std::vector<PixelType> Z(image_size);
    PixelType V,w;

    for (int dz=-m_search_size; dz<=m_search_size; dz++)
    {
        for (int dx=-m_search_size; dx<=m_search_size; dx++)
        {
            for (int dy=-m_search_size; dy<=-m_search_size; dy++)
            {
                V=0;
                for (int z=0; z<image_size; z++)
                {
                    for (int y=0; y<image_size; y++)
                    {
                        for (int x=0; x<image_size; x++)
                        {
                            for (int Z=0; Z<=z; Z++)
                            {
                                for (int Y=0; Y<=y; Y++)
                                {
                                    for (int X=0; X<=x; X++)
                                    {
                                        if (((Z+dz)>=0) && ((Z+dz)<image_size) && ((X+dx)>=0) && ((X+dx)<image_size) && ((Y+dy)>=0) && ((Y+dy)<image_size))
                                        {
                                            V+=(m_image[index3D(X,Y,Z)]-m_image[index3D(X+dx,Y+dy, Z+dz)])*(m_image[index3D(X,Y, Z)]-m_image[index3D(X+dx,Y+dy, Z+dz)]);
                                        }
                                    }
                                }
                            }
                            Sdxyz[index3D(x,y,z)]=V;
                        }
                    }
                }
                for (int sz=0; sz<image_size; sz++)
                {
                    for (int sy=0; sy<image_size; sy++)
                    {
                        for (int sx=0; sx<image_size; sx++)
                        {
                            w=0;
                            if (((sx+m_patch_size)<image_size) && ((sy+m_patch_size)<image_size) && ((sz+m_patch_size)<image_size))
                            {
                                w+=Sdxyz[index3D(sx+m_patch_size,sy+m_patch_size,sz+m_patch_size)];
                            }
                            if (((sx+m_patch_size)<image_size) && ((sy-m_patch_size)>=0) && ((sz+m_patch_size)<image_size))
                            {
                                w-=Sdxyz[index3D(sx+m_patch_size,sy-m_patch_size,sz+m_patch_size)];
                            }
                            if (((sx-m_patch_size)>=0) && ((sy+m_patch_size)<image_size) && ((sz+m_patch_size)<image_size))
                            {
                                w-=Sdxyz[index3D(sx-m_patch_size,sy+m_patch_size,sz+m_patch_size)];
                            }
                            if (((sx+m_patch_size)<image_size) && ((sy+m_patch_size)<image_size) && ((sz-m_patch_size)>=0))
                            {
                                w-=Sdxyz[index3D(sx+m_patch_size,sy+m_patch_size,sz-m_patch_size)];
                            }
                            if (((sx-m_patch_size)>=0) && ((sy-m_patch_size)>=0) && ((sz+m_patch_size)<image_size))
                            {
                                w+=Sdxyz[index3D(sx-m_patch_size,sy-m_patch_size,sz+m_patch_size)];
                            }
                            if (((sx-m_patch_size)>=0) && ((sy+m_patch_size)<image_size) && ((sz-m_patch_size)>=0))
                            {
                                w+=Sdxyz[index3D(sx-m_patch_size,sy+m_patch_size,sz-m_patch_size)];
                            }
                            if (((sx+m_patch_size)<image_size) && ((sy-m_patch_size)>=0) && ((sz-m_patch_size)>=0))
                            {
                                w+=Sdxyz[index3D(sx+m_patch_size,sy-m_patch_size,sz-m_patch_size)];
                            }
                            if (((sx-m_patch_size)>=0) && ((sy-m_patch_size)>=0) && ((sz-m_patch_size)>=0))
                            {
                                w+=Sdxyz[index3D(sx-m_patch_size,sy-m_patch_size,sz-m_patch_size)];
                            }

                            outputI[index3D(sx,sy,sz)]=outputI[index3D(sx,sy,sz)]+w*outputI[index3D(sx+dx,sy+dy,sz+dz)];
                            M[index3D(sx,sy,sz)]=std::max(M[index3D(sx,sy,sz)],w);
                            Z[index3D(sx,sy,sz)]+=w;
                        }
                    }
                }
            }
        }
    }

    for (int sz=0; sz<=image_size; sz++)
    {
        for (int sy=0; sy<image_size; sy++)
        {
            for (int sx=0; sx<image_size; sx++)
            {
                outputI[index3D(sx,sy,sz)]+=M[index3D(sx,sy,sz)]*m_image[index3D(sx,sy,sz)];
                outputI[index3D(sx,sy,sz)]=outputI[index3D(sx,sy,sz)]/(M[index3D(sx,sy,sz)]+Z[index3D(sx,sy,sz)]);
            }
        }
    }

        // Copy output data
        std::memcpy( &m_image[0], &outputI[0],image_size*sizeof(PixelType));

}

