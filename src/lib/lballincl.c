/* $Id: lballincl.c,v 1.1 2008-12-01 13:20:03 mcouprie Exp $ */
/****************************************************************
*
* Routine Names: 
*   compute_min_disk_with_border_constraint
*   compute_min_sphere_with_border_constraint
*
* Purpose: 
* Input:
* Output:
* Written By: John Chaussard - décembre 2008
* D'apres l'article 
*
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

//#include <patch_types.h>

#include <lballincl.h>

inline double det_matrice_4(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o, double p)
{
	return(	 a*(f*(k*p-o*l)+j*(o*h-g*p)+n*(g*l-k*h))
			-e*(j*(o*d-c*p)+n*(c*l-k*d)+b*(k*p-o*l))
			+i*(n*(c*h-g*d)+b*(g*p-o*h)+f*(o*d-c*p))
			-m*(b*(g*l-k*h)+f*(k*d-c*l)+j*(c*h-g*d)) );
}

int32_t compute_min_disk_with_border_constraint(double *tab_point, uint32_t num_point, double *tab_point_border, uint32_t num_point_border, double *c_x, double *c_y, double *c_r)
{
	double restriction[6];
	uint32_t i;

	if( (num_point==0 && num_point_border!=0) || num_point_border==3)
	{
		build_circle(tab_point_border, num_point_border, c_x, c_y, c_r);
		return(0);
	}

	else if(num_point==0 && num_point_border==0)
	{
		*c_x=0.0; *c_y=0.0; *c_r=0.0;
	}

	else
	{
		compute_min_disk_with_border_constraint(tab_point, num_point -1, tab_point_border, num_point_border, c_x, c_y, c_r);

		if( ((tab_point[2*num_point -2]-(*c_x))*(tab_point[2*num_point -2]-(*c_x)) + (tab_point[2*num_point -1]-(*c_y))*(tab_point[2*num_point -1]-(*c_y))) > ((*c_r)*(*c_r) + BALLINCL_EPSILON) )
		{
			//The chosen point is not in the disk...
			for(i=0; i<num_point_border; i++)
			{
				restriction[2*i]=tab_point_border[2*i];
				restriction[2*i +1]=tab_point_border[2*i +1];
			}
			restriction[2*i]=tab_point[2*num_point -2];
			restriction[2*i +1]=tab_point[2*num_point -1];

			compute_min_disk_with_border_constraint(tab_point, num_point -1, &restriction[0], num_point_border+1, c_x, c_y, c_r);
		}
		return(0);
	}

	return(0);
}

int32_t compute_min_sphere_with_border_constraint(double *tab_point, uint32_t num_point, double *tab_point_border, uint32_t num_point_border, double *c_x, double *c_y, double *c_z, double *c_r)
{
	double restriction[12];
	uint32_t i;

	if( (num_point==0 && num_point_border!=0) || num_point_border==4)
	{
		build_sphere(tab_point_border, num_point_border, c_x, c_y, c_z, c_r);
		return(0);
	}

	else if(num_point==0 && num_point_border==0)
	{
		*c_x=0.0; *c_y=0.0; *c_r=0.0;
	}

	else
	{
		compute_min_sphere_with_border_constraint(tab_point, num_point -1, tab_point_border, num_point_border, c_x, c_y, c_z, c_r);

		if( ((tab_point[3*num_point -3]-(*c_x))*(tab_point[3*num_point -3]-(*c_x)) + (tab_point[3*num_point -2]-(*c_y))*(tab_point[3*num_point -2]-(*c_y)) + (tab_point[3*num_point -1]-(*c_z))*(tab_point[3*num_point -1]-(*c_z))) > ((*c_r)*(*c_r) + BALLINCL_EPSILON) )
		{
			//The chosen point is not in the disk...
			for(i=0; i<num_point_border; i++)
			{
				restriction[3*i]=tab_point_border[3*i];
				restriction[3*i +1]=tab_point_border[3*i +1];
				restriction[3*i +2]=tab_point_border[3*i +2];
			}
			restriction[3*i+2]=tab_point[3*num_point -1];
			restriction[3*i]=tab_point[3*num_point -3];
			restriction[3*i+1]=tab_point[3*num_point -2];

			compute_min_sphere_with_border_constraint(tab_point, num_point -1, &restriction[0], num_point_border+1, c_x, c_y, c_z, c_r);
		}
		return(0);
	}

	return(0);
}

//Function that builds a circle which passes by a set of point (given as array) (three points only necessary normally)
int32_t build_circle(double *tab_coord, uint32_t num_points, double* c_x, double* c_y, double* c_r)
{
	double a, a_p, b, b_p, x_c, y_c, r_c;

	if(num_points==0)
	{
		fprintf(stderr, "build_circle: function failed as no points given in entry.\n");
		return(-1);
	}
	//Only one point, return the point
	else if(num_points==1)
	{
		*c_x=tab_coord[0]; *c_y=tab_coord[1]; *c_r=0.0;
		return(0);
	}
	//Two points, return the middle
	else if(num_points==2)
	{
		x_c=(tab_coord[2]+tab_coord[0])/2.0;
		y_c=(tab_coord[3]+tab_coord[1])/2.0;
		*c_r=sqrt( (tab_coord[0]-x_c)*(tab_coord[0]-x_c) + (tab_coord[1]-y_c)*(tab_coord[1]-y_c) );
		*c_x=x_c; *c_y=y_c;
		return(0);
	}
	//Three points or more... Only consider the first three points (no more possible in 100% discrete calculus)
	else if(num_points==3)
	{
		//Compute slope of first bissector, then of second, etc....
		if(tab_coord[3] != tab_coord[1])
		{
			a=-1.0*(tab_coord[2]-tab_coord[0])/(tab_coord[3]-tab_coord[1]);
			b=(tab_coord[2]*tab_coord[2] + tab_coord[3]*tab_coord[3] - tab_coord[0]*tab_coord[0] - tab_coord[1]*tab_coord[1])/(2.0*(tab_coord[3]-tab_coord[1]));
		}
		else
		{
			a=-1.0*(tab_coord[2]-tab_coord[4])/(tab_coord[3]-tab_coord[5]);
			b=(tab_coord[2]*tab_coord[2] + tab_coord[3]*tab_coord[3] - tab_coord[4]*tab_coord[4] - tab_coord[5]*tab_coord[5]) / (2.0*(tab_coord[3]-tab_coord[5]));
		}

		if(tab_coord[5] != tab_coord[1])
		{
			a_p=-1.0*(tab_coord[4]-tab_coord[0])/(tab_coord[5]-tab_coord[1]);
			b_p=(tab_coord[4]*tab_coord[4] + tab_coord[5]*tab_coord[5] - tab_coord[0]*tab_coord[0] - tab_coord[1]*tab_coord[1]) / (2.0*(tab_coord[5]-tab_coord[1]));
		}
		else
		{
			a_p=-1.0*(tab_coord[2]-tab_coord[4])/(tab_coord[3]-tab_coord[5]);
			b_p=(tab_coord[2]*tab_coord[2] + tab_coord[3]*tab_coord[3] - tab_coord[4]*tab_coord[4] - tab_coord[5]*tab_coord[5]) / (2.0*(tab_coord[3]-tab_coord[5]));
		}

		//Three points are aligned ? (should not happen if used with the min circle function)
		if( fabs(a_p-a) < BALLINCL_EPSILON )
		{
			fprintf(stderr, "build_circle: function failed as three points given are aligned.\n");
			return(-1);
		}

		x_c=(b-b_p)/(a_p-a);
		y_c=a*(*c_x)+b;
		*c_r=sqrt( (tab_coord[0]-x_c)*(tab_coord[0]-x_c) + (tab_coord[1]-y_c)*(tab_coord[1]-y_c) );
		*c_x=x_c; *c_y=y_c;
		return(0);
	}

	fprintf(stderr, "build_circle: function failed, too many points given.\n");
	return(-1);
}

int32_t build_sphere(double *tab_coord, uint32_t num_points, double* c_x, double* c_y, double* c_z, double* c_r)
{
	double x_c, y_c, z_c, r_c, a, b, c, d, e, f, g, h, i, j, k, l, det_A, det_M;

	if(num_points==0)
	{
		fprintf(stderr, "build_sphere: function failed as no points given in entry.\n");
		return(-1);
	}

	else if(num_points==1)
	{
		*c_x=tab_coord[0]; *c_y=tab_coord[1]; *c_z=tab_coord[2]; *c_r=0.0;
		return(0);
	}

	else if(num_points==2)
	{
		x_c=(tab_coord[3]+tab_coord[0])/2.0;
		y_c=(tab_coord[4]+tab_coord[1])/2.0;
		z_c=(tab_coord[5]+tab_coord[2])/2.0;
		*c_r=sqrt( (tab_coord[0]-x_c)*(tab_coord[0]-x_c) + (tab_coord[1]-y_c)*(tab_coord[1]-y_c) + (tab_coord[2]-z_c)*(tab_coord[2]-z_c) );
		*c_x=x_c; *c_y=y_c; *c_z=z_c;
		return(0);
	}

	else if(num_points==3)
	{
		//We define the plan ax+by+cz+d=0 as the plane containing our three points...
		a=(tab_coord[4]-tab_coord[1])*(tab_coord[8]-tab_coord[2]) - (tab_coord[5]-tab_coord[2])*(tab_coord[7]-tab_coord[1]);
		b=(tab_coord[5]-tab_coord[2])*(tab_coord[6]-tab_coord[0]) - (tab_coord[8]-tab_coord[2])*(tab_coord[3]-tab_coord[0]);
		c=(tab_coord[3]-tab_coord[0])*(tab_coord[7]-tab_coord[1]) - (tab_coord[4]-tab_coord[1])*(tab_coord[6]-tab_coord[0]);
		d=-(a*tab_coord[0] + b*tab_coord[1] + c*tab_coord[2]);
		if(fabs(a)<BALLINCL_EPSILON && fabs(b)<BALLINCL_EPSILON && fabs(c)<BALLINCL_EPSILON)
		{
			//Check if two points are the same, and therefore recall same fucntion with two disctinct points.
			if(fabs(tab_coord[0]-tab_coord[3])<BALLINCL_EPSILON && fabs(tab_coord[1]-tab_coord[4])<BALLINCL_EPSILON && fabs(tab_coord[2]-tab_coord[5])<BALLINCL_EPSILON)
				return build_sphere(&tab_coord[3], 2, c_x, c_y, c_z, c_r);

			else if(fabs(tab_coord[0]-tab_coord[6])<BALLINCL_EPSILON && fabs(tab_coord[1]-tab_coord[7])<BALLINCL_EPSILON && fabs(tab_coord[2]-tab_coord[8])<BALLINCL_EPSILON)
				return build_sphere(tab_coord, 2, c_x, c_y, c_z, c_r);

			else if(fabs(tab_coord[3]-tab_coord[6])<BALLINCL_EPSILON && fabs(tab_coord[4]-tab_coord[7])<BALLINCL_EPSILON && fabs(tab_coord[5]-tab_coord[8])<BALLINCL_EPSILON)
				return build_sphere(tab_coord, 2, c_x, c_y, c_z, c_r);

			fprintf(stderr, "build_sphere: function failed as the three given points are aligned.\n");
			return(-1);
		}

		//Then, define the plan ex+fy+gz+h=0 orthogonal to AB and passing by the middle of AB
		e=tab_coord[3]-tab_coord[0];
		f=tab_coord[4]-tab_coord[1];
		g=tab_coord[5]-tab_coord[2];
		h=-(e*(tab_coord[3]+tab_coord[0])/2.0 + f*(tab_coord[4]+tab_coord[1])/2.0 + g*(tab_coord[5]+tab_coord[2])/2.0);

		//Then, define the plan ix+jy+kz+l=0 orthogonal to AC and passing by the middle of AC
		i=tab_coord[6]-tab_coord[0];
		j=tab_coord[7]-tab_coord[1];
		k=tab_coord[8]-tab_coord[2];
		l=-(i*(tab_coord[6]+tab_coord[0])/2.0 + j*(tab_coord[7]+tab_coord[1])/2.0 + k*(tab_coord[8]+tab_coord[2])/2.0);

		//Finally, find the intersection point between three plans... We solve the matrix AX=B, with a cramer system
		det_A=a*(f*k-g*j) + e*(j*c-k*b) + i*(b*g-f*c);
		x_c=(d*(g*j-f*k) + h*(k*b-j*c) + l*(f*c-b*g))/det_A;
		y_c=(a*(l*g-h*k) + e*(d*k-l*c) + i*(h*c-d*g))/det_A;
		z_c=(a*(h*j-l*f) + e*(l*b-d*j) + i*(d*f-h*b))/det_A;
		*c_r=sqrt( (tab_coord[0]-x_c)*(tab_coord[0]-x_c) + (tab_coord[1]-y_c)*(tab_coord[1]-y_c) + (tab_coord[2]-z_c)*(tab_coord[2]-z_c) );
		*c_x=x_c; *c_y=y_c; *c_z=z_c;
		return(0);
	}

	else if(num_points==4)
	{
		det_M=det_matrice_4(tab_coord[0], tab_coord[1], tab_coord[2], 1.0, tab_coord[3], tab_coord[4], tab_coord[5], 1.0, tab_coord[6], tab_coord[7], tab_coord[8], 1.0, tab_coord[9], tab_coord[10], tab_coord[11], 1.0);
		if(fabs(det_M)<BALLINCL_EPSILON)
		{
			//The four given points are colinear... Let's build a sphere on three points
			if(build_sphere(tab_coord, 3, &x_c, &y_c, &z_c, &r_c)==-1)
			{
				fprintf(stderr, "build_sphere: function failed as three given points are aligned.\n");
				return(-1);
			}

			//Check if the fourth point is on the sphere...
			if(fabs(r_c*r_c - ((x_c-tab_coord[9])*(x_c-tab_coord[9]) + (y_c-tab_coord[10])*(y_c-tab_coord[10]) + (z_c-tab_coord[11])*(z_c-tab_coord[11]))) > BALLINCL_EPSILON)
			{
				fprintf(stderr, "build_sphere: function failed as four given points are coplanar but not on same circle.\n");
				return(-1);
			}

			//else, good
			*c_x=x_c; *c_y=y_c; *c_z=z_c; *c_r=r_c;
			return(0);
		}

		//else, let's compute the center of the sphere
		x_c=det_matrice_4(tab_coord[0]*tab_coord[0] + tab_coord[1]*tab_coord[1] + tab_coord[2]*tab_coord[2], tab_coord[1], tab_coord[2], 1.0, tab_coord[3]*tab_coord[3] + tab_coord[4]*tab_coord[4] + tab_coord[5]*tab_coord[5], tab_coord[4], tab_coord[5], 1.0, tab_coord[6]*tab_coord[6] + tab_coord[7]*tab_coord[7] + tab_coord[8]*tab_coord[8], tab_coord[7], tab_coord[8], 1.0, tab_coord[9]*tab_coord[9] + tab_coord[10]*tab_coord[10] + tab_coord[11]*tab_coord[11], tab_coord[10], tab_coord[11], 1.0);
		x_c=x_c/(2.0*det_M);
		y_c=det_matrice_4(tab_coord[0]*tab_coord[0] + tab_coord[1]*tab_coord[1] + tab_coord[2]*tab_coord[2], tab_coord[0], tab_coord[2], 1.0, tab_coord[3]*tab_coord[3] + tab_coord[4]*tab_coord[4] + tab_coord[5]*tab_coord[5], tab_coord[3], tab_coord[5], 1.0, tab_coord[6]*tab_coord[6] + tab_coord[7]*tab_coord[7] + tab_coord[8]*tab_coord[8], tab_coord[6], tab_coord[8], 1.0, tab_coord[9]*tab_coord[9] + tab_coord[10]*tab_coord[10] + tab_coord[11]*tab_coord[11], tab_coord[9], tab_coord[11], 1.0);
		y_c=y_c/(-2.0*det_M);
		z_c=det_matrice_4(tab_coord[0]*tab_coord[0] + tab_coord[1]*tab_coord[1] + tab_coord[2]*tab_coord[2], tab_coord[0], tab_coord[1], 1.0, tab_coord[3]*tab_coord[3] + tab_coord[4]*tab_coord[4] + tab_coord[5]*tab_coord[5], tab_coord[3], tab_coord[4], 1.0, tab_coord[6]*tab_coord[6] + tab_coord[7]*tab_coord[7] + tab_coord[8]*tab_coord[8], tab_coord[6], tab_coord[7], 1.0, tab_coord[9]*tab_coord[9] + tab_coord[10]*tab_coord[10] + tab_coord[11]*tab_coord[11], tab_coord[9], tab_coord[10], 1.0);
		z_c=z_c/(2.0*det_M);
		*c_r=sqrt( (tab_coord[0]-x_c)*(tab_coord[0]-x_c) + (tab_coord[1]-y_c)*(tab_coord[1]-y_c) + (tab_coord[2]-z_c)*(tab_coord[2]-z_c) );
		*c_x=x_c; *c_y=y_c; *c_z=z_c;
		return(0);
	}

	fprintf(stderr, "build_sphere: function failed, too many points given.\n");
	return(-1);
}

#ifdef TEST
#include <time.h>
#include <mcimage.h>
#include <mccodimage.h>

int main()
{
	double c_x, c_y, c_r, c_z;
	uint32_t i;
	double *tab=calloc(150, sizeof(double));
	struct xvimage *temp;

	temp=allocimage(NULL, 300, 300, 300, VFF_TYP_1_BYTE);

	srand (time (NULL));

	//On tire des points au hasard
	for(i=0; i<24; i++)
	{
		tab[i]=(float)(rand()%100 +100);
		if(tab[i]<0)
		{
			tab[i]=-1.0*tab[i];
		}
	}

	for(i=0; i<8; i++)
	{
		UCHARDATA(temp)[(int)tab[3*i]+300*(int)tab[3*i+1]+300*300*(int)tab[3*i+2]]=255;
	}

	writeimage(temp, "out.pgm");
	freeimage(temp);

	compute_min_sphere_with_border_constraint(tab, 8, NULL, 0, &c_x, &c_y, &c_z, &c_r);

	fprintf(stdout, "%f %f %f %f\n", c_x, c_y, c_z, c_r);

	return(0);
}

#endif
