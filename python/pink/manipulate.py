# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by aplicable law.
  
# (C) UjoImro, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
# ujoimro@gmail.com

"""
This module contains a function for finding optimal parameters of 
a segmentation by substituting them interactively using a graphical
scalebar. The idea comes from Mathematica's Manipulate function.

Note: part of this file has been generated with Rapyd-TK
"""

from Tkinter import *
from pink.cpp import max as pink_max
from pink import to_photoimage, to_rgb_photoimage

root = Tk()
root.withdraw()

class app(Frame):
    parameter_value = -1
    def __init__(self, Master, function_name, minval, maxval):
        self.function_name = function_name
        self.minval = minval
        self.maxval = maxval
        self.size = function_name(minval).size
        
        apply(Frame.__init__,(self,Master))
        self.__Frame2 = Frame(self)
        self.__Frame2.pack(side='top')
        self.image = Canvas(self.__Frame2, width=self.size[0], height=self.size[1])
        self.image.pack(expand='yes',fill='both',side='top')
        self.__Frame1 = Frame(self)
        self.__Frame1.pack(side='top')
        self.__Frame4 = Frame(self.__Frame1)
        self.__Frame4.pack(side='left')
        self.valuescale = Scale(self.__Frame4,command=self.on_valuescale_command
            ,orient='horizontal', from_=minval, to=maxval)
        self.valuescale.pack(side='top')
        self.seuilcheck = Checkbutton(self.__Frame4
            ,command=self.on_seuilcheck_command,text='Superimpose')
        self.seuilcheck.pack(anchor='w',side='top')
        self.__Frame3 = Frame(self.__Frame1)
        self.__Frame3.pack(side='left')
        self.exitbutton = Button(self.__Frame3
            ,command=self.on_exitbutton_command,text='Exit')
        self.exitbutton.pack(anchor='w',side='top')

    def on_exitbutton_command(self,Event=None):
        self.quit()

    def on_seuilcheck_command(self,Event=None):
        pass

    def on_valuescale_command(self,Event=None):
        self.parameter_value = self.valuescale.get()
        tmpimage = self.function_name(self.parameter_value)
        self.tkimage = to_photoimage(tmpimage, master=self.__Frame2 )
        self.gui_image = self.image.create_image( 1, 1, image=self.tkimage, anchor="nw" )
        

def manipulate(function_name, minval=0, maxval=100):
    global root 
    top = Toplevel(root)
    application = app(top, function_name, minval, maxval)
    application.pack(expand='yes',fill='both')
    top.mainloop()
    top.withdraw()
    return application.parameter_value

# LuM end of file
