/* $Id: lsquelval.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t lsquelval(
  struct xvimage *image, // entree/sortie: image originale / squelette
  struct xvimage *dx,    // entree/sortie: distance / distance topologique
  int32_t connex, 
  int32_t val_inhibit);

extern int32_t lsquelval3d(
  struct xvimage *image, // entree/sortie: image originale / squelette
  struct xvimage *dx,    // entree/sortie: distance / distance topologique
  int32_t connex, 
  int32_t val_inhibit);

extern int32_t lsquelsmoothval(
  struct xvimage *image, // entree/sortie: image originale / squelette
  struct xvimage *dx,    // entree/sortie: distance / distance topologique
  struct xvimage *ni,    // entree/sortie: niveaux - image 1D 
  struct xvimage *gr,    // entree: gradient
  int32_t connex, 
  int32_t val_inhibit,
  int32_t rayon);
