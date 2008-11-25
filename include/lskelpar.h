extern void extract_vois(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t N,                       /* taille image */
  uint8_t *vois);

extern void rotate90_vois(uint8_t *vois);

extern int32_t mc_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N);
extern int32_t mc_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N);
extern int32_t mc_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N);
extern int32_t mc_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N);
extern int32_t mc_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N);

extern int32_t lskelpavlidis(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelpavlidis1(struct xvimage *image, 
			  int32_t pixel);
extern int32_t lskeleckhardt(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelmcultime(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelmccurv(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelmccurvrec(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelrutovitz(struct xvimage *image, 
			 int32_t nsteps,
			 struct xvimage *inhibit);
extern int32_t lskelzhangwang(struct xvimage *image, 
			  int32_t nsteps,
			  struct xvimage *inhibit);
extern int32_t lskelhanlarhee(struct xvimage *image, 
			  int32_t nsteps,
			  struct xvimage *inhibit);
extern int32_t lskelguohall(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit,
			int32_t variante);
extern int32_t lskelchinwan(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskeljang(struct xvimage *image, 
		     int32_t nsteps,
		     struct xvimage *inhibit);
extern int32_t lskeljangcor(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);

extern int32_t lskelmns(struct xvimage *image, 
		    int32_t nsteps,
		    struct xvimage *inhibit);
extern int32_t lskeljangrec(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelchoy(struct xvimage *image, 
		     int32_t nsteps,
		     struct xvimage *inhibit);
extern int32_t lskelmanz(struct xvimage *image, 
		     int32_t nsteps,
		     struct xvimage *inhibit,
		     int32_t variante);
extern int32_t lskelhall(struct xvimage *image, 
		     int32_t nsteps,
		     struct xvimage *inhibit,
		     int32_t variante);
extern int32_t lskelwutsai(struct xvimage *image, 
		       int32_t nsteps,
		       struct xvimage *inhibit);
extern int32_t lskelNK2(struct xvimage *image, 
			      int32_t nsteps,
			      struct xvimage *inhibit);
extern int32_t lskelbertrand_sym(struct xvimage *image, 
			     int32_t nsteps,
			     struct xvimage *inhibit);
extern int32_t lskelbertrand_asym_s(struct xvimage *image, 
				int32_t nsteps,
				struct xvimage *inhibit);
extern int32_t lskelMK2(struct xvimage *image, 
		    int32_t nsteps,
		    struct xvimage *inhibit);
extern int32_t lskelAK2(struct xvimage *image, 
			int32_t nsteps,
			struct xvimage *inhibit);
extern int32_t lskelrosenfeld(struct xvimage *image, 
			      int32_t nsteps,
			      struct xvimage *inhibit);
