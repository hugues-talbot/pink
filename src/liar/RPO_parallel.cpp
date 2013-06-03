#include "RPO_parallel.h"
#include "RPO.hpp"

#include <vector>

RPO_parallel::RPO_parallel(PixelType *_image,std::vector<PixelType*> _output,int _L, int _K, int _R, int _dimx, int _dimy, int _dimz):
                            image(_image),
                            output(_output),
                            L(_L),
                            K(_K),
                            R(_R),
                            dimx(_dimx),
                            dimy(_dimy),
                            dimz(_dimz){}


void RPO_parallel::Execute()
{

    int const image_size=dimx*dimy*dimz;

    // Sorties
   // std::vector<std::vector<PixelType> > output(3);
    std::vector<PixelType*> output(3);

    // Vecteur contenant les 3 orientations
    std::vector<int> orientations(9);
    orientations[0]=1;
    orientations[1]=0;
    orientations[2]=0;
    orientations[3]=0;
    orientations[4]=1;
    orientations[5]=0;
    orientations[6]=0;
    orientations[7]=0;
    orientations[8]=1;
 
 	int i;
    int index=0;
    #pragma omp parallel for private(i)
    for (i=0; i<2; i++)
    {

        std::vector<int> orientation(3);
        orientation[0]=orientations[index];
        orientation[1]=orientations[index+1];
        orientation[2]=orientations[index+2];
        index=index+3;
        RPO RPO1(orientation,L,K,R,image,output[i],dimx,dimy,dimz);
        RPO1.Execute();

    }

}
