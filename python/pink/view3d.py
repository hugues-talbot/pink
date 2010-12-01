# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by aplicable law.
  
# (C) UjoImro, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
# ujoimro@gmail.com

"""
pink's 3d slice visualizer
"""

import Tkinter as tk
from PIL import Image, ImageTk
from pink.cpp import extractplane, seuil, char_image, border, inverse
from pink.cpp import max as pink_max

def to_photoimage(image):
    size  = [ image.size[0], image.size[1] ]
    return ImageTk.PhotoImage(Image.frombuffer("L", size, image.get_pixels() , "raw", "L", 0, 1))


def to_rgb_photoimage(images):
    size  = [ images[0].size[0], images[0].size[1] ]
    tkimage_r = Image.frombuffer("L", size, images[0].get_pixels() , "raw", "L", 0, 1)
    tkimage_g = Image.frombuffer("L", size, images[1].get_pixels() , "raw", "L", 0, 1)
    tkimage_b = Image.frombuffer("L", size, images[2].get_pixels() , "raw", "L", 0, 1)
    tk_image_rgb = Image.merge( "RGB", [tkimage_r, tkimage_g, tkimage_b ])
    result = ImageTk.PhotoImage(tk_image_rgb.copy())
    return result


class main:
    def __init__(self, master, image):
        if len(image.size)!=3:
            raise "error: view3d is for 3D images only."

        self.master=master
        master.title("view3d")

        self.seuilmode=False
        self.surimpmode=False
        self.invertmode=False
        
        self.frame = tk.Frame(master, bg="", colormap="new")
        #self.frame.bind("<Button-1>", callback)
        self.frame.grid()
        self.image = image

        ### Buttons
        self.quit = tk.Button(self.frame, text="Exit", command=self.frame.quit, width=10 )
        self.quit.grid(row=10, column=10)

        self.canvas_x = tk.Canvas( self.frame, height=image.size[1], width=image.size[2] )
        self.canvas_x.grid(row=0, column=1)

        self.canvas_y = tk.Canvas( self.frame, height=image.size[2], width=image.size[0] )
        self.canvas_y.grid(row=1, column=0)

        self.canvas_z = tk.Canvas( self.frame, height=image.size[1], width=image.size[0] )
        self.canvas_z.grid(row=0, column=0)

        self.scale_x = tk.Scale( self.frame, label="X", orient=tk.HORIZONTAL, length=self.image.size[0], from_=0, to=self.image.size[0]-1, command=self.put_x )
        self.scale_x.set( self.image.size[0]/2 )
        self.scale_x.grid(row=2,column=0)

        self.scale_y = tk.Scale( self.frame, label="Y", orient=tk.HORIZONTAL, length=self.image.size[1], from_=0, to=self.image.size[1]-1, command=self.put_y )
        self.scale_y.set( self.image.size[1]/2 )
        self.scale_y.grid(row=3,column=0)

        self.scale_z = tk.Scale( self.frame, label="Z", orient=tk.HORIZONTAL, length=self.image.size[2], from_=0, to=self.image.size[2]-1, command=self.put_z )
        self.scale_z.set( self.image.size[2]/2 )
        self.scale_z.grid(row=4,column=0)

        self.scale_seuil_low = tk.Scale( self.frame, label="Seuil", orient=tk.HORIZONTAL, length=256, from_=0, to=255, showvalue=1, command=self.update )
        self.scale_seuil_low.set(128)
        self.scale_seuil_low.grid(row=5,column=0)

        self.mode = tk.IntVar()
        self.mode.set(0)

        self.photob = tk.Radiobutton( self.frame, text="Image", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=0 )
        self.photob.grid(row=6,column=10)
        
        self.seuil = tk.Radiobutton( self.frame, text="Seuil (Threshold)", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=1 )
        self.seuil.grid(row=8,column=10)

        self.surimp = tk.Radiobutton( self.frame, text="Surimp", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=2 )
        self.surimp.grid(row=7, column=10)

        
        self.invertmode = False
        self.invert = tk.Checkbutton( self.frame, text="Invert", width=10, anchor="nw", command=self.on_invert_click )
        self.invert.grid(row=5, column=10)


    def update(self, n=0):        
        self.put_x(self.scale_x.get())
        self.put_y(self.scale_y.get())
        self.put_z(self.scale_z.get())


    def extract_x(self, n):        
        image = extractplane( self.image, int(n), "zy" )
        size  = [ image.size[0], image.size[1] ]
        return self.to_photoimage(image), size
        
    def put_x(self, n):
        self.tkimage_x, size = self.extract_x(n)
        self.gui_image_x = self.canvas_x.create_image( 1, 1, image=self.tkimage_x, anchor="nw" )
        

    def extract_y(self, n):        
        image = extractplane( self.image, int(n), "xz" )
        size  = [ image.size[0], image.size[1] ]
        return self.to_photoimage(image), size
        
    def put_y(self, n):
        self.tkimage_y, size = self.extract_y(n)
        self.gui_image_y = self.canvas_y.create_image( 1, 1, image=self.tkimage_y, anchor="nw" )

    def extract_z(self, n):        
        image = extractplane( self.image, int(n), "xy" )
        size  = [ image.size[0], image.size[1] ]
        return self.to_photoimage(image), size
        
    def put_z(self, n):
        self.tkimage_z, size = self.extract_z(n)
        self.gui_image_z = self.canvas_z.create_image( 1, 1, image=self.tkimage_z, anchor="nw" )


    def on_mode_change(self):
        self.seuilmode = self.surimpmode = False

        if self.mode.get()==1:
            self.seuilmode=True
        elif self.mode.get()==2:
            self.surimpmode=True
            
        self.update()

    def on_invert_click(self):
        self.invertmode=not self.invertmode
        self.update()

    def to_photoimage(self, image):
        if self.seuilmode:
            image = seuil( image, int(self.scale_seuil_low.get() ))
            result = to_photoimage(image)
        elif self.surimpmode:
            bord = border(seuil(image, int(self.scale_seuil_low.get())), 8 )
            if self.invertmode:
                image = inverse(image)
            #inv_bord = inverse(bord)

            image_r = image
            image_g = pink_max( image, bord )
            image_b = pink_max( image, bord )
            result = to_rgb_photoimage([image_r, image_g, image_b ])

        else:
            if self.invertmode:
                image = inverse(image)
            result = to_photoimage(image)

        return  result 

def view3d(image):
    root = tk.Tk()
    main_window = main(root, image)
    root.mainloop()

    
















































# LuM end of file
