#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <stdlib.h>
#include <queue>


void sort_image_value(	std::vector<int> &Image,
                      	int dim_image,
                      	std::vector<int> &sorted_image_indices);
                      
int pointer_value_comparison( const void * a,
                              const void * b);
                              
void createNeighbourhood(	int nb_col,
			 				int dim_frame,
			 				std::vector<int> & orientation ,	
							std::vector<int > & up_neighbour,
							std::vector<int > & down_neighbour);

void propagateRobustPath(	int seed_index,
                        	std::vector< int > & upList,
                        	std::vector< int > & downList,
                        	std::queue< int> & QueueQ,
                        	std::queue< int > & QueueC,
                        	std::vector< int > & FlagI,
                        	std::vector< int > & DownstreamI);



#endif // _UTIL_H_

