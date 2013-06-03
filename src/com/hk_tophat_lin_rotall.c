/*hk_tophat_lin_rotall.c*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>

//#define DEBUG 

/* =============================================================== */
void setMask(uint8_t * mask, int32_t length, int32_t x, int32_t y) 
/* =============================================================== */
{
    mask[y*length + x] = 1;
}

/* =============================================================== */
void drawLine(uint8_t * mask, int32_t length, int32_t angle) 
/* =============================================================== */
{
    int32_t x=0, y=0, dx=0, dy=0, error=0, delta=0, schritt=0;
    int32_t x1=0, y1=0, x2=45; // TODO: noch nicht ganz korrekt!!!

    int32_t quater = angle/45;

    int32_t y2;
    if(quater%2 == 0){
	y2 = angle%45;
    }
    else{
	y2 = 45 - angle%45;
    }

    dy    = y2 - y1;                               // Hoehenzuwachs berechnen
    dx    = x2 - x1;                               // Schrittweite

// That's the trick:
// After commputing dx, dy set x2 to the lenght
// of the structuring element.
// So you draw a line from 0|0 to 45|angle but only 'length' pixels int32_t 
    x2 = length/2;

    x = x1;                                        // Koordinaten retten
    y = y1;
  
    error = -dx;                                   // momentaner Fehler
    delta = 2*dy;                                  // 'Steigung'
    schritt = -2*dx;                               // Fehlerschrittweite

    while (x <= x2) {                              // fuer jede x-Koordinate
	if (quater == 0){
	    setMask(mask, length, length/2+x , length/2+y);                          // setze Pixel    
	    //printf("setMask: %i %i\n", length/2+x , length/2+y);
	    setMask(mask, length, length/2-x , length/2-y);                          // setze Pixel
	    //printf("setMask: %i %i\n", length/2-x , length/2-y);
	}
	else if (quater == 1){
	    setMask(mask, length , length/2+y, length/2+x);                          // setze Pixel    
	    //printf("setMask: %i %i\n", length/2+y, length/2+x);
	    setMask(mask, length , length/2-y, length/2-x);                          // setze Pixel
	    //printf("setMask: %i %i\n", length/2-y, length/2-x);
	}
	else if (quater == 2){
	    setMask(mask, length , length/2+(-1*y), length/2+x);                          // setze Pixel    
	    //printf("setMask: %i %i\n", length/2+(-1*y), length/2+x);
	    setMask(mask, length , length/2-(-1*y), length/2-x);                          // setze Pixel
	    //printf("setMask: %i %i\n",  length/2-(-1*y), length/2-x);
	}
	else if (quater == 3){
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
    if(rowsize(img1)!=rowsize(img2) || colsize(img1)!=colsize(img2)){
	printf ("Image1: %i x %i\n", rowsize(img1), colsize(img1));
	printf ("Image1: %i x %i\n", rowsize(img2), colsize(img2));
	printf ("Can't get max!\n");
	exit(1);
    }
    int32_t i;
    int32_t N = rowsize(img1)*colsize(img1);
    uint8_t * img1_data = img1->image_data;
    uint8_t * img2_data = img2->image_data;
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
    if(rowsize(img1)!=rowsize(img2) || colsize(img1)!=colsize(img2)){
	printf ("Image1: %i x %i\n", rowsize(img1), colsize(img1));
	printf ("Image2: %i x %i\n", rowsize(img2), colsize(img2));
	printf ("Can't sub!\n");
	exit(1);
    }
    int32_t i;
    int32_t N = rowsize(img1)*colsize(img1);
    uint8_t * img1_data = img1->image_data;
    uint8_t * img2_data = img2->image_data;
    for (i=0; i<N; i++){
	int32_t help = img1_data[i] - img2_data[i];
	if(help < 0){
	    img1_data[i] = 0;
	}
	else{
	    img1_data[i] = help;
	}
    }
    return;
}

// Close the org_image and write the result into img
////////////////////////////////////////////////////////////////////
// Adapter, that creates a copy of the original image 
// and executes an erosion after an dilatation on the copy
// Gets: an result_image, the original imgage, the mask, and the center element coordinates
////////////////////////////////////////////////////////////////////
/* =============================================================== */
void close_image(struct xvimage * img, struct xvimage * org_img, struct xvimage * mask, int32_t ce_x, int32_t ce_y ) 
/* =============================================================== */
{	
    int32_t i=0; 
    int32_t N=rowsize(img)*colsize(img);
    uint8_t * img_data = img->image_data;
    uint8_t * org_img_data = org_img->image_data;
	
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
    struct xvimage * image; // Pointer f�r das Bild
    int32_t rs;    // row_size ^= width, erh�lt man mit(s.u.): rowsize(image)
    int32_t cs;    // col_size ^= height, erh�lt man mit(s.u.): colsize(image)
    int32_t d;     // dimesion, erh�lt man mit(s.u.): depth(image)
    int32_t N;	 // number of pixels ^= rs*cs*d

    if (argc != 5)
    {
	fprintf(stderr, "usage: %s in.pgm length angle out.pgm\n", argv[0]);
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
    int32_t length = atoi(argv[2]);	// length of structuring element
    length = (length/2) * 2 +1; 	// has to be 2*x+1 (impair)
    printf("Length is: %i\n", length);
    int32_t angle = atoi(argv[3]);
    int32_t i=0, j=0, k=0;

    struct xvimage * mask = allocimage(NULL, length, length, d, image->data_storage_type);
    uint8_t * mask_imagedata = mask->image_data;

    struct xvimage * closed_image = allocimage(NULL, rs, cs, d, image->data_storage_type);

    struct xvimage * result_image = allocimage(NULL, rs, cs, d, image->data_storage_type);

// For every angle
    for(k=0; k<180; k+=angle){
	
	//reset mask_imagedata
	for(i=0; i<length*length; i++){
	    mask_imagedata[i] = 0;
	}
	// write new Mask, k^=angle
	drawLine(mask_imagedata, length, k);
	
	//Open
	close_image(closed_image, image, mask, length/2+1, length/2+1);
	//Sub
	sub(closed_image, image);
	//Max
	max(result_image, closed_image);
    }

  
// Test:
    writeimage(mask, "mask.pgm");
    writeimage(closed_image, "open.pgm");
    writeimage(result_image, argv[argc-1]);

    freeimage(mask);
    freeimage(closed_image);
    freeimage(result_image);
    freeimage(image);

    return 0;
} /* main */

#include <stdint.h>
#include <sys/types.h>
