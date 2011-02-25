# UjoImro, 2010
# CeCILL free-software license

# Hugues' TP-1

from pink import cpp as pink

#ex 3.1-1
#help(pink.geoeros)

#ex 3.1-2
I=pink.readimage("../images/cells.pgm")
eroded = pink.erosball( I, 1, 0 )
#show(eroded)

#ex 3.1-3
recons1 = pink.geodilat(eroded, I, 8, 10)  
#recons1.writeimage("recons1.pgm")


#ex 3.1-4
dilated = pink.dilatball(I, 1, 0)
recons2 = pink.geoeros(dilated, I, 8, 10)
#recons2.writeimage("recons2.pgm")

#ex 3.1-5
def filter_noise(image):
    eroded  = pink.erosball( image, 1, 0 )
    recons1 = pink.geodilat(eroded, image, 8, 10)
    dilated = pink.dilatball(recons1, 2, 0)
    result  = pink.geoeros(dilated, recons1, 8, 10)
    return result

cells_filt = filter_noise(I)
cells_filt.writeimage("cells_filt.pgm")


#ex 3.2-2
def remove_touching( image ):
    frame  = pink.frame(image, 0)
    seeds  = image - frame
    #seeds.writeimage("seeds.pgm")
    result = image - pink.geodilat(seeds, image, 8, 100)
    return result
    
cells_nohole = remove_touching(cells_filt)
cells_nohole.writeimage("cells_nohole.pgm")


#ex 3.3-2
inv = pink.inverse(cells_nohole)
holes = remove_touching(inv)

cells_filled = pink.max(cells_nohole, holes)
cells_filled.writeimage("cells_filled.pgm")


#ex 3.4-2
objects = pink.geodilat( holes, cells_filled, 8, 100 )
cells_final = pink.min( cells_nohole, objects )
cells_final.writeimage("cells_final.pgm")

#ex 4
## filtering and inverting
circuit = pink.readimage("../images/circuit.pgm")
inv = pink.inverse(circuit)
inv = pink.seuil(inv, 180, 0 ,255)
inv = filter_noise(inv)
inv.writeimage("circ.pgm")

## dilation
structuring_element = pink.char_image([11,1])
structuring_element.fill(255)
structuring_element.writeimage("se.pgm")
structuring_element.center=[5,0]
dilated = pink.dilation( inv, structuring_element )
dilated.writeimage("dil.pgm")













# LuM end of file
