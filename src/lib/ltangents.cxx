#include <lbdigitalline.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#ifndef UNIXIO
#  include "pinkconst.h"
#endif /* UNIXIO */

#include <mccodimage.h>
#include <mcimage.h>
#include <ltangents.h>



//#define DEBUG
//#define DEBUG_ExtractDSSs3D
#define EPSILON 1E-20

#define Fori(x) for(int i=0;i<(x);i++)
#define Forj(x) for(int j=0;j<(x);j++)

/* ==================================== */
static inline double norm(double x, double y, double z)
/* ==================================== */
{
  return sqrt((x * x) + (y * y) + (z * z));
}

/* ==================================== */
static inline double scalarprod(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return (x1 * x2) + (y1 * y2) + (z1 * z2);
}

/* ==================================== */
static inline double cosinesimilarity(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return acos(scalarprod(x1, y1, z1, x2, y2, z2) / (norm(x1, y1, z1) * norm(x2, y2, z2)));
}

//--------------------------------------------------------------------------
void ExtractDSSs(int32_t npoints, int32_t *X, int32_t *Y, int32_t *end, double *angle)
//--------------------------------------------------------------------------
/*! \fn void ExtractDSSs(int32_t npoints, int32_t *X, int32_t *Y, int32_t *fin, double angle)
    \param npoints (input): number of points in points list
    \param X (input): ordered list of points (1st coord)
    \param Y (input): ordered list of points (2nd coord)
    \param end (output): index of the last point of the current DSS
    \param angle (output): angle of the current DSS
    \brief computes all the maximal DSSs starting at each position of the discrete curve in arrays X,Y. For each index i in [0,npoints[, if end[i] != -1 then there is a maximal DSS starting from i and ending at end[i]. Its orientation (relative to the horizontal axis) is given by angle[i].
    \warning arrays "end" and "angle" must have been allocated.

Reference: 

@Article{	  IGMI_Buz2007,
  author	= {Lilian Buzer},
  title		= {{A Simple Algorithm for Digital Line Recognition in the General Case}},
  journal	= {Pattern Recognition},
  year		= {2007},
  volume	= {40},
  pages		= {1675-1684},
  number	= {6}
}

*/
{
#undef F_NAME
#define F_NAME "ExtractDSSs"
  Reco * M[4];
  int pos = 0;
  int lastend = -1;
  int epp = 10;
  Q ep(epp,10);              // thickness choosen for the recognition
  point *points;
  int aa, bb;

#ifdef DEBUG_ExtractDSSs
  printf("%s: begin\n", F_NAME);
#endif

  points = new point[npoints]; assert(points != NULL);
  Fori(npoints) points[i] = point (X[i], Y[i]);

  M[0] = new RecoLR();
  M[1] = new RecoRL();
  M[2] = new RecoTB();
  M[3] = new RecoBT();

  while ( pos < npoints-1 )
  {
    int typemax = 0;
    int indmax  = 0;
    Q curEp;
    Fori(4)            // look for the longuest segment
    {
      M[i]->Init();
      curEp = 0;
      int k =  0;
      while( (curEp < ep) && (pos+k < npoints) )
      {
        M[i]->Insert(curEp,points[pos+k]);
        if (curEp < ep)
        {
          if ( (indmax==0) && (M[i]->valid()) )  { indmax = 1; typemax = i; }
          if ( (indmax >0) && (k >= indmax)    )  { indmax = k+1; typemax = i; }
          k++;
        }
      }
    }

    if ((lastend == -1) || ((pos+indmax-1) != lastend))
    {
#ifdef DEBUG_ExtractDSSs
      printf("SEGMENT dir : %d [0=LR, 1=RL, 2=TB, 3=BT]\n", typemax);
#endif
      M[typemax]->Init();
      curEp = 0;
      Fori(indmax)
      {
	M[typemax]->Insert(curEp,points[pos+i]);
#ifdef DEBUG_ExtractDSSs
	printf("  point %d %d\n", points[pos+i].x, points[pos+i].y);
#endif
      }
      M[typemax]->Tangent(typemax, aa, bb);
      if (aa == 0) angle[pos] = M_PI_2;
      else angle[pos] = atan((double)bb / (double)aa);
      lastend = end[pos] = pos + indmax -1;
    }
    else
    {
      end[pos] = -1;
    }
    pos++;
  } // while ( pos < npoints-1 )
  end[npoints-1] = -1;

  delete points;
  return;
} // ExtractDSSs()

