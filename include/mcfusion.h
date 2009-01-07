/* $Id: mcfusion.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* types publics  */
/* ============== */

typedef struct LISELT {
  int32_t Data;            /* Ce champ peut contenir 2 informations de natures differentes.*/
                       /* Le 1er element de liste indique le nombre d'elements (outre  */
                       /* le representant lui-meme), et les suivants contiennent les   */
                       /* autres elements representes.                                 */
  struct LISELT * Next;
} LisElt;

typedef struct {
  int32_t Max;             /* taille max de E (en nombre d'elements) */
  int32_t * Tabf;          /* table representant la fonction f */
  LisElt ** Tablis;    /* tableau des pointeurs de liste */
  LisElt * Tabelts;    /* tableau des elements de liste */
} Fus;

/* ============== */
/* prototypes     */
/* ============== */

extern Fus * CreeFus(
  int32_t taillemax
);

extern void FusTermine(
  Fus * L
);

extern void FusReinit(
  Fus * L
);

extern void FusPrint(
  Fus * L
);

extern int32_t FusF(
  Fus * L,
  int32_t i
);

extern int32_t Fusion(
  Fus * L,
  int32_t A, 
  int32_t B
);

extern void Fusion1rep(
  Fus * L,
  int32_t A, 
  int32_t B
);

extern int32_t FusNormalise(
  Fus * L
);

#ifdef __cplusplus
}
#endif
