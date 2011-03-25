# Michel Couprie, 2010
# CeCILL free-software license

# Correction of global bias in image

from pink import imview as imview
from pink import manipulate as manipulate
from pink import cpp as pink
inv=pink.inverse
global DEBUG
DEBUG=1

# bias correction
def correctbias(img, alpha):
    smooth = pink.gaussianfilter(img, alpha)
    mm = pink.minmax(smooth)
    s = smooth - mm[0]
    res1 = img - s
    minmax = pink.minmax(img)
    res = pink.normalize(res1, minmax[0], minmax[1])
    return res

global image
#image = pink.readimage("../images/uo.pgm")
image = pink.readimage("../images/pex2.pgm")

# try gaussian filtering (acts on global 'image' - for use by 'manipulate')
def gaussian(alpha_int):
    alpha = alpha_int / 100.0
    res = pink.gaussianfilter(image, alpha)
    return res

a_int = manipulate(gaussian, 1, 100) # let the user choose parameter value
a = a_int / 100.0

res = correctbias(image, a)
imview(res)

# LuM end of file