//--------------------------------------------------------------------------
void ExtractDSSs3D(int32_t npoints, int32_t *X, int32_t *Y, int32_t *Z, 
	   int32_t *end, double *Xtan, double *Ytan, double *Ztan)
//--------------------------------------------------------------------------
/*! \fn void ExtractDSSs3D(int32_t npoints, int32_t *X, int32_t *Y, int32_t *Z, 
	   int32_t *end, double *Xtan, double *Ytan, double *Ztan)
    \param npoints (input): number of points in points list
    \param X (input): ordered list of points (1st coord)
    \param Y (input): ordered list of points (2nd coord)
    \param Z (input): ordered list of points (3rd coord)
    \param end (output): index of the last point of the current DSS
    \param Xtan (output): normalized tangent vector (1st coord)
    \param Ytan (output): normalized tangent vector (2nd coord)
    \param Ztan (output): normalized tangent vector (3rd coord)
    \brief computes all the maximal DSSs starting at each position of the discrete curve in arrays X,Y,Z. For each index i in [0,npoints[, if end[i] != -1 then there is a maximal DSS starting from i and ending at end[i]. Its orientation is given by the tangent vector Xtan[i],Ytan[i],Ztan[i].
    \warning arrays "end", "Xtan", "Ytan" and "Ztan" must have been allocated.

Reference: 

@Article{	  IGMI_Buz2007,
  author	= {Lilian Buzer},
  title		= {{A Simple Algorithm for Digital Line Recognition in the General Case}},
  journal	= {Pattern Recognition},
  year		= {2007},
  volume	= {40},
  pages		= {1675-1684},
  number	= {6}
}

*/
{
#undef F_NAME
#define F_NAME "ExtractDSSs3D"
	
  int pos;
  int epp = 10;
  Q ep(epp,10); // thickness chosen for the recognition	
  Q curEp;	
  Reco * MOxy[4], * MOxz[4], * MOyz[4];
  point *pointsxy, *pointsyz, *pointsxz;
  int lastend = -1;
  int  xt, yt, zt, xp, yp, zp;
  double nt;

#ifdef DEBUG_ExtractDSSs3D
  printf("%s: begin\n", F_NAME);
#endif
	
  pointsxy = new point[npoints]; assert(pointsxy != NULL);
  pointsyz = new point[npoints]; assert(pointsyz != NULL);
  pointsxz = new point[npoints]; assert(pointsxz != NULL);
  Fori(npoints) 
  {
    pointsxy[i] = point (X[i], Y[i]);
    pointsyz[i] = point (Y[i], Z[i]);
    pointsxz[i] = point (X[i], Z[i]);
  }

  MOxy[0] = new RecoLR(); MOxz[0] = new RecoLR(); MOyz[0] = new RecoLR();
  MOxy[1] = new RecoRL(); MOxz[1] = new RecoRL(); MOyz[1] = new RecoRL();
  MOxy[2] = new RecoTB(); MOxz[2] = new RecoTB(); MOyz[2] = new RecoTB();
  MOxy[3] = new RecoBT(); MOxz[3] = new RecoBT(); MOyz[3] = new RecoBT();
	
  pos = 0;

  //main loop, detects all maximal lenght naive lines in 3D digital curve
  while (pos < npoints-1) 
  { 
    int xytypemax = 0, yztypemax = 0, xztypemax = 0;
    int indmax, xyindmax  = 0, yzindmax  = 0, xzindmax  = 0;
		
    Fori(4)            // look for the longuest valid segment in projection xy
    {
      MOxy[i]->Init(); curEp = 0;
      for (int k = 0; (curEp < ep) && (pos+k < npoints); k++ )
      {
	if ((k > 0) && (pointsxy[pos+k] == pointsxy[pos+k-1]))
	  // projection is not a bijection: invalid segment
	  break;
        MOxy[i]->Insert(curEp,pointsxy[pos+k]);
        if (curEp < ep)
        {
          if ((xyindmax==0) && (MOxy[i]->valid())) { xyindmax = 1; xytypemax = i; }
          if ((xyindmax >0) && (k >= xyindmax)   ) { xyindmax = k+1; xytypemax = i; }
        }
      }
    }
		
    Fori(4)            // look for the longuest valid segment in projection yz
    {
      MOyz[i]->Init(); curEp = 0;
      for (int k = 0; (curEp < ep) && (pos+k < npoints); k++ )
      {
	if ((k > 0) && (pointsyz[pos+k] == pointsyz[pos+k-1]))
	  // projection is not a bijection: invalid segment
	  break;
        MOyz[i]->Insert(curEp,pointsyz[pos+k]);
        if (curEp < ep)
        {
          if ((yzindmax==0) && (MOyz[i]->valid())) { yzindmax = 1; yztypemax = i; }
          if ((yzindmax >0) && (k >= yzindmax)   ) { yzindmax = k+1; yztypemax = i; }
        }
      }
    }

    Fori(4)            // look for the longuest valid segment in projection xz
    {
      MOxz[i]->Init(); curEp = 0;
      for (int k = 0; (curEp < ep) && (pos+k < npoints); k++ )
      {
	if ((k > 0) && (pointsxz[pos+k] == pointsxz[pos+k-1]))
	  // projection is not a bijection: invalid segment
	  break;
        MOxz[i]->Insert(curEp,pointsxz[pos+k]);
        if (curEp < ep)
        {
          if ((xzindmax==0) && (MOxz[i]->valid())) { xzindmax = 1; xztypemax = i; }
          if ((xzindmax >0) && (k >= xzindmax)   ) { xzindmax = k+1; xztypemax = i; }
        }
      }
    }

#ifdef DEBUG_ExtractDSSs3D
    printf("%s: pos=%d indmax xy=%d yz=%d xz=%d typemax xy=%d yz=%d xz=%d \n", F_NAME,
	   pos, xyindmax, yzindmax, xzindmax, xytypemax, yztypemax, xztypemax);
#endif

    // look for the longest 3D segment that has valid projections in at least 2 planes
#define XY 1
#define YZ 2
#define XZ 3
    int elim;

    if ((xyindmax <= yzindmax) && (xyindmax <= xzindmax))
      elim = XY;
    else
    {
      if ((yzindmax <= xyindmax) && (yzindmax <= xzindmax))
	elim = YZ;
      else
	elim = XZ;
    }

    switch (elim)
    {
    case XY: 
      indmax = min(yzindmax,xzindmax);
#ifdef DEBUG_ExtractDSSs3D
      printf("%s: case XY indmax=%d pos+indmax-1=%d lastend=%d\n", F_NAME, indmax, pos+indmax-1, lastend);
#endif
      if ((lastend == -1) || ((pos+indmax-1) != lastend))
      {
	MOyz[yztypemax]->Init(); curEp = 0;
	Fori(indmax)
	  MOyz[yztypemax]->Insert(curEp,pointsyz[pos+i]);
	MOyz[yztypemax]->Tangent(yztypemax, yt, zt);

	MOxz[xztypemax]->Init(); curEp = 0;
	Fori(indmax)
	  MOxz[xztypemax]->Insert(curEp,pointsxz[pos+i]);
	MOxz[xztypemax]->Tangent(xztypemax, xp, zp);

	if (xp == 0) xt = 0;
	else 
	{
	  if (yt == 0) { xt = xp; zt = zp; }
	  else
	  {
	    if ((zt == 0) && (zp == 0)) { xt = xp; }
	    else { xt = xp*zt; yt = yt*zp; zt = zt*zp; }
	  }
	}

	lastend = end[pos] = pos + indmax -1;
      }
      else
	end[pos] = -1;
      break;

    case YZ:
      indmax = min(xyindmax,xzindmax);
#ifdef DEBUG_ExtractDSSs3D
      printf("%s: case YZ indmax=%d pos+indmax-1=%d lastend=%d\n", F_NAME, indmax, pos+indmax-1, lastend);
#endif
      if ((lastend == -1) || ((pos+indmax-1) != lastend))
      {
	MOxy[xytypemax]->Init(); curEp = 0;
	Fori(indmax)
	  MOxy[xytypemax]->Insert(curEp,pointsxy[pos+i]);
	MOxy[xytypemax]->Tangent(xytypemax, xt, yt);

	MOxz[xztypemax]->Init(); curEp = 0;
	Fori(indmax)
	  MOxz[xztypemax]->Insert(curEp,pointsxz[pos+i]);
	MOxz[xztypemax]->Tangent(xztypemax, xp, zp);
/*
printf("point[pos]=%d,%d,%d point[pos+1]=%d,%d,%d\n", 
       X[pos], Y[pos], Z[pos], X[pos+1], Y[pos+1], Z[pos+1]);
printf("xt=%d yt=%d ", xt, yt);
printf("xp=%d zp=%d\n", xp, zp);
*/
	if (zp == 0) zt = 0;
	else 
	{
	  if (yt == 0) { xt = xp; zt = zp; }
	  else
	  {
	    if ((xt == 0) && (xp == 0)) { zt = zp; }
	    else { yt = yt*xp; zt = zp*xt; xt = xp*xt; }
	  }
	}

	lastend = end[pos] = pos + indmax -1;
      }
      else
	end[pos] = -1;
      break;

    case XZ:
      indmax = min(xyindmax,yzindmax);
#ifdef DEBUG_ExtractDSSs3D
      printf("%s: case XZ indmax=%d pos+indmax-1=%d lastend=%d\n", F_NAME, indmax, pos+indmax-1, lastend);
#endif
      if ((lastend == -1) || ((pos+indmax-1) != lastend))
      {
	MOyz[yztypemax]->Init(); curEp = 0;
	Fori(indmax)
	  MOyz[yztypemax]->Insert(curEp,pointsyz[pos+i]);
	MOyz[yztypemax]->Tangent(yztypemax, yt, zt);

	MOxy[xytypemax]->Init(); curEp = 0;
	Fori(indmax)
	  MOxy[xytypemax]->Insert(curEp,pointsxy[pos+i]);
	MOxy[xytypemax]->Tangent(xytypemax, xp, yp);

	if (xp == 0) xt = 0;
	else 
	{
	  if (zt == 0) { xt = xp; yt = yp; }
	  else
	  {
	    if ((yt == 0) && (yp == 0)) { xt = xp; }
	    else { xt = xp*yt; zt = zt*yp; yt = yt*yp; }
	  }
	}

	lastend = end[pos] = pos + indmax -1;
      }
      else
	end[pos] = -1;
      break;
    default: assert(1);
    } // switch (elim)
#ifdef DEBUG_ExtractDSSs3D
    printf("%s: xt=%d yt=%d zt=%d\n", F_NAME, xt, yt, zt);
#endif
    nt = norm((double)xt, (double)yt, (double)zt);
    Xtan[pos] = (double)xt / nt;
    Ytan[pos] = (double)yt / nt;
    Ztan[pos] = (double)zt / nt;
    pos++;
  } // while (pos < npoints-1)
  end[npoints-1] = -1;

  // invert directions if necessary
  double XP, YP, ZP, XT, YT, ZT, D;
  XP = Xtan[0]; YP = Ytan[0]; ZP = Ztan[0];
  for (int i = 1; i < npoints; i++)
  {
    if (end[i] != -1)
    {
      XT = Xtan[i]; YT = Ytan[i]; ZT = Ztan[i];
      D = cosinesimilarity(XP, YP, ZP, XT, YT, ZT);
      if (D > M_PI_2)
      {
	Xtan[i] = -XT; Ytan[i] = -YT; Ztan[i] = -ZT;
      }
      XP = Xtan[i]; YP = Ytan[i]; ZP = Ztan[i];
    }
  }

  delete pointsxy;
  delete pointsyz;
  delete pointsxz;
  return;
} // ExtractDSSs3D()

