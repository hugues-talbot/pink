#ifndef __UNION_RPO_DILAT_CONSTRAINT_HPP__
#define __UNION_RPO_DILAT_CONSTRAINT_HPP__

#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <cassert>



#include "../src/liar/rect3dmm.hpp"
#include "larith.h"


void createNeighbourhoodConstraint(	int nb_col,
							int dim_frame,
	 						std::vector<int> & orientation,
	 						std::vector<int> & upList,
                    		std::vector<int> & downList) {

    int col_shift = orientation[0];
    int line_shift = orientation[1];
    int depth_shift = orientation[2];

    //depth orientation [1 0 0]
    if((depth_shift == 1 && line_shift == 0 && col_shift == 0) ||
       (depth_shift == -1 && line_shift == 0 && col_shift == 0) ) {

       upList.push_back( dim_frame );
       downList.push_back( -dim_frame );

    }
    //from up to down main orientation [0 1 0]
    if((depth_shift == 0 && line_shift == 1 && col_shift == 0) ||
      (depth_shift == 0 && line_shift == -1 && col_shift == 0)) {

       upList.push_back( nb_col );
       downList.push_back( -nb_col );

    }
    //from left to right main orientation [0 0 1]
    if((depth_shift == 0 && line_shift == 0 && col_shift == 1) ||
      (depth_shift == 0 && line_shift == 0 && col_shift == -1)) {

       upList.push_back( 1 );
       downList.push_back( -1 );

    }
    //1st main diagonal [1 1 1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == -1)) {


       upList.push_back(dim_frame + nb_col + 1);
       downList.push_back(-dim_frame - nb_col - 1);

    }
    //2nd main diagonal [1 1 -1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == 1)) {

       upList.push_back( dim_frame + nb_col - 1 );
       downList.push_back( -dim_frame - nb_col + 1 );

    }
    //3 main diagonal [-1 1 1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == -1)) {

        upList.push_back( -dim_frame + nb_col + 1 );
        downList.push_back( dim_frame - nb_col -1 );

    }
    //4 main diagonal [-1 1 -1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == 1)) {

        upList.push_back( -dim_frame + nb_col - 1 );
        downList.push_back( dim_frame - nb_col + 1 );

    }

}

template<typename PixelType,typename IndexType>
void propagate_constraint(IndexType p, std::vector<int>&lambda, std::vector<int>&lambdaC, std::vector<int>&nfC, std::vector<int>&nf, std::vector<int>&nbC, std::vector<int>&nb, std::vector<bool>&b, std::queue<IndexType> &Qc)
// Propagation from pixel p
{
	std::queue<IndexType> Qq;
	lambda[p]=0;
	lambdaC[p]=0;

	std::vector<int>::iterator it;
	for (it=nf.begin(); it!=nf.end();++it)
	{
		if ((p+*it)<b.size() and b[p+*it])
		{
			Qq.push(p+*it);
		}
	}

	while (not Qq.empty())
	{
       //bool testQ=false;
		IndexType q=Qq.front();
		Qq.pop();
		std::vector<int>::iterator it2;
		for (it2=nbC.begin();it2!=nbC.end();++it2)
		{
            int l=lambda[q+*it2]+1;
            if (l<lambdaC[q])
            {
                lambdaC[q]=l;
                std::vector<int>::iterator it3;
                for (it3=nf.begin(); it3!=nf.end(); ++it3)
                {
                    if (b[q+*it3])
                    {
                        Qq.push(q+*it3);
                    }
                    //testQ=true;
                    Qc.push(q);
                }
			}

            std::vector<int>::iterator it4;
			for (it4=nb.begin(); it4!=nb.end();++it4)
            {
                l=std::max(lambdaC[q+*it4]+1,l);
            }

            if (l<lambda[q])
            {
                lambda[q]=l;
                Qc.push(q);
                std::vector<int>::iterator it5;
                for (it5=nfC.begin(); it5!=nfC.end(); ++it5)
                {
                    if (b[q+*it5])
                    {
                        Qq.push(q+*it5);
                    }
                }
            }
            //if (testQ=true)
        }
	}
}

template<typename PixelType>
void PO_constraint(	PixelType* Inputbuffer,
								int dimz,
								int dimy,
								int dimx,
								int L,
								std::vector<int>orientations,
								PixelType* Outputbuffer)
