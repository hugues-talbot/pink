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
/*! \file project_contact.c

\brief xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

<B>Usage:</B> xxxxxxxxxxxxxxxxxxxxxxxxxx

<B>Description:</B>
A ECRIRE




<B>Category:</B> geo
\ingroup  geo

\author Paulin Sanselme
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo3d.h>
#include <lcurves.h>
#include <math.h>
#include <assert.h>
#include <mcutil.h>
#include <mclifo.h>
#include <mcsplines.h>

#define VISU_PROJECTION

struct contact{
	int32_t contact;
	int32_t f1;
	int32_t f2;
	int32_t t1;
	int32_t t2;
	int32_t i11;
	int32_t i12;
	int32_t i21;
	int32_t i22;
	struct contact *next;
};

struct contact_splines{
	int32_t id_contact;
	int32_t id_f1;
	double d1;
	double f1;
	int32_t id_f2;
	double d2;
	double f2;
	struct contact_splines *next;
};

struct resultat{
	int32_t Ncontacts;
	struct contact_splines * liste_contact;
        int32_t * n_ctrl;
        double ** coef_splines;
};

/* ===================================================== */
void extract_fibres(
/* ===================================================== */
	struct xvimage * imlab,			// input : image d'entree labélisée
	int32_t Nfib,				// input : nombre de fibres
	int32_t connex,				// input : connexité

        int32_t *npoints,			// output : sert à mémoriser le nombre de points de chaques fibres
        int32_t **X,				// output : coordonnées des points de chaques fibres
        int32_t **Y,
        int32_t **Z
        // npoints est un tableau de Nfib int32_t
        // X, Y et Z sont des tableaux de Nfib int32_t*
)
{
  int32_t i;

  int32_t rs = rowsize(imlab), ps = rs * colsize(imlab), N= ps * depth(imlab);
  int32_t *data = SLONGDATA(imlab);

  // extractions des fibres
  // parcours de l'image pour trouver une extrémité
  for(i=0;i<N;i++)
  {
    if ( (data[i]>0) && (data[i]<Nfib+1) && (X[data[i]-1]==NULL) && is_endlab(i, data, rs, ps, N, connex) )
          // extraction de la fibre correspondante
	  lcurves_extractcurve3dlab( data, i, rs, ps, N, connex, &X[data[i]-1], &Y[data[i]-1], &Z[data[i]-1], &npoints[data[i]-1]);
  }

} // extract_fibres()

/* ===================================================== */
void surface_to_contour(
/* ===================================================== */
	struct xvimage * image,			// input : image
	int32_t *s,				// input : points de la surface
	int32_t *n,				// input : nombre de point de la surface ; output : nombre de points du contour
	int32_t *c				// output : points du contour (initialisé)
)
{
  int32_t rs = rowsize(image), ps = rs*colsize(image), N = ps*depth(image);
  int32_t i,nc=0,t,tb;

  for(i=0;i<*n;i++) {
    // on cherche les points de contours (t=tb=1)
//mctopo3d_init_topo3d();
    mctopo3d_top26lab( SLONGDATA(image), s[i], rs, ps, N, &t, &tb); // calcul de t et tb
//mctopo3d_termine_topo3d();
    if(t==1 && tb==1) {
      c[nc]=s[i];
      nc++;
    }
  }

  *n = nc;
} // surface_to_contour()

