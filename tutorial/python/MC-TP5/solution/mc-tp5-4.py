# Michel Couprie, 2011
# CeCILL free-software license

# Detection of carbides

from pink import imview as imview
from pink import cpp as pink
global DEBUG
DEBUG=1

# black top hat transformation (residue of the closing)
def black_top_hat(image, es):
    res = pink.closing(image, es) - image
    return res

# white top hat transformation (residue of the opening)
def white_top_hat(image, es):
    res = image - pink.opening(image, es)
    return res

# build disk-shaped structuring element
def disk_structuring_element(radius):
    d = radius + radius + 1
    res = pink.char_image([d,d])
    res.fill(0)
    res.center=[radius,radius]
    res[[radius,radius]] = 255
    res = pink.dilatball(res, radius)
    return res

# carbide detection
def carbide_detection(image, rad_bth, seuil, rad_op):
    es = disk_structuring_element(rad_bth)
    if DEBUG:
        es.writeimage("_es")
    bth = black_top_hat(image, es)
    if DEBUG:
        bth.writeimage("_bth")
    thr = pink.seuil(bth, seuil)
    if DEBUG:
        thr.writeimage("_thr")
    opb = pink.openball(thr, rad_op)
    if DEBUG:
        opb.writeimage("_opb")
    res = pink.attribute(opb, 8, 0, 0, 0)
    return res

img = pink.readimage("../images/carbures.pgm")
res = carbide_detection(img, 9, 40, 2)
imview(res)

# LuM end of file
