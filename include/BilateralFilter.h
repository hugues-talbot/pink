#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

# include <iostream>
#include <ctime>
#include <cstring>


template <typename PixelType>
class BilateralFilter
{
    public :
        BilateralFilter(PixelType *_image, int _window_size, double _alpha, double _beta, int _dimx, int dimy, int dimz=-1);
        void Execute2D();
        void Execute3D();
        PixelType* GetResult();

        inline int index3D(int x, int y, int z);
        inline int index2D(int x, int y);

    private :
        PixelType *image;
        int window_size;
        double alpha;
        double beta;
        int dimx;
        int dimy;
        int dimz;
};

#endif //BILATERALFILTER_H