//--------------------------------------------------------------------------
static double lambda_fct(double eik)
{
  double e2 = eik * eik; 
  double e3 = e2 * eik; 
  return 64 * (-e3*e3 + 3*e3*e2 - 3*e2*e2 + e3);
}

//--------------------------------------------------------------------------
void LambdaMSTD(int32_t npoints, int32_t *end, double *angle, double *mstd)
//--------------------------------------------------------------------------
/*! \fn void LambdaMSTD(int32_t npoints, int32_t *end, double angle, double *mstd)
    \param npoints (input): number of points in points list
    \param end (input): index of the last point of the current DSS
    \param angle (input): angle of the current DSS
    \param mstd (output): maximal segment tangent direction
    \brief computes the lambda-maximal segment tangent direction at each point of the digital curve in "points"
    \warning array "mstd" must have been allocated.
    \warning only an approximation when used in 3D.
Reference: 

@article{LVV07,
  author = {J.-O. Lachaud and A. Vialard and F. de Vieilleville},
  title = {Fast, Accurate and Convergent Tangent Estimation on Digital Contours},
  journal = {Image and Vision Computing},
  year = {2007},
  volume = {25},
  number = {10},
  pages = {1572--1587},
  month = {October}
}

*/
{
#undef F_NAME
#define F_NAME "LambdaMSTD"
  double eik;
  double *sumeik;
  memset((double *)mstd, 0, npoints*sizeof(double));
  sumeik = (double *)malloc(npoints * sizeof(double)); assert(sumeik != NULL);

  Fori(npoints)
  {
    if (end[i] != -1) // beginning of a DSS
    {
      for (int k = i; k <= end[i]; k++)
      {
	eik = (double)(k - i) / (double)(end[i] - i);
	mstd[k] += lambda_fct(eik) * angle[i];
	sumeik[k] += lambda_fct(eik);
      }
    }
  }

  for (int i = 1; i < npoints-1; i++)
  {
    if (sumeik[i] > EPSILON)
      mstd[i] = mstd[i] / sumeik[i];
    else
      mstd[i] = (mstd[i-1] + (mstd[i+1] / sumeik[i+1])) / 2;
  }
  mstd[0] = mstd[1];
  mstd[npoints-1] = mstd[npoints-2];
  free(sumeik);
} // LambdaMSTD()

