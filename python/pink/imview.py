#! /usr/bin/python
# -*- python -*-
# The Famous Imview Viewer
#
"""

This module prototypes interaction with the imview viewer

"""

import pink

def namestr(obj, namespace):
	return [name for name in namespace if namespace[name] is obj]

def imview(image):
    # start an imview
    port = pink.cpp.Pimview()
    # login to imview
    conn = pink.cpp.Pimviewlogin("","",port)
    # upload the data
    imagename = namestr(image,locals())[0]
    up = pink.cpp.Pimviewputimage(image,imagename,conn)
    return




# LuM end of file