/* ===================================================== */
void project_contour_on_curve(
/* ===================================================== */
  int32_t n_contour, // le contour de la surface à projeter
  int32_t *Xcontour,
  int32_t *Ycontour,
  int32_t *Zcontour,
  int32_t n_courbe, // la courbe sur laquelle on projette
  int32_t *Xcourbe,
  int32_t *Ycourbe,
  int32_t *Zcourbe,
  int32_t *nmax, // les points du projeté
  int32_t *X,
  int32_t *Y,
  int32_t *Z
  )
{
  int32_t i, j, argmin = n_courbe-1, argmax = 0, arg;
  double distmin, disttmp;

  for(i=0;i<n_contour;i++)
  {
    distmin = pow(Xcontour[i]*Xcourbe[0]+Ycontour[i]*Ycourbe[0]+Zcontour[i]*Zcourbe[0],0.5);
    arg = 0;
    for(j=1;j<n_courbe;j++)
    {
      disttmp = pow(Xcontour[i]*Xcourbe[j]+Ycontour[i]*Ycourbe[j]+Zcontour[i]*Zcourbe[j],0.5);
      if (disttmp<distmin) { distmin = disttmp; arg = j; }
    }
    argmin = mcmin(argmin,arg);
    argmax = mcmax(argmax,arg);
  } // fin for i,j

  assert(*nmax>argmax-argmin+1);
  for(j=argmin;j<argmax+1;j++)
  {
    i = j-argmin;
    X[i]=Xcourbe[j];
    Y[i]=Ycourbe[j];
    Z[i]=Zcourbe[j];
  }
  *nmax=argmax-argmin+1;
} // project_contour_on_curve()

/* ===================================================== */
void project_contour_on_curve2(
/* ===================================================== */
  int32_t n_contour, // le contour de la surface à projeter
  int32_t *Xcontour,
  int32_t *Ycontour,
  int32_t *Zcontour,
  int32_t n_courbe, // la courbe sur laquelle on projette
  int32_t *Xcourbe,
  int32_t *Ycourbe,
  int32_t *Zcourbe,
  int32_t *n1,			// indices du projeté
  int32_t *n2
  )
{
assert(n_contour>0);
assert(n_courbe>0);

  int32_t i, j, argmin = n_courbe-1, argmax = 0, arg;
  double distmin, disttmp;

  for(i=0;i<n_contour;i++)
  {
    distmin = (Xcontour[i]-Xcourbe[0])*(Xcontour[i]-Xcourbe[0])+(Ycontour[i]-Ycourbe[0])*(Ycontour[i]-Ycourbe[0])+(Zcontour[i]-Zcourbe[0])*(Zcontour[i]-Zcourbe[0]);
    arg = 0;
    for(j=1;j<n_courbe;j++)
    {
      disttmp = (Xcontour[i]-Xcourbe[j])*(Xcontour[i]-Xcourbe[j])+(Ycontour[i]-Ycourbe[j])*(Ycontour[i]-Ycourbe[j])+(Zcontour[i]-Zcourbe[j])*(Zcontour[i]-Zcourbe[j]);
      if (disttmp<distmin) { distmin = disttmp; arg = j; }
    }
    argmin = mcmin(argmin,arg);
    argmax = mcmax(argmax,arg);
  } // fin for i,j
//fprintf(stderr,"project_on_curve2 : 0 < %d < %d < %d\n",argmin,argmax,n_courbe-1);
  *n1 = argmin;
  *n2 = argmax;
} // project_contour_on_curve2()