//--------------------------------------------------------------------------
void LambdaMSTD3D(int32_t npoints, int32_t *end, 
		  double *Xtan, double *Ytan, double *Ztan, 
		  double *Xmstd, double *Ymstd, double *Zmstd
		 )
//--------------------------------------------------------------------------
/*! \fn void LambdaMSTD3D(int32_t npoints, int32_t *end, double *Xtan, double *Ytan, double *Ztan, double *Xmstd, double *Ymstd, double *Zmstd)
    \param npoints (input): number of points in points list
    \param end (input): index of the last point of the current DSS
    \param Xtan (input): normalized tangent vector (1st coord)
    \param Ytan (input): normalized tangent vector (2nd coord)
    \param Ztan (input): normalized tangent vector (3rd coord)
    \param Xmstd (output): normalized mstd vector (1st coord)
    \param Ymstd (output): normalized mstd vector (2nd coord)
    \param Zmstd (output): normalized mstd vector (3rd coord)
    \brief computes the lambda-maximal segment tangent direction at each point of the digital curve in "points"
    \warning arrays "?mstd" must have been allocated.
*/
{
#undef F_NAME
#define F_NAME "LambdaMSTD3D"
  double eik, nt;
  double *sumeik;
  memset((double *)Xmstd, 0, npoints*sizeof(double));
  memset((double *)Ymstd, 0, npoints*sizeof(double));
  memset((double *)Zmstd, 0, npoints*sizeof(double));
  sumeik = (double *)malloc(npoints * sizeof(double)); assert(sumeik != NULL);

  Fori(npoints)
  {
    if (end[i] != -1) // beginning of a DSS
    {
      for (int k = i; k <= end[i]; k++)
      {
	eik = (double)(k - i) / (double)(end[i] - i);
	Xmstd[k] += lambda_fct(eik) * Xtan[i];
	Ymstd[k] += lambda_fct(eik) * Ytan[i];
	Zmstd[k] += lambda_fct(eik) * Ztan[i];
	sumeik[k] += lambda_fct(eik);
      }
    }
  }

  for (int i = 1; i < npoints-1; i++)
  {
    if (sumeik[i] > EPSILON)
    {
      Xmstd[i] = Xmstd[i] / sumeik[i];
      Ymstd[i] = Ymstd[i] / sumeik[i];
      Zmstd[i] = Zmstd[i] / sumeik[i];
    }
    else
    {
      Xmstd[i] = (Xmstd[i-1] + (Xmstd[i+1] / sumeik[i+1])) / 2;
      Ymstd[i] = (Ymstd[i-1] + (Ymstd[i+1] / sumeik[i+1])) / 2;
      Zmstd[i] = (Zmstd[i-1] + (Zmstd[i+1] / sumeik[i+1])) / 2;
    }
  }
  Xmstd[0] = Xmstd[1];
  Ymstd[0] = Ymstd[1];
  Zmstd[0] = Zmstd[1];
  Xmstd[npoints-1] = Xmstd[npoints-2];
  Ymstd[npoints-1] = Ymstd[npoints-2];
  Zmstd[npoints-1] = Zmstd[npoints-2];

  Fori(npoints)
  {
    nt = norm(Xmstd[i], Ymstd[i], Zmstd[i]);
    Xmstd[i] = Xmstd[i] / nt;
    Ymstd[i] = Ymstd[i] / nt;
    Zmstd[i] = Zmstd[i] / nt;
  }

  free(sumeik);
} // LambdaMSTD3D()

