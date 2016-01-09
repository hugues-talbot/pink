# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.

# (C) M. Couprie <coupriem@esiee.fr>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

# watersheds 

import pink

def MeyerWatershed(image, markers, connectivity, mask=None, lines=False):
    """
    Wrapper to the Meyer pixelwise watershed
    Args : image must be an 8-bit image
    markers must be a 32-bit integer image of labels
    connectivity must be 4 or 8 for 2D images, or 6, 18 or 26 for 3D images
    mask is a binary optinal image
    lines is false by default but if true, borders between labels is shown explicitly
    """
    if (image.imtype() != 'uint8_t'):
            print("** Input image must be Unsigned Char (uint8)\n")
            return None
    if (markers.imtype() != 'int32_t'):
            print('** Marker image must be Signed Int (int32)\n')
            return None

    markers_overw = markers.clone() ## hopefully a deep copy

    ## The Meyer watershed implementation overwrite the marker image
    if (connectivity == 4 or connectivity == 8):
        if ( not lines ):
            if (mask != None):
                pink.cpp.watershedMeyerlabnoline(image, markers_overw, mask, connectivity)
            else:
                mask = pink.cpp.seuil(image,0)
                pink.cpp.watershedMeyerlabnoline(image, markers_overw, mask, connectivity)
        else:
            if (mask == None):
                pink.cpp.watershedMeyer2labnomask(image, markers_overw, connectivity)
            else:
                pink.cpp.watershedMeyer2lab(image, markers_overw, mask, connectivity)
    else: 
        ## assuming 3D
        if ( not lines):
            pink.cpp.watershedMeyer3Dlabnoline(image, markers_overw, mask, connectivity)
        else:
            print("** Sorry, 3D Meyer watershed with separating surfaces not implemented\n")
            return None

    return markers_overw
