/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/* 
Librairie mctopo3D : 

Calcul des nombres topologiques en 3D

Version utilisant les nombres de connexité T et Tb précalculés
et stockés dans:
TabSimple26.dat

Les nombres de connexité sont définis dans [Ber94].

[Ber94] G. Bertrand, "Simple points, topological numbers and geodesic
neighborhoods in cubic grids", Pattern Recognition Letters, 
Vol. 15, pp. 1003-1011, 1994.

Michel Couprie 2006
*/

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mclifo.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mctopo3d.h>

#define VERBOSE
#define TABSIMP26NAME "TabSimple26.dat"

static voxel cube_topo3d[27];
static voxel cubec_topo3d[27];

static uint8_t *table_mctopo3d_table_simple26 = NULL;
#define SetSimple26(x) table_mctopo3d_table_simple26[x/8]|=(1<<(x%8))
#define IsSimple26(x) (table_mctopo3d_table_simple26[x/8]&(1<<(x%8)))
  
/* ========================================== */
void mctopo3d_table_init_topo3d()
/* ========================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_init_topo3d"
{
  char tablefilename[128];
  int32_t tablesize, ret;
  FILE *fd;

  if (table_mctopo3d_table_simple26 != NULL)
  {
    fprintf(stderr, "%s: table_mctopo3d_table_simple26 already loaded\n", F_NAME);
    exit(1);
  }
  tablesize = 1<<23; // 2^26 / 8
  table_mctopo3d_table_simple26 = (uint8_t *)malloc(tablesize);
  if (table_mctopo3d_table_simple26 == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(1);
  }
  sprintf(tablefilename, "%s/src/tables/%s", getenv("PINK"), TABSIMP26NAME);
  fd = fopen (tablefilename, "r");
  if (fd == NULL) 
  {   
    fprintf(stderr, "%s: error while opening table\n", F_NAME);
    exit(1);
  }
  ret = fread(table_mctopo3d_table_simple26, sizeof(uint8_t), tablesize, fd);
  if (ret != tablesize)
  {
    fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, tablesize, ret);
    exit(1);
  }
  fclose(fd);
#ifdef VERBOSE
  printf("%s: %s loaded\n", F_NAME, TABSIMP26NAME);
#endif
} /* mctopo3d_table_init_topo3d() */

/* ========================================== */
void mctopo3d_table_termine_topo3d()
/* ========================================== */
{
  free(table_mctopo3d_table_simple26);
  table_mctopo3d_table_simple26 = NULL;
} /* mctopo3d_table_termine_topo3d() */

/* ========================================== */
void mctopo3d_table_construitcube(voxel * cube)
/* ========================================== */
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* mctopo3d_table_construitcube() */

/* ========================================== */
uint32_t mctopo3d_table_encodecube()
/* ========================================== */
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* mctopo3d_table_encodecube() */

/* ========================================== */
void mctopo3d_table_geodesic_neighborhood(voxel * cube, uint8_t connex, uint8_t s)
/* ========================================== */
/* 
  met a 1 le champ lab des points appartenant au voisinage geodesique d'ordre s du point central,
  met a 0 les autres
*/
{  
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* mctopo3d_table_geodesic_neighborhood() */

/* ========================================== */
void mctopo3d_table_G6(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_geodesic_neighborhood(cube, 6, 2);	
} /* mctopo3d_table_G6() */

/* ========================================== */
void mctopo3d_table_G6p(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_geodesic_neighborhood(cube, 6, 3);	
} /* mctopo3d_table_G6p() */

/* ========================================== */
void mctopo3d_table_G18(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_geodesic_neighborhood(cube, 18, 2);	
} /* mctopo3d_table_G18() */

/* ========================================== */
void mctopo3d_table_G26(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_geodesic_neighborhood(cube, 26, 1);	
} /* mctopo3d_table_G26() */

/* ========================================== */
uint8_t mctopo3d_table_nbcomp(voxel * cube, uint8_t connex)
/* ========================================== */
/*
  retourne le nombre de composantes connexes de l'objet marque par un lab=1 
*/
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* mctopo3d_table_nbcomp() */

/* ========================================== */
uint8_t mctopo3d_table_nbvois6(voxel * cube)
/* ========================================== */
/*
  retourne le nombre de 6-voisins du point central appartenant a l'objet
*/
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* mctopo3d_table_nbvois6() */

/* ========================================== */
uint8_t mctopo3d_table_nbvois26(voxel * cube)
/* ========================================== */
/*
  retourne le nombre de 26-voisins du point central appartenant a l'objet
*/
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* mctopo3d_table_nbvois26() */

/* ========================================== */
int32_t mctopo3d_table_nbvoisc6(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/*
  retourne le nombre de 6-voisins du point central de niveau nul
*/
{
  int32_t mctopo3d_table_nbvois = 0;
  if ((i%rs!=rs-1) && !B[i+1])    mctopo3d_table_nbvois++;
  if (((i%ps)>=rs) && !B[i-rs])   mctopo3d_table_nbvois++;
  if ((i%rs!=0) && !B[i-1])       mctopo3d_table_nbvois++;
  if (((i%ps)<ps-rs) && !B[i+rs]) mctopo3d_table_nbvois++;
  if ((i>=ps) && !B[i-ps])        mctopo3d_table_nbvois++;
  if ((i<N-ps) && !B[i+ps])      mctopo3d_table_nbvois++;
  return mctopo3d_table_nbvois;
} /* mctopo3d_table_nbvoisc6() */

/* ========================================== */
int32_t mctopo3d_table_nbvoisc18(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/*
  retourne le nombre de 18-voisins du point central de niveau nul
*/
{
  int32_t mctopo3d_table_nbvois = 0;
  if (((i<N-ps)&&(i%rs!=rs-1)) && !B[ps+i+1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps>=rs)) && !B[ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=0)) && !B[ps+i-1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps<ps-rs)) && !B[ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)) && !B[ps+i]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)) && !B[i+1]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)&&(i%ps>=rs)) && !B[i+1-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)) && !B[i-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)&&(i%rs!=0)) && !B[i-rs-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)) && !B[i-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)&&(i%ps<ps-rs)) && !B[i-1+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)) && !B[i+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)&&(i%rs!=rs-1)) && !B[i+rs+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=rs-1)) && !B[-ps+i+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps>=rs)) && !B[-ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=0)) && !B[-ps+i-1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps<ps-rs)) && !B[-ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)) && !B[-ps+i]) mctopo3d_table_nbvois++;
  return mctopo3d_table_nbvois;
} /* mctopo3d_table_nbvoisc18() */

