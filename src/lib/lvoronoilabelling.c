/** sedtVoronoi : SEDT with Voronoi diagram computation in linear time
 * 
 * David Coeurjolly (david.coeurjolly@liris.cnrs.fr) - Feb. 2005
 *
 * Adapted to Pink by Michel Couprie - Dec. 2008
 *
**/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include "mccodimage.h"
#include "mcimage.h"
#include "lvoronoilabelling.h"
#define INFTY INT_MAX


/* ==================================== */
inline double dist_2(double x1, double y1, double x2, double y2)
/* ==================================== */
{
  return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

/////////Basic functions to handle operations with INFTY

/** 
 **************************************************
 * @b sum
 * @param a Long number with INFTY
 * @param b Long number with INFTY
 * @return The sum of a and b handling INFTY
 **************************************************/
int32_t sum(int32_t a, int32_t b) 
{
  if ((a==INFTY) || (b==INFTY))     
    return INFTY;    
  else 
    return a+b;
}

/** 
 **************************************************
 * @b prod
 * @param a Long number with INFTY
 * @param b Long number with INFTY
 * @return The product of a and b handling INFTY
 **************************************************/
int32_t prod(int32_t a, int32_t b) 
{
  if ((a==INFTY) || (b==INFTY)) 
    return INFTY;  
  else 
    return a*b;
}
/** 
 **************************************************
 * @b opp
 * @param a Long number with INFTY
 * @return The opposite of a  handling INFTY
 **************************************************/
int32_t opp (int32_t a) {
  if (a == INFTY) {
    return INFTY;
  }
  else {
    return -a;
  }
}

/** 
 **************************************************
 * @b intdivint
 * @param divid Long number with INFTY
 * @param divis Long number with INFTY
 * @return The division (integer) of divid out of divis handling INFTY
 **************************************************/
int32_t intdivint (int32_t divid, int32_t divis) {
  if (divis == 0) 
    return  INFTY;
  if (divid == INFTY) 
    return  INFTY;
  else 
    return  divid / divis;
}


////////// Functions F and Sep for the SDT labelling
/** 
 **************************************************
 * @b F
 * @param x 
 * @param i 
 * @param gi2 
 * @return Definition of a parabola
 **************************************************/
inline int32_t F(int32_t x, int32_t i, int32_t gi2)
{
  return sum((x-i)*(x-i), gi2);
}

/** 
 **************************************************
 * @b Sep
 * @param i 
 * @param u 
 * @param gi2 
 * @param gu2 
 * @return The abscissa of the intersection point between two parabolas
 **************************************************/
inline int32_t Sep(int32_t i, int32_t u, int32_t gi2, int32_t gu2) {
  return intdivint(sum( sum((int32_t) (u*u - i*i),gu2), opp(gi2) ), 2*(u-i));
}
/////////


/** 
 **************************************************
 * @b phaseVoronoiX
 * @param V Input volume
 * @param sdt_x SDT along the x-direction
 * @param dx the first component of the Voronoi labeling
 **************************************************/
//First step of  the saito  algorithm 
// (Warning   : we  store the  EDT instead of the SDT)
void phaseVoronoiX(struct xvimage *img, struct xvimage *sdt_x, struct xvimage *dx)
{
  int32_t x,y,z;
  for (z = 0; z < depth(img) ; z++) 	    
    for (y = 0; y < colsize(img) ; y++) 
      {
	if (voxel(img,0,y,z) == 0) 
	  {
	    lvoxel(sdt_x,0,y,z)=0;
	    lvoxel(dx,0,y,z)=0;
	  }
	else 	    
	  lvoxel(sdt_x,0,y,z)=INFTY;  
	  
	// Forward scan
	for (x = 1; x < rowsize(img) ; x++) 	    
	  if (voxel(img,x,y,z) == 0)
	    {
	      lvoxel(sdt_x,x,y,z)=0;
	      lvoxel(dx,x,y,z)=x;
	    }
	  else 
	    {
	      lvoxel(sdt_x,x,y,z)=sum(1, lvoxel(sdt_x,(x-1),y,z));;	  
	      lvoxel(dx,x,y,z)=lvoxel(dx,(x-1),y,z);
	    }
	//Backward scan
	for (x = rowsize(img) -2; x >= 0; x--)      
	  if (lvoxel(sdt_x,(x+1),y,z) < lvoxel(sdt_x,x,y,z)) 
	    {
	      lvoxel(sdt_x,x,y,z)=sum(1, lvoxel(sdt_x,(x+1),y,z));

	      //Voronoi labeling
	      lvoxel(dx,x,y,z)=lvoxel(dx,(x+1),y,z);
	    }
      }
}

/** 
 **************************************************
 * @b phaseVoronoiY
 * @param sdt_x the SDT along the x-direction
 * @param sdt_xy the SDT in the xy-slices
 * @param dx the first component of the Voronoi labeling
 * @param dy the second component of the Voronoi labeling
**************************************************/
//Second      Step   of    the       saito   algorithm    using    the
//[Meijster/Roerdnik/Hesselink] optimization
void phaseVoronoiY(struct xvimage *sdt_x, struct xvimage *sdt_xy, struct xvimage *dx, struct xvimage *dy)
#undef F_NAME
#define F_NAME "phaseVoronoiY"
{
  
  int32_t *s; //Center of the upper envelope parabolas
  int32_t *t; //Separating index between 2 upper envelope parabolas 
  int32_t *dxTemp; //Temp array to backup the Voronoi labeling 
  int32_t q; 
  int32_t w;
  int32_t x, y, z, u;  

  s = (int32_t *)malloc(colsize(sdt_x)*sizeof(int32_t));
  t = (int32_t *)malloc(colsize(sdt_x)*sizeof(int32_t));
  dxTemp = (int32_t *)malloc(colsize(sdt_x)*sizeof(int32_t));
  if ((s == NULL) || (t == NULL) || (dxTemp == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);
  }

  for (z = 0; z < depth(sdt_x); z++) 	    
    for (x = 0; x < rowsize(sdt_x); x++) 
      {
	q=0;
	s[0] = 0;
	t[0] = 0;
	
	dxTemp[0]=lvoxel(dx,x,0,z);


	//Forward Scan
	for (u=1; u < colsize(sdt_x) ; u++) 
	  {
	    while ((q >= 0) &&
		   (F(t[q],s[q],prod(lvoxel(sdt_x,x,s[q],z),lvoxel(sdt_x,x,s[q],z))) > 
		    F(t[q],u,prod(lvoxel(sdt_x,x,u,z),lvoxel(sdt_x,x,u,z))))
		   ) 
	      q--;
	    
	    if (q<0) 
	      {
		q=0;
		s[0]=u;
	      }
	    else 
	      {
		w = 1 + Sep(s[q],
			    u,
			    prod(lvoxel(sdt_x,x,s[q],z),lvoxel(sdt_x,x,s[q],z)),
			    prod(lvoxel(sdt_x,x,u,z),lvoxel(sdt_x,x,u,z)));
	
		if (w < colsize(sdt_x)) 
		  {
		    q++;
		    s[q]=u;
		    t[q]=w;
		  }
	      }

	    dxTemp[u]=lvoxel(dx,x,u,z);
	  }
	
	//Backward Scan
	for (u = colsize(sdt_x)-1; u >= 0; --u) 
	  {
	    lvoxel(sdt_xy,x,u,z) = F(u,s[q],prod(lvoxel(sdt_x,x,s[q],z),lvoxel(sdt_x,x,s[q],z)));	      
	    
	    //Tricky point of the Voronoi labeling
	    lvoxel(dx,x,u,z) =dxTemp[s[q]];
	    lvoxel(dy,x,u,z) = s[q];
	 

	    if (u==t[q]) 
	      q--;
	  }
      }
  free(s);
  free(t);
  free(dxTemp);
}

/** 
 **************************************************
 * @b phaseVoronoiZ
 * @param sdt_xy the SDT in the xy-slices
 * @param sdt_xyz the final SDT
 * @param dx the first component of the Voronoi labeling
 * @param dy the second component of the Voronoi labeling
 * @param dy the third component of the Voronoi labeling
 **************************************************/
//Third   Step      of     the    saito   algorithm     using      the
//[Meijster/Roerdnik/Hesselink] optimization
void phaseVoronoiZ(struct xvimage *sdt_xy, struct xvimage *sdt_xyz, struct xvimage *dx, struct xvimage *dy, struct xvimage *dz)
#undef F_NAME
#define F_NAME "phaseVoronoiZ"
{  
  int32_t *s; //Center of the upper envelope parabolas
  int32_t *t; //Separating index between 2 upper envelope parabolas 
  int32_t *dyTemp; //Temp array to backup the Voronoi labeling 
  int32_t *dxTemp; //Temp array to backup the Voronoi labeling 
  int32_t q; 
  int32_t w;
  int32_t x, y, z, u;

  s = (int32_t *)malloc(depth(sdt_xy)*sizeof(int32_t));
  t = (int32_t *)malloc(depth(sdt_xy)*sizeof(int32_t));
  dxTemp = (int32_t *)malloc(depth(sdt_xy)*sizeof(int32_t));
  dyTemp = (int32_t *)malloc(depth(sdt_xy)*sizeof(int32_t));
  if ((s == NULL) || (t == NULL) || (dxTemp == NULL) || (dyTemp == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    exit(0);
  }

  for (y = 0; y < colsize(sdt_xy); y++) 	    
    for (x = 0; x < rowsize(sdt_xy); x++) 
      {
	q=0;
	s[0] = 0;
	t[0] = 0;
	
	dyTemp[0]=lvoxel(dy,x,y,0);
	dxTemp[0]=lvoxel(dx,x,y,0);

	//Forward Scan
	for (u=1; u < depth(sdt_xy) ; u++) 
	  {
	    while ((q >= 0) &&
		   (F(t[q],s[q],lvoxel(sdt_xy,x,y,s[q])) > 
		    F(t[q],u,lvoxel(sdt_xy,x,y,u)))
		   ) 
	      q--;
	    
	    if (q<0) 
	      {
		q=0;
		s[0]=u;
	      }
	    else 
	      {
		w = 1 + Sep(s[q],
			    u,
			    lvoxel(sdt_xy,x,y,s[q]),
			    lvoxel(sdt_xy,x,y,u));
	
		if (w < depth(sdt_xy)) 
		  {
		    q++;
		    s[q]=u;
		    t[q]=w;
		  }
	      }

	    dyTemp[u]=lvoxel(dy,x,y,u);
	    dxTemp[u]=lvoxel(dx,x,y,u);	    
	  }
	

	//Backward Scan
	for (u = depth(sdt_xy)-1; u >= 0; --u) 
	  {
	    lvoxel(sdt_xyz,x,y,u) = F(u,s[q],lvoxel(sdt_xy,x,y,s[q]));	      

	    //Tricky point of the Voronoi labeling
	    lvoxel(dx,x,y,u) = dxTemp[s[q]];
	    lvoxel(dy,x,y,u) = dyTemp[s[q]];
	    lvoxel(dz,x,y,u) = s[q];
	      
	      if (u==t[q]) 
	      q--;
	  }
      }
  free(s);
  free(t);
  free(dxTemp);
}

/* ==================================== */
int32_t lvoronoilabelling(struct xvimage *img,   /* donnee: image binaire */
			  struct xvimage *dist,  /* resultat: SEDT */
			  struct xvimage *vor    /* resultat: labelling */
)
/* ==================================== */
/* 
  Voronoi labelling and squared Euclidean distance transform of image img.
  Resulting images dist and vor must be allocated.
*/
#undef F_NAME
#define F_NAME "lvoronoilabelling"
{ 
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t ds = depth(img);
  int32_t ps = rs * cs;
  int32_t i, N = ps * ds;
  struct xvimage *sdt_tmp;
  struct xvimage *dx;
  struct xvimage *dy;
  struct xvimage *dz;
  int32_t *X, *Y, *Z;
  int32_t *V = ULONGDATA(vor);

  if ((datatype(img) != VFF_TYP_1_BYTE) || (datatype(dist) != VFF_TYP_4_BYTE) || (datatype(vor) != VFF_TYP_4_BYTE))
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return(0);
  }

  if ((rowsize(dist) != rs) || (colsize(dist) != cs) || (depth(dist) != ds) ||
      (rowsize(vor) != rs) || (colsize(vor) != cs) || (depth(vor) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    return 0;
  }    

  sdt_tmp = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  dx = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  dy = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  dz = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if ((sdt_tmp == NULL) || (dx == NULL) ||  (dy == NULL) ||  (dz == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return(0);
  }
  X = ULONGDATA(dx);
  Y = ULONGDATA(dy);
  Z = ULONGDATA(dz);  

  if (ds != 1)
  {
    phaseVoronoiX(img,dist,dx);
    phaseVoronoiY(dist,sdt_tmp,dx,dy);
    phaseVoronoiZ(sdt_tmp,dist,dx,dy,dz);
    for (i = 0; i < N; i++)
      V[i] = Z[i]*ps + Y[i]*rs + X[i];
  }
  else
  {
    phaseVoronoiX(img,sdt_tmp,dx);
    phaseVoronoiY(sdt_tmp,dist,dx,dy);
    for (i = 0; i < N; i++)
      V[i] = Y[i]*rs + X[i];
  }

  freeimage(sdt_tmp);
  freeimage(dx);
  freeimage(dy);
  freeimage(dz);
  return(1);
} // lvoronoilabelling()

/* ======================================================== */
int32_t lmedax_Hesselink(struct xvimage* f, struct xvimage* res, double gamma)
/* ======================================================== */
/* 
Integer Medial Axis
from: "Euclidean Skeletons of 3D Data Sets in Linear Time 
by the Integer Medial Axis Transform", 
W.H. Hesselink and B.T.M. Roerdink,
Computational Imaging and Vision, Vol. 30, 
Mathematical Morphology: 40 Years On, Springer, 2005, pp. 259-268
*/
#undef F_NAME
#define F_NAME "lmedax_Hesselink"
{
  struct xvimage *tmp;
  struct xvimage *dis;
  struct xvimage *dx;
  struct xvimage *dy;
  int32_t i, j, k, p, medax, ftp_x, ftp_y, ftp_z, ftpe_x, ftpe_y, ftpe_z;
  int32_t rs = rowsize(f), cs = colsize(f), ds = depth(f), N = rs * cs * ds;
  uint8_t *F = UCHARDATA(f);
  uint8_t *R = UCHARDATA(res);
  int32_t *X, *Y, *T;
  double mx, my, gamma2 = gamma*gamma;

  if ((rowsize(res) != rs) || (colsize(res) != cs) || (depth(res) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    return 0;
  }    

  if ((datatype(f) != VFF_TYP_1_BYTE) || (datatype(res) != VFF_TYP_1_BYTE))
  {
    fprintf(stderr, "%s: bad image type\n", F_NAME);
    return 0;
  }

  tmp = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  dis = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  dx = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  dy = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if ((tmp == NULL) || (dis == NULL) || (dx == NULL) ||  (dy == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return(0);
  }
  T = ULONGDATA(tmp);
  X = ULONGDATA(dx);
  Y = ULONGDATA(dy);

  if (ds == 1)
  {
    phaseVoronoiX(f,tmp,dx);
    phaseVoronoiY(tmp,dis,dx,dy);

    for (p = j = 0; j < cs; j++) 
    for (i = 0; i < rs; i++, p++) 
    if (F[p])
    {
      medax = 0;
      ftp_x = X[p]; ftp_y = Y[p];
      //printf("i,j=%d,%d; ftp=%d,%d\n", i, j, ftp_x, ftp_y);
      if ((i > 0) && F[p-1])
      {
	ftpe_x = X[p-1]; ftpe_y = Y[p];
	mx = i - 0.5; my = j;
	if ((dist_2(ftpe_x, ftpe_y, ftp_x, ftp_y) > gamma2) &&
	    (dist_2(mx, my, ftpe_x, ftpe_y) <= (dist_2(mx, my, ftp_x, ftp_y))))
	  medax = 1;
      }	  
      if (!medax && (i < rs-1) && F[p+1])
      {
	ftpe_x = X[p+1]; ftpe_y = Y[p];
	mx = i + 0.5; my = j;
	if ((dist_2(ftpe_x, ftpe_y, ftp_x, ftp_y) > gamma2) &&
	    (dist_2(mx, my, ftpe_x, ftpe_y) <= (dist_2(mx, my, ftp_x, ftp_y))))
	  medax = 1;
      }	  
      if (!medax && (j < cs-1) && F[p+rs])
      {
	ftpe_x = X[p]; ftpe_y = Y[p+rs];
	mx = i; my = j + 0.5;
	if ((dist_2(ftpe_x, ftpe_y, ftp_x, ftp_y) > gamma2) &&
	    (dist_2(mx, my, ftpe_x, ftpe_y) <= (dist_2(mx, my, ftp_x, ftp_y))))
	  medax = 1;
      }	  
      if (!medax && (j > 0) && F[p-rs])
      {
	ftpe_x = X[p]; ftpe_y = Y[p-rs];
	mx = i; my = j - 0.5;
	if ((dist_2(ftpe_x, ftpe_y, ftp_x, ftp_y) > gamma2) &&
	    (dist_2(mx, my, ftpe_x, ftpe_y) <= (dist_2(mx, my, ftp_x, ftp_y))))
	  medax = 1;
      }	  
      if (medax) R[p] = NDG_MAX; else R[p] = 0;
    }
    else R[p] = 0;
  }
  else
  {
    fprintf(stderr, "%s: 3d not implemented\n", F_NAME);
    return 0;
    /*
    FT3d_line(F, T, rs, cs, ds);
    FT3d_column(T, R, rs, cs, ds);
    FT3d_planes(R, T, rs, cs, ds);
    */
  }

  freeimage(tmp);
  freeimage(dis);
  freeimage(dx);
  freeimage(dy);
  return 1;
} // lmedax_Hesselink()
