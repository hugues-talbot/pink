/*
 * The Eden process
 *
 *
 * Hugues Talbot	16 Mar 2009
 */

/*
 * Algorithm is as follows
 * for growth :
 *  - start from known set
 *  - take pixels from the border into a queue
 *  - choose randomly one such pixels
 *  - set it to 1
 *  - update queues
 *  - repeat
 * For shrinking
 *  - same thing but set to 0
 * For adding boundary noise
 *  - alternate growth and shrinking
 * With topological constraints
 *  - make sure the point to add or remove is simple
 *
 */

#include <deque>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include <mctopo.h>

#define MAX_RANDOM INT_MAX
#define INQUEUE    128 // in queue marker

/* this is not very efficient but quite readable */
/* count the number of neighbours exactly equal to val */
static int nb4vois(uint8_t *image, long p, int dimx, int dimy, int val)
{
    int x, y, nbvois;

    assert(p < dimx*dimy);
    
    x = p%dimx;
    y = p/dimx;
    
    /* take care of border effects */
    nbvois = 0;
    if (x < (dimx-1))
        nbvois += (image[y*dimx + x+1] == val);
    if (x > 0)
        nbvois += (image[y*dimx + x-1] == val);
    if (y > 0)
        nbvois += (image[(y-1)*dimx + x] == val);
    if (y < (dimy -1))
        nbvois += (image[(y+1)*dimx + x] == val);

    return(nbvois);
}

static void makebinary(uint8_t *in, uint8_t *out, int dimx, int dimy)
{
    long nbpix = dimx*dimy;
    uint8_t *p, *q, *end;
    
    p = in;
    q = out;
    end = out+nbpix;
    for ( ; q < end ; ++p, ++q) {
        *q = *p > 0;
    }
    return ;
}

// this will return a 0-1 binary image
// maybe change this if a 0-255 binary image is required as output
static void cleanout(uint8_t *out, int dimx, int dimy, int value)
{
    long nbpix = dimx*dimy;
    
    uint8_t *p, *end;
    
    p = out;
    end = out+nbpix;
    for ( ; p < end ; ++p) {
        if (*p == INQUEUE) 
            *p = value; // remove the INQUEUE markers
    }
    return ;
}

static void makebinoutput(uint8_t *out, int dimx, int dimy)
{
    long nbpix = dimx*dimy;
    
    uint8_t *p, *end;
    
    p = out;
    end = out+nbpix;
    for ( ; p < end ; ++p) {
        if (*p) 
            *p = 255;
    }
    return ;
}

int32_t ledengrowth(uint8_t *in,
       	            uint8_t *out,
           	    uint32_t       dimx,
                    uint32_t       dimy,
                    int32_t       nbiter,
                    int32_t       grow,
                    int32_t       shrink,
                    int32_t       topo)
{
    int       i, nbpix = dimx*dimy;
    long      delta, size;
    uint8_t *p, *end;
    std::deque< uint8_t* > borderqueue;
    
    // initiate random number generator
    srandom(time(0));

    // make the output binary 
    makebinary(in, out, dimx, dimy);

    // from there, work on out only
    if (grow) {
        p = out;
        end = out + nbpix;
        /* put external border of set in queue */
        while (p < end) {
            if (!(*p) && (nb4vois(out, p-out, dimx, dimy, 1) > 0)) {
                borderqueue.push_back(p);
                *p = INQUEUE;
            }
            p++;
        }

        /* turn pixels on at random */
        for (i = 0 ; i < nbiter ; ++i) {
            uint8_t *pix;
            int       dx, dy;
            
            size = borderqueue.size();
            int chosen = static_cast<int>(static_cast<double>(random())/MAX_RANDOM * size);
            pix = borderqueue.at(chosen);
            // remove that pixel from the queue
            borderqueue.erase(borderqueue.begin()+chosen);

	    if ((topo == 0) || 
	        ((topo == 4) && (simple4(out, (int32_t)(pix-out), dimx, nbpix))) ||
	        ((topo == 8) && (simple8(out, (int32_t)(pix-out), dimx, nbpix)))
	       )
            {
	      // turn that pixel to 1
              *pix = 1;
              // add the new border pixels onto the queue
              delta = pix-out;
              dx = delta % dimx;
              dy = delta / dimx;
            
              // if pixels are enqueue already they won't be selected again
              if (dx > 0) {
                if (*(pix-1) == 0) {
                    borderqueue.push_back(pix-1);
                    *(pix-1) = INQUEUE;
                }
              }
              if (dx < (dimx-1)) {
                if (*(pix+1) == 0) {
                    borderqueue.push_back(pix+1);
                    *(pix+1) = INQUEUE;
                }
              }
              if (dy > 0) {
                if (*(pix-dimx) == 0) {
                    borderqueue.push_back(pix-dimx);
                    *(pix-dimx) = INQUEUE;
                }
              }
              if (dy < (dimy-1)) {
                if (*(pix+dimx) == 0) {
                    borderqueue.push_back(pix+dimx);
                    *(pix+dimx) = INQUEUE;
                }
              }
	    }
        }
        /* remove the INQUEUE markers by replacing them with zeroes */
        cleanout(out, dimx, dimy, 0);
        /* clean out the queue */
        borderqueue.clear();
    }

    
    
    
    /* shrink */
    if (shrink) {
        p = out;
        end = out + nbpix;
        /* put internal border of set in queue */
        while (p < end) {
            if ((*p) && (nb4vois(out, p-out, dimx, dimy, 0) > 0)) {
                borderqueue.push_back(p);
                *p = INQUEUE;
            }
            p++;
        }

        /* turn pixels off at random */
        for (i = 0 ; i < nbiter ; ++i) {
            uint8_t *pix;
            int       dx, dy;
            
            size = borderqueue.size();
            int chosen = static_cast<int>(static_cast<double>(random())/MAX_RANDOM * size);
            pix = borderqueue.at(chosen);
            // remove that pixel from the queue
            borderqueue.erase(borderqueue.begin()+chosen);


	    if ((topo == 0) || 
	        ((topo == 4) && (simple4(out, (int32_t)(pix-out), dimx, nbpix))) ||
	        ((topo == 8) && (simple8(out, (int32_t)(pix-out), dimx, nbpix)))
	       )
            {
              // turn that pixel to 0
              *pix = 0;
              // add the new border pixels onto the queue
              delta = pix-out;
              dx = delta % dimx;
              dy = delta / dimx;
            
              // if pixels are enqueue already they won't be added again
              // here there is no need to check that the neighbours of pix are
              // on the border since pix is now on the border
              if (dx > 0) {
                if (*(pix-1) == 1) {
                    borderqueue.push_back(pix-1);
                    *(pix-1) = INQUEUE;
                }
              }
              if (dx < (dimx-1)) {
                if (*(pix+1) == 1) {
                    borderqueue.push_back(pix+1);
                    *(pix+1) = INQUEUE;
                }
              }
              if (dy > 0) {
                if (*(pix-dimx) == 1) {
                    borderqueue.push_back(pix-dimx);
                    *(pix-dimx) = INQUEUE;
                }
              }
              if (dy < (dimy-1)) {
                if (*(pix+dimx) == 1) {
                    borderqueue.push_back(pix+dimx);
                    *(pix+dimx) = INQUEUE;
                }
              }
            }
        }
        
        /* remove the INQUEUE markers by replacing them with ones this time */
        makebinoutput(out, dimx, dimy);
        /* flush the queue */
        borderqueue.clear();
    }

    return 1;
}


