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

// PixType defined in NonLocalFilterSioux as long


template<typename PixType>
void RPO_dilat3D(	PixType* input_buffer,
								PixType* output_buffer,
								int L,
								std::vector<int>orientation,
								int dimx,
								int dimy,
								int dimz)

{	
	PixType *res=new PixType[dimx*dimy*dimz];
	PixType *image=new PixType[dimx*dimy*dimz];
	memcpy(&image[0],&input_buffer[0],(dimx*dimy*dimz)*sizeof(PixType));
    // Dilation by a cube of size 3x3x3
    rect3dminmax<PixType>(image, dimx, dimy, dimz, 3,3,3,false);
	
	PO_3D<PixType>(image,dimz,dimy,dimx,L,orientation,res);
	
	// Minimum between the computed RPO on the dilation and the initial image
		for(int i=0; i<dimx*dimy*dimz;i++)
			output_buffer[i]=std::min(res[i],input_buffer[i]);
    
}

#endif






