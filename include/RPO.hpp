#ifndef _RPO_H_
#define _RPO_H_

#include <queue>
#include <iostream>
#include <ctime>
#include <cstring>

#include "RPO_util.hpp"

typedef int PixelType; 

class RPO {
	
	private :
	std::vector<int> orientation;
	int L;
	int K;
	int reconstruction;
	int dimx, dimy, dimz;
	PixelType *input_buffer;
	PixelType *output_buffer;
	
	public :
	RPO(std::vector<int>& _orientation, int _L, int _K, int _reconstruction, PixelType *_input_buffer, PixelType *_output_buffer, int _dimx, int _dimy, int _dimz); 
	void Execute();
	PixelType * GetResult();
	
};


#endif

