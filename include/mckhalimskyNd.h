/* $Id: mckhalimskyNd.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
  int32_t N;               /* la dimension de l'espace */
  int32_t *C;              /* les cotes de l'hyperpave */
  int32_t *D;              /* les offsets pour le codage des points */
  uint8_t *S;
} Khalimsky;

extern Khalimsky * AllocKhalimskyNd(int32_t dim, int32_t *userC);
extern void InitNd(int32_t N, int32_t *userC);
extern void TermineNd();
extern int32_t EffaceCliquesSimplesNd(Khalimsky *K);
extern int32_t EnsembleSimpleNd(Khalimsky *K);
extern int32_t CliqueSimpleNd(Khalimsky *K, int32_t x);
extern Khalimsky * KhalimskizeNd(struct xvimage *o);
extern struct xvimage *DeKhalimskizeNd(Khalimsky * K);
extern void Connex8ObjNd(Khalimsky *K);
extern void Connex4ObjNd(Khalimsky *K);
extern void PrintKhalimsky(Khalimsky * K);
extern void InitKhalimsky3Nd(uint32_t c, Khalimsky * K);
extern void InitCubeKhalimsky3Nd(uint32_t c, Khalimsky * K);
extern void InitCubeKhalimsky4Nd(uint32_t c, Khalimsky * K);
extern int32_t CourbeSimpleNd(Khalimsky *K);
extern int32_t SurfaceSimpleNd(Khalimsky *K, Khalimsky *Kp);

#ifdef __cplusplus
}
#endif
