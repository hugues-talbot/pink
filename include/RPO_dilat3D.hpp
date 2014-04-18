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


#include "RPO.h"
#include "liarp.h"
#include "rect3dmm.hpp"
#include "genfmin.hpp"
#include "genfmax.hpp"



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
void RPO_dilat3D(	PixelType* input_buffer,
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

	PixelType *output_buffer=new PixelType[dimx*dimy*dimz];
	PixelType *input_buffer=new PixelType[dimx*dimy*dimz];
    PixelType *image=((PixelType*)(nim->data));

    memcpy(&input_buffer[0],&image[0],(dimx*dimy*dimz)*sizeof(PixelType));

    // Dilation by a cube of size 3x3x3
    rect3dminmax(image, dimx, dimy, dimz, 3,3,3,false);

	// Calling PO for each orientation
	   #pragma omp parallel sections
	   {
	   #pragma omp section
	   {
		 PO_3D<PixelType>(image,dimz,dimy,dimx,L,orientation1,res1);
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

    std::cout<<"test"<<std::endl;

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
    
    struct xvimage * image1;
    struct xvimage * image2;
    struct xvimage * image3;

    int32_t typepixel = VFF_TYP_4_BYTE;

    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=input_buffer;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=output_buffer;

    image3=copyimage(image1);
    // lmin ecrase image3->image_data
    lmin(image3,image2);

    PixelType* output_buffer=(PixelType*)(image3->image_data);

    free(image1);
    free(image2);
    free(image3);
    
}


void createNeighbourhood(	int nb_col,
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
       upList.push_back( dim_frame - nb_col - 1);
       upList.push_back( dim_frame - nb_col + 1);
       upList.push_back( dim_frame + nb_col -1 );
       upList.push_back( dim_frame + nb_col + 1);

       upList.push_back( dim_frame - 1);
       upList.push_back( dim_frame - nb_col);
       upList.push_back( dim_frame + 1);
       upList.push_back( dim_frame + nb_col);
       upList.push_back( dim_frame );

       downList.push_back( -dim_frame + nb_col + 1);
       downList.push_back( -dim_frame + nb_col - 1);
       downList.push_back( -dim_frame - nb_col + 1 );
       downList.push_back( -dim_frame - nb_col - 1);

       downList.push_back( -dim_frame + 1);
       downList.push_back( -dim_frame + nb_col);
       downList.push_back( -dim_frame - 1);
       downList.push_back( -dim_frame - nb_col);
       downList.push_back( -dim_frame );

    }
    //from up to down main orientation [0 1 0]
    if((depth_shift == 0 && line_shift == 1 && col_shift == 0) ||
      (depth_shift == 0 && line_shift == -1 && col_shift == 0)) {

       upList.push_back(dim_frame + nb_col - 1 );
       upList.push_back(dim_frame + nb_col + 1);
       upList.push_back( -dim_frame + nb_col -1 );
       upList.push_back( -dim_frame + nb_col + 1);


       upList.push_back(nb_col - 1);
       upList.push_back(dim_frame + nb_col);
       upList.push_back( nb_col + 1);
       upList.push_back( -dim_frame + nb_col);
       upList.push_back( nb_col );

       downList.push_back(-dim_frame - nb_col + 1 );
       downList.push_back(-dim_frame - nb_col - 1);
       downList.push_back( dim_frame - nb_col + 1 );
       downList.push_back( dim_frame - nb_col - 1);

       downList.push_back(-nb_col + 1);
       downList.push_back(-dim_frame - nb_col);
       downList.push_back( -nb_col - 1);
       downList.push_back( dim_frame - nb_col);
       downList.push_back( -nb_col );

    }
    //from left to right main orientation [0 0 1]
    if((depth_shift == 0 && line_shift == 0 && col_shift == 1) ||
      (depth_shift == 0 && line_shift == 0 && col_shift == -1)) {

       upList.push_back(-nb_col +1 - dim_frame);
       upList.push_back(-nb_col +1 + dim_frame);
       upList.push_back(nb_col +1 - dim_frame);
       upList.push_back( nb_col +1 + dim_frame);

       upList.push_back(dim_frame + 1);
       upList.push_back(-nb_col + 1);
       upList.push_back(-dim_frame + 1);
       upList.push_back( nb_col + 1);
       upList.push_back( 1 );

       downList.push_back(nb_col -1 + dim_frame);
       downList.push_back(nb_col -1 - dim_frame);
       downList.push_back(-nb_col -1 + dim_frame);
       downList.push_back( -nb_col -1 - dim_frame);

       downList.push_back(-dim_frame - 1);
       downList.push_back(nb_col - 1);
       downList.push_back(dim_frame - 1);
       downList.push_back( -nb_col - 1);
       downList.push_back( -1 );

    }
    //1st main diagonal [1 1 1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == -1)) {

	   upList.push_back(1);
	   upList.push_back(dim_frame);
	   upList.push_back(nb_col);

	   upList.push_back(dim_frame + nb_col);
       upList.push_back(nb_col + 1);
       upList.push_back(dim_frame + 1 );
       //main direction
       upList.push_back(dim_frame + nb_col + 1);

	   downList.push_back(-1);
	   downList.push_back(-dim_frame);
	   downList.push_back(-nb_col);

       downList.push_back( -dim_frame - nb_col);
       downList.push_back( -nb_col - 1);
       downList.push_back( -dim_frame - 1 );

       //main direction
       downList.push_back(-dim_frame - nb_col - 1);

    }
    //2nd main diagonal [1 1 -1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == 1)) {

	   upList.push_back(-1);
	   upList.push_back(dim_frame);
	   upList.push_back(nb_col);

       upList.push_back(dim_frame + nb_col);
       upList.push_back(nb_col - 1);
       upList.push_back(dim_frame - 1);

       //main direction
       upList.push_back( dim_frame + nb_col - 1 );

       downList.push_back( 1 );
	   downList.push_back( -dim_frame  );
	   downList.push_back(-nb_col);

	   downList.push_back(-dim_frame - nb_col);
       downList.push_back(-nb_col +1);
       downList.push_back(-dim_frame + 1 );

       //main direction
       downList.push_back( -dim_frame - nb_col + 1 );

    }
    //3 main diagonal [-1 1 1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == -1)) {

        upList.push_back( 1 );
		upList.push_back( -dim_frame );
        upList.push_back(nb_col);

		upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col + 1);
        upList.push_back( -dim_frame + 1 );

        //main direction
        upList.push_back( -dim_frame + nb_col + 1 );

		downList.push_back( -1 );
		downList.push_back( dim_frame );
		downList.push_back(-nb_col);

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col -1);
        downList.push_back(  dim_frame - 1 );

        //main direction
        downList.push_back( dim_frame - nb_col -1 );

    }
    //4 main diagonal [-1 1 -1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == 1)) {

		upList.push_back( -1 );
		upList.push_back( -dim_frame );
		upList.push_back(nb_col);

        upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col - 1);
        upList.push_back( -dim_frame - 1 );

        //main direction
        upList.push_back( -dim_frame + nb_col - 1 );

		downList.push_back( 1 );
		downList.push_back( dim_frame );
		downList.push_back(-nb_col);

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col + 1);
        downList.push_back(  dim_frame + 1 );

        //main direction
        downList.push_back( dim_frame - nb_col + 1 );

    }

}

