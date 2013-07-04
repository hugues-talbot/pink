/******** RPO 3D OPERATOR ****/
//F.COKELAER - IFPEN - 06/2013


#ifndef __RPO_HPP__
#define __RPO_HPP__

#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <cstdlib>

#define max(a,b) ((a > b) ? a : b)



/******create neighbour pixel's index from a given orientation vector ********/
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
    
    //1st main diagonal (version2) [2 2 2]
    if((depth_shift == 2 && line_shift == 2 && col_shift == 2) ||
      (depth_shift == -2 && line_shift == -2 && col_shift == -2)) {

	   upList.push_back(dim_frame + nb_col);
       upList.push_back(nb_col + 1); 
       upList.push_back(dim_frame + 1 );  
       //main direction
       upList.push_back(dim_frame + nb_col + 1);


       downList.push_back( -dim_frame - nb_col);
       downList.push_back( -nb_col - 1);
       downList.push_back( -dim_frame - 1 );
       //main direction
       downList.push_back(-dim_frame - nb_col - 1);

    }

	 //2nd main diagonal (version2) [2 2 -2]
    if((depth_shift == 2 && line_shift == 2 && col_shift == -2) ||
      (depth_shift == -2 && line_shift == -2 && col_shift == 2)) {

       upList.push_back(dim_frame + nb_col);
       upList.push_back(nb_col - 1);
       upList.push_back(dim_frame - 1);
       //main direction
       upList.push_back( dim_frame + nb_col - 1 );

	   downList.push_back(-dim_frame - nb_col);
       downList.push_back(-nb_col +1);
       downList.push_back(-dim_frame + 1 );
       //main direction
       downList.push_back( -dim_frame - nb_col + 1 );

    }

	 //3 main diagonal (version2) [-2 2 2]
    if((depth_shift == -2 && line_shift == 2 && col_shift == 2) ||
      (depth_shift == 2 && line_shift == -2 && col_shift == -2)) {

		upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col + 1);
        upList.push_back( -dim_frame + 1 );
        //main direction
        upList.push_back( -dim_frame + nb_col + 1 );

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col -1);
        downList.push_back(  dim_frame - 1 );
        //main direction
        downList.push_back( dim_frame - nb_col -1 );

    }

	 //4 main diagonal (version2) [-2 2 -2]
    if((depth_shift == -2 && line_shift == 2 && col_shift == -2) ||
      (depth_shift == 2 && line_shift == -2 && col_shift == 2)) {

        upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col - 1);
        upList.push_back( -dim_frame - 1 );
        //main direction
        upList.push_back( -dim_frame + nb_col - 1 );

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col + 1);
        downList.push_back(  dim_frame + 1 );
        //main direction
        downList.push_back( dim_frame - nb_col + 1 );

    }

}
/**************************** Robust Path propagation from seed points *********************************************/
inline void propagateRobustPath(		 	 int seed_index,
                      						 std::vector< int > & upList,
                      						 std::vector< int > & downList,                               
                      						 std::queue< int > & QueueC,
											 std::queue< int>  & QueueQ,
                      						 std::vector< short > & FlagI,
                      						 std::vector< int> & DownstreamI
                                  ) {
    int upstream_from_seed;
    int downstream_from_upstream;
    int propagate_upstream;
    //max lenght from downstream neighbours
    int max_lenght;

    //enqueue in Qq all neigbours of current px based on given direction
    for(int i = 0; i < (int) upList.size(); ++i) {
        upstream_from_seed = upList[i] + seed_index;
        // if active and not in queueQ
        if ( FlagI[upstream_from_seed]&(1<<0) &&
            !(FlagI[upstream_from_seed]&(1<<1))) {
        	//put in Qq
            QueueQ.push(upstream_from_seed);
            //flag
            FlagI[upstream_from_seed] |= (1<<1);
        }
        //if noise and not in queueQ
        if ( FlagI[upstream_from_seed]&(1<<4) &&
            !(FlagI[upstream_from_seed]&(1<<1))) {
        	//put in Qq
            QueueQ.push(upstream_from_seed);
            //flag
            FlagI[upstream_from_seed] |= (1<<1);
        }
    }

    while(!QueueQ.empty()) {
    	//get first queueC element 
        upstream_from_seed = QueueQ.front();
        max_lenght = -1;
        for (int i = 0; i < (int) downList.size(); ++i) {
        	//get downstream pixel index
            downstream_from_upstream = upstream_from_seed + downList[i];
            //select max lenght 
            if ( DownstreamI[downstream_from_upstream] > max_lenght) {
            	max_lenght = DownstreamI[downstream_from_upstream] ;
            }
        }
        //check changes in lenght
        if ( (max_lenght + 1) <  DownstreamI[upstream_from_seed] ) {
            //if new lenght is smaller than previous,
            //enqueue upstream pixels (active and noise)
            DownstreamI[upstream_from_seed] =  max_lenght + 1;
            for(int i = 0; i < (int) upList.size(); ++i) {
                propagate_upstream = upstream_from_seed + upList[i];
                if(FlagI[propagate_upstream]&(1<<0) &&
                !(FlagI[propagate_upstream]&(1<<1)) ) {
                	QueueQ.push(propagate_upstream);
                	FlagI[propagate_upstream] |= (1<<1);
                }
                //robust part
                if( FlagI[propagate_upstream]&(1<<4) &&
                !(FlagI[propagate_upstream]&(1<<1)) ) {
                	QueueQ.push(propagate_upstream);
                	FlagI[propagate_upstream] |= (1<<1);
                }
            }
            //enqueue current neighbour in Qc 
            if(!(FlagI[upstream_from_seed]&(1<<2))) {
            	QueueC.push(upstream_from_seed);
                FlagI[upstream_from_seed] |= (1<<2);
            }
        }
        //pop first element from queueQ
        QueueQ.pop();
        //to corresponding flagQ bit to 0
        FlagI[upstream_from_seed] &= ~(1<<1);
    } //end while
 }
