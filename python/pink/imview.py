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

def imview(image,debug=False):
	"""A function to display an image in Pink/Python"""
	if (debug):
		PLiarEnableDebug()
	# start an imview
	port = pink.cpp.Pimview()
	# login to imview
	conn = pink.cpp.Pimviewlogin("","",port)
	# upload the data
	imagename = namestr(image,locals())[0]
	up = pink.cpp.Pimviewputimage(image,imagename,conn)
	return

class Imview:
	"""A class to display images in Pink/Python"""
	def __init__(self, image):
		# start an imview
		self.port=pink.cpp.Pimview()
		# login to that imview
		self.conn = pink.cpp.Pimviewlogin("","",self.port)
		# upload the data
		# self.imagename = namestr(image, locals())[0]
		self.up = pink.cpp.Pimviewputimage(image,"image",self.conn)
	def debug(self,d=False):
		self.debugstatus = pink.cpp.Pimview_setdebug(d)
	def show(self,image, name="image"):
		# reup an image
		self.up = pink.cpp.Pimviewputimage(image,name,self.conn)
		if (self.up > 0):
			# reinit oneself
			self.__init__(image)

		

# LuM end of file
