/* $Id: jcimage.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototypes for jcimage.c    */
/* ============== */

// ! jcimage s'ajoute mcimage!
// Les deux deux fichiers doivent etre utilisés conjointement

extern struct xvimage4D *allocimage4D(int32_t ss);
extern struct xvimage *allocGAimage(char * name, int32_t rs, int32_t cs, int32_t d, int32_t t);
extern void writerawGAimage(struct xvimage * image, char *filename);
extern struct xvimage * readGAimage(char *filename);
extern struct xvimage4D *readimage4D(char *prefix, int32_t first, int32_t last);     
extern struct xvimage4D *freeimage4D(struct xvimage4D  *im);
extern struct GA4d * readGA4d(char *filename);
extern struct GA4d * allocGA4d(char * name, int32_t rs, int32_t cs, int32_t d, int32_t ss);
extern void writeGA4d(struct GA4d * image, char *filename);
extern struct GA4d *freeGA4d(struct GA4d  *im);
void writeimage4D(struct xvimage4D * image, char *prefix, int32_t first, int32_t last);
