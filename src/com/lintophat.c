/* $Id: lintophat.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file lintophat.c

\brief max of morphological black top hats by linear structuring elements

<B>Usage:</B> lintophat in.pgm length out.pgm

<B>Description:</B> 
Max of morphological black top hats by linear structuring elements, 
in all possible discrete directions.
Let F be the original image and E be a structuring element, the black
top hat of F by E is defined by F - closing(F, E).
The closing deletes dark structures that do not match the 
structuring element, thus the black top hat detects those dark structures.
For a linear structuring element, the detected structures are those which
are orthogonal to the se.
The length if the linear structuring elements is given by \b length.

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author Hildegard Koehler 2003

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>

//#define DEBUG 
#define THICK

/* =============================================================== */
void setMask(uint8_t * mask, int32_t length, int32_t x, int32_t y) 
/* =============================================================== */
{
	mask[y*length + x] = 1;
}

/* =============================================================== */
void drawLine(uint8_t * mask, int32_t length, int32_t x2, int32_t y2, int32_t quater) 
/* =============================================================== */
{
 int32_t x=0, y=0, dx=0, dy=0, error=0, delta=0, schritt=0;
 int32_t x1=0, y1=0;
 
 dy    = y2 - y1;                               // Hoehenzuwachs berechnen
 dx    = x2 - x1;                               // Schrittweite

 x = x1;                                        // Koordinaten retten
 y = y1;
   
 error = -dx;                                   // momentaner Fehler
 delta = 2*dy;                                  // 'Steigung'
 schritt = -2*dx;                               // Fehlerschrittweite

 while (x <= x2) {                              // fuer jede x-Koordinate
   if (quater == 1){
	    setMask(mask, length, length/2+x , length/2+y);                          // setze Pixel    
 		//printf("setMask: %i %i\n", length/2+x , length/2+y);
	    setMask(mask, length, length/2-x , length/2-y);                          // setze Pixel
 		//printf("setMask: %i %i\n", length/2-x , length/2-y);
 	}
   else if (quater == 2){
	    setMask(mask, length , length/2+y, length/2+x);                          // setze Pixel    
 		//printf("setMask: %i %i\n", length/2+y, length/2+x);
	    setMask(mask, length , length/2-y, length/2-x);                          // setze Pixel
 		//printf("setMask: %i %i\n", length/2-y, length/2-x);
 	}
   else if (quater == 3){
	    setMask(mask, length , length/2+(-1*y), length/2+x);                          // setze Pixel    
 		//printf("setMask: %i %i\n", length/2+(-1*y), length/2+x);
	    setMask(mask, length , length/2-(-1*y), length/2-x);                          // setze Pixel
 		//printf("setMask: %i %i\n",  length/2-(-1*y), length/2-x);
 	}
   else if (quater == 4){
	    setMask(mask, length, length/2+x , length/2+(-1*y));                          // setze Pixel    
 		//printf("setMask: %i %i\n", length/2+x , length/2+(-1*y));
	    setMask(mask, length, length/2-x , length/2-(-1*y));                          // setze Pixel
 		//printf("setMask: %i %i\n", length/2-x , length/2-(-1*y));
 	}
   x++;                                         // naechste x-Koordinate
   error += delta;                              // Fehler aktualisieren
   if (error > 0) {                             // naechste Zeile erreicht?
     y++;                                       // neue y-Koordinate
     error += schritt;                          // Fehler anpassen
   }
 }
	
}


// Get the max_value of two images and write it into img1 

/* =============================================================== */
void max(struct xvimage * img1, struct xvimage * img2) 
/* =============================================================== */
{
	int32_t i, N;
	uint8_t * img1_data;
	uint8_t * img2_data;
	if(rowsize(img1)!=rowsize(img2) || colsize(img1)!=colsize(img2)){
		printf ("Image1: %i x %i\n", rowsize(img1), colsize(img1));
		printf ("Image1: %i x %i\n", rowsize(img2), colsize(img2));
		printf ("Can't get max!\n");
		exit(1);
	}
	N = rowsize(img1)*colsize(img1);
	img1_data = UCHARDATA(img1);
	img2_data = UCHARDATA(img2);
	for (i=0; i<N; i++){
		if(img1_data[i] < img2_data[i]){
			img1_data[i] = img2_data[i];
		}
	}
	return;
}

// Subtract two images and write the result into img1 

/* =============================================================== */
void sub(struct xvimage * img1, struct xvimage * img2) 
/* =============================================================== */
{
	int32_t i, help, N;
	uint8_t * img1_data;
	uint8_t * img2_data;
	if(rowsize(img1)!=rowsize(img2) || colsize(img1)!=colsize(img2)){
		printf ("Image1: %i x %i\n", rowsize(img1), colsize(img1));
		printf ("Image2: %i x %i\n", rowsize(img2), colsize(img2));
		printf ("Can't get sub!\n");
		exit(1);
	}
	N = rowsize(img1)*colsize(img1);
	img1_data = UCHARDATA(img1);
	img2_data = UCHARDATA(img2);
	for (i=0; i<N; i++){
			help = img1_data[i] - img2_data[i];
			if(help < 0){
				img1_data[i] = 0;
			}
			else{
				img1_data[i] = help;
			}
	}
	return;
}

