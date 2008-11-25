/* $Id: jclderiche.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototype for lderiche.c */
/* ============== */

extern int32_t lderiche(
        struct xvimage *image1,
        double alpha,
        int32_t function,
        double l
);

extern void derichegen(double *x,               /* image a traiter */
                int32_t M,                   /* taille ligne */
                int32_t N,                   /* taille colonne */
                double *y1,              /* zone temporaire de la taille d'une colonne */
                double *y2,              /* zone temporaire de la taille d'une ligne */
                double *y,               /* stocke un resultat temporaire, puis le resultat final */ 
                double a1, double a2, double a3, double a4, 
                double a5, double a6, double a7, double a8, 
                double b1, double b2, double b3, double b4);

