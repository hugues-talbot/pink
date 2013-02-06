/*
 * File:		setneigh.c
 *
 * Written by:		Image Analysis Group staff,
 * 			CSIRO Mathematical and Information Sciences.
 *
 * Date:		March 2001
 *
 *
 * CSIRO Mathematical and Information Sciences is the owner of all
 * copyright subsisting in the software contained in this file. It may
 * not be disclosed to, or used by, anyone without the prior approval 
 * of the Chief, CSIRO Mathematical and Information Sciences.
 *
*/

/* 16 Jan 98 - added missing break */

void setNeighFunc6(long *Nf, int con, int nx, int ny) {
  Nf[4] = -nx*ny; Nf[5] = nx*ny;
  Nf[0] = -1; Nf[1] = 1; 
  Nf[2] = -nx; Nf[3] = nx;
}

void setNeighFunc18(long *Nf, int con, int nx, int ny) {
  long psize;

  psize = nx*ny;
  Nf[0] = -1; Nf[1] = 1; 
  Nf[2] = -nx;Nf[3] = nx;
  Nf[4] = -nx - 1; Nf[5] = -nx + 1;
  Nf[6] = nx - 1; Nf[7] = nx + 1;
  Nf[8] = psize + nx; Nf[9] = psize - nx;
  Nf[10] = psize - 1; Nf[11] = psize; Nf[12] = psize + 1;
  Nf[13] = -psize + nx; Nf[14] = -psize - nx;
  Nf[15] = -psize -1; Nf[16] = -psize; Nf[17] = -psize + 1;
}

void setNeighFunc26(long *Nf, int con, int nx, int ny) {
  long psize;

  psize = nx*ny;
  Nf[0] = -1; Nf[1] = 1; 
  Nf[2] = -nx;Nf[3] = nx;
  Nf[4] = -nx - 1; Nf[5] = -nx + 1;
  Nf[6] = nx - 1; Nf[7] = nx + 1;
  Nf[9] = psize + nx; Nf[8] = Nf[9] - 1; Nf[10] = Nf[9] + 1;
  Nf[12] = psize - nx; Nf[11] = Nf[12] - 1; Nf[13] = Nf[12] + 1;
  Nf[14] = psize - 1; Nf[15] = psize; Nf[16] = psize + 1;
  Nf[18] = -psize + nx; Nf[17] = Nf[18] - 1; Nf[19] = Nf[18] + 1;
  Nf[21] = -psize - nx; Nf[20] = Nf[21] - 1; Nf[22] = Nf[21] + 1;
  Nf[23] = -psize -1; Nf[24] = -psize; Nf[25] = -psize + 1;
}

int setNeighFunc(long *Nf, int con, int nx, int ny)
{
    switch(con) {
      case 2:
      case 21: Nf[0] = -1; Nf[1] = 1; break;
      case 22: Nf[0] = -nx; Nf[1] = nx; break;
      case 8: 
	Nf[4] = -nx - 1; Nf[5] = -nx + 1;
	Nf[6] = nx - 1; Nf[7] = nx + 1;
	Nf[0] = -1; Nf[1] = 1; 
	Nf[2] = -nx; Nf[3] = nx;
	break;
      case 4: 
	Nf[0] = -1; Nf[1] = 1; 
	Nf[2] = -nx; Nf[3] = nx;
	break;
      case 6: 
	setNeighFunc6(Nf, con, nx, ny);
	break;
      case 18:
	setNeighFunc18(Nf, con, nx, ny) ;
	break;
      case 26:
	setNeighFunc26(Nf, con, nx, ny) ;
	break;
      default: return 0;
    }
    return 1;
}

