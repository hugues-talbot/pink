#ifndef RPO_PARALLEL_H
#define RPO_PARALLEL_H

#include <vector>
typedef int PixelType;

class RPO_parallel
{
    public:
        RPO_parallel(PixelType *_image, std::vector<PixelType*> _output, int _L, int _K, int _R, int _dimx, int _dimy, int _dimz);
        void Execute();

    private:
        PixelType *image;
        std::vector<PixelType*> output;
        int L;
        int K;
        int R;
        int dimx;
        int dimy;
        int dimz;

};

#endif // RPO_PARALLEL_H
