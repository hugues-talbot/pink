#define min(x,y)          ((x)<(y)?(x):(y)) 
#define max(x,y)          ((x)<(y)?(y):(x))

extern int MSF(struct xvimage *ga, struct xvimage *marqueurs);
extern int32_t MSF3d(struct xvimage *ga, struct xvimage *marqueurs);
extern int32_t MSF4d(struct GA4d *ga, struct xvimage4D *marqueurs);
