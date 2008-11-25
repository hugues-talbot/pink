#define TYP_VRAGARC int32_t
#define TYP_VARC int32_t
#define TYP_VSOM int32_t
#ifndef HUGE
#define HUGE HUGE_VAL
#endif
#define SHRT_MIN -32767 
#define SHRT_MAX +32767 
#define USHRT_MAX 65535 
#define INT_MIN -32767 
#define INT_MAX +32767 
#define UINT_MAX 65535 
#define LONG_MIN -2147483647 
#define LONG_MAX +2147483647
#define ULONG_MAX 4294967295
#define M_PI	 3.14159265358979323846
#define IN_PROCESS -2
#define NO_LABEL -1

#define max(X,Y) ((X)>=(Y)?(X):(Y))
#define min(X,Y) ((X)<=(Y)?(X):(Y))

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define IN_PROCESS -2
#define NO_LABEL -1

extern struct xvimage *mBorderWshed(struct xvimage *ga);
struct xvimage *SeparatingEdge(struct xvimage *labels);
struct xvimage *mBorderWshed2d(struct xvimage *ga);
struct xvimage *mBorderWshed2drapide(struct xvimage *ga);
int32_t flowMapping(struct  xvimage* ga, int32_t* Label);
int32_t flowMappingRecursif(struct  xvimage* ga, int32_t* Label);
int32_t flowMappingFloat(struct  xvimage* ga, int32_t* Label);
int32_t flowMappingDouble(struct  xvimage* ga, int32_t* Label);
int32_t lpeGrapheAreteValuee(GrapheValue *gv, int32_t* Label);
