# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by aplicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
Solution of TP5 exercise 3 conserning fourier transformation
"""

from pink import *
import pink.cpp as pink

trois = pink.readimage("../images/trois003.pgm")
complex = pink.any2complex(trois, pink.char_image(trois.size))
fft = pink.fft(complex, 0)
cm = pink.complex_modulus(fft)
cm = pink.float2byte(cm, 4)

imview(cm)



























# LuM end of file
