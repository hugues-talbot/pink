/* $Id: mctopo3d.h,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {centre, face, arete, coin} typevoxel;

typedef struct VOXEL {
  /* nombres de voisins selon le type d'adjacence */
  uint8_t n6v;       /* les faces */
  uint8_t n12v;      /* les aretes */
  uint8_t n8v;       /* les coins */
  uint8_t n18v;      /* les faces et les aretes */
  uint8_t n26v;      /* les faces et les aretes et les coins */
  /* pointeurs sur les voxels voisins selon le type d'adjacence */
  struct VOXEL * v6[6];     /* les 6-voisins */
  struct VOXEL * v12[12];   /* les purs 18-voisins */
  struct VOXEL * v8[8];     /* les purs 26-voisins */
  struct VOXEL * v18[18];   /* les 18-voisins au sens large */
  struct VOXEL * v26[26];   /* les 26-voisins au sens large */
  /* champs de donnees */
  uint8_t val;
  uint8_t lab;
  uint8_t lab2;
  uint8_t lab3;
  uint8_t lab4;
  uint8_t lab5;
  /* divers */
  typevoxel type; /* peut prendre les valeurs : centre, face, arete, coin */
  uint8_t x,y,z;    /* coordonnes du point */
  uint32_t n;        /* indice du point dans le tableau */
} voxel;

typedef voxel * pvoxel;

#define encode(x,y,z) ((x)+(y)*3+(z)*9)

extern void init_topo3d();
extern void termine_topo3d();
extern uint32_t encodecube();
extern void construitcube(voxel * cube);
extern void top6(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t,
  int32_t *tb);                    /* resultats */
extern void top18(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t,
  int32_t *tb);                    /* resultats */
extern void top26(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t,
  int32_t *tb);                    /* resultats */
extern int32_t simple6(               /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t simple18(              /* pour un objet en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t simple26(              /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t simple6h(              /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t simple18h(             /* pour un objet en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t simple26h(             /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t tbar6h(               /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t tbar26h(              /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern uint8_t P_simple(voxel * cube, voxel * cubep, voxel * cubec, uint8_t connex);




extern int32_t pdestr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t pdestr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t pdestr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t plevdestr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t plevdestr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t plevdestr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t pconstr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t pconstr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t pconstr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t plevconstr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t plevconstr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t plevconstr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t peak6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t peak26(                   /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t well6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern int32_t well26(                   /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern uint8_t alpha26m(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern uint32_t alpha26m_l(
  uint32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern uint8_t alpha6m(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern uint8_t alpha26p(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern uint8_t alpha6p(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                       /* taille image */
extern uint8_t delta6m( 
/* retourne la valeur max. a laquelle p est destructible - minima 6-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern uint8_t delta26m( 
/* retourne la valeur max. a laquelle p est destructible - minima 26-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern uint8_t delta6p( 
/* retourne la valeur min. a laquelle p est constructible - minima 6-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern uint8_t delta26p( 
/* retourne la valeur min. a laquelle p est constructible - minima 26-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t separant6(  /* teste si un point est separant - minima 6-connexes
	         ie- s'il est separant pour une coupe <= img[p] */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t hseparant6(  /* teste si un point est hseparant - minima 6-connexes
	         ie- s'il est separant pour la coupe h */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* parametre */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t hfseparant6(  /* teste si un point est hfseparant - minima 6-connexes
	         ie- s'il est separant pour une coupe c telle que h < c <= img[p] */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* parametre */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t filsombre6(                /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t filsombre26(               /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t filclair6(                 /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t filclair26(                /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t t6mm(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t6m(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t6pp(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t6p(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t26mm(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t26m(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t26pp(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t26p(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t26pp_l(uint32_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t t6pp_l(uint32_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t bordext6(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N);
extern int32_t bordext26(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N);
extern void nbtopoh3d26_l( /* pour les minima en 26-connexite */ 
  uint32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  uint32_t h,
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t6p,
  int32_t *t26mm);
extern void nbtopoh3d6_l( /* pour les minima en 26-connexite */ 
  uint32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  uint32_t h,
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t26p,
  int32_t *t6mm);
extern int32_t nbvoisc6(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t nbvoisc18(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t nbvoisc26(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t nbvoiso6(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t nbvoiso18(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t nbvoiso26(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N);                      /* taille image */
extern int32_t curve6(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t curve18(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
extern int32_t curve26(uint8_t *img, int32_t p, int32_t rs, int32_t ps, int32_t N);
#ifdef __cplusplus
}
#endif
