#!/bin/sh
#Computation of constrain connectivity using hierarchical edge-segmentation
USAGE="Usage: $0 ImageIn ImageOut"
if [ $# -gt 2 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 2 ]
then
	echo $USAGE
        exit
fi
#Double the image size
${PINK}/linux/bin/zoomint $1 2 _double.pgm
#Compute the gradient on the edges
${PINK}/linux/bin/pgm2GA _double.pgm 0 _GA.pgm
#Compute the alpha hierarchy
${PINK}/linux/bin/saliencyGA _GA.pgm 1 _alphaGA.pgm
#Compute the alpha-omega hierarchy
${PINK}/linux/bin/saliencyGA _alphaGA.pgm 3 _alphaOmegaGA.pgm _double.pgm
#To visualize, make it khalimsky
${PINK}/linux/bin/GA2khalimsky _alphaOmegaGA.pgm 0 $2
