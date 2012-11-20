/*
Copyright ESIEE (2011)

h.talbot@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file liar_segmentation.cxx

\brief Some interesting segmentation methods

<B>Description:</B>

Functions in this file implement various general-purpose segmentation
methods, complementary to the watershed.

<B>Types supported:</B> byte 2d, byte3d

<B>Category:</B> morpho
\ingroup morpho

\author Hugues Talbot, based on earlier code by Ed Breen and others.
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

#include "pink.h"
#include "liarp.h"
#include "liar_segmentation.h"


/*-------------------------------------------------------------------------------------------------*/
/*! Seeded Region Growing */
/* Adams & Bischof PAMI 16(6) 641--647 1994 */
int imsrgrow3d(struct xvimage *seedsout, /**< [inout] seed (a.k.a. markers) image */
               struct xvimage *input,    /**< [in] input image */
	       int metric,                  /**< [in] metric used */
	       int borders,                 /**< [in] boolean showing borders or not */
               int connectivity,            /**< [in] connectivity */
               int showgrey)                /**< [in] showing average grey-levels or not */
{
    int retval = 1; // means it worked by default
    void **srcbuf = NULL;
    
    srcbuf = (void**)malloc(sizeof(void*)); /* only one buffer */

    switch (input->data_storage_type) {
    case    VFF_TYP_1_BYTE:
    {
        if (seedsout->data_storage_type != VFF_TYP_4_BYTE) {
            pink_warning("Seed/marker (second arg) pixel type currently not supported, must be int32 "<< seedsout->data_storage_type);
        }
        
        int inpixtype = UINT1_CODE; /* not sure about this, means 1-byte unsigned integer */
        long nbpix = rowsize(seedsout)*colsize(seedsout)*depth(seedsout); /* total nb of pixels */
        
        srcbuf[0] = UCHARDATA(input);
        SEED_TYPE *outputdatabuf = (SEED_TYPE*)calloc(nbpix,sizeof(SEED_TYPE));

        if (outputdatabuf != NULL) {
            int liar_errcode = 0;
            
            SEED_TYPE *seeddatabuf = SLONGDATA(seedsout);
            
            /* because Pink requires the SECOND image to be in/out ??? */
            memset(outputdatabuf, 0, nbpix*sizeof(SEED_TYPE));

            if ((liar_errcode = lsrgrow3d (srcbuf,
                                           &outputdatabuf,
                                           seeddatabuf,
                                           inpixtype,
                                           1, /* pixel size of input data = char = 8bit = one byte */
                                           4, /* pixel size of seed data = int32 = 32bits = 4 bytes */
                                           rowsize(input), /* rowsize = ncol = number of columns = nx */
                                           colsize(input), /* ny */
                                           depth(input), /* nz */
                                           1, /* only one component in the image */
                                           metric, /* not sure what this is */
                                           borders, /* 1/0 = display region borders or not */
                                           connectivity, /* 4/8 for 2d, 6-18-26 for 3d */
                                           showgrey) /* 1/0 = show average grey in regions or not */
                    ) != 0) {
                pink_warning("Seeded region growing failed with code = " << liar_errcode << "\n");
                retval = 0; // funny how that works out
                break;
            }

            /* copy output data back, since pink requires output to be in seedout */
            memcpy(seeddatabuf, outputdatabuf, nbpix*sizeof(SEED_TYPE));
        } else {
            pink_warning("Seeded region growing failed: not enough memory\n");
            retval = 0; /* didn't work */
            break;
        }
    }
    break;
        
        

    default:
        pink_warning("Input (first arg) pixel type currently not supported, must be CHAR "<< input->data_storage_type);
        retval = 0;
        break;
    }
    return(retval); // value of 1 means it worked well !
}


