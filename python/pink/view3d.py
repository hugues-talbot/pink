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


class canvas:
    def __init__(self, parent, side, color_x, color_y):
        self.parent = parent
        self.side = side
        self.pressed = False
        self.inside = False
        self.color_x = color_x
        self.color_y = color_y
        self.pos_x = 0
        self.pos_y = 0
        self.size = [ extractplane(self.parent.image, 0, self.side).size[0], extractplane(self.parent.image, 0, self.side).size[1] ]        
        self.canvas = tk.Canvas( self.parent.frame, width=self.size[0], height=self.size[1] )
        self.canvas.bind('<Button-1>', self.on_mouse_down )
        self.canvas.bind('<ButtonRelease-1>', self.on_mouse_up )
        self.canvas.bind('<Motion>', self.on_mouse_move )
        self.canvas.bind('<Enter>', self.on_mouse_enter )
        self.canvas.bind('<Leave>', self.on_mouse_leave )
        

    def on_refresh(self, n):
        self.tkimage = self.parent.to_photoimage(extractplane( self.parent.image, int(n), self.side ))
        self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )
        self.line_x = self.canvas.create_line( self.pos_x, 0, self.pos_x, self.size[1]-1, fill=self.color_x )
        self.line_y = self.canvas.create_line( 0, self.pos_y, self.size[0]-1, self.pos_y, fill=self.color_y )

    def on_mouse_down(self, n):
        self.pressed = True
        self.on_mouse_move(n)        

    def on_mouse_up(self, n):
        self.pressed = False        

    def on_mouse_leave(self,n):
        self.inside = False

    def on_mouse_enter(self,n):
        self.inside = True

    def on_mouse_move(self, n):
        if self.pressed and self.inside:
            self.parent.on_mouse_move(self.side, int(n.x), int(n.y))            

    def put_line(self, x, y):
        self.pos_x=x
        self.pos_y=y        
        

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
        self.quit.grid(row=6, column=3)

        self.canvas_x = canvas(parent=self, side="zy", color_x="green", color_y="blue" )
        self.canvas_x.canvas.grid(row=0, column=2)

        self.canvas_y = canvas(parent=self, side="xz", color_x="red", color_y="blue" )
        self.canvas_y.canvas.grid(row=1, column=1)

        self.canvas_z = canvas(parent=self, side="xy", color_x="red", color_y="green" )
        self.canvas_z.canvas.grid(row=0, column=1)

        self.scale_x = tk.Scale( self.frame, orient=tk.HORIZONTAL, length=self.image.size[0], from_=0, to=self.image.size[0]-1, command=self.update )
        self.scale_x.set( self.image.size[0]/2 )
        self.scale_x.grid(row=2,column=1)

        self.scale_y = tk.Scale( self.frame, orient=tk.HORIZONTAL, length=self.image.size[1], from_=0, to=self.image.size[1]-1, command=self.update )
        self.scale_y.set( self.image.size[1]/2 )
        self.scale_y.grid(row=3,column=1)

        self.scale_z = tk.Scale( self.frame, orient=tk.HORIZONTAL, length=self.image.size[2], from_=0, to=self.image.size[2]-1, command=self.update )
        self.scale_z.set( self.image.size[2]/2 )
        self.scale_z.grid(row=4,column=1)

        self.scale_seuil_low = tk.Scale( self.frame, label="Seuil", orient=tk.HORIZONTAL, length=256, from_=0, to=255, showvalue=1, command=self.update )
        self.scale_seuil_low.set(128)
        self.scale_seuil_low.grid(row=5,column=1)

        self.mode = tk.IntVar()
        self.mode.set(0)

        self.invertmode = False
        self.invert = tk.Checkbutton( self.frame, text="Invert", width=10, anchor="nw", command=self.on_invert_click )
        self.invert.grid(row=2, column=3)

        self.photob = tk.Radiobutton( self.frame, text="Image", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=0 )
        self.photob.grid(row=3,column=3)

        self.surimp = tk.Radiobutton( self.frame, text="Surimp", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=2 )
        self.surimp.grid(row=4, column=3)

        self.seuil = tk.Radiobutton( self.frame, text="Seuil (Threshold)", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=1 )
        self.seuil.grid(row=5,column=3)

        self.checkbutton_x = tk.Checkbutton( self.frame, anchor='se', text="X" )
        self.checkbutton_x.configure(selectcolor="red")
        self.checkbutton_x.grid(row=2, column=0)
        self.checkbutton_x.select()

        self.checkbutton_y = tk.Checkbutton( self.frame, anchor='se', text="Y" )
        self.checkbutton_y.configure(selectcolor="green")
        self.checkbutton_y.grid(row=3, column=0)
        self.checkbutton_y.select()

        self.checkbutton_z = tk.Checkbutton( self.frame, anchor='se', text="Z" )
        self.checkbutton_z.configure(selectcolor="blue")
        self.checkbutton_z.grid(row=4, column=0)
        self.checkbutton_z.select()

    def canvas_x_on_mouse_up(self, n):
        print "release me! n =", n

    def update(self, n=0):        
        self.canvas_x.on_refresh(self.scale_x.get())
        self.canvas_y.on_refresh(self.scale_y.get())
        self.canvas_z.on_refresh(self.scale_z.get())
        self.canvas_x.put_line( int(self.scale_y.get()), int(self.scale_z.get()))
        self.canvas_y.put_line( int(self.scale_x.get()), int(self.scale_z.get()))
        self.canvas_z.put_line( int(self.scale_x.get()), int(self.scale_y.get()))

    def on_mouse_move(self, side, a, b):
        if side == "zy":
            self.scale_z.set(a)
            self.scale_y.set(b)
        elif side == "xz":
            self.scale_x.set(a)
            self.scale_z.set(b)
        elif side == "xy":
            self.scale_x.set(a)
            self.scale_y.set(b)
        else:
            raise "internal error bad slice mode"

        self.update()

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