/* ========================================== */
int32_t mctopo3d_table_nbvoisc26(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/*
  retourne le nombre de 26-voisins du point central de niveau nul
*/
{
  int32_t mctopo3d_table_nbvois = 0;
  if (((i<N-ps)&&(i%rs!=rs-1)) && !B[ps+i+1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=rs-1)&&(i%ps>=rs)) && !B[ps+i+1-rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps>=rs)) && !B[ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps>=rs)&&(i%rs!=0)) && !B[ps+i-rs-1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=0)) && !B[ps+i-1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=0)&&(i%ps<ps-rs)) && !B[ps+i-1+rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps<ps-rs)) && !B[ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps<ps-rs)&&(i%rs!=rs-1)) && !B[ps+i+rs+1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)) && !B[ps+i]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)) && !B[i+1]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)&&(i%ps>=rs)) && !B[i+1-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)) && !B[i-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)&&(i%rs!=0)) && !B[i-rs-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)) && !B[i-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)&&(i%ps<ps-rs)) && !B[i-1+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)) && !B[i+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)&&(i%rs!=rs-1)) && !B[i+rs+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=rs-1)) && !B[-ps+i+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=rs-1)&&(i%ps>=rs)) && !B[-ps+i+1-rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps>=rs)) && !B[-ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps>=rs)&&(i%rs!=0)) && !B[-ps+i-rs-1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=0)) && !B[-ps+i-1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=0)&&(i%ps<ps-rs)) && !B[-ps+i-1+rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps<ps-rs)) && !B[-ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps<ps-rs)&&(i%rs!=rs-1)) && !B[-ps+i+rs+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)) && !B[-ps+i]) mctopo3d_table_nbvois++;
  return mctopo3d_table_nbvois;
} /* mctopo3d_table_nbvoisc26() */

/* ========================================== */
int32_t mctopo3d_table_nbvoiso6(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/*
  retourne le nombre de 6-voisins du point central de niveau NON nul
*/
{
  int32_t mctopo3d_table_nbvois = 0;
  if ((i%rs!=rs-1) && B[i+1])    mctopo3d_table_nbvois++;
  if (((i%ps)>=rs) && B[i-rs])   mctopo3d_table_nbvois++;
  if ((i%rs!=0) && B[i-1])       mctopo3d_table_nbvois++;
  if (((i%ps)<ps-rs) && B[i+rs]) mctopo3d_table_nbvois++;
  if ((i>=ps) && B[i-ps])        mctopo3d_table_nbvois++;
  if ((i<N-ps) && B[i+ps])      mctopo3d_table_nbvois++;
  return mctopo3d_table_nbvois;
} /* mctopo3d_table_nbvoiso6() */

/* ========================================== */
int32_t mctopo3d_table_nbvoiso18(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/*
  retourne le nombre de 18-voisins du point central de niveau NON nul
*/
{
  int32_t mctopo3d_table_nbvois = 0;
  if (((i<N-ps)&&(i%rs!=rs-1)) && B[ps+i+1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps>=rs)) && B[ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=0)) && B[ps+i-1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps<ps-rs)) && B[ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)) && B[ps+i]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)) && B[i+1]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)&&(i%ps>=rs)) && B[i+1-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)) && B[i-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)&&(i%rs!=0)) && B[i-rs-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)) && B[i-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)&&(i%ps<ps-rs)) && B[i-1+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)) && B[i+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)&&(i%rs!=rs-1)) && B[i+rs+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=rs-1)) && B[-ps+i+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps>=rs)) && B[-ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=0)) && B[-ps+i-1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps<ps-rs)) && B[-ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)) && B[-ps+i]) mctopo3d_table_nbvois++;
  return mctopo3d_table_nbvois;
} /* mctopo3d_table_nbvoiso18() */

/* ========================================== */
int32_t mctopo3d_table_nbvoiso26(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ========================================== */
/*
  retourne le nombre de 26-voisins du point central de niveau NON nul
*/
{
  int32_t mctopo3d_table_nbvois = 0;
  if (((i<N-ps)&&(i%rs!=rs-1)) && B[ps+i+1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=rs-1)&&(i%ps>=rs)) && B[ps+i+1-rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps>=rs)) && B[ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps>=rs)&&(i%rs!=0)) && B[ps+i-rs-1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=0)) && B[ps+i-1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%rs!=0)&&(i%ps<ps-rs)) && B[ps+i-1+rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps<ps-rs)) && B[ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i<N-ps)&&(i%ps<ps-rs)&&(i%rs!=rs-1)) && B[ps+i+rs+1]) mctopo3d_table_nbvois++;
  if (((i<N-ps)) && B[ps+i]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)) && B[i+1]) mctopo3d_table_nbvois++;
  if (((i%rs!=rs-1)&&(i%ps>=rs)) && B[i+1-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)) && B[i-rs]) mctopo3d_table_nbvois++;
  if (((i%ps>=rs)&&(i%rs!=0)) && B[i-rs-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)) && B[i-1]) mctopo3d_table_nbvois++;
  if (((i%rs!=0)&&(i%ps<ps-rs)) && B[i-1+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)) && B[i+rs]) mctopo3d_table_nbvois++;
  if (((i%ps<ps-rs)&&(i%rs!=rs-1)) && B[i+rs+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=rs-1)) && B[-ps+i+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=rs-1)&&(i%ps>=rs)) && B[-ps+i+1-rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps>=rs)) && B[-ps+i-rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps>=rs)&&(i%rs!=0)) && B[-ps+i-rs-1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=0)) && B[-ps+i-1]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%rs!=0)&&(i%ps<ps-rs)) && B[-ps+i-1+rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps<ps-rs)) && B[-ps+i+rs]) mctopo3d_table_nbvois++;
  if (((i>=ps)&&(i%ps<ps-rs)&&(i%rs!=rs-1)) && B[-ps+i+rs+1]) mctopo3d_table_nbvois++;
  if (((i>=ps)) && B[-ps+i]) mctopo3d_table_nbvois++;
  return mctopo3d_table_nbvois;
} /* mctopo3d_table_nbvoiso26() */

