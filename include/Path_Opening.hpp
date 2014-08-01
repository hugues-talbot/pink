#ifndef PATH_OPENING_INCLUDED
#define PATH_OPENING_INCLUDED


#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#include <cassert>

#undef max


typedef unsigned long IndexType;

void Neighbourhood(	int nb_col,
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

template<typename PixType>
bool my_sorting_function (const PixType *i,const PixType *j)
// Input: i, j : two variables containing memory adress pointing to PixType variables.
// Return : True if the variable pointed by i is smaller than the variabled pointed by j
{
	return (*i<*j);
}

template<typename PixType>
std::vector<IndexType> sort_image_value(std::vector<PixType> I)
//  Return pixels index of image I sorted according to intensity
{
	std::vector<IndexType> index_image(I.size());
	std::vector<PixType*>index_pointer_adress(I.size());
	IndexType it;
	typename std::vector<PixType>::iterator it1;
	typename std::vector<PixType*>::iterator it2;
	typename std::vector<IndexType>::iterator it3;

	// Fill index_pointer_adress with memory adress of variables in I
	for (it=0,it2=index_pointer_adress.begin(); it!=I.size(); ++it, ++it2)
	{
		*it2=&I[it];
	}

	// Sorting adresses according to intensity
	std::sort(index_pointer_adress.begin(),index_pointer_adress.end(),my_sorting_function<PixType>);

	// Conversion from adresses to index of image I
	for (it3=index_image.begin(),it=0; it!=I.size(); ++it,++it3)
	{
		*it3=static_cast<IndexType>(index_pointer_adress[it]-&I[0]);
	}
	return index_image;
}



template<typename PixelType>
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

template<typename PixType>
void PO_3D(	PixType* Inputbuffer,
								int dimz,
								int dimy,
								int dimx,
								int L,
								std::vector<int>orientations,
								PixType* Outputbuffer)
//Path opening with orientation
{

	// Add border of 2 pixels which value is 0 to *Inputbuffer
	std::vector<PixType> Image((dimz+4)*(dimy+4)*(dimx+4),0);
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
	index_image=sort_image_value<PixType>(Image);

	// Create the offset np and nm
	std::vector<int>np;
	std::vector<int>nm;
	Neighbourhood(new_dimx, dim_frame,orientations,np,nm);

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
			propagate<PixType>(*it,Lm,np,nm,b,Qc);
			propagate<PixType>(*it,Lp,nm,np,b,Qc);



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

// Compute PO with an image which already has a 2-pixel border (good to compute the 7 orientation in parallel)
template<typename PixelType>
void PO_3D(	std::vector<PixelType>Image,
								int new_dimz,
								int new_dimy,
								int new_dimx,
								int L,
								std::vector<int>orientations,
								PixelType* Outputbuffer)
//Path opening with orientation
{

	// Create the temporary image b  (0 for a 1-pixel border, 1 elsewhere)
	std::vector<bool>b(Image.size(),0);
	for (int z=0; z<new_dimz-2; ++z){
		for (int y=0; y<new_dimy-2; ++y){
			for (int x=0; x<new_dimx-2; ++x)
			{
				b[(z+1)*(new_dimx*new_dimy)+(y+1)*new_dimx+(x+1)]=1;
			}
		}
	}

	int dim_frame=new_dimx*new_dimy;

	// Create the sorted list of index of Image according to intensity
	std::vector<IndexType>index_image;
	index_image=sort_image_value<PixelType>(Image);

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
			propagate<PixelType>(*it,Lm,np,nm,b,Qc);
			propagate<PixelType>(*it,Lp,nm,np,b,Qc);



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
	for (int z=0; z<new_dimz-4; ++z){
		for (int y=0; y<new_dimy-4; ++y){
			for (int x=0; x<new_dimx-4; ++x){
				Outputbuffer[z*(new_dimx-4)*(new_dimy-4)+y*(new_dimx-4)+x]=Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)];
			}
		}
	}
	return ;
}

template<typename PixType>
void Union_PO3D(	PixType* input_buffer,
								PixType* output_buffer,
								int L,
								int dimx,
								int dimy,
								int dimz){


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
	//std::vector<PixType> res(dimx*dimy*dimz); // allocation dynamique cache
	PixType *res1=new PixType[dimx*dimy*dimz]; //allocation dynamique
	PixType *res2=new PixType[dimx*dimy*dimz];
	PixType *res3=new PixType[dimx*dimy*dimz];
	PixType *res4=new PixType[dimx*dimy*dimz];
	PixType *res5=new PixType[dimx*dimy*dimz];
	PixType *res6=new PixType[dimx*dimy*dimz];
	PixType *res7=new PixType[dimx*dimy*dimz];


	// Calling PO for each orientation
	   #pragma omp parallel sections
	   {
	   #pragma omp section
	   {
		 PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation1,res1);
		 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation2,res2);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation3,res3);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation4,res4);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   {
	       PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation5,res5);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	  {
	       PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation6,res6);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       PO_3D<PixType>(input_buffer,dimz,dimy,dimx,L,orientation7,res7);
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

}

#endif // PATH_OPENING_INCLUDED




