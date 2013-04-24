#ifndef NONLOCALFILTER_H
#define NONLOCALFILTER_H

# include <iostream>
#include <ctime>
#include <cstring>

template <typename PixelType>
class NonLocalFilter
{
    public:
        NonLocalFilter(PixelType *_image, int _window_size, int _search_size, double _alphan, int dimx, int dimy, int dimz=-1);
        inline int index2D(int x, int y);
        inline int index3D(int x, int y, int z);
        void Execute3D();
        void Execute2D();
        PixelType* GetResult();

    private:
    PixelType *m_image;
    int m_patch_size;
    int m_search_size;
    double m_alpha;
    int m_dimx;
    int m_dimy;
    int m_dimz;
};

#endif // NONLOCALFILTER_H
