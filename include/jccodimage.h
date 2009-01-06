#define Sommetx(u,N,rs)   ((u)<(N)?(u):(u-N)) 
#define Sommety(u,N,rs)   ((u)<(N)?(u+1):(u-N+rs))

extern int32_t voisinGA(int32_t i, int32_t k, int32_t rs, int32_t nb);
extern int32_t incidente(int32_t i, int32_t k, int32_t rs, int32_t nb); 
extern int32_t incidente3d(int32_t i, int32_t k, int32_t rs, int32_t nb,int32_t ps);
extern int32_t incidente4d(int32_t i, int32_t k, int32_t rs, int32_t nb, int32_t ps, int32_t vs);
extern int32_t Sommetx3d(int32_t u, int32_t N, int32_t rs, int32_t ps);
extern int32_t Sommety3d(int32_t u, int32_t N, int32_t rs, int32_t ps);
extern int32_t Sommetx4d(int32_t u, int32_t N, int32_t rs, int32_t ps, int32_t vs);
extern int32_t Sommety4d(int32_t u, int32_t N, int32_t rs, int32_t ps, int32_t vs);
extern int32_t Arete(int32_t x, int32_t y, int32_t rs, int32_t N);

#define VFF_TYP_GABYTE          12      /* graphe d'arete code sur 1 octet */
#define VFF_TYP_GAFLOAT         13      /* graphe d'arete code en float */
#define VFF_TYP_GADOUBLE        14      /* graphe d'arete code en double */

/* Rajoute pour MSE 4D */
struct GA4d {
  char *name;
  uint32_t row_size;
  uint32_t col_size;
  uint32_t depth_size;

  uint32_t num_data_bands;	    /* Number of bands per data pixel,
				       or number of bands per image, or
				       dimension of vector data, or
				       number of elements in a vector */
  uint32_t seq_size;           /* Number of frame in the sequence */
  
  uint32_t data_storage_type;  /* storage type for disk data */
  double xdim, ydim, zdim, tdim;    /* voxel dimensions in real world */
  uint8_t image_data[1];
};

struct xvimage4D {
  int32_t ss;                            /* sequence size */
  struct xvimage ** frame; 
};

#define seqsizeGA(I) ((I)->seq_size)

/*            
		Codage du voisinage


		3	2	1			
		4	X	0
		5	6	7
*/

#define nonbord(p,rs,N) ((p%rs!=rs-1)&&(p>=rs)&&(p%rs!=0)&&(p<N-rs))
#define nonbord3d(p,rs,ps,N) ((p>=ps)&&(p<N-ps)&&(p%ps>=rs)&&(p%ps<ps-rs)&&(p%rs!=0)&&(p%rs!=rs-1))

/* ============== */
/* prototypes     */
/* ============== */

extern int32_t voisin4D8(int32_t i, int32_t k, int32_t rs, int32_t ps, int32_t N, int32_t Nt);

