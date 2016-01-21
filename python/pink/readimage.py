# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
Readimage

reads an image in the PNM format

"""
import pink



def readimage( path ):
    """
        Reads an image in the PNM format
    """

    try:
        image=pink.cpp.readimage(path)
        return pink.pink2numpy(image)

    except:
        print("Read error (%s)" % path)
        return

# LuM end of file
