# UjoImro, 2010
# CeCILL free-software license

from pink import *
pink = cpp


# sol_muscle
# extract the cells of a musle-fiber-cut image
def extract_cells(image, threshold=24):

    # creating the structuring element 
    elem = pink.char_image([3,3])
    elem.fill(1)
    elem.center=[1,1]

    grad = pink.gradmorph( muscle, elem )
    seuil = pink.seuil( grad, threshold )
    frame = pink.frame( pink.char_image(image.size), 255 )
    dilated = pink.geodilat( frame, seuil, 8 )
    skeleton = pink.skeleton( dilated, 0, 8 )
    inv = pink.inverse(skeleton)
    eroded = pink.erosball( inv, 5 )
    inv = pink.inverse(eroded)
    skeleton2 = pink.skeleton( inv, image, 4 )

    return skeleton2

muscle = pink.readimage("images/muscle.pgm")
imview(muscle)

cells = extract_cells(muscle)

def th(q):
    return pink.seuil(muscle, q)
manipulate(th, 0, 255)

elem = pink.char_image([3,3])
elem.fill(1)
elem.center=[1,1]

grad = pink.gradmorph( muscle, elem )
imview(grad)

def ec(q):
    return extract_cells(muscle, q)

manipulate(ec, 0, 100, muscle)






#imview(cells)

# LuM end of file