// close the org_image and write the result into img
////////////////////////////////////////////////////////////////////
// Adapter, that creates a copy of the original image 
// and executes an erosion after an dilation on the copy
// Gets: an result_image, the original imgage, the mask, and the center element coordinates
////////////////////////////////////////////////////////////////////
/* =============================================================== */
void close_image(struct xvimage * img, struct xvimage * org_img, struct xvimage * mask, int32_t ce_x, int32_t ce_y ) 
/* =============================================================== */
{	
	int32_t i=0; 
	int32_t N=rowsize(img)*colsize(img);
	uint8_t * img_data = UCHARDATA(img);
	uint8_t * org_img_data = UCHARDATA(org_img);
	
	// Copy org_img to img
	for(i=0; i<N; i++){
		img_data[i] = org_img_data[i];
	}
	
 	//Dilation
 	ldilatnum(img, mask, ce_x, ce_y);
 	//Erosion
	lerosnum(img, mask, ce_x, ce_y);

	return;
}

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
 int argc; char **argv; 
{
 struct xvimage * image; // Pointer fr das Bild
 int32_t rs;    // row_size ^= width, erhlt man mit(s.u.): rowsize(image)
 int32_t cs;    // col_size ^= height, erhlt man mit(s.u.): colsize(image)
 int32_t d;     // dimesion, erhlt man mit(s.u.): depth(image)
 int32_t N;	 // number of pixels ^= rs*cs*d
 int32_t length, i, j, k; 
 struct xvimage * mask;
 uint8_t * mask_data;
 struct xvimage * closed_image;
 struct xvimage * result_image;
#ifdef DEBUG
 struct xvimage * result_imageII;
 uint8_t * result_imageII_data;
#endif

 if (argc != 4)
 {
   fprintf(stderr, "usage: %s in.pgm length out.pgm\n", argv[0]);
   exit(1);
 }

 image = readimage(argv[1]);
 if (image == NULL)
 {
   fprintf(stderr, "histstretch: readimage failed\n");
   exit(1);
 }

 rs = rowsize(image);     /* taille ligne */
 cs = colsize(image);     /* taille colonne */
 d = depth(image);        /* nombre plans */
 N = rs * cs * d;         /* taille image */
 length = atoi(argv[2]);	// length of structuring element
 length = (length/2) * 2 +1; 	// has to be 2*x+1 (ungerade)
 printf("Length is: %i\n", length);
 i=0; j=0; k=0;
 
 mask = allocimage(NULL, length, length, d, (*image).data_storage_type);
 mask_data = UCHARDATA(mask);
 closed_image = allocimage(NULL, rs, cs, d, (*image).data_storage_type);
 result_image = allocimage(NULL, rs, cs, d, (*image).data_storage_type);
 
#ifdef DEBUG
 result_imageII = allocimage(NULL, rs, cs, d, (*image).data_storage_type);
 result_imageII_data = UCHARDATA(result_imageII); 
 for(i=0; i<rs*cs; i++){
 	result_imageII_data[i] = 0;
 }
#endif
 
 // For every possible point in this quater
 for(k=0; k<=length/2; k++){
 	
 	// First Quater
 	//reset mask_data
 	for(i=0; i<length*length; i++){
 		mask_data[i] = 0;
 	}
 	// write new Mask (erster Quadrant, Teil I)
 	drawLine(mask_data, length, length/2, k, 1);
#ifdef THICK
        ldilatfast(mask, vois4);
#endif

 	close_image(closed_image, image, mask, length/2+1, length/2+1);
#ifdef DEBUG
	max(result_imageII, closed_image);
#endif
 	sub(closed_image, image);
 	max(result_image, closed_image);
 	
 	
 	//Second Quater
 	//reset mask_data
 	for(i=0; i<length*length; i++){
 		mask_data[i] = 0;
 	}
 	// write new Mask (erster Quadrant, Teil II)
 	drawLine(mask_data, length, length/2, k, 2);
#ifdef THICK
        ldilatfast(mask, vois4);
#endif

 	close_image(closed_image, image, mask, length/2+1, length/2+1);
#ifdef DEBUG
	max(result_imageII, closed_image);
#endif
 	sub(closed_image, image);
 	max(result_image, closed_image);
 	
 	
 	//Third Quater
 	//reset mask_data
 	for(i=0; i<length*length; i++){
 		mask_data[i] = 0;
 	}
 	// write new Mask (zweiter Quadrant, Teil I)
 	drawLine(mask_data, length, length/2, k, 3);
#ifdef THICK
        ldilatfast(mask, vois4);
#endif
 	
 	close_image(closed_image, image, mask, length/2+1, length/2+1);
#ifdef DEBUG
	max(result_imageII, closed_image);
#endif
 	sub(closed_image, image);
 	max(result_image, closed_image);
 	  	
 	//Fourth Quater
 	//reset mask_data
 	for(i=0; i<length*length; i++){
 		mask_data[i] = 0;
 	}
 	// write new Mask (zweiter Quadrant, Teil II)
 	drawLine(mask_data, length, length/2, k, 4);
#ifdef THICK
        ldilatfast(mask, vois4);
#endif
 	
 	close_image(closed_image, image, mask, length/2+1, length/2+1);
#ifdef DEBUG
	max(result_imageII, closed_image);
#endif
 	sub(closed_image, image);
 	max(result_image, closed_image);
 }
 
   
#ifdef DEBUG
 writeimage(mask, "mask.pgm");
 writeimage(closed_image, "close.pgm");
 writeimage(result_imageII, "closed_result.pgm");
#endif

 writeimage(result_image, argv[argc-1]);
 freeimage(mask);
 freeimage(closed_image);
 freeimage(result_image);
 freeimage(image);

 return 0;
} /* main */