/* ===================================================== */
struct resultat* project_all_surfaces_on_fibres(
/* ===================================================== */
	struct xvimage * imlab,			// input : image d'entree labélisée
	int32_t Nfib,				// input : nombre de fibres
	int32_t connex,				// input : connexité
        double deltamax,			// input : tolérance pour les splines 
        double p,				// input : precision pour scn_splinequerypoint3d()

	struct xvimage ** pimvisu		// output : pointeur image pour visualiser les projection (mode VISU_PROJECTION)
){

#undef F_NAME
#define F_NAME "project_all_surfaces_on_fibres"

  int32_t i, j, x, v, f1, f2, i11, i12, i21, i22, k, npointsmax=0;
  double *d = NULL;
  int32_t rs = rowsize(imlab);         /* taille ligne */
  int32_t cs = colsize(imlab);         /* taille colonne */
  int32_t ds = depth(imlab);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t * data = SLONGDATA(imlab);
  int32_t label, s[500], n=0;
//  int32_t c[500];

  Lifo * LIFO=CreeLifoVide(500);
  struct contact *list_contact = NULL;
  struct contact *contact_temp = NULL;
  struct contact_splines *list_contact_splines = NULL;
  struct contact_splines *contact_splines_temp = NULL;

  struct resultat *result = (struct resultat*)malloc(sizeof(struct resultat));

#ifdef VISU_PROJECTION
    *pimvisu = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    razimage(*pimvisu);
    uint8_t * visu  = UCHARDATA(*pimvisu);
#endif

// on extrait les fibres
  int32_t * npoints = (int32_t*)malloc(Nfib*sizeof(int32_t));
  int32_t **X, **Y, **Z;
  X = (int32_t**)malloc(sizeof(int32_t*)*Nfib);
  Y = (int32_t**)malloc(sizeof(int32_t*)*Nfib);
  Z = (int32_t**)malloc(sizeof(int32_t*)*Nfib);

  for(i=0;i<Nfib;i++){
    npoints[i]=0;
    X[i]=NULL; Y[i]=NULL; Z[i]=NULL;
  }

  extract_fibres(imlab, Nfib, connex, npoints, X, Y, Z);

  for(i=0;i<Nfib;i++){ npointsmax = mcmax(npointsmax,npoints[i]); }

// tant que l'on a pas fini
  // on parcours l'image à la recherche d'un point d'une surface non traitée
  for(i=0;i<N;i++)
  {
    if(data[i]>Nfib)
    {
    // on extrait la surface correspondante de l'image (et on l'efface)
      label = data[i];
      s[n] = i;
      n++;
      data[i] = 0;
      LifoPush(LIFO, i);   /* on va parcourir la composante a laquelle appartient i */

      while (! LifoVide(LIFO))
      {
	assert(n<500); // ne pas dépasser la mémoire allouée
        x = LifoPop(LIFO);
	switch (connex)
	{
	case 4:
	  for (k = 0; k < 8; k += 2)
	  {
	    v = voisin(x, k, rs, N);
	    if ((v != -1)  && (data[v]==label))
	    {
	      s[n] = v;
	      n++;
	      data[v] = 0;
	      LifoPush(LIFO, v);
	    }
	  }
	  break;
	case 8: 
	  for (k = 0; k < 8; k += 1)
	  {
	    v = voisin(x, k, rs, N);
	    if ((v != -1)  && (data[v]==label))
	    {
	      s[n] = v;
	      n++;
	      data[v] = 0;
	      LifoPush(LIFO, v);
	    }
	  }
	  break;
	case 6: 
	  for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
	  {
            v = voisin6(x, k, rs, ps, N);
	    if ((v != -1)  && (data[v]==label))
	    {
	      s[n] = v;
	      n++;
	      data[v] = 0;
	      LifoPush(LIFO, v);
	    }
	  }
	  break;
	case 18:
	  for (k = 0; k < 18; k += 1) /* parcourt les 18 voisins */
	  {
            v = voisin18(x, k, rs, ps, N);
	    if ((v != -1)  && (data[v]==label))
	    {
	      s[n] = v;
	      n++;
	      data[v] = 0;
	      LifoPush(LIFO, v);
	    }
	  }
	  break;
	case 26:
	  for (k = 0; k < 26; k += 1) /* parcourt les 26 voisins */
	  {
            v = voisin26(x, k, rs, ps, N);
	    if ((v != -1)  && (data[v]==label))
	    {
	      s[n] = v;
	      n++;
	      data[v] = 0;
	      LifoPush(LIFO, v);
	    }
	  }
	  break;
	default:
	  fprintf(stderr, "%s : %d : bad connexity\n", F_NAME, connex);
	  return(0);
	} // switch (connex)
      } // while(! LifoVide(LIFO))

    // on extrait le contour de cette surface
//      surface_to_contour( imlab, s, &n, c);

	int32_t Xcontour[n], Ycontour[n], Zcontour[n];
// changer s[x] en c[x] pour traivailler avec les contours
	for(x=0;x<n;x++){
	  Xcontour[x]=s[x]%rs;
	  Ycontour[x]=(s[x]%ps)/rs;
	  Zcontour[x]=s[x]/ps;
      }


    // on projete ce contour sur chacunes des fibres correspondantes
	f1 = label%(Nfib+1);
	f2 = label/(Nfib+1);
//fprintf(stderr,"label=%d ; Nfib=%d ; f1=%d ; f2=%d\n",label,Nfib,f1,f2);

	project_contour_on_curve2( n, Xcontour, Ycontour, Zcontour, npoints[f1-1], X[f1-1], Y[f1-1], Z[f1-1], &i11, &i12);
	project_contour_on_curve2( n, Xcontour, Ycontour, Zcontour, npoints[f2-1], X[f2-1], Y[f2-1], Z[f2-1], &i21, &i22);

#ifdef VISU_PROJECTION
    // on recopie le resultat dans l'image de visualisation
      for(x=i11;x<=i12;x++) visu[X[f1-1][x]+Y[f1-1][x]*rs+Z[f1-1][x]*ps]=255;
      for(x=i21;x<=i22;x++) visu[X[f2-1][x]+Y[f2-1][x]*rs+Z[f2-1][x]*ps]=255;
#endif

    // on stock le contact
	contact_temp = (struct contact*)malloc(sizeof(struct contact));
	contact_temp->contact = label;
	contact_temp->f1 = f1;
	contact_temp->f2 = f2;
//	contact_temp->t1 = X[f1-1][0]+Y[f1-1][0]*rs+Z[f1-1][0]*ps;
//	contact_temp->t2 = X[f2-1][0]+Y[f2-1][0]*rs+Z[f2-1][0]*ps;
	contact_temp->i11 = i11;
	contact_temp->i12 = i12;
	contact_temp->i21 = i21;
	contact_temp->i22 = i22;
	contact_temp->next = list_contact;
	list_contact = contact_temp;

    }  // if(data[i]>Nfib)
    n=0;
  }  // for i
  LifoTermine(LIFO);

  // on trouve l'équation des splines des fibres
  int32_t *n_ctrl=(int32_t*)malloc(Nfib*sizeof(int32_t));
  int32_t *C=(int32_t*)malloc(npointsmax*sizeof(int32_t));
  double *C0=(double*)malloc(npointsmax*sizeof(double));
  double *C1=(double*)malloc(npointsmax*sizeof(double));
  double *C2=(double*)malloc(npointsmax*sizeof(double));
  double *C3=(double*)malloc(npointsmax*sizeof(double));
  double *D0=(double*)malloc(npointsmax*sizeof(double));
  double *D1=(double*)malloc(npointsmax*sizeof(double));
  double *D2=(double*)malloc(npointsmax*sizeof(double));
  double *D3=(double*)malloc(npointsmax*sizeof(double));
  double *E0=(double*)malloc(npointsmax*sizeof(double));
  double *E1=(double*)malloc(npointsmax*sizeof(double));
  double *E2=(double*)malloc(npointsmax*sizeof(double));
  double *E3=(double*)malloc(npointsmax*sizeof(double));

  double **coef_splines =(double**)malloc(Nfib*sizeof(double*));

  for(i=0;i<Nfib;i++) {
    scn_approxcurve3d( X[i], Y[i], Z[i], npoints[i]-2, deltamax, C, n_ctrl+i, C0, C1, C2, C3, D0, D1, D2, D3, E0, E1, E2, E3);
    coef_splines[i] =(double*)malloc((n_ctrl[i]-1)*12*sizeof(double));
    for(j=0;j<n_ctrl[i]-1;j++){
      d = coef_splines[i];
      d[j] = C0[j];
      d[(n_ctrl[i]-1)+j] = C1[j];
      d[2*(n_ctrl[i]-1)+j] = C2[j];
      d[3*(n_ctrl[i]-1)+j] = C3[j];
      d[4*(n_ctrl[i]-1)+j] = D0[j];
      d[5*(n_ctrl[i]-1)+j] = D1[j];
      d[6*(n_ctrl[i]-1)+j] = D2[j];
      d[7*(n_ctrl[i]-1)+j] = D3[j];
      d[8*(n_ctrl[i]-1)+j] = E0[j];
      d[9*(n_ctrl[i]-1)+j] = E1[j];
      d[10*(n_ctrl[i]-1)+j] = E2[j];
      d[11*(n_ctrl[i]-1)+j] = E3[j];
    } // for j

  } // for i

  // on trouve les indices des contacts
  int32_t Ncontacts = 0;
  for(contact_temp=list_contact;contact_temp!=NULL;contact_temp=contact_temp->next)
  {
    Ncontacts++;
    label = contact_temp->contact;
    f1 = contact_temp->f1;
    f2 = contact_temp->f2;
    i11 = contact_temp->i11;
    i12 = contact_temp->i12;
    i21 = contact_temp->i21;
    i22 = contact_temp->i22;

    contact_splines_temp = (struct contact_splines*)malloc(sizeof(struct contact_splines));
    contact_splines_temp->id_contact = label;
    contact_splines_temp->id_f1 = f1;

    contact_splines_temp->d1 = scn_splinequerypoint3d( X[f1-1][i11], Y[f1-1][i11], Z[f1-1][i11], p, n_ctrl[f1-1], 
			coef_splines[f1-1], coef_splines[f1-1]+n_ctrl[f1-1]-1, coef_splines[f1-1]+2*(n_ctrl[f1-1]-1), coef_splines[f1-1]+3*(n_ctrl[f1-1]-1),
			coef_splines[f1-1]+4*(n_ctrl[f1-1]-1), coef_splines[f1-1]+5*(n_ctrl[f1-1]-1), coef_splines[f1-1]+6*(n_ctrl[f1-1]-1), coef_splines[f1-1]+7*(n_ctrl[f1-1]-1),
			coef_splines[f1-1]+8*(n_ctrl[f1-1]-1), coef_splines[f1-1]+9*(n_ctrl[f1-1]-1), coef_splines[f1-1]+10*(n_ctrl[f1-1]-1), coef_splines[f1-1]+11*(n_ctrl[f1-1]-1));

    contact_splines_temp->f1 = scn_splinequerypoint3d( X[f1-1][i12], Y[f1-1][i12], Z[f1-1][i12], p, n_ctrl[f1-1], 
			coef_splines[f1-1], coef_splines[f1-1]+n_ctrl[f1-1]-1, coef_splines[f1-1]+2*(n_ctrl[f1-1]-1), coef_splines[f1-1]+3*(n_ctrl[f1-1]-1),
			coef_splines[f1-1]+4*(n_ctrl[f1-1]-1), coef_splines[f1-1]+5*(n_ctrl[f1-1]-1), coef_splines[f1-1]+6*(n_ctrl[f1-1]-1), coef_splines[f1-1]+7*(n_ctrl[f1-1]-1),
			coef_splines[f1-1]+8*(n_ctrl[f1-1]-1), coef_splines[f1-1]+9*(n_ctrl[f1-1]-1), coef_splines[f1-1]+10*(n_ctrl[f1-1]-1), coef_splines[f1-1]+11*(n_ctrl[f1-1]-1));

    contact_splines_temp->id_f2 = f2;

    contact_splines_temp->d2 = scn_splinequerypoint3d( X[f2-1][i21], Y[f2-1][i21], Z[f2-1][i21], p, n_ctrl[f2-1], 
			coef_splines[f2-1], coef_splines[f2-1]+n_ctrl[f2-1]-1, coef_splines[f2-1]+2*(n_ctrl[f2-1]-1), coef_splines[f2-1]+3*(n_ctrl[f2-1]-1),
			coef_splines[f2-1]+4*(n_ctrl[f2-1]-1), coef_splines[f2-1]+5*(n_ctrl[f2-1]-1), coef_splines[f2-1]+6*(n_ctrl[f2-1]-1), coef_splines[f2-1]+7*(n_ctrl[f2-1]-1),
			coef_splines[f2-1]+8*(n_ctrl[f2-1]-1), coef_splines[f2-1]+9*(n_ctrl[f2-1]-1), coef_splines[f2-1]+10*(n_ctrl[f2-1]-1), coef_splines[f2-1]+11*(n_ctrl[f2-1]-1));

    contact_splines_temp->f2 = scn_splinequerypoint3d( X[f2-1][i22], Y[f2-1][i22], Z[f2-1][i22], p, n_ctrl[f2-1], 
			coef_splines[f2-1], coef_splines[f2-1]+n_ctrl[f2-1]-1, coef_splines[f2-1]+2*(n_ctrl[f2-1]-1), coef_splines[f2-1]+3*(n_ctrl[f2-1]-1),
			coef_splines[f2-1]+4*(n_ctrl[f2-1]-1), coef_splines[f2-1]+5*(n_ctrl[f2-1]-1), coef_splines[f2-1]+6*(n_ctrl[f2-1]-1), coef_splines[f2-1]+7*(n_ctrl[f2-1]-1),
			coef_splines[f2-1]+8*(n_ctrl[f2-1]-1), coef_splines[f2-1]+9*(n_ctrl[f2-1]-1), coef_splines[f2-1]+10*(n_ctrl[f2-1]-1), coef_splines[f2-1]+11*(n_ctrl[f2-1]-1));

    contact_splines_temp->next=list_contact_splines;
    list_contact_splines=contact_splines_temp;

  }

  // retourne les resultats
  result->Ncontacts=Ncontacts;
  result->liste_contact = list_contact_splines;
  result->n_ctrl = n_ctrl;
  result->coef_splines = coef_splines;

  return result;

} // project_all_surfaces_on_fibres()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imlab = readimage(argv[1]);
  int32_t connex = 26;
  int32_t Nfib = atoi(argv[2]);				
  double deltamax = atof(argv[3]);
  double p = atof(argv[4]);
  double *d;
  struct xvimage * imvisu=NULL;
  struct contact_splines *contacts = NULL;

  struct resultat* result = project_all_surfaces_on_fibres( imlab, Nfib, connex, deltamax, p, &imvisu);

  FILE * fd = fopen("out.txt","w");

  fprintf(fd,"fibres %d\n",Nfib);

  int32_t i, j, n;
  for(i=0;i<Nfib;i++){
    n = result->n_ctrl[i];
    fprintf(fd,"F %d %d %d\n",i+1,i+1,n);
    for(j=0;j<(n-1);j++){
      d = result->coef_splines[i];
      fprintf(fd,"%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",d[j],d[j+(n-1)],d[j+2*(n-1)],d[j+3*(n-1)],d[j+4*(n-1)],d[j+5*(n-1)],d[j+6*(n-1)],d[j+7*(n-1)],d[j+8*(n-1)],d[j+9*(n-1)],d[j+10*(n-1)],d[j+11*(n-1)]);
    }
  }

  fprintf(fd,"contacts %d\n",result->Ncontacts);

  for(contacts=result->liste_contact;contacts!=NULL;contacts=contacts->next){
    fprintf(fd,"C %d %d %.4f %.4f %d %.4f %.4f\n",contacts->id_contact,contacts->id_f1,contacts->d1,contacts->f1,contacts->id_f2,contacts->d2,contacts->f2);
  }

  fclose(fd);

#ifdef VISU_PROJECTION
  writeimage(imvisu, argv[argc-1]); 
#endif
  freeimage(imvisu);

  return 0;
} // main()