template<typename PixelType>
bool my_sorting_function (const PixelType *i,const PixelType *j)
// Input: i, j : two variables containing memory adress pointing to PixelType variables.
// Return : True if the variable pointed by i is smaller than the variabled pointed by j
{
	return (*i<*j);
}

template<typename PixelType,typename IndexType>
std::vector<IndexType> sort_image_value(std::vector<PixelType> I)
//  Return pixels index of image I sorted according to intensity
{
	std::vector<IndexType> index_image(I.size());
	std::vector<PixelType*>index_pointer_adress(I.size());
	IndexType it;
	typename std::vector<PixelType>::iterator it1;
	typename std::vector<PixelType*>::iterator it2;
	typename std::vector<IndexType>::iterator it3;

	// Fill index_pointer_adress with memory adress of variables in I
	for (it=0,it2=index_pointer_adress.begin(); it!=I.size(); ++it, ++it2)
	{
		*it2=&I[it];
	}

	// Sorting adresses according to intensity
	std::sort(index_pointer_adress.begin(),index_pointer_adress.end(),my_sorting_function<PixelType>);

	// Conversion from adresses to index of image I
	for (it3=index_image.begin(),it=0; it!=I.size(); ++it,++it3)
	{
		*it3=static_cast<IndexType>(index_pointer_adress[it]-&I[0]);
	}
	return index_image;
}


template<typename PixelType,typename IndexType>
void propagate(IndexType p, std::vector<int>&lambda, std::vector<int>&nf, std::vector<int>&nb, std::vector<bool>&b, std::queue<IndexType> &Qc)
// Propagation from pixel p
{
	std::queue<IndexType> Qq;
	lambda[p]=0;

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
		IndexType q=Qq.front();
		Qq.pop();
		int l=0;
		for (it=nb.begin(); it!=nb.end();++it)
		{
			l=std::max(lambda[q+*it],l);
		}
		l+=1;

		if (l<lambda[q])
		{
			lambda[q]=l;
			Qc.push(q);
			for (it=nf.begin(); it!=nf.end(); ++it)
			{
				if (b[q+*it])
				{
					Qq.push(q+*it);
				}
			}
		}
	}
}

template<typename PixelType>
void PO_3D(	PixelType* Inputbuffer,
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

	//Create other temporary images
	std::vector<int>Lp(Image.size(),L);
	std::vector<int>Lm(Image.size(),L);

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
			propagate<PixelType,IndexType>(*it,Lm,np,nm,b,Qc);
			propagate<PixelType,IndexType>(*it,Lp,nm,np,b,Qc);



			while (not Qc.empty())
			{
				IndexType q=Qc.front();
				Qc.pop();
				if (Lp[q]+Lm[q]-1<L)
				{
					//std::cout <<"Image["<<q<< "]= "<< Image[*it]<< std::endl;
					Image[q]=Image[*it];
					b[q]=0;
					Lp[q]=0;
					Lm[q]=0;
				}
			}
		}
	}

	//Outputbuffer=&Image[0];

	// Remove border
	for (int z=0; z<dimz; ++z){
		for (int y=0; y<dimy; ++y){
			for (int x=0; x<dimx; ++x){
				Outputbuffer[z*dimx*dimy+y*dimx+x]=Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)];
			}
		}
	}
	return ;
}










