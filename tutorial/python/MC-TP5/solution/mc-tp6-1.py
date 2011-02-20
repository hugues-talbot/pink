# Michel Couprie, 2011
# CeCILL free-software license

# Segmentation of cells

from pink import imview as imview
from pink import cpp as pink
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
    blank = pink.char_image(im.size)
    wshed1 = pink.watershedMeyer2lab(mark1, inv(im), inv(blank), 4)
    if DEBUG:
        wshed1.writeimage("_wshed1")

    return wshed1

im = pink.readimage("uo.pgm")
seg = segment(im, 20)

# LuM end of file
