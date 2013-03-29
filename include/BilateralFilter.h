#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

# include <iostream>
#include <ctime>
#include <cstring>



typedef int PixelType;

class BilateralFilter
{
    public :
        BilateralFilter(int *_image, int _window_size, int _alpha, int _beta, int _dimx, int dimy, int dimz);
        void Execute();

    private :
        int *image;
        int window_size;
        int alpha;
        int beta;
        int dimx;
        int dimy;
        int dimz;
};

#endif //BILATERALFILTER_H
