#include <lbdigitalline.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <assert.h>

#include <ltangents.h>

//#define DEBUG
#define EPSILON 1E-20

#define Fori(x) for (int i = 0 ;  i < (x) ; i++)
#define Forj(x) for (int j = 0 ;  j < (x) ; j++)

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

#ifdef DEBUG
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
    int a, b, aa, bb;
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

#ifdef DEBUG
      printf("SEGMENT dir : %d [0=LR, 1=RL, 2=TB, 3=BT]\n", typemax);
#endif

      M[typemax]->Init();
      Fori(indmax)
      {
	M[typemax]->Insert(curEp,points[pos+i]);
#ifdef DEBUG
	printf("  point %d %d\n", points[pos+i].x, points[pos+i].y);
#endif
      }
      M[typemax]->Tangent(aa,bb);
      switch(typemax)
      {
      case 0:/*LR*/ a = aa; b = bb; break;
      case 1:/*RL*/ a = -aa; b = bb; break;
      case 2:/*TB*/ a = -bb; b = -aa; break;
      case 3:/*BT*/ a = bb; b = aa; break;
      }

#ifdef DEBUG
      printf("Tangent %d %d\n", a, b);
#endif

      lastend = end[pos] = pos + indmax -1;
      if (a == 0)
	angle[pos] = M_PI_2;
      else
	angle[pos] = atan((double)b / (double)a);
    }
    else
    {
      end[pos] = -1;
    }
    pos++;
  }
  end[npoints-1] = -1;

  delete points;
  return;
} // ExtractDSSs()

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
double ComputeLength(int32_t npoints, double *mstd)
//--------------------------------------------------------------------------
/*! \fn double ComputeLength(int32_t npoints, double *mstd)
    \param npoints (input): number of points in the curve
    \param mstd (input): maximal segment tangent direction
    \return the length
    \brief computes the length of the digital curve described by the list "mstd" of its orientations

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
#define F_NAME "ComputeLength"

  double L = 0.0;
  Fori(npoints)
  {
    L += 1.0 / (fabs(cos(mstd[i])) + fabs(sin(mstd[i])));
  }
  return L;
} // ComputeLength()

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
