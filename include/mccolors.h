/* $Id: mccolors.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define CN_RED     "red"
#define CN_YELLOW  "yellow"
#define CN_GREEN   "green"
#define CN_CYAN    "cyan"
#define CN_BLUE    "blue"
#define CN_MAGENTA "magenta"

#define CN_ROUGE   "rouge"
#define CN_JAUNE   "jaune"
#define CN_VERT    "vert"
#define CN_BLEU    "bleu"

extern void rgb2hls(double r, double g, double b, double *h, double *l, double *s);
extern void hls2rgb(double *r, double *g, double *b, double h, double l, double s);
extern char * colorname(double h);
extern char * nomcouleur(double h);
extern int32_t huedistance(int32_t h1, int32_t h2);
int32_t circulardiff(int32_t h1, int32_t h2);
