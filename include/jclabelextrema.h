#define LABMAX   0
#define LABMIN   1

int32_t jclabelextrema(struct xvimage *in,         /* GA de depart */
		   uint32_t **labels,                /* resultat: carte de labels pour les minima du GA */
		   int32_t minimum,                /* booleen, 1: on recherche les minima, 0: on recherche les maxima */
		   int32_t *nlabels                /* resultat: le nombre d'extrema*/
		   );

int32_t jcfindextrema(struct xvimage *in,         /* GA de depart */
		  struct xvimage *out,        /* resultat: GA tq les aretes appartenant a des extrema sont à 255 */
		  int32_t minimum,                /* booleen, 1: on recherche les minima, 0: on recherche les maxima */
		  int32_t *nlabels               /* resultat: le nombre d'extrema*/
		  );
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <stdlib.h>
