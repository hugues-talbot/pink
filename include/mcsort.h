/* $Id: mcsort.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t Partitionner(int32_t *A, int32_t p, int32_t r);
extern int32_t PartitionStochastique(int32_t *A, int32_t p, int32_t r);
extern void TriRapide (int32_t * A, int32_t p, int32_t r);
extern void TriRapideStochastique (int32_t * A, int32_t p, int32_t r);
extern int32_t SelectionStochastique (int32_t * A, int32_t p, int32_t r, int32_t i);
extern int32_t ElimineDupliques(int32_t *A, int32_t n);
