/* $Id: mcsort.h,v 1.2 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t Partitionner(int32_t *A, int32_t p, int32_t r);
extern int32_t PartitionStochastique(int32_t *A, int32_t p, int32_t r);
extern void TriRapide (int32_t * A, int32_t p, int32_t r);
extern void TriRapideStochastique (int32_t * A, int32_t p, int32_t r);
extern int32_t SelectionStochastique (int32_t * A, int32_t p, int32_t r, int32_t i);
extern int32_t ElimineDupliques(int32_t *A, int32_t n);
#ifdef __cplusplus
}
#endif
