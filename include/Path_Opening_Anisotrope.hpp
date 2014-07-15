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

#undef max


typedef unsigned long IndexType;
// PixelType defined in NonLocalFilterSioux as long


void Neighbourhood_isotrope(	int nb_col,
							int dim_frame,
	 						std::vector<int> & orientation,
	 						std::vector<int> & upList,
                    		std::vector<int> & downList,
                    		std::vector<float> & length,
                    		float dim_vox_z,
                    		float dim_vox_y,
                    		float dim_vox_x) {

    int col_shift = orientation[0];
    int line_shift = orientation[1];
    int depth_shift = orientation[2];
    
    int min_dist1=sqrt(dim_vox_x*dim_vox_x+dim_vox_y*dim_vox_y);
    int min_dist2=sqrt(dim_vox_z*dim_vox_z+dim_vox_y*dim_vox_y);
    int min_dist3=sqrt(dim_vox_z*dim_vox_z+dim_vox_x*dim_vox_x);
    int max_dist=sqrt(dim_vox_x*dim_vox_x+dim_vox_y*dim_vox_y+dim_vox_z*dim_vox_z);

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
       
      
       length.push_back(max_dist);
       length.push_back(max_dist);
       length.push_back(max_dist);
       length.push_back(max_dist);
       
       length.push_back(min_dist3);
       length.push_back(min_dist2);
       length.push_back(min_dist3);
       length.push_back(min_dist2);
       length.push_back(dim_vox_z);
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
       
       
	   length.push_back(max_dist);
       length.push_back(max_dist);
       length.push_back(max_dist);
       length.push_back(max_dist);
       
       length.push_back(min_dist1);
       length.push_back(min_dist2);
       length.push_back(min_dist1);
       length.push_back(min_dist2);
       length.push_back(dim_vox_y);

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
       
	   length.push_back(max_dist);
       length.push_back(max_dist);
       length.push_back(max_dist);
       length.push_back(max_dist);
       
       length.push_back(min_dist3);
       length.push_back(min_dist1);
       length.push_back(min_dist3);
       length.push_back(min_dist1);
       length.push_back(dim_vox_x);

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
       
       length.push_back(dim_vox_x);
       length.push_back(dim_vox_z);
       length.push_back(dim_vox_y);

       length.push_back(min_dist2);
       length.push_back(min_dist1);
       length.push_back(min_dist3);
       length.push_back(max_dist);

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
       
       length.push_back(dim_vox_x);
       length.push_back(dim_vox_z);
       length.push_back(dim_vox_y);

       length.push_back(min_dist2);
       length.push_back(min_dist1);
       length.push_back(min_dist3);
       length.push_back(max_dist);

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
        
       length.push_back(dim_vox_x);
       length.push_back(dim_vox_z);
       length.push_back(dim_vox_y);

       length.push_back(min_dist2);
       length.push_back(min_dist1);
       length.push_back(min_dist3);
       length.push_back(max_dist);

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


       length.push_back(dim_vox_x);
       length.push_back(dim_vox_z);
       length.push_back(dim_vox_y);

       length.push_back(min_dist2);
       length.push_back(min_dist1);
       length.push_back(min_dist3);
       length.push_back(max_dist);

    }

}





template<typename PixelType,typename IndexType>
void propagation_anisotrope(IndexType p, std::vector<float>&lambda, std::vector<int>&nf, std::vector<int>&nb, std::vector<bool>&b, std::queue<IndexType> &Qc, std::vector<float>&length)
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
		float l=0;
		int ind=0;
		float l_modif=0;
		float add_length=0;
		for (it=nb.begin(); it!=nb.end();++it)
		{
			l_modif=std::max(lambda[q+*it],l);
			if (l_modif>l){
				l=l_modif;
				add_length=length[ind];}
			ind+=1;
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
void PO_3D_anisotrope(	PixelType* Inputbuffer,
								int dimz,
								int dimy,
								int dimx,
								int L,
								std::vector<int>orientations,
								PixelType* Outputbuffer,
								int dim_vox_z,
								int dim_vox_y,
								int dim_vox_x)
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
	std::vector<float>length;
	Neighbourhood_isotrope(new_dimx, dim_frame,orientations,np,nm,length,dim_vox_z,dim_vox_y,dim_vox_x);

	//Create other temporary images
	std::vector<float>Lp(Image.size(),L);
	std::vector<float>Lm(Image.size(),L);

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
			propagation_anisotrope<PixelType,IndexType>(*it,Lm,np,nm,b,Qc,length);
			propagation_anisotrope<PixelType,IndexType>(*it,Lp,nm,np,b,Qc,length);



			while (not Qc.empty())
			{
				IndexType q=Qc.front();
				Qc.pop();
				if (Lp[q]+Lm[q]-1<L)
				{
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


template<typename PixelType>
void RPO_anisotrope(	PixelType* input_buffer,
								PixelType* output_buffer,
								int L,
								std::vector<int>orientation,
								int dimx,
								int dimy,
								int dimz,
								float dim_vox_z,
								float dim_vox_y,
								float dim_vox_x)

{	

	PixelType *res=new PixelType[dimx*dimy*dimz];
	PixelType *image=new PixelType[dimx*dimy*dimz];
	memcpy(&image[0],&input_buffer[0],(dimx*dimy*dimz)*sizeof(PixelType));

    // Dilation by a cube of size 3x3x3
    rect3dminmax(image, dimx, dimy, dimz, 3,3,3,false);
	
	PO_3D_anisotrope<PixelType>(image,dimz,dimy,dimx,L,orientation,res,dim_vox_z,dim_vox_y,dim_vox_x);
	
	// Minimum between the computed RPO on the dilation and the initial image
    rect3dminmax(res, dimx, dimy, dimz, 3,3,3,true);
	
	// Minimum between the computed RPO on the dilation and the initial image
		for(int i=0; i<dimx*dimy*dimz;i++)
			output_buffer[i]=std::min(res[i],input_buffer[i]);
    
}


template<typename PixelType>
void Union_RPO_Anisotrope(	PixelType* input_buffer,
								PixelType* output_buffer,
								int L,
								int dimx,
								int dimy,
								int dimz,
								float dim_vox_z,
								float dim_vox_y,
								float dim_vox_x
								){


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


	// Calling PO for each orientation
	   #pragma omp parallel sections
	   {
	   #pragma omp section
	   {
		RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation1, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
		 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation2, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation3, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation4, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   {
	       RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation5, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	  {
	       RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation6, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
	       RPO_anisotrope<PixelType>(input_buffer,output_buffer, L, orientation7, dimz,dimy,dimx,dim_vox_z,dim_vox_y,dim_vox_x);
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

