# Michel Couprie, 2011
# CeCILL free-software license

# Segmentation of cells

from pink import imview as imview
from pink import cpp as pink
from pink import show_histo
inv=pink.inverse
global DEBUG
DEBUG=1

# segmentation
def segment(im, height):
    # obtain markers
    mark1 = pink.segmentheight(im, 4, height, 0)
    if DEBUG:
        mark1.writeimage("_mark1")
    # label markers
    lab1 = pink.labelfgd(mark1, 4)

    # watershed
    wshed1 = pink.watershedMeyer2lab(lab1, inv(im), 4)
    if DEBUG:
        wshed1.writeimage("_wshed1")

    return wshed1

im = pink.readimage("../images/uo.pgm")
show_histo(im)
seg = segment(im, 20)

# LuM end of file
