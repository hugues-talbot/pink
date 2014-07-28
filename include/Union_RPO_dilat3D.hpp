#ifndef __UNION_RPO_DILAT3D_HPP__
#define __UNION_RPO_DILAT3D_HPP__

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


void usage()
{
	std::cout<<" The number of parameters isn't correct !"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"./PO image_path path_length writing_path"<<std::endl;
	std::cout<<" image_path : string"<<std::endl;
	std::cout<<" writing_path : string"<<std::endl;

	exit(1);

}

template<typename PixelType>
void Union_RPO_dilat3D(	PixelType* input_buffer,
								PixelType* output_buffer,
								int L,
								int dimx,
								int dimy,
								int dimz)

{	

	//--------------  Run PO for each orientation ----------------------

	// orientation vector
	std::vector<int> orientation1(3);
	orientation1[0] = 1;
	orientation1[1] = 0;
	orientation1[2] = 0;
	std::vector<int> orientation2(3);
	orientation2[0] = 0;
	orientation2[1] = 1;
	orientation2[2] = 0;
	std::vector<int> orientation3(3);
	orientation3[0] = 0;
	orientation3[1] = 0;
	orientation3[2] = 1;
	std::vector<int> orientation4(3);
	orientation4[0] = 1;
	orientation4[1] = 1;
	orientation4[2] = 1;
	std::vector<int> orientation5(3);
	orientation5[0] = 1;
	orientation5[1] = 1;
	orientation5[2] = -1;
	std::vector<int> orientation6(3);
	orientation6[0] = -1;
	orientation6[1] = 1;
	orientation6[2] = 1;
	std::vector<int> orientation7(3);
	orientation7[0] = -1;
	orientation7[1] = 1;
	orientation7[2] = -1;

	// Buffer for results
	//std::vector<PixelType> res(dimx*dimy*dimz); // allocation dynamique cache
	PixelType *res1=new PixelType[dimx*dimy*dimz]; //allocation dynamique
	PixelType *res2=new PixelType[dimx*dimy*dimz];
	PixelType *res3=new PixelType[dimx*dimy*dimz];
	PixelType *res4=new PixelType[dimx*dimy*dimz];
	PixelType *res5=new PixelType[dimx*dimy*dimz];
	PixelType *res6=new PixelType[dimx*dimy*dimz];
	PixelType *res7=new PixelType[dimx*dimy*dimz];

    PixelType *image=new PixelType[dimx*dimy*dimz];
	memcpy(&image[0],&input_buffer[0],(dimx*dimy*dimz)*sizeof(PixelType));

    // Dilation by a cube of size 3x3x3
    // rect3dminmax erases its input buffer so a copy of input_buffer is done before in image.
	rect3dminmax(image, dimx, dimy, dimz, 3,3,3,false);
	
	// Calling PO for each orientation
	   #pragma omp parallel sections
	   {
	   #pragma omp section
	   {
		 PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation1,res1); // Defined in /include/Path_Opening.hpp
		 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation2,res2);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation3,res3);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation4,res4);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   {
	       PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation5,res5);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	  {
	       PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation6,res6);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation7,res7);
		   std::cout<<"orientation7 -1 1 -1 : passed"<<std::endl;
		}
	 }
	 //Union of orientations
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=res1[i];
		
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::max(res2[i],output_buffer[i]);

	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::max(res3[i],output_buffer[i]);

	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::max(res4[i],output_buffer[i]);

	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::max(res5[i],output_buffer[i]);

	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::max(res6[i],output_buffer[i]);

	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::max(res7[i],output_buffer[i]);

    //Erosion
    rect3dminmax(output_buffer, dimx, dimy, dimz, 3,3,3,true);
    
    // Minimum between the computed RPO on the dilation and the initial image
    for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=std::min(output_buffer[i],input_buffer[i]);
	
	//memcpy(&output_buffer[0],&image[0],(dimx*dimy*dimz)*sizeof(PixelType));	
    
}

#endif