/* ========================================== */
uint8_t mctopo3d_table_T6(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_G6(cube);
  return mctopo3d_table_nbcomp(cube, 6);
} /* mctopo3d_table_T6() */

/* ========================================== */
uint8_t mctopo3d_table_T6p(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_G6p(cube);
  return mctopo3d_table_nbcomp(cube, 6);
} /* mctopo3d_table_T6p() */

/* ========================================== */
uint8_t mctopo3d_table_T18(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_G18(cube);
  return mctopo3d_table_nbcomp(cube, 18);
} /* mctopo3d_table_T18() */

/* ========================================== */
uint8_t mctopo3d_table_T26(voxel * cube)
/* ========================================== */
{
  mctopo3d_table_G26(cube);
  return mctopo3d_table_nbcomp(cube, 26);
} /* mctopo3d_table_T26() */

/* ==================================== */
static void preparecubes(
  uint8_t *B,            /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
  Transfere le voisinage de i pour l'image 3d img dans les 
  structures cube_topo3d (vois. original) et cubec_topo3d (complementaire).
  ATTENTION: i ne doit pas etre un point de bord (test a faire avant).
 */
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* preparecubes() */

/* ==================================== */
static void preparecubesh(
  uint8_t *img,          /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
  Transfere le voisinage de i pour l'image 3d img seuillee au niveau h dans les 
  structures cube_topo3d (vois. original) et cubec_topo3d (complementaire).
  ATTENTION: i ne doit pas etre un point de bord (test a faire avant).
 */
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* preparecubesh() */

/* ==================================== */
static void preparecubesh_l(
  int32_t *img,          /* pointeur base image */
  int32_t i,                       /* index du point */
  int32_t h,                      /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
  Transfere le voisinage de i pour l'image 3d img seuillee au niveau h dans les 
  structures cube_topo3d (vois. original) et cubec_topo3d (complementaire).
  ATTENTION: i ne doit pas etre un point de bord (test a faire avant).
 */
{
  fprintf(stderr,"NOT AVAILABLE WITH TABULATED VERSION (see mctopo3d.c)\n");
  exit(1);
} /* preparecubesh_l() */

/* ******************************************************************************* */
/* ******************************************************************************* */
/*                               PRIMITIVES 3D BINAIRES                            */
/* ******************************************************************************* */
/* ******************************************************************************* */

/* ==================================== */
void mctopo3d_table_top6(                   /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t,
  int32_t *tb)                     /* resultats */
/* ==================================== */
/*
  ATTENTION: p ne doit pas etre un point de bord (test a faire avant).
*/
{
  preparecubes(img, p, rs, ps, N);
  *t = mctopo3d_table_T6(cube_topo3d);
  *tb = mctopo3d_table_T26(cubec_topo3d);
} /* mctopo3d_table_top6() */

/* ==================================== */
void mctopo3d_table_top18(                   /* pour un objet en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t,
  int32_t *tb)                     /* resultats */
/* ==================================== */
/*
  ATTENTION: p ne doit pas etre un point de bord (test a faire avant).
*/
{
  preparecubes(img, p, rs, ps, N);
  *t = mctopo3d_table_T18(cube_topo3d);
  *tb = mctopo3d_table_T6p(cubec_topo3d);
} /* mctopo3d_table_top18() */

/* ==================================== */
void mctopo3d_table_top26(                   /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *t,
  int32_t *tb)                     /* resultats */
/* ==================================== */
/*
  ATTENTION: p ne doit pas etre un point de bord (test a faire avant).
*/
{
  preparecubes(img, p, rs, ps, N);
  *t = mctopo3d_table_T26(cube_topo3d);
  *tb = mctopo3d_table_T6(cubec_topo3d);
} /* mctopo3d_table_top26() */

/* ==================================== */
int32_t mctopo3d_table_simple6(                   /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple6"
{
  fprintf(stderr, "%s: tabulated version not implrmented\n", F_NAME);
  exit(1);
} /* mctopo3d_table_simple6() */

/* ==================================== */
int32_t mctopo3d_table_simple18(                  /* pour un objet en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple18"
{
  fprintf(stderr, "%s: tabulated version not implrmented\n", F_NAME);
  exit(1);
} /* mctopo3d_table_simple18() */

/* ==================================== */
int32_t mctopo3d_table_simple26(                  /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple26"
{
  uint32_t mask = 0, v, k;
  for (k = 0; k < 26; k++)
  {    
    v = voisin26(p, k, rs, ps, N);
    if (img[v]) mask = mask | (1 << k);
//if (img[v]) printf("1"); else printf("0");
  }
//printf("\nmask = %x\n\n", mask);
  if (IsSimple26(mask)) return 1;
  return 0;
} /* mctopo3d_table_simple26() */

/* ==================================== */
int32_t mctopo3d_table_simple26mask(              /* pour un objet en 26-connexite */
  uint32_t mask)
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple26mask"
{
  if (IsSimple26(mask)) return 1;
  return 0;
} /* mctopo3d_table_simple26mask() */

/* ==================================== */
int32_t mctopo3d_table_simple6h(                   /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple6h"
{
  fprintf(stderr, "%s: tabulated version not implrmented\n", F_NAME);
  exit(1);
} /* mctopo3d_table_simple6h() */

/* ==================================== */
int32_t mctopo3d_table_simple18h(                  /* pour un objet en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple18h"
{
  fprintf(stderr, "%s: tabulated version not implrmented\n", F_NAME);
  exit(1);
} /* mctopo3d_table_simple18h() */

/* ==================================== */
int32_t mctopo3d_table_simple26h(                  /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_simple26h"
{
  fprintf(stderr, "%s: tabulated version not implrmented\n", F_NAME);
  exit(1);
} /* mctopo3d_table_simple26h() */

/* ==================================== */
int32_t mctopo3d_table_tbar6h(               /* pour un objet en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return -1;
  preparecubesh(img, p, h, rs, ps, N);
  return mctopo3d_table_T26(cubec_topo3d);
} /* mctopo3d_table_tbar6h() */

/* ==================================== */
int32_t mctopo3d_table_tbar26h(              /* pour un objet en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* seuil */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return -1;
  preparecubesh(img, p, h, rs, ps, N);
  return mctopo3d_table_T6(cubec_topo3d);
} /* mctopo3d_table_tbar26h() */

/* ========================================== */
uint8_t mctopo3d_table_P_simple(voxel * cube, voxel * cubep, voxel * cubec, uint8_t connex)
/* ========================================== */
#undef F_NAME
#define F_NAME "mctopo3d_table_P_simple"
/*
  cube contient X
  cubep contient P
  cubec (auxiliaire) n'a pas besoin d'etre initialise
  d'apres: "Some topological properties of surfaces in Z3", G. Bertrand & R. Malgouyres
           Theoreme 6
*/
{
  uint8_t n;
  uint8_t v;
  pvoxel x;  /* point central de cube */
  pvoxel y;  /* point de cube */
  pvoxel xc; /* point central de cubec */
  pvoxel yc; /* point de cubec */
  pvoxel xp; /* point central de cubep */
  pvoxel yp; /* point de cubep */

  for (n = 0; n < 27; n++) if (cube[n].val == 1) cubec[n].val = 0; else cubec[n].val = 1;

  switch (connex) /* teste la condition 2 (theoreme 6) */
  {
    case 6:  
      if (mctopo3d_table_T26(cubec) != 1) return 0; 
      break;
    case 18: 
      if (mctopo3d_table_T6p(cubec) != 1) return 0; 
      break;
    case 26: 
      if (mctopo3d_table_T6(cubec) != 1) return 0; 
      break;
    default: 
      fprintf(stderr, "mctopo3d_table_P_simple: mauvaise connexite : %d\n", connex); 
      exit(0); 
  } /* switch (connex) */
  
  x = &(cube[13]);
  xc = &(cubec[13]);
  xp = &(cubep[13]);
  switch (connex) /* teste la condition 4 (theoreme 6) */
  {
    case 6: 
      for (n = 0; n < x->n26v; n++)
      {
        yp = xp->v26[n];
        if (yp->val)
        {
          yc = xc->v26[n];
          v = yc->val;
          yc->val = 1;
          if (mctopo3d_table_T26(cubec) != 1) return 0;
          yc->val = v;
        } /* if (yp->val) */
      } /* for (n = 0; n < x->n26v; n++) */
      break;
    case 18: 
      for (n = 0; n < x->n6v; n++)
      {
        yp = xp->v6[n];
        if (yp->val)
        {
          yc = xc->v6[n];
          v = yc->val;
          yc->val = 1;
          if (mctopo3d_table_T6p(cubec) != 1) return 0;
          yc->val = v;
        } /* if (yp->val) */
      } /* for (n = 0; n < x->n6v; n++) */
      break;
    case 26: 
      for (n = 0; n < x->n6v; n++)
      {
        yp = xp->v6[n];
        if (yp->val)
        {
          yc = xc->v6[n];
          v = yc->val;
          yc->val = 1;
          if (mctopo3d_table_T6(cubec) != 1) return 0;
          yc->val = v;
        } /* if (yp->val) */
      } /* for (n = 0; n < x->n6v; n++) */
      break;
    default: 
      fprintf(stderr, "mctopo3d_table_P_simple: mauvaise connexite : %d\n", connex); 
      exit(0); 
  } /* switch (connex) */
  
  for (n = 0; n < 27; n++) /* calcule et range dans cubec l'ensemble R = X - P  */
  {
    y = &(cube[n]);
    yp = &(cubep[n]);
    yc = &(cubec[n]);
    if (y->val && !yp->val) yc->val = 1; else yc->val = 0;
  } /* for (n = 0; n < 27; n++) */

  switch (connex) /* teste la condition 1 (theoreme 6) */
  {
    case 6:  
      if (mctopo3d_table_T6(cubec) != 1) return 0;
      break;
    case 18: 
      if (mctopo3d_table_T18(cubec) != 1) return 0;
      break;
    case 26: 
      if (mctopo3d_table_T26(cubec) != 1) return 0;
      break;
    default: 
      fprintf(stderr, "mctopo3d_table_P_simple: mauvaise connexite : %d\n", connex); 
      exit(0); 
  } /* switch (connex) */
  
  switch (connex) /* teste la condition 3 (theoreme 6) */
  {
    case 6: 
      for (n = 0; n < x->n6v; n++)
      {
        yp = xp->v6[n];
        if (yp->val)
        {
          yc = xc->v6[n];
          v = yc->val;
          yc->val = 1;
          if (mctopo3d_table_T6(cubec) != 1) return 0;
          yc->val = v;
        } /* if (yp->val) */
      } /* for (n = 0; n < x->n6v; n++) */
      break;
    case 18: 
      for (n = 0; n < x->n18v; n++)
      {
        yp = xp->v18[n];
        if (yp->val)
        {
          yc = xc->v18[n];
          v = yc->val;
          yc->val = 1;
          if (mctopo3d_table_T18(cubec) != 1) return 0;
          yc->val = v;
        } /* if (yp->val) */
      } /* for (n = 0; n < x->n18v; n++) */
      break;
    case 26: 
      for (n = 0; n < x->n26v; n++)
      {
        yp = xp->v26[n];
        if (yp->val)
        {
          yc = xc->v26[n];
          v = yc->val;
          yc->val = 1;
          if (mctopo3d_table_T26(cubec) != 1) return 0;
          yc->val = v;
        } /* if (yp->val) */
      } /* for (n = 0; n < x->n26v; n++) */
      break;
    default: 
      fprintf(stderr, "mctopo3d_table_P_simple: mauvaise connexite : %d\n", connex); 
      exit(0); 
  } /* switch (connex) */
  return 1;
} /* mctopo3d_table_P_simple() */





/* ******************************************************************************* */
/* ******************************************************************************* */
/*                                  PRIMITIVES 3D NDG                              */
/* ******************************************************************************* */
/* ******************************************************************************* */

/* ==================================== */
int32_t mctopo3d_table_pdestr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return ((mctopo3d_table_T26(cube_topo3d) == 1) && (mctopo3d_table_T6(cubec_topo3d) == 1));
} /* mctopo3d_table_pdestr6() */

/* ==================================== */
int32_t mctopo3d_table_pdestr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return ((mctopo3d_table_T6p(cube_topo3d) == 1) && (mctopo3d_table_T18(cubec_topo3d) == 1));
} /* mctopo3d_table_pdestr18() */

/* ==================================== */
int32_t mctopo3d_table_pdestr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return ((mctopo3d_table_T6(cube_topo3d) == 1) && (mctopo3d_table_T26(cubec_topo3d) == 1));
} /* mctopo3d_table_pdestr26() */

/* ==================================== */
int32_t mctopo3d_table_plevdestr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return (mctopo3d_table_T6(cubec_topo3d) == 1);
} /* mctopo3d_table_plevdestr6() */

/* ==================================== */
int32_t mctopo3d_table_plevdestr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return (mctopo3d_table_T18(cubec_topo3d) == 1);
} /* mctopo3d_table_plevdestr18() */

/* ==================================== */
int32_t mctopo3d_table_plevdestr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return (mctopo3d_table_T26(cubec_topo3d) == 1);
} /* mctopo3d_table_plevdestr26() */

/* ==================================== */
int32_t mctopo3d_table_pconstr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return ((mctopo3d_table_T26(cube_topo3d) == 1) && (mctopo3d_table_T6(cubec_topo3d) == 1));
} /* mctopo3d_table_pconstr6() */

/* ==================================== */
int32_t mctopo3d_table_pconstr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return ((mctopo3d_table_T6p(cube_topo3d) == 1) && (mctopo3d_table_T18(cubec_topo3d) == 1));
} /* mctopo3d_table_pconstr18() */

/* ==================================== */
int32_t mctopo3d_table_pconstr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return ((mctopo3d_table_T6(cube_topo3d) == 1) && (mctopo3d_table_T26(cubec_topo3d) == 1));
} /* mctopo3d_table_pconstr26() */

/* ==================================== */
int32_t mctopo3d_table_plevconstr6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return (mctopo3d_table_T26(cube_topo3d) == 1);
} /* mctopo3d_table_plevconstr6() */

/* ==================================== */
int32_t mctopo3d_table_plevconstr18(                  /* pour des minima en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return (mctopo3d_table_T6p(cube_topo3d) == 1);
} /* mctopo3d_table_plevconstr18() */

/* ==================================== */
int32_t mctopo3d_table_plevconstr26(                  /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return (mctopo3d_table_T6(cube_topo3d) == 1);
} /* mctopo3d_table_plevconstr26() */

/* ==================================== */
int32_t mctopo3d_table_peak6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return (mctopo3d_table_T26(cube_topo3d) == 0);
} /* mctopo3d_table_peak6() */

/* ==================================== */
int32_t mctopo3d_table_peak26(                    /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return (mctopo3d_table_T6(cube_topo3d) == 0);
} /* mctopo3d_table_peak26() */

/* ==================================== */
int32_t mctopo3d_table_well6(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return (mctopo3d_table_T6(cubec_topo3d) == 0);
} /* mctopo3d_table_well6() */

/* ==================================== */
int32_t mctopo3d_table_well26(                    /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return (mctopo3d_table_T26(cubec_topo3d) == 0);
} /* mctopo3d_table_well26() */

/* ==================================== */
uint8_t mctopo3d_table_alpha26m(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* retourne le sup des valeurs < img[x] dans le 26-voisinage de x, */
/* ou img[x] si pas de telles valeurs */
/* ==================================== */
{
	register uint8_t val = *(img+p);
	register int32_t q;
	register uint8_t v;
	register int32_t mctopo3d_table_alpha = NDG_MIN - 1;
        register int32_t k;

        for (k = 0; k < 26; k += 1)
        {
          q = voisin26(p, k, rs, ps, N);
          if ((q != -1) && ((v=img[q]) < val) && ((int32_t)v > mctopo3d_table_alpha)) mctopo3d_table_alpha = (int32_t)v;
	}
        if (mctopo3d_table_alpha == NDG_MIN - 1) 
          return val;
        else
          return (uint8_t)mctopo3d_table_alpha;
} /* mctopo3d_table_alpha26m() */

/* ==================================== */
int32_t mctopo3d_table_alpha26m_l(
  int32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* retourne le sup des valeurs < img[x] dans le 26-voisinage de x, */
/* ou img[x] si pas de telles valeurs */
/* ==================================== */
{
	register int32_t val = *(img+p);
	register int32_t q;
	register int32_t v;
	register int32_t mctopo3d_table_alpha = NDG_MIN - 1;
        register int32_t k;

        for (k = 0; k < 26; k += 1)
        {
          q = voisin26(p, k, rs, ps, N);
          if ((q != -1) && ((v=img[q]) < val) && ((int32_t)v > mctopo3d_table_alpha)) mctopo3d_table_alpha = (int32_t)v;
	}
        if (mctopo3d_table_alpha == NDG_MIN - 1) 
          return val;
        else
          return (int32_t)mctopo3d_table_alpha;
} /* mctopo3d_table_alpha26m_l() */

/* ==================================== */
uint8_t mctopo3d_table_alpha6m(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* retourne le sup des valeurs < img[x] dans le 6-voisinage de x, */
/* ou img[x] si pas de telles valeurs */
/* ==================================== */
{
	register uint8_t val = *(img+p);
	register int32_t q;
	register uint8_t v;
	register int32_t mctopo3d_table_alpha = NDG_MIN - 1;
        register int32_t k;

        for (k = 0; k <= 10; k += 2)
        {
          q = voisin6(p, k, rs, ps, N);
          if ((q != -1) && ((v=img[q]) < val) && ((int32_t)v > mctopo3d_table_alpha)) mctopo3d_table_alpha = (int32_t)v;
	}
        if (mctopo3d_table_alpha == NDG_MIN - 1) 
          return val;
        else
          return (uint8_t)mctopo3d_table_alpha;
} /* mctopo3d_table_alpha6m() */

/* ==================================== */
uint8_t mctopo3d_table_alpha26p(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* retourne le inf des valeurs > img[x] dans le 26-voisinage de x, */
/* ou img[x] si pas de telles valeurs */
/* ==================================== */
{
	register uint8_t val = *(img+p);
	register int32_t q;
	register uint8_t v;
	register int32_t mctopo3d_table_alpha = NDG_MAX + 1;
        register int32_t k;

        for (k = 0; k < 26; k += 1)
        {
          q = voisin26(p, k, rs, ps, N);
          if ((q != -1) && ((v=img[q]) > val) && ((int32_t)v < mctopo3d_table_alpha)) mctopo3d_table_alpha = (int32_t)v;
	}
        if (mctopo3d_table_alpha == NDG_MAX + 1) 
          return val;
        else
          return (uint8_t)mctopo3d_table_alpha;
} /* mctopo3d_table_alpha26p() */

/* ==================================== */
uint8_t mctopo3d_table_alpha6p(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* retourne le inf des valeurs > img[x] dans le 6-voisinage de x, */
/* ou img[x] si pas de telles valeurs */
/* ==================================== */
{
	register uint8_t val = *(img+p);
	register int32_t q;
	register uint8_t v;
	register int32_t mctopo3d_table_alpha = NDG_MAX + 1;
        register int32_t k;

        for (k = 0; k <= 10; k += 2)
        {
          q = voisin6(p, k, rs, ps, N);
          if ((q != -1) && ((v=img[q]) > val) && ((int32_t)v < mctopo3d_table_alpha)) mctopo3d_table_alpha = (int32_t)v;
	}
        if (mctopo3d_table_alpha == NDG_MAX + 1) 
          return val;
        else
          return (uint8_t)mctopo3d_table_alpha;
} /* mctopo3d_table_alpha6p() */

/* ==================================== */
uint8_t mctopo3d_table_delta6m( 
/* retourne la valeur max. a laquelle p est destructible - minima 6-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{	
  uint8_t ret, sav = img[p];
  while (mctopo3d_table_pdestr6(img, p, rs, ps, N)) img[p] = mctopo3d_table_alpha26m(img, p, rs, ps, N);
  ret = img[p];
  img[p] = sav;
  return ret;
} /* mctopo3d_table_delta6m() */

/* ==================================== */
uint8_t mctopo3d_table_delta26m( 
/* retourne la valeur max. a laquelle p est destructible - minima 26-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{	
  uint8_t ret, sav = img[p];
  while (mctopo3d_table_pdestr26(img, p, rs, ps, N)) img[p] = mctopo3d_table_alpha26m(img, p, rs, ps, N);
  ret = img[p];
  img[p] = sav;
  return ret;
} /* mctopo3d_table_delta26m() */

/* ==================================== */
uint8_t mctopo3d_table_delta6p( 
/* retourne la valeur min. a laquelle p est constructible - minima 6-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{	
  uint8_t ret, sav = img[p];
  while (mctopo3d_table_pconstr6(img, p, rs, ps, N)) img[p] = mctopo3d_table_alpha26p(img, p, rs, ps, N);
  ret = img[p];
  img[p] = sav;
  return ret;
} /* mctopo3d_table_delta6p() */

/* ==================================== */
uint8_t mctopo3d_table_delta26p( 
/* retourne la valeur min. a laquelle p est constructible - minima 26-connexes */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{	
  uint8_t ret, sav = img[p];
  while (mctopo3d_table_pconstr26(img, p, rs, ps, N)) img[p] = mctopo3d_table_alpha26p(img, p, rs, ps, N);
  ret = img[p];
  img[p] = sav;
  return ret;
} /* mctopo3d_table_delta26p() */

/* ==================================== */
int32_t mctopo3d_table_separant6(  /* teste si un point est separant - minima 6-connexes
	         ie- s'il est mctopo3d_table_separant pour une coupe <= img[p] */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  int32_t k, q;

  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;

  preparecubesh(img, p, img[p], rs, ps, N);
  if (mctopo3d_table_T6(cubec_topo3d) >= 2) return 1;

  for (k = 0; k < 26; k += 1)
  {
    q = voisin26(p, k, rs, ps, N);
    if ((q != -1) && (img[q] <= img[p]))
    {
      preparecubesh(img, p, img[q], rs, ps, N);
      if (mctopo3d_table_T6(cubec_topo3d) >= 2) return 1;
    }
  }	
  return 0;
} /* mctopo3d_table_separant6() */

/* ==================================== */
int32_t hmctopo3d_table_separant6(  /* teste si un point est mctopo3d_table_hseparant - minima 6-connexes
	         ie- s'il est mctopo3d_table_separant pour la coupe h */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* parametre */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;

  preparecubesh(img, p, h, rs, ps, N);
  if (mctopo3d_table_T6(cubec_topo3d) >= 2) return 1;
  return 0;
} /* hmctopo3d_table_separant6() */

/* ==================================== */
int32_t hfmctopo3d_table_separant6(  /* teste si un point est mctopo3d_table_hfseparant - minima 6-connexes
	         ie- s'il est mctopo3d_table_separant pour une coupe c telle que h < c <= img[p] */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,                       /* parametre */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  int32_t k, q;

  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;

  preparecubesh(img, p, img[p], rs, ps, N);
  if (mctopo3d_table_T6(cubec_topo3d) >= 2) return 1;

  for (k = 0; k < 26; k += 1)
  {
    q = voisin26(p, k, rs, ps, N);
    if ((q != -1) && (img[q] > h) && (img[q] <= img[p]))
    {
      preparecubesh(img, p, img[q], rs, ps, N);
      if (mctopo3d_table_T6(cubec_topo3d) >= 2) return 1;
    }
  }	
  return 0;
} /* hfmctopo3d_table_separant6() */

/* ==================================== */
int32_t mctopo3d_table_filsombre6(                /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
   pour la coupe K (>img[p]), le point doit etre
   - soit un point isole (T == 1; Tb == 0)
   - soit une extremite de courbe (T == 1; Tb == 1 et card(K) = 1)
   - soit un point de courbe (T == 1; Tb == 2 et card(K) = 2)
   - soit un point de croisement de courbe (T == 1; Tb == n et card(K) = n)

*/
{
  int32_t T, Tb, Nb;
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  T = mctopo3d_table_T26(cube_topo3d);
  if (T != 1) return 0;
  Tb = mctopo3d_table_T6(cubec_topo3d);
  if (Tb == 0) return 1;
  Nb = mctopo3d_table_nbvois6(cubec_topo3d);
  if (Tb > 0) return (Nb == Tb);
  return 0;
} /* mctopo3d_table_filsombre6() */

/* ==================================== */
int32_t mctopo3d_table_filsombre26(               /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
   pour la coupe K (>img[p]), le point doit etre
   - soit un point isole (T == 1; Tb == 0)
   - soit une extremite de courbe (T == 1; Tb == 1 et card(K) = 1)
   - soit un point de courbe (T == 1; Tb == 2 et card(K) = 2)
   - soit un point de croisement de courbe (T == 1; Tb == n et card(K) = n)

*/
{
  int32_t T, Tb, Nb;
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  T = mctopo3d_table_T6(cube_topo3d);
  if (T != 1) return 0;
  Tb = mctopo3d_table_T26(cubec_topo3d);
  if (Tb == 0) return 1;
  Nb = mctopo3d_table_nbvois26(cubec_topo3d);
  if (Tb > 0) return (Nb == Tb);
  return 0;
} /* mctopo3d_table_filsombre26() */

/* ==================================== */
int32_t mctopo3d_table_filclair6(                /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
   pour la coupe K (>=img[p]), le point doit etre
   - soit un point isole (Tb == 1; T == 0)
   - soit une extremite de courbe (Tb == 1; T == 1 et card(K) = 1)
   - soit un point de courbe (Tb == 1; T == 2 et card(K) = 2)
   - soit un point de croisement de courbe (Tb == 1; T == n et card(K) = n)
*/
{
  int32_t T, Tb, Nb;
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  Tb = mctopo3d_table_T6(cubec_topo3d);
  if (Tb != 1) return 0;
  T = mctopo3d_table_T26(cube_topo3d);
  if (T == 0) return 1;
  Nb = mctopo3d_table_nbvois26(cube_topo3d);
  if (T > 0) return (Nb == T);
  return 0;
} /* mctopo3d_table_filclair6() */

/* ==================================== */
int32_t mctopo3d_table_filclair26(                /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*
   pour la coupe K (>=img[p]), le point doit etre
   - soit un point isole (Tb == 1; T == 0)
   - soit une extremite de courbe (Tb == 1; T == 1 et card(K) = 1)
   - soit un point de courbe (Tb == 1; T == 2 et card(K) = 2)
   - soit un point de croisement de courbe (Tb == 1; T == n et card(K) = n)
*/
{
  int32_t T, Tb, Nb;
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  Tb = mctopo3d_table_T26(cubec_topo3d);
  if (Tb != 1) return 0;
  T = mctopo3d_table_T6(cube_topo3d);
  if (T == 0) return 1;
  Nb = mctopo3d_table_nbvois6(cube_topo3d);
  if (T > 0) return (Nb == T);
  return 0;
} /* mctopo3d_table_filclair26() */

/* ==================================== */
int32_t mctopo3d_table_t6mm(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return mctopo3d_table_T6(cubec_topo3d);
} /* mctopo3d_table_t6mm() */

/* ==================================== */
int32_t mctopo3d_table_t6m(                   /* pour des minima en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return mctopo3d_table_T6(cubec_topo3d);
} /* mctopo3d_table_t6m() */

/* ==================================== */
int32_t mctopo3d_table_t26mm(                   /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return mctopo3d_table_T26(cubec_topo3d);
} /* mctopo3d_table_t26mm() */

/* ==================================== */
int32_t mctopo3d_table_t26m(                   /* pour des minima en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return mctopo3d_table_T26(cubec_topo3d);
} /* mctopo3d_table_t26m() */

/* ==================================== */
int32_t mctopo3d_table_t6pp(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return mctopo3d_table_T6(cube_topo3d);
} /* mctopo3d_table_t6pp() */

/* ==================================== */
int32_t mctopo3d_table_t6p(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return mctopo3d_table_T6(cube_topo3d);
} /* mctopo3d_table_t6p() */

/* ==================================== */
int32_t mctopo3d_table_t26pp(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p]+1, rs, ps, N);
  return mctopo3d_table_T26(cube_topo3d);
} /* mctopo3d_table_t26pp() */

/* ==================================== */
int32_t mctopo3d_table_t26p(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh(img, p, img[p], rs, ps, N);
  return mctopo3d_table_T26(cube_topo3d);
} /* mctopo3d_table_t26p() */

/* ==================================== */
int32_t mctopo3d_table_t26pp_l(
  int32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh_l(img, p, img[p]+1, rs, ps, N);
  return mctopo3d_table_T26(cube_topo3d);
} /* mctopo3d_table_t26pp_l() */

/* ==================================== */
int32_t mctopo3d_table_t6pp_l(
  int32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    return 0;
  preparecubesh_l(img, p, img[p]+1, rs, ps, N);
  return mctopo3d_table_T6(cube_topo3d);
} /* mctopo3d_table_t6pp_l() */

/* ==================================== */
void mctopo3d_table_nbtopoh3d26_l( /* pour les minima en 26-connexite */ 
  int32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *mctopo3d_table_t6p,
  int32_t *mctopo3d_table_t26mm)
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    {
      printf("ERREUR: mctopo3d_table_nbtopoh3d26_l: point de bord\n");
      exit(0);
    }
  preparecubesh_l(img, p, h, rs, ps, N);
  *mctopo3d_table_t6p = mctopo3d_table_T6(cube_topo3d);
  *mctopo3d_table_t26mm = mctopo3d_table_T26(cubec_topo3d);
} /* mctopo3d_table_nbtopoh3d26_l() */

/* ==================================== */
void mctopo3d_table_nbtopoh3d6_l( /* pour les minima en 6-connexite */ 
  int32_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t h,
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  int32_t *mctopo3d_table_t26p,
  int32_t *mctopo3d_table_t6mm)
/* ==================================== */
{
  if ((p < ps) || (p >= N-ps) ||         /* premier ou dernier plan */
      (p%ps < rs) || (p%ps >= ps-rs) ||  /* premiere ou derniere colonne */
      (p%rs == 0) || (p%rs == rs-1))     /* premiere ou derniere ligne */
    {
      printf("ERREUR: mctopo3d_table_nbtopoh3d6_l: point de bord\n");
      exit(0);
    }
  preparecubesh_l(img, p, h, rs, ps, N);
  *mctopo3d_table_t26p = mctopo3d_table_T26(cube_topo3d);
  *mctopo3d_table_t6mm = mctopo3d_table_T6(cubec_topo3d);
} /* mctopo3d_table_nbtopoh3d6_l() */

/* ==================================== */
int32_t mctopo3d_table_bordext6(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
/* teste si x a un 6-voisin a 0 */
{
  int32_t k, y;
  for (k = 0; k <= 10; k += 2) /* parcourt les voisins en 6-connexite */
  {
    y = voisin6(x, k, rs, ps, N);
    if ((y != -1) && (F[y] == 0)) return 1;
  } /* for k */      
  return 0;
} /* mctopo3d_table_bordext6() */

/* ==================================== */
int32_t mctopo3d_table_bordext26(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
/* teste si x a un 26-voisin a 0 */
{
  int32_t k, y;
  for (k = 0; k < 26; k += 1) /* parcourt les voisins en 26-connexite */
  {
    y = voisin26(x, k, rs, ps, N);
    if ((y != -1) && (F[y] == 0)) return 1;
  } /* for k */      
  return 0;
} /* mctopo3d_table_bordext26() */

/* ==================================== */
int32_t mctopo3d_table_curve6( /* point de courbe en 6-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*  ATTENTION: i ne doit pas etre un point de bord (test a faire avant). */
{
  if (img[p] == 0) return 0;
  preparecubes(img, p, rs, ps, N);
  if ((mctopo3d_table_T6(cube_topo3d) == 2) && (mctopo3d_table_nbvoiso6(img, p, rs, ps, N) == 2)) return 1;
  return 0;
} /* mctopo3d_table_curve6() */

/* ==================================== */
int32_t mctopo3d_table_curve18( /* point de courbe en 18-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*  ATTENTION: i ne doit pas etre un point de bord (test a faire avant). */
{
  if (img[p] == 0) return 0;
  preparecubes(img, p, rs, ps, N);
  if ((mctopo3d_table_T18(cube_topo3d) == 2) && (mctopo3d_table_nbvoiso18(img, p, rs, ps, N) == 2)) return 1;
  return 0;
} /* mctopo3d_table_curve18() */

/* ==================================== */
int32_t mctopo3d_table_curve26( /* point de courbe en 26-connexite */
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
/*  ATTENTION: i ne doit pas etre un point de bord (test a faire avant). */
{
  if (img[p] == 0) return 0;
  preparecubes(img, p, rs, ps, N);
  if ((mctopo3d_table_T26(cube_topo3d) == 2) && (mctopo3d_table_nbvoiso26(img, p, rs, ps, N) == 2)) return 1;
  return 0;
} /* mctopo3d_table_curve26() */
