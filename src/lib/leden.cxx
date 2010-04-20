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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <mcset.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <leden.h>
#include <mccodimage.h>

#define MAX_RANDOM INT_MAX
#define INQUEUE    128 // in queue marker

/* this is not very efficient but quite readable */
/* count the number of neighbours exactly equal to val */
static int nb1vois(uint8_t *image, long p, int dimx, int dimy, int dimz, int val)
{
    int x, y, z, nbvois, ps;

    ps = dimx*dimy;
    x = p%dimx;
    y = (p%ps)/dimx;
    z = p/ps;

    /* take care of border effects */
    nbvois = 0;
    if (x < (dimx-1))
        nbvois += (image[z*ps + y*dimx + x+1] == val);
    if (x > 0)
        nbvois += (image[z*ps + y*dimx + x-1] == val);
    if (y > 0)
        nbvois += (image[z*ps + (y-1)*dimx + x] == val);
    if (y < (dimy -1))
        nbvois += (image[z*ps + (y+1)*dimx + x] == val);
    if (z > 0)
        nbvois += (image[(z-1)*ps + y*dimx + x] == val);
    if (z < (dimz -1))
        nbvois += (image[(z+1)*ps + y*dimx + x] == val);

    return(nbvois);
}

int32_t ledengrowth(uint8_t *in,
       	            int32_t dimx,
                    int32_t dimy,
                    int32_t dimz,
                    int32_t nbiter,
                    int32_t grow,
                    int32_t shrink,
                    int32_t topo)
{
	int32_t i, ps, nbpix, dx, dy, dz, chosen;
	long delta, size;
	uint8_t *p, *end, *pix;
	std::deque< uint8_t* > borderqueue;
	Set * parcouru;





	ps= dimx*dimy;
	nbpix = dimx*dimy*dimz;

	// initiate random number generator
	srandom(time(0) * getpid());

	// Init topo 3d
	if (dimz>1)
	{
		init_topo3d();
	}

	parcouru=CreateEmptySet((nbpix/32)+1);
	if(parcouru == NULL)
	{
		fprintf(stderr, "ledengrowth() : Memory allocation error.\n");
		return 0;
	}



	//Starts the growth process if necessary (adding random points to object)
	if (grow)
	{
		p = in;
		end = in + nbpix;

		/* put external border of set in queue */
		while (p < end)
		{
			if (!(*p) && (nb1vois(in, p-in, dimx, dimy, dimz, 255) > 0) && ((dimz==1 && bord((uint32_t)(p-in), dimx, ps)==0) || (dimz>1 && bord3d((uint32_t)(p-in), dimx, ps, nbpix)==0)))
			{
				borderqueue.push_back(p);
				SetElement(parcouru, (uint32_t)(p-in));
			}
			p++;
		}


		/* turn pixels on at random */
		for (i = 0 ; i < nbiter ; ++i)
		{
			size = borderqueue.size();
			if(size==0)
			{
				printf("Bouh\n");

				break;
			}
			chosen = static_cast<uint32_t>(static_cast<double>(random())/MAX_RANDOM * (size-1)); //Should check here that list is not empty
			pix = borderqueue.at(chosen);

			// remove that pixel from the queue
			borderqueue.erase(borderqueue.begin()+chosen);
			ResetElement(parcouru, (uint32_t)(pix-in));

			
			if(	(topo == 0) ||
				((topo == 4) && (simple4(in, (int32_t)(pix-in), dimx, nbpix))) ||
				((topo == 8) && (simple8(in, (int32_t)(pix-in), dimx, nbpix))) ||
				((topo == 6) && (simple6(in, (int32_t)(pix-in), dimx, ps, nbpix))) ||
				((topo == 26) && (simple26(in, (int32_t)(pix-in), dimx, ps, nbpix))) )
				{
					// turn on that pixel
					*pix = 255;
					

					// add the new border pixels onto the queue
					delta = pix-in;
					dx = delta % dimx;
					dy = (delta % ps) / dimx;
					dz = delta /ps;

					// if pixels are enqueue already they won't be selected again
					if( (dx > 0) &&
						(*(pix-1) == 0) &&
						!InSet(parcouru, (int32_t)(pix-in-1)) && 
						((dimz==1 && bord((uint32_t)(pix-in-1), dimx, ps)==0) || (dimz>1 && bord3d((uint32_t)(pix-in-1), dimx, ps, nbpix)==0)) )
						{
							borderqueue.push_back(pix-1);
							SetElement(parcouru, (int32_t)(pix-in-1));
						}

					if( (dx < (dimx-1)) &&
						(*(pix+1) == 0) &&
						!InSet(parcouru, (int32_t)(pix-in+1)) &&
						((dimz==1 && bord((uint32_t)(pix-in+1), dimx, ps)==0) || (dimz>1 && bord3d((uint32_t)(pix-in+1), dimx, ps, nbpix)==0)) )
						{
							borderqueue.push_back(pix-1);
							SetElement(parcouru, (int32_t)(pix-in+1));
						}

					if( (dy > 0) &&
						(*(pix-dimx) == 0) &&
						!InSet(parcouru, (int32_t)(pix-in-dimx)) &&
						((dimz==1 && bord((uint32_t)(pix-in-dimx), dimx, ps)==0) || (dimz>1 && bord3d((uint32_t)(pix-in-dimx), dimx, ps, nbpix)==0)) )
						{
							borderqueue.push_back(pix-dimx);
							SetElement(parcouru, (int32_t)(pix-in-dimx));
						}

					if( (dy < (dimy-1)) &&
						(*(pix+dimx) == 0) &&
						!InSet(parcouru, (int32_t)(pix-in+dimx)) &&
						((dimz==1 && bord((uint32_t)(pix-in+dimx), dimx, ps)==0) || (dimz>1 && bord3d((uint32_t)(pix-in+dimx), dimx, ps, nbpix)==0)) )
						{
							borderqueue.push_back(pix+dimx);
							SetElement(parcouru, (int32_t)(pix-in+dimx));
						}

					if(	(dz > 0) &&
						(*(pix-ps) == 0) &&
						!InSet(parcouru, (int32_t)(pix-in-ps)) &&
						(bord3d((uint32_t)(pix-in-ps), dimx, ps, nbpix)==0) )
						{
							borderqueue.push_back(pix-ps);
							SetElement(parcouru, (int32_t)(pix-in-ps));
						}

					if(	(dz < (dimz-1)) &&
						(*(pix+ps) == 0) &&
						!InSet(parcouru, (int32_t)(pix-in+ps)) &&
						(bord3d((uint32_t)(pix-in+ps), dimx, ps, nbpix)==0) )
						{
							borderqueue.push_back(pix+ps);
							SetElement(parcouru, (int32_t)(pix-in+ps));
						}
				}
		}

				

		/* clean out the queue */
		borderqueue.clear();
		SetEmpty(parcouru);
	}




    //Starts the shrink process if necessary (removing random points to object)
	if (shrink)
	{
		p = in;
		end = in + nbpix;

		/* put internal border of set in queue */
		while (p < end)
		{
			if ((*p) && (nb1vois(in, p-in, dimx, dimy, dimz, 0) > 0))
			{
				borderqueue.push_back(p);
				SetElement(parcouru, (uint32_t)(p-in));
            }
            p++;
        }

		/* turn pixels off at random */
		for (i = 0 ; i < nbiter ; ++i)
		{
			size = borderqueue.size();
			chosen = static_cast<int>(static_cast<double>(random())/MAX_RANDOM * (size-1));
			pix = borderqueue.at(chosen);

            // remove that pixel from the queue
			borderqueue.erase(borderqueue.begin()+chosen);


			if(	(topo == 0) ||
				((topo == 4) && (simple4(in, (int32_t)(pix-in), dimx, nbpix))) ||
				((topo == 8) && (simple8(in, (int32_t)(pix-in), dimx, nbpix))) ||
				((topo == 6) && (simple6(in, (int32_t)(pix-in), dimx, ps, nbpix))) ||
				((topo == 26) && (simple26(in, (int32_t)(pix-in), dimx, ps, nbpix))) )
				{
					// turn off that pixel
					*pix = 0;

					// add the new border pixels onto the queue
					delta = pix-in;
					dx = delta % dimx;
					dy = (delta % ps) / dimx;
					dz = delta /ps;


					// if pixels are enqueue already they won't be added again
					// here there is no need to check that the neighbours of pix are
					// on the border since pix is now on the border
					// if pixels are enqueue already they won't be selected again
					// if pixels are enqueue already they won't be selected again
					if( (dx > 0) &&
						((*pix-1) != 0) &&
						!InSet(parcouru, (int32_t)(pix-in-1)) )
						{
							borderqueue.push_back(pix-1);
							SetElement(parcouru, (int32_t)(pix-in-1));
						}

					if( (dx < (dimx-1)) &&
						((*pix+1) != 0) &&
						!InSet(parcouru, (int32_t)(pix-in+1)) )
						{
							borderqueue.push_back(pix-1);
							SetElement(parcouru, (int32_t)(pix-in+1));
						}

					if( (dy > 0) &&
						((*pix-dimx) != 0) &&
						!InSet(parcouru, (int32_t)(pix-in-dimx)) )
						{
							borderqueue.push_back(pix-dimx);
							SetElement(parcouru, (int32_t)(pix-in-dimx));
						}

					if( (dy < (dimy-1)) &&
						((*pix+dimx) != 0) &&
						!InSet(parcouru, (int32_t)(pix-in+dimx)) )
						{
							borderqueue.push_back(pix+dimx);
							SetElement(parcouru, (int32_t)(pix-in+dimx));
						}

					if(	(dz > 0) &&
						((*pix-ps) != 0) &&
						!InSet(parcouru, (int32_t)(pix-in-ps)) )
						{
							borderqueue.push_back(pix-ps);
							SetElement(parcouru, (int32_t)(pix-in-ps));
						}

					if(	(dz < (dimz-1)) &&
						((*pix+ps) != 0) &&
						!InSet(parcouru, (int32_t)(pix-in+ps)) )
						{
							borderqueue.push_back(pix+ps);
							SetElement(parcouru, (int32_t)(pix-in+ps));
						}
				}
        }
    }

	// Clear topo 3d
	if (dimz>1)
	{
		termine_topo3d();
	}
	free(parcouru);

    return 1;
}
