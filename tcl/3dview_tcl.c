/*! \file 3dview.tcl

\brief interactive visualization / segmentation of a 3D image

<B>Usage:</B> 3dview.tcl in.pgm out.pgm [mark.pgm]<br>
3dview in.pgm out.pgm [mark.pgm]

<B>Description:</B>

Displays three cross sections of a 3D grayscale image, and allows for 
simple segmentation by thresholding. If given, the parameter \b mark.pgm
must be a binary image (to be superimposed with the input image \b in.pgm).
Segmentation result is saved in \b out.pgm.

Menus: 
\li File: choose \b Quit to save and exit.
\li Analyze: to compute the histogram of the full image (item \b Histogram)
or of the Region Of Interest specified by the current segmentation  
(item \b ROI Histogram).
\li Threshold: simple of double thresholding.
\li Visualize: choose \b Revert to inverse the gray levels, check \b Surimp to 
superimpose the binary image to the grayscale data, check \b Light for 
superimposition to be made with a light color, choose \b Render or 
\b Rendersmooth to launch a 3D rendering of the binary image (a smoothing of 
the surface is done with the last choice).

<B>Types supported:</B> byte 3d

<B>Category:</B> inter
\ingroup inter

\author Michel Couprie
*/
