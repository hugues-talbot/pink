/* $Id: mckhalimsky2d.h,v 1.7 2009-09-08 09:06:02 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#define CARRE(i,j) ((i%2)+(j%2)==2)
#define INTER(i,j) ((i%2)+(j%2)==1)
#define INTERH(i,j) ((i%2)&&(!(j%2)))
#define INTERV(i,j) ((!(i%2))&&(j%2))
#define SINGL(i,j) ((i%2)+(j%2)==0)
#define DIM2D(i,j) ((i%2)+(j%2))
#define NDG_CARRE 255
#define NDG_INTER 200
#define NDG_SINGL 128
#define GRS2D 3
#define GCS2D 3

#define VAL_NULLE    0
#define VAL_OBJET  255
#define VAL_MARQUE   1

extern void InitPileGrilles2d();
extern void TerminePileGrilles2d();
extern struct xvimage * Khalimskize2d(struct xvimage *o);
extern struct xvimage * KhalimskizeNDG2d(struct xvimage *o);
extern struct xvimage * DeKhalimskize2d(struct xvimage *o);
extern void Khalimskize2d_noalloc(struct xvimage *o, struct xvimage *k);
extern void KhalimskizeNDG2d_noalloc(struct xvimage *o, struct xvimage *k);
extern void DeKhalimskize2d_noalloc(struct xvimage *o, struct xvimage *r);
extern void ndgmin2d(struct xvimage *k);
extern void ndgminbeta2d(struct xvimage *k);
extern void ndgmax2d(struct xvimage *k);
extern void ndgmaxbeta2d(struct xvimage *k);
extern void ndgmoy2d(struct xvimage *k);
extern void ndg2grad2d(struct xvimage *k);
extern void ndg4grad2d(struct xvimage *k);
extern void Connex8Obj2d(struct xvimage *o);
extern void Connex4Obj2d(struct xvimage *o);
extern void Betapoint2d(int32_t rs, int32_t cs, int32_t i, int32_t j, int32_t *tab, int32_t *n);
extern void Alphapoint2d(int32_t rs, int32_t cs, int32_t i, int32_t j, int32_t *tab, int32_t *n);
extern void Betacarre2d(int32_t rs, int32_t cs, int32_t i, int32_t j, int32_t *tab, int32_t *n);
extern void Alphacarre2d(int32_t rs, int32_t cs, int32_t i, int32_t j, int32_t *tab, int32_t *n);
extern void Thetacarre2d(int32_t rs, int32_t cs, int32_t i, int32_t j, int32_t *tab, int32_t *n);
extern int32_t CardBetapoint2d(uint8_t *K, int32_t rs, int32_t cs, int32_t i, int32_t j);
extern int32_t CardThetaCarre2d(struct xvimage *k, int32_t i, int32_t j, uint8_t val);
extern int32_t BetaTerminal2d(uint8_t *K, int32_t rs, int32_t cs, int32_t i, int32_t j);
extern int32_t ExactementUnBetaTerminal2d(uint8_t *K, int32_t rs, int32_t cs);
extern void SatureAlphacarre2d(struct xvimage *k);
extern void AjouteAlphacarre2d(struct xvimage *k);
extern void AjouteBetacarre2d(struct xvimage *k);
extern void MaxAlpha2d(struct xvimage *k);
extern void MaxBeta2d(struct xvimage *k);
extern void ColorieKh2d(struct xvimage *k);
extern void EffaceLiensLibres2d(struct xvimage *k);
extern void CopieAlphacarre2d(uint8_t *G,uint8_t *K,int32_t rs,int32_t cs,int32_t i,int32_t j);
extern int32_t EffaceBetaTerminauxSimples2d(struct xvimage *k);
extern int32_t EnsembleSimple2d(struct xvimage *k);
extern int32_t Ensemble2Contractile2d(struct xvimage *b);
extern void Htkern2d(struct xvimage *b, int32_t n);
extern int32_t AlphaSimple2d(struct xvimage *b, int32_t i, int32_t j);
extern int32_t BetaSimple2d(struct xvimage *b, int32_t i, int32_t j);
extern int32_t Alpha2Simple2d(struct xvimage *b, int32_t i, int32_t j);
extern int32_t Beta2Simple2d(struct xvimage *b, int32_t i, int32_t j);
extern int32_t EulerKh2d(struct xvimage *b);
extern int32_t FaceLibre2d(struct xvimage *b, int32_t i, int32_t j);
extern int32_t PaireLibre2d(struct xvimage *b, int32_t i, int32_t j);
extern int32_t Collapse2d(struct xvimage *b, int32_t i, int32_t j);
#ifdef __cplusplus
}
#endif