//Path opening with orientation
{

	// Add border of 2 pixels which value is 0 to *Inputbuffer
	std::vector<PixelType> Image((dimz+4)*(dimy+4)*(dimx+4),0);
    //std::vector<bool>b(Image.size(),0);
	int new_dimy=dimy+4;
	int new_dimx=dimx+4;
	for (int z=0; z<dimz; ++z){
		for (int y=0; y<dimy; ++y){
			for (int x=0; x<dimx; ++x){
				Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)]=Inputbuffer[z*dimx*dimy+y*dimx+x];
				//b[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)]=1;
			}
		}
	}

	// Create the temporary image b  (0 for a 1-pixel border, 1 elsewhere)
	std::vector<bool>b(Image.size(),0);
	for (int z=0; z<dimz+2; ++z){
		for (int y=0; y<dimy+2; ++y){
			for (int x=0; x<dimx+2; ++x)
			{
				b[(z+1)*(new_dimx*new_dimy)+(y+1)*new_dimx+(x+1)]=1;
			}
		}
	}

	int dim_frame=new_dimx*new_dimy;

	// Create the sorted list of index of Image according to intensity
	std::vector<IndexType>index_image;
	index_image=sort_image_value<PixelType,IndexType>(Image);

	// Create the offset np and nm
	std::vector<int>np;
	std::vector<int>nm;
	createNeighbourhood(new_dimx, dim_frame,orientations,np,nm);

	// Create the offset npC and nmC
    std::vector<int>npC;
	std::vector<int>nmC;
	createNeighbourhoodConstraint(new_dimx, dim_frame,orientations,npC,nmC);

	//Create other temporary images
	std::vector<int>Lp(Image.size(),L);
	std::vector<int>Lm(Image.size(),L);
    std::vector<int>LpC(Image.size(),L);
	std::vector<int>LmC(Image.size(),L);

	//Create FIFO queue Qc
	std::queue<IndexType> Qc;

	// Propagate
	std::vector<IndexType>::iterator it;
	//std::cerr<<"Avant propagation"<<std::endl;
	//std::cerr<<"size np et nm:"<<np.size()<<" "<<nm.size()<<std::endl;

	int indice;
	for (it=index_image.begin(), indice=0; it!=index_image.end(); ++it, ++indice)
	{

		//std::cerr<<"propagation"<<std::endl;
		if (b[*it])
		{
			propagate_constraint<PixelType,IndexType>(*it,Lm,LmC,npC,np,nmC,nm,b,Qc);
			propagate_constraint<PixelType,IndexType>(*it,Lp,LpC,nmC,nm,npC,np,b,Qc);

			while (not Qc.empty())
			{
				IndexType q=Qc.front();
				Qc.pop();
				if ((Lp[q]+LmC[q]-1)<L || (LpC[q]+Lm[q]-1)<L)
				{

					Image[q]=Image[*it];

					b[q]=0;
					Lp[q]=0;
					LpC[q]=0;
					Lm[q]=0;
					LmC[q]=0;
				}
			}
		}
	}

	//Outputbuffer=&Image[0];

	// Remove border
	for (int z=0; z<dimz; ++z)
	{
		for (int y=0; y<dimy; ++y){
			for (int x=0; x<dimx; ++x){
				Outputbuffer[z*dimx*dimy+y*dimx+x]=Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)];
			}
		}
	}
	return ;
}


template<typename PixelType>
void Union_RPO_constraint(	PixelType* input_buffer,
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
    rect3dminmax(image, dimx, dimy, dimz, 3,3,3,false);

	// Calling PO for each orientation
    #pragma omp parallel sections
    {
	   #pragma omp section
	   {
		   PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation1,res1);
		   std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation2,res2);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation3,res3);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation4,res4);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   {
	       PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation5,res5);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation6,res6);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_constraint<PixelType>(image,dimz,dimy,dimx,L,orientation7,res7);
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

    // Minimum between the computed RPO on the dilation and the initial image
    rect3dminmax(output_buffer, dimx, dimy, dimz, 3,3,3,true);
	// Minimum between the computed RPO on the dilation and the initial image
		for(int i=0; i<dimx*dimy*dimz;i++)
			output_buffer[i]=std::min(output_buffer[i],input_buffer[i]);

}


#endif






