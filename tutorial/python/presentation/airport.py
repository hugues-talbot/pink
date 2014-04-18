# UjoImro, 2010

from pink import *
pink = cpp

debug = False

airport = pink.readimage("images/airport.pgm")

imview(airport)

def th(q):
    return pink.seuil(airport, q)

manipulate(th, 0, 255)

# sol_airport
# runway extraction
def extract_runways( image, brightness_threshold=23, beed_filter_radius=3 ):
    seuil = pink.seuil( image, brightness_threshold )
    inv = pink.inverse(seuil)
    asf = pink.asfbin(inv, beed_filter_radius)
    #imview(asf)
    skeleton = pink.skeleton(asf, 0, 8)
    ptcurve = pink.ptcurve(skeleton, 8)
    dilated = pink.geodilat(ptcurve, asf, 8)
    skelcurv = pink.skelcurv(dilated, 0, 8)
    return skelcurv
    

def ext(q):
    return extract_runways(airport, 23, q)

manipulate(ext, 0, 100, airport)
    
runways = extract_runways(airport)

if debug: 
    runways.writeimage("runways.pgm")
    pink.surimp(airport, runways, "surimp.ppm")

if debug:
    imview([airport, runways])















# LuM end of file
