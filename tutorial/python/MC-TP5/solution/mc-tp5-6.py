# Michel Couprie, 2011
# CeCILL free-software license

# segmentation of 3D concrete 

from pink import view3d, render, seuilmesh
from pink import imview as imview
from pink import cpp as pink
global DEBUG
DEBUG=1

img = pink.readimage("../images/mortier_3d.pgm")


# LuM end of file
