# Michel Couprie, 2010
# CeCILL free-software license

# Correction of global bias in image

from pink import imview as imview
from pink import cpp as pink
inv=pink.inverse
global DEBUG
DEBUG=1

# returns the minimum value in image
def min1(image):
    res = image[0]
    for i in range(image.size.prod()):
        if image[i] < res:
            res = image[i]
    return res

# segmentation
def correctbias(image, alpha):
    smooth = pink.gaussianfilter(image, alpha)
    if DEBUG:
        smooth.writeimage("_smooth")
    m = min1(smooth)
    s = smooth - m
    res = image - s
    if DEBUG:
        res.writeimage("_res")
    return res

img = pink.readimage("pex1.pgm")
res = correctbias(img, 0.005)
imview(res)

# LuM end of file
