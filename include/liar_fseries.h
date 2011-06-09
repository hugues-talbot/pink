/*
 * header for the fseries
 *
 * Hugues Talbot	 3 Jun 2011
 */

#ifdef __cplusplus
extern "C" {
#endif

/* these are all pure C functions */

/* square morphological operation*/
int imferode3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfdilat3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfopen3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfclose3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

/* Line morphological operation*/
int imferode3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

int imfdilat3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz, int lenght,struct xvimage *output);

int imfopen3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

int imfclose3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

#ifdef __cplusplus
}
#endif
