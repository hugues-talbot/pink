# Michel Couprie, 2011
# CeCILL free-software license

# illustration of basic binary morphological operators

from pink import imview
from pink import manipulate as manipulate
from pink import cpp as pink

global image
image = pink.readimage("../images/blobs.pgm")

def try_erosion4(radius):
    res = pink.erosball(image, radius, 4)
    return res

def try_erosion8(radius):
    res = pink.erosball(image, radius, 8)
    return res

def try_erosion(radius):
    res = pink.erosball(image, radius)
    return res

def try_dilation4(radius):
    res = pink.dilatball(image, radius, 4)
    return res

def try_dilation8(radius):
    res = pink.dilatball(image, radius, 8)
    return res

def try_dilation(radius):
    res = pink.dilatball(image, radius)
    return res

def try_opening(radius):
    res = pink.openball(image, radius)
    return res

def try_closing(radius):
    res = pink.closeball(image, radius)
    return res
print("Demonstration des operateurs morphologiques de base")
print("Activer l'option 'Superimpose' dans l'interface")
print("Cliquer sur 'Exit' pour passer au suivant")
print("             -----------")
print("demonstration de l'erosion")
print("  - es = boule de la 4-distance")
manipulate(try_erosion4, 0, 40, image)
print("  - es = boule de la 8-distance")
manipulate(try_erosion8, 0, 40, image)
print("  - es = boule euclidienne")
manipulate(try_erosion, 0, 40, image)
print("demonstration de la dilatation")
print("  - es = boule de la 4-distance")
manipulate(try_dilation4, 0, 100, image)
print("  - es = boule de la 8-distance")
manipulate(try_dilation8, 0, 100, image)
print("  - es = boule euclidienne")
manipulate(try_dilation, 0, 100, image)
print("demonstration de l'ouverture")
manipulate(try_opening, 0, 40, image)
print("demonstration de la fermeture")
manipulate(try_closing, 0, 100, image)

global image2
image2 = pink.openball(image, 29)

def try_geodilat4(n):
    res = pink.geodilat(image2, image, 4, n)
    return res

def try_geodilat8(n):
    res = pink.geodilat(image2, image, 8, n)
    return res

print("demonstration de la dilatation geodesique")
print("  - es = 4-voisinage")
manipulate(try_geodilat4, 0, 250, image)
print("  - es = 8-voisinage")
manipulate(try_geodilat8, 0, 250, image)

# LuM end of file
