/* $Id: llpeGA.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#define TYP_VRAGARC int32_t
#define TYP_VARC int32_t
#define TYP_VSOM int32_t
#ifndef HUGE
#define HUGE HUGE_VAL
#endif
#define IN_PROCESS -2
#define NO_LABEL -1

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define IN_PROCESS -2
#define NO_LABEL -1

extern struct xvimage *mBorderWshed(struct xvimage *ga);
extern struct xvimage *SeparatingEdge(struct xvimage *labels);
extern struct xvimage *mBorderWshed2d(struct xvimage *ga);
extern struct xvimage *mBorderWshed2drapide(struct xvimage *ga);
extern int32_t flowMapping(struct  xvimage* ga, int32_t* Label);
extern int32_t flowMappingRecursif(struct  xvimage* ga, int32_t* Label);
extern int32_t flowMappingFloat(struct  xvimage* ga, int32_t* Label);
extern int32_t flowMappingDouble(struct  xvimage* ga, int32_t* Label);
extern int32_t lpeGrapheAreteValuee(GrapheValue *gv, int32_t* Label);
extern int32_t altitudePoint(struct xvimage *ga, int32_t i);
#ifdef __cplusplus
}
#endif
