# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009-2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
This module contains the conversion functions for 
Python Tkinter visualisation
"""

from pink import python_component_missing
from numpy import copy

try:
    from PIL import Image, ImageTk
except:
    print("error: could not import PIL.Image or PIL.ImageTk, try to install python-imaging and python-imaging-tk")
    raise python_component_missing


def to_photoimage(image, master=0):
    tmpimage = copy(image)
    tmpimage.shape = (image.shape[1], image.shape[0])
    if master==0:
        return ImageTk.PhotoImage(Image.fromarray(tmpimage))
    else:
        return ImageTk.PhotoImage(Image.fromarray(tmpimage), master=master )

def to_rgb_photoimage(images, master=0):
    assert( len(images)==3 )
    arrays = map( Image.fromarray, images )
    imrgb = Image.merge('RGB', ( arrays[0], arrays[1], arrays[2] ) ) 

    return ImageTk.PhotoImage( imrgb, master=master )
