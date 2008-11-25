/* $Id: mcset.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

typedef struct {
  int32_t setsize;      /* taille du tableau de bits (a multiplier par 32) */
  uint32_t tab[1];
} Set;

typedef struct {
  int32_t n;            /* nombre d'ensembles dans la liste */
  int32_t nmax;         /* nombre maximal d'ensembles dans la liste */
  Set * tab[1];     /* table des ensembles */
} SetList;

/* ================================================================== */
/* PROTOTYPES DES FONCTIONS POUR LES ENSEMBLES (Set) */
/* ================================================================== */

extern Set *   CreateEmptySet(int32_t setsize);
extern void    FreeSet(Set *s);
extern void    PrintSet(Set *s);
extern void    PrintSetLine(Set *s);
extern int32_t IsEmpty(Set *s);
extern void    SetEmpty(Set *s);
extern void    SetElement(Set *s, int32_t e);
extern void    ResetElement(Set *s, int32_t e);
extern int32_t InSet(Set *s, int32_t e);
extern void    Complement(Set *s);
extern void    Union(Set *s, Set *sa);
extern void    Inter(Set *s, Set *sa);
extern int32_t EqualSets(Set *s1, Set *s2);
extern void    CopySet(Set *sd, Set *ss);
extern int32_t Included(Set *s1, Set *s2);
extern int32_t StrictlyIncluded(Set *s1, Set *s2);
extern int32_t     CardSet(Set *s);

/* ================================================================== */
/* PROTOTYPES DES FONCTIONS POUR LES LISTES D'ENSEMBLES (SetList) */
/* ================================================================== */

extern void      PrintSetList(SetList * l);
extern void      PrintGraph(SetList * graph);
extern SetList * InitSetList(int32_t n, int32_t setsize);
extern void      TermineSetList(SetList * l);
extern void      FlushSetList(SetList * l);
extern void      PushSetList(SetList * l, Set *s);
extern int32_t   InSetList(SetList * l, Set *s);
extern int32_t   InclusSetList(SetList * l, Set *s);
