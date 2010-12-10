# UjoImro, 2010
# CeCILL free-software license

# Michel's TP-3 for 3D images

from pink import view3d, render, seuilmesh
from pink import cpp as pink
inv = pink.inverse

lettre_a = pink.readimage("../images/lettre_a.pgm")
carotide = pink.readimage("../images/carotide.pgm")

#view3d(lettre_a)
#view3d(carotide)
#seuilmesh(carotide)

seuil = pink.seuil(carotide, 105)
dilated = pink.dilatball(seuil, 2)
#view3d(carotide, dilated)
diff = carotide - dilated

seuil = pink.seuil(diff, 94)
#view3d(carotide, seuil)
component = pink.selectcomp(seuil, 26, 58, 46, 0)
#view3d(carotide, component)
carotide_seg = component

###  the skeleton operator
axonepair = pink.readimage("../images/axonepair.pgm")
#imview(axonepair) ## HUGUES WRITE ME!
blank = pink.char_image(axonepair.size)
skeleton = pink.skeleton(axonepair, blank, 8)
#imview(skeleton)
pink.surimp(axonepair, skeleton, "axoskel.ppm")


###  distances
dist = pink.distc(axonepair, 4)
dist.writeimage("imagea.pgm")
### randrgb will be replaced with imview colourmap (rand rgb)
dist = pink.distc(axonepair, 8)
dist.writeimage("imageb.pgm")

dist = pink.distc(axonepair, 0)
dist.writeimage("imagec.pgm")

### skeletons guided by a priority image
skeleton = pink.skeleton(axonepair, 4, 8)
pink.surimp(axonepair, skeleton, "imagea.ppm")

skeleton = pink.skeleton(axonepair, 8, 8)
pink.surimp(axonepair, skeleton, "imageb.ppm")

skeleton = pink.skeleton(axonepair, 0, 8)
pink.surimp(axonepair, skeleton, "imagec.ppm")

### skeleton with forbidden points
blank[[85,93]] = 255
blank[[201, 199]] = 255
skeleton = pink.skeleton(axonepair, 0, 8, blank)
pink.surimp(axonepair, skeleton, "image.ppm")

### curvilinear skeleton
skelcurv = pink.skelcurv(axonepair, 4, 4)
pink.surimp(axonepair, skelcurv, "imagea.ppm")

skelcurv = pink.skelcurv(axonepair, 8, 8)
pink.surimp(axonepair, skelcurv, "imageb.ppm")

skelcurv = pink.skelcurv(axonepair, 0, 4)
pink.surimp(axonepair, skelcurv, "imagec.ppm")

skelcurv = pink.skelcurv(axonepair, 0, 8)
pink.surimp(axonepair, skelcurv, "imaged.ppm")

### 3D curvilinear skeleton
skelcurv = pink.skelcurv(lettre_a, 6, 6)
#render(lettre_a, pink.inverse(skelcurv))

### curvilinear skeleton of the carotide
skelcurv = pink.skelcurv(carotide_seg, 0, 26)
#render(skelcurv, skelcurv)
# we can see, that there's a cavity and a cycle in the skeleton
closed = pink.closeball(carotide_seg, 1)
skelcurv = pink.skelcurv(closed, 0, 26)
#render(skelcurv)
# now we eliminate the border points
skelcurv = skelcurv - pink.frame(pink.char_image(skelcurv.size),255)
#render(skelcurv)
skelcurv = pink.skelcurv(skelcurv, 0, 26)
#render(carotide_seg, skelcurv)


### the image of the brain
cerveau = pink.readimage("../images/cerveau.pgm")
#view3d(cerveau)
seuil = pink.seuil(cerveau, 95)
test_tp = pink.skeleton(seuil, 0, 6)
#render(test_tp)
blank = pink.char_image(cerveau.size)
blank[[85,80,5]]=255
blank = pink.inverse(blank)
#view3d(cerveau, blank)
cerveau_inv = pink.inverse(cerveau)
seuil_inv = pink.inverse(seuil)
skeleton = pink.skeleton(blank, cerveau_inv, 26, seuil_inv)
skeleton = pink.inverse(skeleton)
# decimate!
#render(skeleton, skeleton)

### Topology controlled opening
opened = pink.openball(skeleton, 1)
skeleton = pink.skeleton(skeleton, 6, 6, opened)
#render(skeleton, skeleton)

### surfasic skeleton
prioimage = pink.inverse(pink.distc(skeleton, 0))

skelsurf = pink.skelsurf(skeleton, prioimage, 6)
#render(skelsurf, skelsurf)
#render(skeleton, skelsurf)

### angiography of the liver
foie = pink.readimage("../images/foie.pgm")
# view3d(foie)
seuil = pink.seuil(foie,150)
# skeletonization for topology testing
skeleton = pink.skeleton(seuil, 0, 26)
#render(skeleton)

# homotopic growing
blank = pink.char_image(foie.size)
blank[[171, 126, 42]]=255
skeleton1 = pink.skeleton( inv(blank), inv(foie), 6, inv(seuil) )
#render(inv(skeleton))

# homotopic schrinking
framed = pink.frame(foie, 255)
skeleton2 = pink.skeleton( inv(framed), foie, 26, seuil )
#render(skeleton)
skelcurv = pink.skelcurv(skeleton2, 0, 26)
#render(skelcurv)

#render(skeleton2, skeleton1)



















# LuM end of file
