# UjoImro, 2011

import pink

## #from pink import windowing
## #windowing.options.silent=False

I=pink.cpp.readimage("circuit2.pgm")

def circuit(value):
    global I
    return pink.cpp.seuil(I,value)

pink.manipulate(circuit, 0, 256)


















# LuM end of file