extern int setNeighFunc3d(long *Nf,	 /* pointer to a long vector of size 26  */
			  int con,	 /*  */
			  int nx,	 /*  */
			  int ny,	 /*  */
			  int nz)	 /*  */
{
/** This function sets up the neighbourhood function of a point in a 3D context

RETURN VALUE:	int
0 if an unknown value of con (connectivity) has been received
1 if all has gone well.

DESCRIPTION:
This function calculates the offset inside an image data buffer between 
the data for a given point and the data for each of its neighbours.
The number of neighbours of a point (also called connectivity)
is passed to the function as argument con. 

A point has 2 neighbours in the case of a 1-D image.
In a 2-D image the connectivity can be 4 or 8.
In a 3-D image the connectivity can be 6, 18 or 26.

The neighbours of a point are numbered from 0 to a maximum of 25.

The offsets are written to vector Nf, the first offset being for neighbour 0,
the second offset for neighbour1, and so on.

Written for the 3D version of the Z-Image srgrow function.
The difference with setNeighFunc() is for the case where
the image is 2D, but in the xz or yz planes. In the past this
case was treated as being a 3-D image, but is now treated
as a 2-D one.

The code for a 1-D image looks different but the outcome is the
same, because con=22 when nx=1, so the offsets are always 1 in 
the 1-D case.

Here is a little diagram of the offsets in the three different axis planes:
------------------------------------------------
  xy plane:
   4       2       5
     +---+---+--> x         The neighbours of the point in the centre
     |               |                  are points 0 to 3 or 0 to 7 depending on
  0 +      .       + 1               the connectivity.
     |               |                  The offsets for these points are in the following 
  6 +---+---+ 7               diagram:
     |       3                         
  y v                                    

     -nx-1      -nx    -nx+1              
              +---+---+--> x         
              |               |              
        -1  +      .       +  +1              
              |               |              
    +nx-1 +---+---+ +nx+1             
              |     +nx                      
           y v                               
                                                                           
------------------------------------------------


HISTORY:
written by Annick LeHegarat 30 May 2000

TESTS:
Works in place.

REFERENCES:

KEYWORDS:
**/
    switch(con) {
      case 2:
	Nf[0] = -1; Nf[1] = +1;
	break;
      case 8: 
	if (nz==1) { /* xy plane */
	  Nf[4] = -nx - 1; Nf[5] = -nx + 1;
	  Nf[6] = +nx - 1; Nf[7] = +nx + 1;
	  Nf[0] = -1; Nf[1] = 1; 
	  Nf[2] = -nx; Nf[3] = nx;
	}
	else if (ny == 1) { /* xz plane */
	  Nf[4] = -nx - 1; Nf[5] = -nx + 1;
	  Nf[6] = nx - 1; Nf[7] = nx + 1;
	  Nf[0] = -1; Nf[1] = 1; 
	  Nf[2] = -nx; Nf[3] = nx;
	}
	else if (nx == 1) { /* yz plane */
	  Nf[4] = -ny -1; Nf[5] = -ny + 1;
	  Nf[6] = +ny -1; Nf[7] = +ny +1;
	  Nf[0] = -1;       Nf[1] = 1;
	  Nf[2] = -ny;     Nf[3] = ny;
	}
	break;
      case 4: 
	/* the neighbourhood function depends which plane we're in: */
	if (nz==1) { /* xy plane */
	Nf[0] = -1; Nf[1] = 1; 
	Nf[2] = -nx; Nf[3] = nx;
	}
	else if (ny == 1){ /* xz plane */
	Nf[0] = -1; Nf[1] = 1; 
	Nf[2] = -nx; Nf[3] = nx;
	}
	else if (nx == 1) { /* yz plane */
	Nf[0] = -1; Nf[1] = 1;
	Nf[2] = -ny; Nf[3] = ny;
	}
	else return 0;
	break;
      case 6: 
	setNeighFunc6(Nf, con, nx, ny);
	break;
      case 18:
	setNeighFunc18(Nf, con, nx, ny) ;
	break;
      case 26:
	setNeighFunc26(Nf, con, nx, ny) ;
	break;
    default: return 0;
    }
    return 1;
}













