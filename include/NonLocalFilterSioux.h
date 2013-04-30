#ifndef NONLOCALFILTERSIOUX_H
#define NONLOCALFILTERSIOUX_H

#include <iostream>
#include <ctime>
#include <cstring>

typedef int PixelType;

class NonLocalFilterSioux
{
    public:
        NonLocalFilterSioux(PixelType *_image, int _patch_size, int _search_size, double _alpha, int _dimx, int _dimy, int _dimz=-1);
        inline int index2D(int _x, int _y);
        inline int index3D(int _x, int _y, int _z);
        void Execute2D();
        void Execute3D();
        PixelType* GetResult();

    private:
        PixelType *m_image;
        PixelType *m_output;
        int m_patch_size;
        int m_search_size;
        double m_alpha;
        int m_dimx;
        int m_dimy;
        int m_dimz;


};

#endif // NONLOCALFILTERSIOUX_H
