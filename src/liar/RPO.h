#ifndef _RPO_H_
#define _RPO_H_

#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkIntArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkImageFlip.h>
#include <vtkImageConstantPad.h>
#include <vtkImageCast.h>
#include <vtkImageClip.h>
#include "libRPO/util.h"
#include <queue>
#include <iostream>
#include <ctime>


class RPO {
	
	private :
	std::vector<int> orientation;
	int L;
	int K;
	int reconstruction;
	vtkImageData *im_input;
	vtkImageData *im_output;
	
	public :
	RPO(std::vector<int>& _orientation, int _L, int _K, int _reconstruction, vtkImageData* _im_input); 
	void Execute();
	vtkImageData* GetResult();
	
};


#endif

