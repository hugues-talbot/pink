//#ifndef RORPO_MULTISCALE_HPP_INCLUDED
//#define RORPO_MULTISCALE_HPP_INCLUDED

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#ifdef _OPENMP_
# include <omp.h>
#endif // _OPENMP_
#include <cstdlib>

#include "Call_RORPO.hpp"

template<typename PixType>
int RORPO_multiscale(PixType* image, PixType* output, int Smin, float factor, int nb_scales, int nb_core, int dimx, int dimy, int dimz, int debug_flag=-1)
{

// ################################# Scales Computations ###################################

	const int image_size=dimx*dimy*dimz;
	std::vector<int> S_list(nb_scales);
	S_list[0]=Smin;

	if (debug_flag){
	    std::cout<<" "<<std::endl;
		std::cout<<"Scales : ";
		std::cout<<S_list[0]<< " ";
	}
			
	for (int i=1; i<nb_scales;++i)
	{
		S_list[i]=int(Smin*pow(factor,i));
		if (debug_flag){
			std::cout<<S_list[i]<< " ";
			
		}
	}
	std::cout<<std::endl;
	
	/*for (int i=1; i<nb_scales-1;i++)
	{
	    S_list[i]=Smin+i*((factor-Smin)/(nb_scales-1));
	    std::cout<<S_list[i]<< " ";
	}
	S_list[nb_scales-1]=factor;
	std::cout<<factor<< " ";*/
	
	
	
	// ---------- Find min and max value --------
	int min_value=image[0];
	int max_value=image[0];
	
	for (int i=0; i<image_size;++i)
	{
	
		if (image[i]<0){
			std::cerr<<"Image contain negative values"<<std::endl;
			return 0;
		}
		if (image[i]<min_value)
			min_value=image[i];

		if (image[i]>max_value)
			max_value=image[i];
	}

	if (debug_flag){
		std::cout<<"min value: "<<min_value<<std::endl;
		std::cout<<"max value: "<<max_value<<std::endl;
		std::cout<<" "<<std::endl;
	}

	// Run RORPO
	PixType* multiscale;
	PixType* image_dilat=new PixType[image_size];
	//output=new PixType[image_size];
	memcpy(image_dilat,image,image_size*sizeof(PixType));

	multiscale=Call_RORPO<PixType>(image_dilat, S_list, nb_scales,nb_core, max_value, dimx, dimy, dimz, debug_flag);	
	memcpy(output,multiscale,image_size*sizeof(PixType));
	
	delete[](multiscale);
	delete[](image_dilat);
	return 1;
}

//#endif // RORPO_MULTISCALE_HPP_INCLUDED

