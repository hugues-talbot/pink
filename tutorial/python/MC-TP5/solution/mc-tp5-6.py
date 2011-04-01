# Michel Couprie, 2011
# CeCILL free-software license

# segmentation of 3D concrete 
# the radial bias must have been corrected beforehand

from pink import view3d, render, seuilmesh
from pink import imview as imview
from pink import cpp as pink
from pink import show_histo
inv=pink.inverse

global DEBUG
DEBUG=1

# image smoothing
def filtering(image, rad_es):
    es = pink.genball(rad_es,3)
    if DEBUG:
        es.writeimage("_es")
    res = pink.medianfilter(image, es)
    return(res)

# segmentation of white particles
def seg_white(image, thr, size_open, size_close):
    r1 = pink.seuil(image, thr)
    r2 = pink.areaopening(r1, 26, size_open)
    res = pink.areaclosing(r1, 6, size_close)
    return(res)

# segmentation of black regions (cavities)
def seg_black(image, thr1):
    t1 = pink.seuil(image, thr1)
    return(inv(t1))

# segmentation of dark gray regions
def seg_gray(image, thr1, thr2):
    t1 = pink.seuil(image, thr1)
    t2 = pink.seuil(image, thr2)
    res = pink.asf(t1 - t2, 2)
    return(res)

# combine three binary images into one label image
def combine(im1, im2, im3):
    i1 = pink.normalize(im1, 0, 1)
    i2 = pink.normalize(im2, 0, 2)
    i3 = pink.normalize(im3, 0, 3)
    return(i1 + i2 + i3)

img = pink.readimage("../images/mortier_3d.pgm")

show_histo(img)

imgf = filtering(img, 2)

white = seg_white(imgf, 197, 80, 80)
if DEBUG:
    view3d(img, white)

black = seg_black(imgf, 128)
if DEBUG:
    view3d(img, black)

gray = seg_gray(imgf, 128, 152)
if DEBUG:
    view3d(img, gray)

labels = combine(black, gray, white)
if DEBUG:
    view3d(pink.normalize(labels))

# LuM end of file
