# Michel Couprie, 2010
# CeCILL free-software license

# Segmentation and analysis of corroded joints

from pink import imview as imview
from pink import cpp as pink
inv=pink.inverse

global DEBUG
DEBUG=1

# counts the number of non-null elements
def area(image):
    res = 0
    for i in range(image.size.prod()):
        if image[i] != 0:
            res = res + 1
    return(res)

# segmentation
def segment(image, height):
    # gradient
    ball = pink.genball(2)
    grad = pink.gradmorph(image,ball)
    if DEBUG:
        grad.writeimage("_grad")
    # obtain markers
    mark1 = pink.segmentheight(inv(grad), 4, height, 0)
    if DEBUG:
        mark1.writeimage("_mark1")
    # watershed
    wshed1 = pink.watershed(inv(grad), mark1, 4)
    if DEBUG:
        wshed1.writeimage("_wshed1")
    # suppress tiny components
    mark2 = pink.segmentarea(wshed1, 4, 100, 0)
    if DEBUG:
        mark2.writeimage("_mark2")
    wshed2 = pink.watershed(inv(grad), mark2, 4)
    if DEBUG:
        wshed2.writeimage("_wshed2")
    return(inv(wshed2))

# detect joints
def analyzejoints(image, seg):
    # extracts thin pores
    ball = pink.genball(3)
    tophat = pink.erosion(pink.dilation(image, ball), ball) - image
    if DEBUG:
        tophat.writeimage("_tophat")
    thinpores = pink.seuil(tophat, 17)
    if DEBUG:
        thinpores.writeimage("_thinpores")
    # extracts thin pores
    black = inv(pink.seuil(image, 150))
    thickpores = pink.segmentarea(black, 4, 100, 0)
    thickpores = pink.dilation(thickpores, pink.genball(1))
    if DEBUG:
        thickpores.writeimage("_thickpores")
    # extracts joints
    joint = seg - thickpores
    if DEBUG:
        joint.writeimage("_joint")
    jointlen = area(joint)
    print("longueur totale des joints : "+str(jointlen))
    # extracts coroded parts of joints
    corojoint = pink.min(pink.dilation(thinpores, pink.genball(1)), joint)
    if DEBUG:
        corojoint.writeimage("_corojoint")
        pink.surimp(joint, corojoint, "_res")
    corojointlen = area(corojoint)
    print("longueur des joints corrodes : "+str(corojointlen))

    return()

joints = pink.readimage("../images/joints.pgm")
seg = segment(joints, 6)
imview(seg)
analyzejoints(joints, seg)














# LuM end of file
