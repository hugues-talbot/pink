# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
Erosball

Morphological binary erosion by a metric ball.

"""
import pink


def erosball( image, radius_opening, mode=0 ):
    """
        Morphological binary erosion by a metric ball.
    """

    try:
        image=pink.numpy2pink(image)
        image=pink.cpp.erosball(image, radius_opening, mode)
        return pink.pink2numpy(image)

    except:
        print("erosball error")
        return

# LuM end of file
