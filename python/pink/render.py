# -*- python -*-
# UjoImro, 2010

import vtk
from vtk.tk.vtkTkRenderWindowInteractor import vtkTkRenderWindowInteractor
from vtk.tk.vtkTkRenderWidget import vtkTkRenderWidget
import Tkinter as tk
import tkSimpleDialog
from pink.cpp import mcube

class render_class:
    def __init__(self, mesh1, mesh2=0):
        self.name = "render"
        self.rs = 600
        self.cs = 600
        self.mesh1 = mesh1
        self.mesh2 = mesh2
        self.ren1 = vtk.vtkRenderer()
        self.ren1.SetViewport([0, 0, 1, 1])
        self.ren1.SetBackground([0.93, 0.93, 0.97])
        self.ren_win = vtk.vtkRenderWindow()
        self.ren_win.AddRenderer(self.ren1)
        
        # load geometry
        ## first object
        self.obj1 = obj(mesh1)        
        self.ren1.AddViewProp(self.obj1.objActor)

        ## optional second object
        if mesh2!=0:
            self.obj2 = obj(mesh2)
            self.obj2.objActor.GetProperty().SetColor([1.0, 0.49, 0.25])
            self.ren1.AddViewProp(self.obj2.objActor)

        self.ren1.ResetCamera()


class obj:
    def __init__(self, mesh):
        self.obj = vtk.vtkPolyDataReader()
        self.obj.SetInputString(mesh)
        self.obj.ReadFromInputStringOn()
        self.normals = vtk.vtkPolyDataNormals()
        self.normals.SetInput(self.obj.GetOutput())
        self.normals.FlipNormalsOn()
        self.objMapper = vtk.vtkPolyDataMapper()
        self.objMapper.SetInput(self.normals.GetOutput())
        self.objActor = vtk.vtkActor()
        self.objActor.SetMapper(self.objMapper)

    
class app:
    def __init__(self, render):
        self.render = render # the vtk renderer object

        # # then binds rendering window to a tk widget
        self.root = tk.Tk()
        self.root.title("Multiple Renderer")
        self.vtkw = vtkTkRenderWidget(self.root, width=600, height=600, rw=self.render.ren_win)
        self.vtkw.BindTkRenderWidget()

        self.commands = tk.Frame(self.root)
        self.buttons = tk.Frame(self.root)

        self.buttons.pack( fill="both", expand=1, side="top" )
        self.commands.pack( fill="both", expand=1, side="bottom" )
        self.quitbutton = tk.Button(self.buttons, text="Exit", activebackground="white", command=self.bye)
        self.savebutton = tk.Button(self.buttons, text="Save", activebackground="white", command=self.savepng)        
        self.wirebutton1 = wirebutton(frame=self.buttons, text="Wireframe1", ren_win=self.render.ren_win, obj_actor=self.render.obj1.objActor)

        if render.mesh2!=0:
            self.wirebutton1 = wirebutton(frame=self.buttons, text="Wireframe2", ren_win=self.render.ren_win, obj_actor=self.render.obj2.objActor)

        self.opacity = tk.Scale(self.commands, from_=0.0, to=1.0, res=0.01, orient="horizontal", label="Opacity", command=self.set_opacity)
                
        self.quitbutton.pack(side="right")
        self.savebutton.pack(side="left")        
        self.vtkw.pack(side="top", fill="both", expand="yes")
        self.opacity.pack(side="top", anchor="nw", fill="both")
        

    def set_opacity(self, f):
        self.render.obj1.objActor.GetProperty().SetOpacity(float(f))
        self.render.ren_win.Render()

    def mainloop(self):
        self.root.mainloop()
    
    def bye(self):
    #vtkCommand DeleteAllObjects
        self.root.quit()


    def savepng(self):
        
        w2i=vtk.vtkWindowToImageFilter()
        writer = vtk.vtkPNGWriter()
        w2i.SetInput(self.render.ren_win)
        writer.SetInput(w2i.GetOutput())
        writer.SetFileName(str(tkSimpleDialog.askstring( "Save a Snapshot", "Specify the filename to save a snapshot:", initialvalue="_sav01.png", parent=self.root)))
        writer.Write()
        



class wirebutton:
    def __init__(self, frame, text, ren_win, obj_actor):
        self.ren_win = ren_win
        self.obj_actor = obj_actor
        self.wireframe = tk.IntVar()
        self.wireframe.set(0)
        self.button = tk.Checkbutton(frame, text=text, variable=self.wireframe, activebackground="white", command=self.togglewire)
        self.button.pack(side="left")

    def togglewire(self):
        if int(self.wireframe.get())==1:
            self.obj_actor.GetProperty().SetRepresentationToWireframe()
        else:
            self.obj_actor.GetProperty().SetRepresentationToSurface()
            
        self.ren_win.Render()



# def renderfiles( filename1, filename2="" ):
#     render_obj = render_class( filename1, filename2 )
#     #renderer( "/home/ujoimro/doc/projects/Tutorial-Pink/MC-TP3/solution/TP3cor/lettre_a.vtk",
#     #                     "/home/ujoimro/doc/projects/Tutorial-Pink/MC-TP3/solution/TP3cor/carotide_seg.vtk"
#     #                     )
#     application = app(render_obj)
#     application.mainloop()

def render( image1, image2=0 ):
    mesh1 = mcube(image1, 93, 0, 0)
    if image2!=0:
        mesh2 = mcube(image2, 93, 0, 0)
    else:
        mesh2 = 0

    render_obj=render_class(mesh1, mesh2)
    application = app(render_obj)
    application.mainloop()
    #mesh2 = 
 






# LuM end of file