//
template <class T>
bool pointer_value_comparison(const T *a, const T *b)
{

        return ( (*a) < (*b));
}


//sorting pixel grey level value of input image by ascending order
template <class T>
void sort_image_value(	std::vector<T> &originalI,
                  		int image_size,
                  		std::vector<int> &im_idx_sort)
{
        //allocate memory for sorted pointers array
        std::vector<T*> sorted_ptr(image_size);
        //fill sorted pointers array
        for (int i=0; i<image_size; ++i)
            sorted_ptr[i] = &originalI[0] + i;

        //sorting ptr's int value
		std::sort(sorted_ptr.begin(), sorted_ptr.end(), pointer_value_comparison<T>);
        //associated image indices to sorted ptr value
        for (int i =0;i<image_size;++i)
        	im_idx_sort[i] = (int) (sorted_ptr[i] - &originalI[0]);

}
//RPO 3D by orientation
template <class T>
	void RPO3D( T* input_buffer, //input image
				T* output_buffer, //ouput image
				std::vector<int> & orientation, // orientation vector
				int L, // path length 
				int G, // gap length 
				int reconstruction, // if == 1 THEN CLOSING AFTER THE OPENING , if == 0 ONLY OPENING
				int dimx, // image dimensions
				int dimy,
				int dimz
			)
{
	
	//get padded image dimensions
	int* dim_padded = new int[3];
	dim_padded[0] = dimx+4; // x axis
	dim_padded[1] = dimy+4; // y axis
	dim_padded[2] = dimz+4; // z axis

	//padded image size
	int image_size = dim_padded[0]*dim_padded[1]*dim_padded[2];
	int frame_size = dim_padded[0]*dim_padded[1];
	
	//creating and init temporaries buffers
	std::vector<T> originalI(image_size,0);
	std::vector<int> downstreamI(image_size,L);
	std::vector<int> upstreamI(image_size,L);
	std::vector<short> flagI(image_size,1);
	//vector of sorted index of the image
	std::vector<int> im_idx_sort(image_size);

	//FIFO queue init
	std::queue< int > queueQ; 
	std::queue< int > queueC;
	 
	//neighbour's index vector init
	std::vector< int > up_neighbour;
	std::vector< int > down_neighbour;
    
	/***********flags description***************/
	/* in flagI, 6 flags are used in this implementation :
	/*         1            0
	flag 0 : active / desactive
	flag 1 : in queueQ / not in queueQ
	flag 2 : in queueC / not in queueC
	flag 3 : desactivated during propagation / not desactivated during propagation
	flag 4 : noise / not noise
	flag 5 : not noise at next level / can be noise at next level 
	flag 6 : temp flag for noise pixels research ( in queueN2)
	*/	
	
	//copy input_buffer into originalI vector
	for(int k = 0; k<dimz; k++) {
		for(int j=0; j<dimy; j++) {
			for (int i=0; i<dimx; i++) {
				originalI[((dimx+4)*(dimy+4))*(k+2) + (dimx+4)*(j+2) + (i+2)]
				 = input_buffer[(dimx*dimy)*(k) + dimx*(j) + (i)];
			}
		}
	}

	//create neighbourhood index from a given direction
	createNeighbourhood(	dim_padded[0],
							frame_size,
							orientation,	
							up_neighbour,
							down_neighbour);
							
	//sorting image values
    sort_image_value(	originalI,
                  		image_size,
                  		im_idx_sort);
   						
   	/*for(int i = 0; i<image_size;++i)
   		std::cout<<(int) originalI[im_idx_sort[i]]<<" ";*/
    //border treatment : to prevent border pixels to be flagged as noise pixels
    for(int k=0; k <2; ++k ) {
       for(int i =0; i<dim_padded[1]; ++i) {
           for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               flagI[k*frame_size + i*dim_padded[0] + j] &= ~(1<<0);
            }
        }
     }
     for(int k=(dim_padded[2]-2); k <dim_padded[2]; ++k ) {
         for(int i =0; i<dim_padded[1]; ++i) {
             for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
              flagI[k*frame_size + i*dim_padded[0] + j] &= ~(1<<0);
             }
         }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =0; i<dim_padded[1]; ++i) {
             for(int j=0;j<2;j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               flagI[k*frame_size + i*dim_padded[0] + j] &= ~(1<<0);
             }
          }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =0; i<dim_padded[1]; ++i) {
             for(int j=(dim_padded[0]-2);j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               flagI[k*frame_size + i*dim_padded[0] + j] &= ~(1<<0);
             }
         }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =0; i<2; ++i) {
             for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               flagI[k*frame_size + i*dim_padded[0] + j] &= ~(1<<0);
             }
         }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =(dim_padded[1]-2); i<dim_padded[1]; ++i) {
             for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               flagI[k*frame_size + i*dim_padded[0] + j] &= ~(1<<0);
              }
         }
     }			
     		
    //queue used to store noise pixel at different steps
    std::queue< int > queueNoise;
    std::queue< int > queueNoise_check;
    std::queue< int > prop_Noise;
    int in_queueNoise;
    int in_propNoise;
    
    //seed index
    int iii = 0;
    //temp index
    int iiii = 0;
    //corresponding image memory address
    int seed_index;
    int temp_seed_index;
    //seed pixel value
    T threshold;
    double temp_threshold = -1;
    double temp_threshold_noise = -1;
    //longest path through a pixel
    int longest_path = 0;
    //pixel index in queueC
    int in_queueC;

    //pixel coordinate in frame, row, col
    int px_frame_idx;
    int px_line_idx;
    int px_col_idx;
    /*noise pixel research var init */
    //noise pixel index
    int temp_noise_index;
    int nb_prop_up = 1;
    int nb_prop_down = 1;
    //queues used to propagate noise path
    std::queue<int> queueN1;
    std::queue<int> queueN2;
    int in_queueN1;
    int in_queueN2;
    int upstream_noise_px;
    int downstream_noise_px;
    int it_active_pixel_found_during_up_prop = 0;
    int it_active_pixel_found_during_down_prop = 0;
    bool active_pixel_found_during_up_prop = false;
    bool active_pixel_found_during_down_prop = false;

    //scanning entire image
    while(iii<image_size) {
        //get seed pixel index 
        seed_index = im_idx_sort[iii];
        //corresponding coord
        px_frame_idx = seed_index/frame_size;
        px_line_idx = (seed_index- px_frame_idx*frame_size)/dim_padded[0];
        px_col_idx = (seed_index - px_frame_idx*frame_size)%dim_padded[0];
        //propagation is allowed if
        //not in the border 
        //not desactivated during the propagation of the path
        //not noise pixel
        if( !(flagI[seed_index]&(1<<3)) &&
        	!(flagI[seed_index]&(1<<4)) &&
        	 px_line_idx > 0 &&
             px_line_idx <(dim_padded[1]-1) &&
             px_col_idx > 0 &&
             px_col_idx <(dim_padded[0]-1)  &&
             px_frame_idx>0 &&
            px_frame_idx<(dim_padded[2]-1)
            ) {
        	//get seed pixel value
        	threshold = originalI[seed_index];
        	
        	//desactivate each of the pixels active and whose value <= threshold
        	if(temp_threshold < threshold) {
            	iiii=iii;
            	temp_seed_index = im_idx_sort[iiii];
            	//int temp_val = (int) originalI[temp_seed_index]; 
            	while(originalI[temp_seed_index]<= threshold &&
                  	iiii<image_size) {
                	//if pixel is active
                	if(flagI[temp_seed_index]&(1<<0)) {
                    	//set to inactive
                    	flagI[temp_seed_index] &= ~(1<<0);
                	}
                	++iiii;
                	//update temp_pixel_coord
                	if ( iiii < image_size) {
                    	temp_seed_index = im_idx_sort[iiii];
                	}
                	if (iiii == image_size)
                    	break;
            	}
            temp_threshold = threshold;
        	}
        	
        	/*****noise pixel research ********/
    		/***this research is made within the previous 
    		desactivated set *****/
        	if(temp_threshold_noise<threshold) {
        		//get current seed index
        		iiii=iii;
            	temp_noise_index = im_idx_sort[iiii];
            	//int temp_val = (int) originalI[temp_seed_index]; 
        		while((int)originalI[temp_noise_index]<= threshold &&
                  	iiii<image_size) {
                  	//if this pixel is possibly a noise pixel                 	
                	if (!(flagI[temp_noise_index]&(1<<5))) {
                		//re-init var
                		active_pixel_found_during_up_prop = false;
    					active_pixel_found_during_down_prop = false;
    					//init number of iteration to encounter an active pixel
    					nb_prop_up = 1;
                    	nb_prop_down = 1;
                		//upstream direction
    					//add seed pixel in queueN1
    					queueN1.push(temp_noise_index);
    					// while don't reach maximum of K consecutive propagation
    					while(nb_prop_up <= G) {
    						while(!queueN1.empty()) {
       							//get first element of queueN1
        						in_queueN1 = queueN1.front();
        						//propagate path on upstream neighbour
        						// in the desactivate set
        						for(int i = 0; i < (int) up_neighbour.size();++i){
        							//check if pixel is inactive, possibly noise pixel and not in the border
            						upstream_noise_px = in_queueN1 + up_neighbour[i];
            						// if :  
            						//we don't reach the maximum allowed iteration
            						//upstream neighbour  is desactivated
            						// not in queueN2 and possibly noise pixel
            						if((nb_prop_up < G) && 
            						!(flagI[upstream_noise_px]&(1<<0)) &&
            						!(flagI[upstream_noise_px]&(1<<6)) &&
            						!(flagI[upstream_noise_px]&(1<<5)) ) {
            							//push pixel in queueN2
             							queueN2.push(upstream_noise_px);
             							//set its corresponding flag to 1
                						flagI[upstream_noise_px] |= (1<<6);
            						}
            						//if upstream noise pixel is active and the number of max propagation
            						//don't reach
            						if(flagI[upstream_noise_px]&(1<<0) &&
            						nb_prop_up <= G) {
            							active_pixel_found_during_up_prop = true;
            							//save upstream propagation number
            							it_active_pixel_found_during_up_prop = nb_prop_up;
            							break;
            						}
        						}
       						//get next queueN1 element
       						queueN1.pop();
							if(active_pixel_found_during_up_prop == true)
           						break; 
      						}	
        					//ping ponging process between queueN1 and queueN2
        					while(!queueN2.empty()) {
        						//get first element of queueN2
            					in_queueN2 = queueN2.front();
            					//push in queue1
            					queueN1.push(in_queueN2);
           				 		//delete element from queue2
            					queueN2.pop();
            					//deflag
            					flagI[in_queueN2] &= ~(1<<6);
        					}
        
        					if(active_pixel_found_during_up_prop == true)
           						break; 
           						
							//if no active pixel found until the maximum number of
							//iteration
        					if(nb_prop_up==G &&
          					active_pixel_found_during_up_prop == false) {
           						//pixel canno't be a noise pixel at next level
            					flagI[temp_noise_index] |= (1<<5);
           						//Set corresponding lenght value to 0
            					downstreamI[temp_noise_index] = 0;
           						upstreamI[temp_noise_index] = 0;
           					}
            				//increment upstream propagation number
        					nb_prop_up++;
        				}
        
						//re-init queueN1 and queueN2 for downstream propagation
    					while(!queueN1.empty())
       						queueN1.pop();

    					while(!queueN2.empty())
       						queueN2.pop();
						//downstream direction
        				//the implementation is the same as for upstream propagation
        				queueN1.push(temp_noise_index);
        				while(nb_prop_down <= G &&
            				!(flagI[temp_noise_index]&(1<<5))) {
        					while(!queueN1.empty()) {
            					//get first element of queueN1
                				in_queueN1 = queueN1.front();
                				for(int i =0; i < (int) down_neighbour.size(); ++i) {
                    				downstream_noise_px = in_queueN1 + down_neighbour[i];
                    				if((nb_prop_down < G) &&
                    				!(flagI[downstream_noise_px]&(1<<0)) &&
                    				!(flagI[downstream_noise_px]&(1<<6)) &&
                    				!(flagI[downstream_noise_px]&(1<<5))) {
                    					//put downstream neighbour in queueN2
                        				queueN2.push(downstream_noise_px);
                        				//set corresponding flag
                        				flagI[downstream_noise_px] |= (1<<6);
                    				}
                    				//if downstream neighbour is active and number of propagation is not reached
                    				if(flagI[downstream_noise_px]&(1<<0) &&
                    				nb_prop_down <= G) {
                     					//save downstream iteration 
                        				active_pixel_found_during_down_prop = true;
                        				it_active_pixel_found_during_down_prop = nb_prop_down;
                         				break;
                    				}	
                				}
                 				//next queueN1 element
                 				queueN1.pop();
                 				if(active_pixel_found_during_down_prop==true)
                   	 				break;
             				}
             				//ping ponging process between queueN1 and queueN2
             				while(!queueN2.empty()) {
             					//get first element of queueN2
            	 				in_queueN2 = queueN2.front();
             					//push in queue1
             					queueN1.push(in_queueN2);
             					//delete element from queue2
             					queueN2.pop();
             					//deflag
             					flagI[in_queueN2] &= ~(1<<6);
             				}  
             				   
             				if(active_pixel_found_during_down_prop == true)
             					break;
							
							//if no active pixel found until the maximum number of
							//iteration
             				if(nb_prop_down==G &&
            			 	active_pixel_found_during_down_prop == false ) {
             					//pixel canno't be a noise pixel at next level
             					flagI[temp_noise_index] |= (1<<5);
                				//Set corresponding lenght value to 0
                				downstreamI[temp_noise_index] = 0;
                				upstreamI[temp_noise_index] = 0;
              				}
             				nb_prop_down++;
        				}
						//re-init queueN1 and queueN2 for downstream propagation
   						while(!queueN1.empty())
    						queueN1.pop();

    					while(!queueN2.empty())
    						queueN2.pop();

    					//get temp_px_noise status (i.e noise or not noise at next threshold)
    					if ( active_pixel_found_during_down_prop == true &&
    					active_pixel_found_during_up_prop == true ) {
    						if ( (it_active_pixel_found_during_down_prop +
        					it_active_pixel_found_during_up_prop -1) <= G ) {
								//current pixel is a noise pixel
        						queueNoise.push(temp_noise_index);
            					flagI[temp_noise_index] |= (1<<4);

        					}
        					else {
        						//current pixel is not a noise pixel
            					flagI[temp_noise_index] |= (1<<5);
            					downstreamI[temp_noise_index] = 0;
            					upstreamI[temp_noise_index] = 0;
            				}
    					}
    			
    				} // endif possibly noise pixel
    				//next pixel	
            		iiii++;

            		if(iiii == image_size)
                 		break;
             
             		temp_noise_index = im_idx_sort[iiii];
				}
        
           
        		/******update noise pixel status obtained from previous threshold**********/
        		/******the implementation is the same as for noise research process ****/
        		while(!queueNoise_check.empty()) {
        			//get pixel index
        			temp_noise_index = queueNoise_check.front();
    		
        			if(flagI[temp_noise_index]&(1<<4)) {
        				active_pixel_found_during_up_prop = false;
    					active_pixel_found_during_down_prop = false;
    					nb_prop_up = 1;
                    	nb_prop_down = 1;
        				queueN1.push(temp_noise_index);

    					while(nb_prop_up <= G) {
    						while(!queueN1.empty()) {
       							//get first element of queueN1
        						in_queueN1 = queueN1.front();
        						//propagate path on upstream neighbour
        						// in the desactivate set
        						for(int i = 0; i < (int) up_neighbour.size();++i){

            						upstream_noise_px = in_queueN1 + up_neighbour[i];

            						if((nb_prop_up < G) && 
            						!(flagI[upstream_noise_px]&(1<<0)) &&
            						!(flagI[upstream_noise_px]&(1<<6)) &&
            						!(flagI[upstream_noise_px]&(1<<5)) ) {
            							//push pixel in queueN2
             							queueN2.push(upstream_noise_px);
             							//set its corresponding flag to 1
                						flagI[upstream_noise_px] |= (1<<6);
            						}

            						if(flagI[upstream_noise_px]&(1<<0) &&
            						nb_prop_up <= G) {
            							active_pixel_found_during_up_prop = true;
            							//save upstream propagation number
            							it_active_pixel_found_during_up_prop = nb_prop_up;
            							break;
            						}
        						}
       							//get next queueN1 element
       							queueN1.pop();
       							
								if(active_pixel_found_during_up_prop == true)
           							break; 
      						}
        					//ping ponging process between queueN1 and queueN2
        					while(!queueN2.empty()) {
        						//get first element of queueN2
            					in_queueN2 = queueN2.front();
            					//push in queue1
            					queueN1.push(in_queueN2);
            					//delete element from queue2
            					queueN2.pop();
           						//deflag
            					flagI[in_queueN2] &= ~(1<<6);
        					}
        
        					if(active_pixel_found_during_up_prop == true)
           						break; 

        					if(nb_prop_up==G &&
          					active_pixel_found_during_up_prop == false) {
           						//pixel canno't a noise pixel at next level
           						flagI[temp_noise_index] &= ~(1<<4);
            					flagI[temp_noise_index] |= (1<<5);
            					//Set corresponding lenght value to 0
            					downstreamI[temp_noise_index] = 0;
            					upstreamI[temp_noise_index] = 0;
            					//push in propagation queue
            					prop_Noise.push(temp_noise_index);
            					//reconstruction 
            					if (reconstruction == 1)
            						originalI[temp_noise_index] = threshold;
           					}
            				//increment upstream propagation number
        					nb_prop_up++;
        				}
        
						//re-init queueN1 and queueN2 for downstream propagation
    					while(!queueN1.empty())
        					queueN1.pop();

   						while(!queueN2.empty())
       						queueN2.pop();
						//downstream propagation

        				queueN1.push(temp_noise_index);
        				while(nb_prop_down <= G &&
            			!(flagI[temp_noise_index]&(1<<5))) {
        					while(!queueN1.empty()) {
            					//get first element of queueN1
                				in_queueN1 = queueN1.front();
                				//propagate path on downstream neighbour
                				// in the desactivate set
                				for(int i =0; i < (int) down_neighbour.size(); ++i) {
                					//get current downstream neighbour coord
                    				downstream_noise_px = in_queueN1 + down_neighbour[i];
                    				if((nb_prop_down < G) &&
                    				!(flagI[downstream_noise_px]&(1<<0)) &&
                    				!(flagI[downstream_noise_px]&(1<<6)) &&
                    				!(flagI[downstream_noise_px]&(1<<5))) {
                    					//put downstream neighbour in queueN2
                        				queueN2.push(downstream_noise_px);
                        				//set corresponding flag
                        				flagI[downstream_noise_px] |= (1<<6);
                    				}
                    				//if downstream neighbour is active and number of propagation is not reached
                    				if(flagI[downstream_noise_px]&(1<<0) &&
                    				nb_prop_down <= G) {
                     					//save downstream propagation number
                        				active_pixel_found_during_down_prop = true;
                        				it_active_pixel_found_during_down_prop = nb_prop_down;
                        				break;
                    				}
                				}
                 				//next queueN1 element
                 				queueN1.pop();
                 				
                 				if(active_pixel_found_during_down_prop==true)
                   				 	break;
             				}

             				//ping ponging process between queueN1 and queueN2
             				while(!queueN2.empty()) {
             					//get first element of queueN2
             					in_queueN2 = queueN2.front();
             					//push in queue1
             					queueN1.push(in_queueN2);
             					//delete element from queue2
             					queueN2.pop();
             					//deflag
             					flagI[in_queueN2] &= ~(1<<6);
             				}     
             				
            				if(active_pixel_found_during_down_prop == true)
             					break;

             				if(nb_prop_down==G &&
             				active_pixel_found_during_down_prop == false ) {
             					//pixel canno't be a noise pixel at further level
                				flagI[temp_noise_index] &= ~(1<<4);
             					flagI[temp_noise_index] |= (1<<5);
                				//Set corresponding lenght value to 0
                				downstreamI[temp_noise_index] = 0;
                				upstreamI[temp_noise_index] = 0;
                				prop_Noise.push(temp_noise_index);
                				if (reconstruction == 1)
            						originalI[temp_noise_index] = threshold;
              				}
              				nb_prop_down++;
        				}
						//re-init queueN1 and queueN2 for downstream propagation
   						while(!queueN1.empty())
    						queueN1.pop();

    					while(!queueN2.empty())
    						queueN2.pop();

    					//get seed_px_noise status ( i.e noise or not noise at higher threshold level)
    					if ( active_pixel_found_during_down_prop == true &&
    					active_pixel_found_during_up_prop == true ) {
    						if ( (it_active_pixel_found_during_down_prop +
        					it_active_pixel_found_during_up_prop -1) <= G ) {
        						queueNoise.push(temp_noise_index);
        					}
        				else {
        						flagI[temp_noise_index] &= ~(1<<4);
            					flagI[temp_noise_index] |= (1<<5);
            					downstreamI[temp_noise_index] = 0;
            					upstreamI[temp_noise_index] = 0;
            					prop_Noise.push(temp_noise_index);
            					//reconstruction
            					if (reconstruction == 1)
            						originalI[temp_noise_index] = threshold;
            					}
    					}
    				}		
        			queueNoise_check.pop();
        		}
        		//update noise threshold
        	    temp_threshold_noise = threshold;	
        	    
        	    //ping-ponging queueNoise and queueNoise_check
        		while(!(queueNoise.empty())) {
            		//get first queue element
            		in_queueNoise = queueNoise.front();
           			queueNoise_check.push(in_queueNoise);
            		queueNoise.pop();
        		}
        	} // endif temp_threshold_noise<threshold
        	
        	/*******path propagation from prop_Noise queue pixels******/
        	/** i.e pixels that are no longer flag as noise after 
        	the updating process ***/
        	while(!(prop_Noise.empty())) {

            	in_propNoise = prop_Noise.front();

            	propagateRobustPath(in_propNoise,
                                	up_neighbour,
                                	down_neighbour,
                                	queueC,
									queueQ,
                                	flagI,
                                	downstreamI);
            	propagateRobustPath(in_propNoise,
                                	down_neighbour,
                                	up_neighbour,
                                	queueC,
									queueQ,
                                	flagI,
                                	upstreamI);

            	//check queueC if total lenght trought pixel fell down to L
            	while(!queueC.empty()) {
                	in_queueC = queueC.front();
                    //compute lenght
                    longest_path = upstreamI[in_queueC] +
                                downstreamI[in_queueC] - 1;
                    //check longest path
                    if(longest_path < L) {
                        //is active pixel
                        if(flagI[in_queueC]&(1<<0)) {
                            //write into output image ( not be part of a L path at higher level)
                            originalI[in_queueC] = threshold;
                            //desactivate pixel
                            flagI[in_queueC] &= ~(1<<0);
                            //reset length
                            upstreamI[in_queueC] = 0;
                            downstreamI[in_queueC] = 0;
                            // set to 1 
                            //desactivated during propagation
                            //canno't be noise at next thtreshold
                            flagI[in_queueC] |= (1<<3);
                            flagI[in_queueC] |= (1<<5);
                        }
                        //if noise pixel
                        if(flagI[in_queueC]&(1<<4)) {
                        	//reset noise flag
                        	flagI[in_queueC] &= ~(1<<4);
                             //cannot be noise pixel at further level
                            flagI[in_queueC] |= (1<<5);
                            flagI[in_queueC] |= (1<<3);
                            //reset length
                            downstreamI[in_queueC] = 0;
                            upstreamI[in_queueC] = 0;
                            //id reconstruction is set to 1
                            if (reconstruction == 1)
            						originalI[in_queueC] = threshold;
                        }
                    }
                    //pop pixel from queueC
                    queueC.pop();
                    //set flag to 0
                    flagI[in_queueC] &= ~(1<<2);
                }
            	prop_Noise.pop();
        	}   	
    		/***************  path propagation from seed **************/   		
        	propagateRobustPath(seed_index,
                      up_neighbour,
                      down_neighbour,
                      queueC,
					  queueQ,
                      flagI,
                      downstreamI);
        	propagateRobustPath(seed_index,
                      down_neighbour,
                      up_neighbour,
                      queueC,
					  queueQ,
                      flagI,
                      upstreamI);
                      
         	//check queueC if total lenght trought pixel fell down to L
            while(!queueC.empty()){
            	in_queueC = queueC.front();
            	//compute lenght
                longest_path = upstreamI[in_queueC] +
                                downstreamI[in_queueC] - 1;
                //check longest path
                if(longest_path < L) {
                	//if active pixel
                    if(flagI[in_queueC]&(1<<0)) {
                       //write into output image ( not be part of a L path at higher level)
                       originalI[in_queueC] = threshold;
                       //desactivate pixel
                       flagI[in_queueC] &= ~(1<<0);
                       //set his downstream and upstream lenght to 0
                       upstreamI[in_queueC] = 0;
                       downstreamI[in_queueC] = 0;
                       //not enter into noise research process
                       flagI[in_queueC] |= (1<<5);
                       //not need to be considered as seed at next threshold
                       flagI[in_queueC] |= (1<<3);    
                    }
                    //if noise pixel
                    if(flagI[in_queueC]&(1<<4)) {
                    	//reset noise flag
                       	flagI[in_queueC] &= ~(1<<4);
                        //cannot be noise pixel at further level
                        flagI[in_queueC] |= (1<<5);
                        flagI[in_queueC] |= (1<<3);
                        //reset length
                        downstreamI[in_queueC] = 0;
                        upstreamI[in_queueC] = 0;
                        //if reconstruction is set to 1
                        if (reconstruction == 1)
            				originalI[in_queueC] = threshold;        
                    }
                }
                //pop pixel from queueC
                queueC.pop();
                //set flag to 0
                flagI[in_queueC] &= ~(1<<2);
             }
         ++iii;
        } else {
           ++iii;
        }     
    }
    
	//copy output buffer
	for(int k = 0; k<dimz; k++) {
		for(int j=0; j<dimy; j++) {
			for (int i=0; i<dimx; i++) {
				output_buffer[(dimx*dimy)*(k) + dimx*(j) + (i)] =
					originalI[((dimx+4)*(dimy+4))*(k+2) + (dimx+4)*(j+2) + (i+2)];
			}
		}
	}
		
	//free memory
	delete[] dim_padded;	
}
//performing the union of RPO3D on seven orientations
template <class T>
	void UNION_RPO3D(T* input_buffer,
					T* output_buffer,
					int L,
					int G,
					int reconstruction,
					int dimx,
					int dimy,
					int dimz)
{
	std::cout<<"ROBUST PATH OPENINGS UNION ON 7 ORIENTATIONS : L = "<<L<<" "<<"G = "<<G<<std::endl;
	//allocate result memory
	T* res1= new T[	dimx*dimy*dimz];
	T* res2= new T[	dimx*dimy*dimz];
	T* res3= new T[	dimx*dimy*dimz];
	T* res4= new T[	dimx*dimy*dimz];
	T* res5= new T[	dimx*dimy*dimz];
	T* res6= new T[	dimx*dimy*dimz];
	T* res7= new T[	dimx*dimy*dimz];

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
	//
	//calling RPO
	 #pragma omp parallel sections 
	 {
		#pragma omp section 
	   { 
		 RPO3D<T>(input_buffer,res1,orientation1,L,G,reconstruction,dimx,dimy,dimz);
		 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
		   RPO3D<T>( input_buffer,res2,orientation2,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res3,orientation3,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res4,orientation4,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res5,orientation5,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res6,orientation6,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res7,orientation7,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation7 -1 1 -1 : passed"<<std::endl;
	   }
	 }
	
	 //Union
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=res1[i];
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res2[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res3[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res4[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res5[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res6[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res7[i],output_buffer[i]);

	//desallocation
	delete[] res1;
	delete[] res2;
	delete[] res3;
	delete[] res4;
	delete[] res5;
	delete[] res6;
	delete[] res7;
}


//performing the union of RPO3D on seven orientations
template <class T>
	void UNION_RPO3D_new_diag(T* input_buffer,
					T* output_buffer,
					int L,
					int G,
					int reconstruction,
					int dimx,
					int dimy,
					int dimz)
{
	std::cout<<"ROBUST PATH OPENINGS UNION ON 7 ORIENTATIONS : L = "<<L<<" "<<"G = "<<G<<std::endl;
	//allocate result memory
	T* res1= new T[	dimx*dimy*dimz];
	T* res2= new T[	dimx*dimy*dimz];
	T* res3= new T[	dimx*dimy*dimz];
	T* res4= new T[	dimx*dimy*dimz];
	T* res5= new T[	dimx*dimy*dimz];
	T* res6= new T[	dimx*dimy*dimz];
	T* res7= new T[	dimx*dimy*dimz];

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
	orientation4[0] = 2;
	orientation4[1] = 2;
	orientation4[2] = 2;
	std::vector<int> orientation5(3);
	orientation5[0] = 2;
	orientation5[1] = 2;
	orientation5[2] = -2;
	std::vector<int> orientation6(3);
	orientation6[0] = -2;
	orientation6[1] = 2;
	orientation6[2] = 2;
	std::vector<int> orientation7(3);
	orientation7[0] = -2;
	orientation7[1] = 2;
	orientation7[2] = -2;
	//
	//calling RPO
	 #pragma omp parallel sections 
	 {
		#pragma omp section 
	   { 
		 RPO3D<T>(input_buffer,res1,orientation1,L,G,reconstruction,dimx,dimy,dimz);
		 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
		   RPO3D<T>( input_buffer,res2,orientation2,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res3,orientation3,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res4,orientation4,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res5,orientation5,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res6,orientation6,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   { 
	       RPO3D<T>( input_buffer,res7,orientation7,L,G,reconstruction,dimx,dimy,dimz);
		   std::cout<<"orientation7 -1 1 -1 : passed"<<std::endl;
	   }
	 }
	
	 //Union
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=res1[i];
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res2[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res3[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res4[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res5[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res6[i],output_buffer[i]);
	//
	#pragma omp parallel for
	for(int i=0; i<dimx*dimy*dimz;i++)
		output_buffer[i]=max(res7[i],output_buffer[i]);

	//desallocation
	delete[] res1;
	delete[] res2;
	delete[] res3;
	delete[] res4;
	delete[] res5;
	delete[] res6;
	delete[] res7;
}


#endif
