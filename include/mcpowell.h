
typedef struct ensemble {
  double *X;
  int32_t n;
  double *Y;
  int32_t m;
  double *Tmp;
} ensemble;

#define SQRT_DBL_EPSILON   1e-8		/* should be >=< sqrt(DBL_EPSILON) */


#define    M_OK  	0
#define    M_FOUND  	0
#define    M_NOT_FOUND	1
#define    M_FAILURE	2
#define    M_MAX_ITER	3
#define    M_LAST_CODE	4


#undef abs
#define abs(x)		((x)>=0?(x):-(x))

#undef max
#define max(a,b)        ((a)<(b)?(b):(a))	/* WARNING: side effects */

#undef min
#define min(a,b)        ((a)>(b)?(b):(a))

#undef sign
#define sign(x)		((x)>0?1.0:((x)<0?-1.0:0.0))

/* useful definitions for functions */

typedef double (*FUNCe)(double, ensemble *);		      
typedef double (*PFNe)(double *, ensemble *);		/* f: R^N -> R */

/* prototypes */

extern int32_t powell(PFNe f, ensemble *ens, double p[], int32_t n, double tol,
		  double scale, int32_t maxiter, double *fmin);



