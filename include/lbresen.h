/* $Id: lbresen.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#include "mcliste.h"
typedef struct {
  int32_t rs, cs;
  int32_t *xmin;
  int32_t *xmax;
} ellipse;

#define InEllipse(ell,x,y) (((x)>=(ell)->xmin[(y)])&&((x)<=(ell)->xmax[(y)]))

extern void      lbresen(uint8_t *I, int32_t rs, int32_t Ax, int32_t Ay, int32_t Bx, int32_t By);
extern void      lbresenlist(int32_t rs, int32_t Ax, int32_t Ay, int32_t Bx, int32_t By, int32_t *lp, int32_t *n);
extern ellipse * AllocEllipseList(int32_t rs, int32_t cs);
extern void      InitEllipseList(ellipse * ell);
extern void      FreeEllipseList(ellipse *ell);
extern void      lconic(uint8_t *I, int32_t rs, int32_t cs, int32_t xs, int32_t ys, int32_t xe, int32_t ye,
                      int32_t A, int32_t B, int32_t C, int32_t D, int32_t E, int32_t F, int32_t xoffset, int32_t yoffset);
extern void      lellipsearc(uint8_t *I, int32_t rs, int32_t cs,
                      int32_t xp, int32_t yp, int32_t xq, int32_t yq, int32_t xoffset, int32_t yoffset);
extern void      lellipse(uint8_t *I, int32_t rs, int32_t cs, 
                      int32_t xp, int32_t yp, int32_t xq, int32_t yq, int32_t xoffset, int32_t yoffset);
extern void      lellipsearclist(ellipse *ell, int32_t rs, int32_t cs,
                      int32_t xp, int32_t yp, int32_t xq, int32_t yq, int32_t xoffset, int32_t yoffset);
extern void      lellipselist(ellipse *ell, int32_t rs, int32_t cs, 
                      int32_t xp, int32_t yp, int32_t xq, int32_t yq, int32_t xoffset, int32_t yoffset);
extern void lplotfilledellipse(ellipse *ell, uint8_t *I);
extern void lellipsearcliste(Liste *lp, int32_t rs, int32_t cs, int32_t xp, int32_t yp, int32_t xq, int32_t yq, 
			     int32_t xoffset, int32_t yoffset);
extern Liste *lellipseliste(int32_t rs, int32_t cs, int32_t xp, int32_t yp, int32_t xq, int32_t yq, 
			    int32_t xoffset, int32_t yoffset);
extern void lellipseliste2(Liste *lp, int32_t rs, int32_t cs, int32_t xp, int32_t yp, int32_t xq, int32_t yq, 
			    int32_t xoffset, int32_t yoffset);
extern void lplotellipseliste(Liste *lp, uint8_t *I);
