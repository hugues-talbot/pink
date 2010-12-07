/*
 * File:		genfmin.c
 *
 Hugues Talbot	 7 Dec 2010
 
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
 *
*/

/* Ed Breen wrote a first version of this
 * converted into a stand-alone, generic C file by Hugues Talbot	13 Mar 1995
 */

#include <stdio.h>
#include <math.h>
#include "liarp.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE PIX_TYPE
#define FNAME_EXT _char
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE PIX_TYPE
#define FNAME_EXT _CHAR 
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE INT4_TYPE
#define FNAME_EXT _int4 
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE INT4_TYPE
#define FNAME_EXT _INT4 
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE LONG_TYPE
#define FNAME_EXT _long 
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE DBL_TYPE
#define FNAME_EXT _dbl 
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 
#define DATA_TYPE DBL_TYPE
#define FNAME_EXT _DBL 
#include "genfmin_generic.h"

#undef DATA_TYPE
#undef FNAME_EXT 