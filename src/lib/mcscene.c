#include <stddef.h>
#include <stdio.h>

#include "mcscn.h"
// Functions for the management of 3D scenes
// Michel Couprie - Juillet 2004

/* =============================================================== */
void write3Dscene(scene3d *S)
/* =============================================================== */
#undef F_NAME
#define F_NAME "write3Dscene"
{
} // write3Dscene()

/* =============================================================== */
scene3d *init3Dscene(int32_t nobj)
/* =============================================================== */
#undef F_NAME
#define F_NAME "init3Dscene"
{
  scene3d *S = (scene3d *)calloc(1,sizeof(scene3d));
  if (S == NULL)
  {   
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return NULL;
  }
  S->nobj = nobj;
  S->objs = (scene_obj *)calloc(1,nobj * sizeof(scene_obj));
  if (S->objs == NULL)
  {   
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return NULL;
  }
  return S;
} // init3Dscene()

/* =============================================================== */
scene3d *read3Dscene(char *filename)
/* =============================================================== */
#undef F_NAME
#define F_NAME "read3Dscene"
{
  scene3d *S;

  return S;
} // read3Dscene()
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
