/////////////////////////////////////////////////
///***   IN include/liar_segmentation.h   ***///
////////////////////////////////////////////////
/*
 * header for the liar_segmentation
 *
 * Lilian Santos, Hugues Talbot	 20 Nov 2012
 *
 * These functions are the C <-> C++ bridge to access the function of segmentation from the command line
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
/* these are all pure C functions */

/*1/2/3D Segmentation by Seeded Region Growing method*/
int imsrgrow3d(struct xvimage *input, /**< [in] input image */
               struct xvimage *seedsout, /**< [inout] seed (a.k.a. markers) image */
               int metric,                  /**< [in] metric used */
               int borders,                 /**< [in] boolean showing borders or not */
               int connectivity,            /**< [in] connectivity */
               int showgrey);                /**< [in] showing average grey-levels or not */


#ifdef __cplusplus
}
#endif
