/* $Id: lskeletons.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern int32_t lskelubp(
        struct xvimage *image,
        struct xvimage *prio,
        int32_t connex,
        int32_t inhibit
);
extern int32_t lskelubp2(
        struct xvimage *image,
        struct xvimage *prio,
        int32_t connex,
        struct xvimage *imageinhib
);
extern int32_t lskelubp3d(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        int32_t connex, 
        int32_t val_inhibit
);
extern int32_t lskelubp3d2(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        int32_t connex, 
        struct xvimage *imageinhib
);
extern int32_t lskelcurv(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        struct xvimage *inhibit, 
        int32_t connex
);
extern int32_t lskelcurv3d(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        struct xvimage *inhibit, 
        int32_t connex
);
extern int32_t lskelsurf3d(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        struct xvimage *inhibit, 
	int32_t connex
);
extern int32_t ltoposhrink(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        int32_t connex, 
        int32_t tmin, 
        int32_t tmax, 
        int32_t tbmin, 
        int32_t tbmax, 
        struct xvimage *imageinhibit
);
extern int32_t ltoposhrink3d(
        struct xvimage *image, 
        struct xvimage *imageprio, 
        int32_t connex, 
        int32_t tmin, 
        int32_t tmax, 
        int32_t tbmin, 
        int32_t tbmax, 
        struct xvimage *imageinhibit
);

extern int32_t lskeleucl(
	struct xvimage *image, 
        int32_t connex, 
        struct xvimage *imageinhib
);

extern int32_t lskelend3d(
        struct xvimage *image, 
	int32_t connex, 
	uint8_t *endpoint
);
#ifdef __cplusplus
}
#endif
