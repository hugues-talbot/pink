#ifndef __RPO_DILAT3D_HPP__
#define __RPO_DILAT3D_HPP__

#include <iostream>
#include <string>
#include <omp.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <queue>
#include <cassert>


#include "../src/liar/rect3dmm.hpp"
#include "larith.h"

// PixelType defined in NonLocalFilterSioux as long


template<typename PixelType>
void RPO_dilat3D(	PixelType* input_buffer,
								PixelType* output_buffer,
								int L,
								std::vector<int>orientation,
								int dimx,
								int dimy,
								int dimz)

{	

	PixelType *res=new PixelType[dimx*dimy*dimz];
	PixelType *image=new PixelType[dimx*dimy*dimz];
	memcpy(&image[0],&input_buffer[0],(dimx*dimy*dimz)*sizeof(PixelType));

    // Dilation by a cube of size 3x3x3
    rect3dminmax(image, dimx, dimy, dimz, 3,3,3,false);
	
	PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation,res);
	
	// Minimum between the computed RPO on the dilation and the initial image
    rect3dminmax(res, dimx, dimy, dimz, 3,3,3,true);
	
	// Minimum between the computed RPO on the dilation and the initial image
		for(int i=0; i<dimx*dimy*dimz;i++)
			output_buffer[i]=std::min(res[i],input_buffer[i]);
    
}

#endif






