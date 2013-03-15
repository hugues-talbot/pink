#include "libRPO/RPO.h"


//initialize constructor attribute

RPO::RPO(std::vector<int>& _orientation, int _L, int _K, int _reconstruction, vtkImageData* _im_input):
	orientation(_orientation), L(_L), K(_K), reconstruction(_reconstruction), im_input(_im_input),im_output(NULL) {}
	
//processing function
void RPO::Execute() 
{	
	//cast input image to int
	vtkImageCast *castFilter = vtkImageCast::New();
  	castFilter->SetOutputScalarTypeToInt();
  	castFilter->SetInput(im_input);
  	castFilter->Update();
	//define scalar typede
	typedef int PixelType; 
	
	//get image dimensions
	PixelType* dim = (castFilter->GetOutput())->GetDimensions();
	
	//creating a two pixels wide dark border around the image (for opening)
	//to constrain the path to image domain
	vtkImageConstantPad *pad = vtkImageConstantPad::New();
	pad->SetInput(castFilter->GetOutput());
	pad->SetConstant(0);
	pad->SetOutputWholeExtent (-2,dim[0]+1,-2,dim[1]+1,-2,dim[2]+1);	
	pad->Update();
	
	//get padded image dimensions
	int* dim_padded = new int[3];
	dim_padded[0] = dim[0]+4; // x axis
	dim_padded[1] = dim[1]+4; // y axis
	dim_padded[2] = dim[2]+4; // z axis

	int image_size = dim_padded[0]*dim_padded[1]*dim_padded[2];
	int frame_size = dim_padded[0]*dim_padded[1];
	
	//get input image binary data
	PixelType *input_buffer = static_cast<PixelType*> (pad->GetOutput()->GetScalarPointer());
	
	//creating and init temporaries buffers
	std::vector<PixelType> originalI(image_size);
	std::vector<PixelType> downstreamI(image_size,L);
	std::vector<PixelType> upstreamI(image_size,L);
	std::vector<PixelType> flagI(image_size,1);
	
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
	//vector of sorted index of the image
	std::vector<PixelType> im_idx_sort(image_size);
	
	//copy input data
	memcpy(	&originalI[0],
			&input_buffer[0],
			image_size*sizeof(PixelType));

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
    					
    //border treatment : to prevent border pixels to be flagged as noise pixels
    for(int k=0; k <2; ++k ) {
       for(int i =0; i<dim_padded[1]; ++i) {
           for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
            }
        }
     }
     for(int k=(dim_padded[2]-2); k <dim_padded[2]; ++k ) {
         for(int i =0; i<dim_padded[1]; ++i) {
             for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
             }
         }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =0; i<dim_padded[1]; ++i) {
             for(int j=0;j<2;j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
             }
          }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =0; i<dim_padded[1]; ++i) {
             for(int j=(dim_padded[0]-2);j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
             }
         }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =0; i<2; ++i) {
             for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
             }
         }
     }
     for(int k=0; k <dim_padded[2]; ++k ) {
         for(int i =(dim_padded[1]-2); i<dim_padded[1]; ++i) {
             for(int j=0;j<dim_padded[0];j++) {
               flagI[k*frame_size + i*dim_padded[0] + j] |= (1<<5);
               downstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
               upstreamI[k*frame_size + i*dim_padded[0] + j] = 0;
              }
         }
     }			
     		
    //queue used to store noise pixel at different steps
    std::queue< int > queueNoise;
    std::queue< int > queueNoise_check;
    std::queue< int > prop_Noise;
    int in_queueNoise;
    int in_queueNoise_check;
    int in_propNoise;
    
    //seed index
    int iii = 0;
    //temp index
    int iiii = 0;
    //corresponding image memory address
    int seed_index;
    int temp_seed_index;
    //seed pixel value
    int threshold;
    int temp_threshold = -1;
    int temp_threshold_noise = -1;
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
            	while(originalI[temp_seed_index] <= threshold &&
                  	iiii<image_size) {
                	//if pixel is active
                	if(flagI[temp_seed_index]&(1<<0)) {
                    	//set to inactive
                    	flagI[temp_seed_index] &= ~(1<<0);
                    	//write in output image ?
                    	if ( (downstreamI[temp_seed_index] +
                        	upstreamI[temp_seed_index] - 1 ) >= L) {
                            	originalI[temp_seed_index] = threshold;
                     	}
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
        		while(originalI[temp_noise_index] <= threshold &&
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
    					while(nb_prop_up <= K) {
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
            						if((nb_prop_up < K) && 
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
            						nb_prop_up <= K) {
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
        					if(nb_prop_up==K &&
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
        				while(nb_prop_down <= K &&
            				!(flagI[temp_noise_index]&(1<<5))) {
        					while(!queueN1.empty()) {
            					//get first element of queueN1
                				in_queueN1 = queueN1.front();
                				for(int i =0; i < (int) down_neighbour.size(); ++i) {
                    				downstream_noise_px = in_queueN1 + down_neighbour[i];
                    				if((nb_prop_down < K) &&
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
                    				nb_prop_down <= K) {
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
             				if(nb_prop_down==K &&
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
        					it_active_pixel_found_during_up_prop -1) <= K ) {
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

    					while(nb_prop_up <= K) {
    						while(!queueN1.empty()) {
       							//get first element of queueN1
        						in_queueN1 = queueN1.front();
        						//propagate path on upstream neighbour
        						// in the desactivate set
        						for(int i = 0; i < (int) up_neighbour.size();++i){

            						upstream_noise_px = in_queueN1 + up_neighbour[i];

            						if((nb_prop_up < K) && 
            						!(flagI[upstream_noise_px]&(1<<0)) &&
            						!(flagI[upstream_noise_px]&(1<<6)) &&
            						!(flagI[upstream_noise_px]&(1<<5)) ) {
            							//push pixel in queueN2
             							queueN2.push(upstream_noise_px);
             							//set its corresponding flag to 1
                						flagI[upstream_noise_px] |= (1<<6);
            						}

            						if(flagI[upstream_noise_px]&(1<<0) &&
            						nb_prop_up <= K) {
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

        					if(nb_prop_up==K &&
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
        				while(nb_prop_down <= K &&
            			!(flagI[temp_noise_index]&(1<<5))) {
        					while(!queueN1.empty()) {
            					//get first element of queueN1
                				in_queueN1 = queueN1.front();
                				//propagate path on downstream neighbour
                				// in the desactivate set
                				for(int i =0; i < (int) down_neighbour.size(); ++i) {
                					//get current downstream neighbour coord
                    				downstream_noise_px = in_queueN1 + down_neighbour[i];
                    				if((nb_prop_down < K) &&
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
                    				nb_prop_down <= K) {
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

             				if(nb_prop_down==K &&
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
        					it_active_pixel_found_during_up_prop -1) <= K ) {
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
                                	queueQ,
                                	queueC,
                                	flagI,
                                	downstreamI);
            	propagateRobustPath(in_propNoise,
                                	down_neighbour,
                                	up_neighbour,
                                	queueQ,
                                	queueC,
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
                      queueQ,
                      queueC,
                      flagI,
                      downstreamI);
        	propagateRobustPath(seed_index,
                      down_neighbour,
                      up_neighbour,
                      queueQ,
                      queueC,
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
	int* temp = new int[image_size];
	memcpy(&temp[0],&originalI[0],image_size*sizeof(int));
	
	// allocate output image
	vtkImageData* temp_im_output = NULL;
	temp_im_output = vtkImageData::New();
	vtkIntArray *array = vtkIntArray::New();
	array->SetArray(&temp[0],dim_padded[0]*dim_padded[1]*dim_padded[2],0);
	temp_im_output->GetPointData()->SetScalars(array);
	temp_im_output->SetNumberOfScalarComponents(1);
	temp_im_output->SetDimensions(dim_padded);
	temp_im_output->SetScalarTypeToInt();
	
	//delete output image's borders
	im_output = vtkImageData::New();
	vtkImageClip *clip = vtkImageClip::New();
	clip->SetOutputWholeExtent(2,dim[0]+1,2,dim[1]+1,2,dim[2]+1);
	clip->SetInput(temp_im_output);
	clip->ClipDataOn();
	clip->Update();
	im_output = clip->GetOutput();
	//free memory
	delete[] dim_padded;
	
}

vtkImageData* RPO::GetResult() 
{
	return im_output;
}
