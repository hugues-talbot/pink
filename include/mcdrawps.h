/* $Id: mcdrawps.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern void EPSHeader(FILE *fd, double figure_width, double figure_height, double line_width, int font_size );
extern void PSHeader(FILE *fd, double figure_width, double figure_height, double line_width, int font_size );
extern void PSMove (FILE *fd, double x, double y) ;
extern void PSDraw (FILE *fd, double x, double y);
extern void PSLine (FILE *fd, double xd, double yd, double xf, double yf);
extern void PSDrawcircle (FILE *fd, double x, double y, double r);
extern void PSDrawdisc (FILE *fd, double x, double y, double r);
extern void PSString (FILE *fd, double x, double y, char *s);
extern void PSFooter(FILE *fd);
#ifdef __cplusplus
}
#endif
