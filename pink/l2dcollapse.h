/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

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
#define TF_NOT_IN_I  0
#define TF_HEAD      1
#define TF_TAIL      2
#define TF_PERMANENT 3
#include <mcgraphe.h>
#ifdef __cplusplus
extern "C" {
#endif
  extern int32_t l2dcollapse(             struct xvimage * k, const struct xvimage * prio, const struct xvimage * inhibit );
  extern int32_t l2dpardircollapse_short( struct xvimage * k, int32_t nsteps );
  extern int32_t l2dpardircollapse(       struct xvimage * k, int32_t nsteps,              const struct xvimage * inhibit );
  extern int32_t l2dpardircollapse_l(     struct xvimage * k, const struct xvimage * prio, const struct xvimage * inhibit, int32_t priomax );
  extern int32_t l2dpardircollapse_f(     struct xvimage * k, const struct xvimage * prio, const struct xvimage * inhibit, float   priomax );
  extern graphe * l2dtopoflow_l(          struct xvimage * k, const struct xvimage * prio, const struct xvimage * inhibit, int32_t priomax );
  extern graphe * l2dtopoflow_f(          struct xvimage * k, const struct xvimage * prio, const struct xvimage * inhibit, float   priomax );
  extern int32_t l2dflowskeleton(         struct xvimage * k, int32_t mode, double level,  const struct xvimage * func);
#ifdef __cplusplus
}
#endif