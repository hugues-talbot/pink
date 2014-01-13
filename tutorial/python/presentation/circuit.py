# UjoImro, 2010
# presentation

# fill in the holes
def fill_the_holes(image):
    frame = pink.frame(pink.char_image(image.size), 255)
    inv = pink.inverse(image)
    dilated = pink.geodilat(frame, inv, 8)
    result = pink.inverse(dilated)
    return result


# extraction of the wires from a circuit
def find_the_wires(image, seuil):
    # binarize 
    binary = pink.seuil(image, seuil, 0, 255)
    inv = pink.inverse(binary)

    # eliminate the little objects
    #filtered = filter(inv, 2, 8)
    eros = pink.erosball(inv, 2)
    filtered = pink.geodilat(eros, inv, 8)

    # fill the holes
    filled = fill_the_holes(filtered)

    # detect the joints
    eros = pink.erosball(filled, 6)
    dilat = pink.dilatball(eros, 6)
    joints = pink.geodilat(dilat, filled, 8, 1)

    # extract the wires
    result = filled - joints

    return result


def try_seuil(s):
    return pink.seuil(circuit, s, 0, 255)

circuit = pink.readimage("images/circuit.pgm")
seuil = manipulate(try_seuil, 0, 255, circuit)

def fw(seuil):
    return find_the_wires(circuit, seuil)

manipulate(fw, 0, 100, circuit)

if debug: imview([circuit, wires])
#wires.writeimage("wires.pgm")













# LuM end of file