//--------------------------------------------------------------------------
double ComputeLength(int32_t npoints, double *mstd)
//--------------------------------------------------------------------------
/*! \fn double ComputeLength(int32_t npoints, double *mstd)
    \param npoints (input): number of points in the curve
    \param mstd (input): maximal segment tangent direction
    \return the length
    \brief computes the length of the digital curve described by the list "mstd" of its orientations
*/
{
#undef F_NAME
#define F_NAME "ComputeLength"

  double L = 0.0;
  double alpha;

  Fori(npoints)
  {
#ifdef LVV07
    L += 1.0 / (fabs(cos(mstd[i])) + fabs(cos(mstd[i])));
    // très mauvaise estimation - remplacée par la suivante :
#else
    alpha = fabs(mstd[i]);
    if (alpha > M_PI_4) alpha = M_PI_2 - alpha;
    L += 1.0 / fabs(cos(alpha));
#endif
  }
  return L;
} // ComputeLength()

//--------------------------------------------------------------------------
double ComputeLength3D(int32_t npoints, double *Xmstd, double *Ymstd, double *Zmstd)
//--------------------------------------------------------------------------
/*! \fn double ComputeLength3D(int32_t npoints, double *Xmstd, double *Ymstd, double *Zmstd)
    \param npoints (input): number of points in the curve
    \param Xmstd (input): maximal segment tangent direction (1st coord)
    \param Ymstd (input): maximal segment tangent direction (2nd coord)
    \param Zmstd (input): maximal segment tangent direction (3rd coord)
    \return the length
    \brief computes the length of the digital curve described by the lists "?mstd" its orientations
*/
/*
  Suppose that the current tangent vector is (x,y,z) with 0 <= z <= y <= x.
  Then the elementary length contribution is the norm of the vector (1,y/x,z/x).
 */
{
#undef F_NAME
#define F_NAME "ComputeLength3D"

  double L = 0.0, C, x, y, z;

  Fori(npoints)
  {
    x = fabs(Xmstd[i]); y = fabs(Ymstd[i]); z = fabs(Zmstd[i]);
    if ((x >= y) && (x >= z))
      C = sqrt(1.0 + ((y*y + z*z) / (x*x)));
    else
    {
      if (y >= z)
	C = sqrt(1.0 + ((x*x + z*z) / (y*y)));
      else
	C = sqrt(1.0 + ((x*x + y*y) / (z*z)));      
    }  
#ifdef DEBUG_ComputeLength3D
    printf("%s: C = %g\n", F_NAME, C);
#endif
    L = L + C;
  }
  return L;
} // ComputeLength3D()

#ifdef TESTLTANGENTS
/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t npoints;
  char type;
  int32_t *X, *Y;
  int32_t *end;
  double *angle;
  double *mstd;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s curve.list\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d", &npoints);
  X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
  Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

  Fori(npoints)
  {
    fscanf(fd, "%d %d", &(X[i]), &(Y[i]));
  }
  fclose(fd);

  
  end = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(end != NULL);
  angle = (double *)malloc(npoints * sizeof(double)); assert(angle != NULL);
  mstd = (double *)malloc(npoints * sizeof(double)); assert(mstd != NULL);

  ExtractDSSs(npoints, X, Y, end, angle);

  printf("npoints = %d\n", npoints);
  Fori(npoints)
    if (end[i] != -1)
      printf("DSS %d-%d, angle %g (%g)\n", i, end[i], angle[i], angle[i]*180/M_PI);

  LambdaMSTD(npoints, end, angle, mstd);

  Fori(npoints)
    printf("point %d, angle %g (%g)\n", i, mstd[i], mstd[i]*180/M_PI);

  free(X);
  free(Y);
  free(end);
  free(angle);
  free(mstd);
  return 0;
}
#endif
