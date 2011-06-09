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
/*! liar_fseries.cpp

\brief fast flat structuring elements morphological methods

<B>Description:</B>

Methods in this file implent fast recursive morphological operators, both 2D and 3D, using decomposition by lines.
Running time in independent of the size of the SE.

\author Hugues Talbot, based on early code by Ed Breen
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

#include "liarp.h"
#include "fseries.hpp"
#include "liar_fseries.h"

/* erosion by a flat square structuring element */
int imferode3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output)
{
//    switch (input->data_storage_type) {
//     case    VFF_TYP_1_BYTE:
//    	return( lferode3d_rect_char(UCHARDATA(input),
//                           UCHARDATA(output),
//                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//                           colsize(input),
//                           depth(input),
//                           SEnx, SEny, SEnz));
//     break;
//
//    //case  VFF_TYP_2_BYTE:
//    //	return( lferode3d_rect_short(USHORTDATA(input),
//    //                       USHORTDATA(output),
//    //                       rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//    //                       colsize(input),
//    //                       depth(input),
//    //                       SEnx, SEny, SEnz));
//    //break;
//
//    case  VFF_TYP_4_BYTE:
//    	return( lferode3d_rect_int4(SLONGDATA(input),
//                           SLONGDATA(output),
//                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//                           colsize(input),
//                           depth(input),
//                           SEnx, SEny, SEnz));
//    break;
//    }
    return 0;
}

/* dilatation by a flat square structuring element */
int imfdilat3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output)
{
//	switch (input->data_storage_type) {
//     case    VFF_TYP_1_BYTE:
//    	return( lfdilate3d_rect_char(UCHARDATA(input),
//                           UCHARDATA(output),
//                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//                           colsize(input),
//                           depth(input),
//                           SEnx, SEny, SEnz));
//     break;
//
//    //case  VFF_TYP_2_BYTE:
//    //	return( lfdilate3d_rect_short(USHORTDATA(input),
//    //                       USHORTDATA(output),
//    //                       rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//    //                       colsize(input),
//    //                       depth(input),
//    //                       SEnx, SEny, SEnz));
//    //break;
//
//    case  VFF_TYP_4_BYTE:
//    	return( lfdilate3d_rect_int4(SLONGDATA(input),
//                           SLONGDATA(output),
//                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//                           colsize(input),
//                           depth(input),
//                           SEnx, SEny, SEnz));
//    break;
//    }

    return 0;
}


/* openning by a flat square structuring element */
int imfopen3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output)
{
    switch (input->data_storage_type) {
     case    VFF_TYP_1_BYTE:
    	return( lfopen3d_rect< PIX_TYPE >(UCHARDATA(input),
                           UCHARDATA(output),
                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
                           colsize(input),
                           depth(input),
                           SEnx, SEny, SEnz));
     break;

    //case  VFF_TYP_2_BYTE:
    //	return( lfopen3d_rect_short(USHORTDATA(input),
    //                       USHORTDATA(output),
    //                       rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
    //                       colsize(input),
    //                       depth(input),
    //                       SEnx, SEny, SEnz));
    //break;

    case  VFF_TYP_4_BYTE:
    	return( lfopen3d_rect< INT4_TYPE >(SLONGDATA(input),
                           SLONGDATA(output),
                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
                           colsize(input),
                           depth(input),
                           SEnx, SEny, SEnz));
    break;
    }
}

/* closing by a flat square structuring element */
int imfclose3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output)
{
//   switch (input->data_storage_type) {
//     case    VFF_TYP_1_BYTE:
//    	return( lfclose3d_rect_char(UCHARDATA(input),
//                           UCHARDATA(output),
//                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//                           colsize(input),
//                           depth(input),
//                           SEnx, SEny, SEnz));
//     break;
//
//    //case  VFF_TYP_2_BYTE:
//    //	return( lfclose3d_rect_short(USHORTDATA(input),
//    //                       USHORTDATA(output),
//    //                       rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//    //                       colsize(input),
//    //                       depth(input),
//    //                       SEnx, SEny, SEnz));
//    //break;
//
//    case  VFF_TYP_4_BYTE:
//    	return( lfclose3d_rect_int4(SLONGDATA(input),
//                           SLONGDATA(output),
//                           rowsize(input),  /* careful: rowsize is the size of a row <=> nx = number of columns = ncol */
//                           colsize(input),
//                           depth(input),
//                           SEnx, SEny, SEnz));
//    break;
//    }

    return 0;
}

