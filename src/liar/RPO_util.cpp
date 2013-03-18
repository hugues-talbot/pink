#include "RPO_util.hpp"
//sorting pixel grey level value of input image by ascending order
void sort_image_value(	std::vector<int> &Image,
                      	int dim_image,
                      	std::vector<int> &sorted_image_indices)
{
        //allocate memory for sorted pointers array
        std::vector<int*> sorted_ptr(dim_image);
        //fill sorted pointers array
        for (int i=0; i<dim_image; ++i)
            sorted_ptr[i] = &Image[0] + i;

        //sorting ptr's int value
        qsort(	&sorted_ptr[0],
        		dim_image,
        		sizeof(int*),
        		pointer_value_comparison);

        //associated image indices to sorted ptr value
        for (int i =0;i<dim_image;++i)
        	sorted_image_indices[i] = (int) (sorted_ptr[i] - &Image[0]);

}

int pointer_value_comparison( const void * a,
                              const void * b)
{

        return ( ( **(int**)a - **(int**)b ) );
}

/******create neighbour pixel's index from a given orientation vector ********/
void createNeighbourhood(	int nb_col, 
							int dim_frame,
			 				std::vector<int> & orientation,	
			 				std::vector<int > & upList,
                            std::vector<int > & downList) {
                                
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

       upList.push_back(dim_frame + nb_col);
       upList.push_back( nb_col +1);
       upList.push_back(nb_col);
       upList.push_back( dim_frame + 1 );
       //main direction
       upList.push_back( dim_frame + nb_col + 1 );

       downList.push_back(-dim_frame - nb_col);
       downList.push_back( -nb_col -1);
       downList.push_back(-nb_col);
       downList.push_back( - dim_frame - 1 );
       //main direction
       downList.push_back( -dim_frame - nb_col -1 );

    }
    //2nd main diagonal [1 1 -1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == 1)) {

       upList.push_back(dim_frame + nb_col);
       upList.push_back( nb_col -1);
       upList.push_back(nb_col);
       upList.push_back( dim_frame - 1 );
       //main direction
       upList.push_back( dim_frame + nb_col - 1 );

       downList.push_back(-dim_frame - nb_col);
       downList.push_back( -nb_col +1);
       downList.push_back(-nb_col);
       downList.push_back( - dim_frame + 1 );
       //main direction
       downList.push_back( -dim_frame - nb_col + 1 );

    }
    //3nd main diagonal [-1 1 1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == -1)) {

        upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col +1);
        upList.push_back(nb_col);
        upList.push_back( -dim_frame + 1 );
        //main direction
        upList.push_back( -dim_frame + nb_col + 1 );

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col -1);
        downList.push_back(-nb_col);
        downList.push_back(  dim_frame - 1 );
        //main direction
        downList.push_back( dim_frame - nb_col -1 );

    }
    //3nd main diagonal [-1 1 -1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == 1)) {


        upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col - 1);
        upList.push_back(nb_col);
        upList.push_back( -dim_frame - 1 );
        //main direction
        upList.push_back( -dim_frame + nb_col - 1 );

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col + 1);
        downList.push_back(-nb_col);
        downList.push_back(  dim_frame + 1 );
        //main direction
        downList.push_back( dim_frame - nb_col + 1 );

    }

}

/**************************** Robust Path propagation from seed points *********************************************/
void propagateRobustPath(		  int seed_index,
                                  std::vector< int > & upList,
                                  std::vector< int > & downList,
                                  std::queue< int> & QueueQ,
                                  std::queue< int > & QueueC,
                                  std::vector< int > & FlagI,
                                  std::vector< int > & DownstreamI
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
 


