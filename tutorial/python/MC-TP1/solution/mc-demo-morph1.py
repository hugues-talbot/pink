# Michel Couprie, 2011
# CeCILL free-software license

# illustration of basic binary morphological operators

from pink import imview
from pink import manipulate as manipulate
from pink import cpp as pink

global image
image = pink.readimage("../images/blobs.pgm")

# print("Demonstration des operateurs morphologiques de base")
# print("Activer l'option 'Superimpose' dans l'interface")
# print("Cliquer sur 'Exit' pour passer au suivant")
# print("             -----------")
print("\n\nDemonstration of basic morphological operators")
print("Check the 'Superimpose' button in the window")
print("Click 'Exit' to close the window and continue")
print("              ----------------")

# print("demonstration de l'erosion")
print("Demonstration of erosions")

# print("  - es = boule de la 4-distance")
print("  - structuring element (se) = 4-distance ball")
def try_erosion4(radius): return pink.erosball(image, radius, 4)
manipulate(try_erosion4, 0, 40, image)

# print("  - es = boule de la 8-distance")
print("  - se = 8-distance ball")
def try_erosion8(radius): return pink.erosball(image, radius, 8)
manipulate(try_erosion8, 0, 40, image)

# print("  - es = boule euclidienne")
print("  - se = Euclidean ball")
def try_erosion(radius): return pink.erosball(image, radius)
manipulate(try_erosion, 0, 40, image)

# print("demonstration de la dilatation")
# print("  - es = boule de la 4-distance")

print("Demonstration of dilations")
print("  - se = 4-distance ball")
def try_dilation4(radius): return pink.dilatball(image, radius, 4)
manipulate(try_dilation4, 0, 100, image)

# print("  - es = boule de la 8-distance")
print("  - se = 8-distance ball")
def try_dilation8(radius): return pink.dilatball(image, radius, 8)
manipulate(try_dilation8, 0, 100, image)

# print("  - es = boule euclidienne")
print("  - se = Euclidean ball")
def try_dilation(radius): return pink.dilatball(image, radius)
manipulate(try_dilation, 0, 100, image)

# print("demonstration de l'ouverture")
print("Demonstration of morphological opening")
def try_opening(radius): return pink.openball(image, radius)
manipulate(try_opening, 0, 40, image)

# print("demonstration de la fermeture")
print("Demonstration of morphological closing")
def try_closing(radius): return pink.closeball(image, radius)
manipulate(try_closing, 0, 100, image)

global image2
image2 = pink.openball(image, 29)

# print("demonstration de la dilatation geodesique")
print("Demonstration of geodesic dilation")
# print("  - es = 4-voisinage")
print("  - se = 4-distance ball")
def try_geodilat4(n): return pink.geodilat(image2, image, 4, n)
manipulate(try_geodilat4, 0, 250, image)

# print("  - es = 8-voisinage")
print("  - se = 8-distance ball")
def try_geodilat8(n): return pink.geodilat(image2, image, 8, n)
manipulate(try_geodilat8, 0, 250, image)

# LuM end of file
