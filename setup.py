# UjoImro, 2011
# Pink's Python installer
# This file is used to generate a Python installer
# executable on Microsoft Windows


from distutils.core import setup

setup( name         = 'pink-image',
       version      = '0.9.r557',
       description  = 'Pink Image Processing Library',
       author       = 'Michel Couprie et al.',
       author_email = 'pink-image-devel@lists.berlios.de',
       license      = 'CeCILL',
       url          = 'http://www.pinkhq.com',
       packages     = [ 'pink', 'pink.cpp', 'pink.MICCAI' ],
       data_files = [ ('DLLs', ['libcpp_pink.pyd'])]
     )




# LuM end of file
