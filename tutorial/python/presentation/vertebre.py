# UjoImro, 2010
# CeCILL free-software license

# Michel's TP-4 for 3D images

from pink import *
pink = cpp

inv = pink.inverse

debug = False

lettre_a = pink.readimage("images/lettre_a.pgm")
if debug:
    view3d(lettre_a)

### General skeletonization with toposhrink
shrunk = pink.toposhrink(lettre_a, pink.distc(lettre_a,0), 26, 1, 1, 1, 1)
if debug: # for package generation
    view3d(shrunk)
# if debug: # for package generation
#render(lettre_a, shrunk)
render(lettre_a)
