/* $Id: mcprobas.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcprobas.h>

static int32_t Randx = 237465;

/* --------------------------------------------------------------------------
 * Function      : Prob
 * Result        : double
 * Parameters    : void
 *    Name              Type              Usage
 *
 * Functionality : [0, 1[ uniform probability distribution
 * -------------------------------------------------------------------------*/

double Prob(void)
{
  static double divis = 2147483648.0;  /*  = 2**31, cycle of rand()  */

  Randx = Randx * 1103515245 + 12345;

  return(((double) (Randx & 0x7fffffff)) / divis);
}

/* --------------------------------------------------------------------------
 * Function      : ResetProb
 * Result        : void
 * Parameters    :
 *    Name              Type              Usage
 *    num               int32_t               New value for the random number
 *                                        generator
 *
 * Functionality : Reset probability
 * -------------------------------------------------------------------------*/

void ResetProb(int32_t num)
{
  Randx = (int32_t) num;
}


/* --------------------------------------------------------------------------
 * Function      : Erlang
 * Result        : double
 * Parameters    :
 *    Name              Type              Usage
 *    mean              double            mean
 *    variance          double            variance
 *
 * Functionality : Calculate the erlang distribution
 * -------------------------------------------------------------------------*/

double Erlang(double mean, double variance)
{
  int32_t k, i;
  float a, y;
  double prod;

  k = (mean*mean/variance + 0.5);
  k = (k > 0) ? k : 1;
  a = k/mean;
  prod = 1.0;
  for(i=1; i<=k; i++)
    prod *= Prob();

  y = -log(prod)/a;

  return(y);
}


/* --------------------------------------------------------------------------
 * Function      : Expntl
 * Result        : double
 * Parameters    :
 *    Name              Type              Usage
 *    mean              double            mean
 *
 * Functionality : Calculate the exponential distribution
 * -------------------------------------------------------------------------*/

double Expntl(double mean)
{
  double x;

  x = Prob();
  return(-mean * log(x));
}


/* --------------------------------------------------------------------------
 * Function      : HyperExpntl
 * Result        : double
 * Parameters    :
 *    Name              Type              Usage
 *    mean              double            mean
 *    variance          double            variance
 *
 * Functionality : Return the random derivate drawn from a hyperexponential
 *                 distribution with mean mean and variance variance
 * -------------------------------------------------------------------------*/

double HyperExpntl(double mean, double variance)
{
  float z,p,d;
  double y;

  z = variance / (mean*mean);
  p = 0.5 * (1.0 - sqrt((z - 1.0) / (z + 1.0)));

  d = (Prob() > p) ? 1.0 - p : p;
  y = -mean * log(Prob()) / (2.0 * d);

  return(y);
}


/* --------------------------------------------------------------------------
 * Function      : Normal
 * Result        : double
 * Parameters    :
 *    Name              Type              Usage
 *    mean              double            mean
 *    deviation         double            standard deviation
 *
 * Functionality : Return the random derivate drawn from a normal
 *                 distribution with mean mean and standard deviation
 *                 deviation
 * -------------------------------------------------------------------------*/

double Normal(double mean, double deviation)
{
  int32_t i;
  double sum, y;

  sum = 0.0;
  for(i=1; i <= 12; i++)
    sum += Prob();

  y = deviation * (sum - 6.0) + mean;

  return(y);
}

/* --------------------------------------------------------------------------
 * Function      : Random
 * Result        : int32_t
 * Parameters    :
 *    Name              Type              Usage
 *    low               int32_t               Low limit
 *    high              int32_t               High limit
 *
 * Functionality : Return an integer random number in [low, high]
 * -------------------------------------------------------------------------*/

int32_t Random(int32_t low, int32_t high)
{
  return(low + (high-low+1)*Prob());
}

/* --------------------------------------------------------------------------
 * Function      : Uniform
 * Result        : double
 * Parameters    :
 *    Name              Type              Usage
 *    low               double            low limit
 *    high              double            high limit
 *
 * Functionality : Floating point uniform probability
 * -------------------------------------------------------------------------*/

double Uniform(double low, double high)
{
  return(low+(high-low)*Prob());
}



