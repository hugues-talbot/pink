/* $Id: mcindic.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t Indicstype;

extern Indicstype *Indics;       /* en global pour etre efficace */

#define Set(x,INDIC)   Indics[x]|=(1<<INDIC)
#define UnSet(x,INDIC) Indics[x]&=~(1<<INDIC)
#define UnSetAll(x)    Indics[x]=0
#define IsSet(x,INDIC) (Indics[x]&(1<<INDIC))
#define IsSetAny(x)    (Indics[x])

#define Set1(x)   Indics[x/8]|=(1<<(x%8))
#define UnSet1(x) Indics[x/8]&=~(1<<(x%8))
#define IsSet1(x) (Indics[x/8]&(1<<(x%8)))

/* ============== */
/* prototypes     */
/* ============== */

extern void IndicsInit(int32_t Size); 

extern void Indics1bitInit(int32_t Size); 

extern void IndicsTermine();


#ifdef __cplusplus
}
#endif
