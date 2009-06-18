/* $Id: lskelpar3d.h,v 1.3 2009-06-18 06:34:55 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lskelMK3a(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelEK3(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelCK3a(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelCK3b(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelCK3(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelAK3(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit,
			int32_t filter);
extern int32_t lskelMK3(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t ldistaxetopo3(struct xvimage *image, 
			     struct xvimage *inhibit,
			     struct xvimage *res);
extern int32_t ldisttopo3(struct xvimage *image, 
			  struct xvimage *inhibit,
			  struct xvimage *res);
extern int32_t lskelAMK3(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelACK3a(struct xvimage *image, 
			  int32_t nsteps,
			  struct xvimage *inhibit);
extern int32_t lskelACK3(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelRK3(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelSK3(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
#ifdef __cplusplus
}
#endif
