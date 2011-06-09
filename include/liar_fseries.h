/*
 * header for the fseries
 *
 * Hugues Talbot	 3 Jun 2011
 */

#ifdef __cplusplus
extern "C" {
#endif

/* these are all pure C functions */

int imferode3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfdilat3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfopen3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfclose3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

#ifdef __cplusplus
}
#endif
