/* $Id: mcgraphe.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

/* ================================================ */
/* types publics */
/* ================================================ */

typedef struct cell {
  uint32_t val;
  struct cell * next;
} cell;
typedef cell * pcell;

typedef struct {
  uint32_t nbsom;
  uint32_t nbmaxar;
  uint32_t nbar;  
  cell * tasar;    /* tableau des cellules (tas) */
  cell * librear;  /* liste des cellules libres geree en pile lifo */
  pcell * listar;  /* tableau des listes d'aretes indexe par les sommets */
} graphe;

/* ================================================ */
/* prototypes */
/* ================================================ */

extern graphe * initgraphe(uint32_t nbsom, uint32_t nbmaxar);
extern void terminegraphe(graphe * g);
extern pcell allouecell(graphe * g);
extern void liberecell(graphe * g, pcell p);
extern void retiretete(graphe * g, pcell * pliste);
extern void retirearete(graphe * g, uint32_t som, uint32_t a);
extern int32_t estarete(graphe * g, uint32_t som, uint32_t a);
extern void ajoutearete(graphe * g, uint32_t som, uint32_t a);
extern void maille4graphe(graphe * g, uint32_t rs, uint32_t cs);
extern void nettoiegraphe(graphe * g);
extern void aretesommets(uint32_t a, uint32_t N, uint32_t rs, uint32_t * s1, uint32_t * s2);
extern int32_t estsuccesseur(graphe * g, uint32_t som, uint32_t a);
extern int32_t estsymetrique(graphe * g);
