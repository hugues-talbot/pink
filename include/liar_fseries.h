/*
 * header for the fseries
 *
 * Hugues Talbot	 3 Jun 2011
 */

#ifdef __cplusplus
extern "C" {
#endif

/* these are all pure C functions */

/* 3D square morphological operation*/
int imferode3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfdilat3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfopen3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfclose3D_rect( struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

/* 3D Line morphological operation*/
int imferode3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

int imfdilat3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz, int lenght,struct xvimage *output);

int imfopen3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

int imfclose3D_line( struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);


/* 2D square morphological operation*/
int imferode_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

int imfdilat_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

int imfopen_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

int imfclose_rect( struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

/* 2D Line morphological operation*/
int imferode_line( struct xvimage *input, int length,int angle, struct xvimage *output);

int imfdilat_line( struct xvimage *input, int length,int angle, struct xvimage *output);

int imfopen_line( struct xvimage *input, int length,int angle, struct xvimage *output);

int imfclose_line( struct xvimage *input, int length,int angle, struct xvimage *output);
#ifdef __cplusplus
}
#endif
