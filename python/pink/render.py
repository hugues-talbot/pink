# -*- python -*-
# UjoImro, 2010

import vtk
from vtk.tk.vtkTkRenderWindowInteractor import vtkTkRenderWindowInteractor
from vtk.tk.vtkTkRenderWidget import vtkTkRenderWidget
import Tkinter as tk
import tkSimpleDialog
from pink.cpp import mcube


def bye():
    root.quit()



def savepng():        
    global root
    w2i=vtk.vtkWindowToImageFilter()
    writer = vtk.vtkPNGWriter()
    w2i.SetInput(ren_win)
    writer.SetInput(w2i.GetOutput())
    writer.SetFileName(str(tkSimpleDialog.askstring( "Save a Snapshot", "Specify the filename to save a snapshot:", initialvalue="_sav01.png", parent=root)))
    writer.Write()


def togglewire1():
    if int(wireframe1.get())==1:
        objActor1.GetProperty().SetRepresentationToWireframe()
    else:
        objActor1.GetProperty().SetRepresentationToSurface()        
    ren_win.Render()

def togglewire2():
    if int(wireframe2.get())==1:
        objActor2.GetProperty().SetRepresentationToWireframe()
    else:
        objActor2.GetProperty().SetRepresentationToSurface()        
    ren_win.Render()


def set_opacity(f):
    global objActor1
    global ren_win

    objActor1.GetProperty().SetOpacity(float(f))
    ren_win.Render()



root = tk.Toplevel()
ren1 = vtk.vtkRenderer()
ren_win = vtk.vtkRenderWindow()
ren_win.AddRenderer(ren1)

pdr1 = vtk.vtkPolyDataReader()
pdr2 = vtk.vtkPolyDataReader()
normals1 = vtk.vtkPolyDataNormals()
normals2 = vtk.vtkPolyDataNormals()
objMapper1 = vtk.vtkPolyDataMapper()
objMapper2 = vtk.vtkPolyDataMapper()
objActor1 = vtk.vtkActor()
objActor2 = vtk.vtkActor()
ren1.AddViewProp(objActor1)
ren1.AddViewProp(objActor2)
vtkw = vtkTkRenderWidget(root, width=600, height=600, rw=ren_win)

# # then binds rendering window to a tk widget        
root.title("Multiple Renderer")

vtkw.BindTkRenderWidget()

commands = tk.Frame(root)
buttons = tk.Frame(root)

buttons.pack( fill="both", expand=1, side="top" )
commands.pack( fill="both", expand=1, side="bottom" )
quitbutton = tk.Button(buttons, text="Exit", activebackground="white", command=bye)
savebutton = tk.Button(buttons, text="Save", activebackground="white", command=savepng)        

wireframe1 = tk.IntVar()
wireframe1.set(0)
button = tk.Checkbutton(buttons, text="Wireframe1", variable=wireframe1, activebackground="white", command=togglewire1)
button.pack(side="left")

wireframe2 = tk.IntVar()
wireframe2.set(0)
button = tk.Checkbutton(buttons, text="Wireframe2", variable=wireframe2, activebackground="white", command=togglewire2)
button.pack(side="left")




opacity = tk.Scale(commands, from_=0.0, to=1.0, res=0.01, orient="horizontal", label="Opacity", command=set_opacity)

quitbutton.pack(side="right")
savebutton.pack(side="left")        
vtkw.pack(side="top", fill="both", expand="yes")
opacity.pack(side="top", anchor="nw", fill="both")

    









def render_images(mesh1, mesh2=0):
    global ren1        
    global ren_win
    
    ren1.SetViewport([0, 0, 1, 1])
    ren1.SetBackground([0.93, 0.93, 0.97])
    
    pdr1.SetInputString(mesh1)
    pdr1.ReadFromInputStringOn()
    normals1.SetInput(pdr1.GetOutput())
    normals1.FlipNormalsOn()
    objMapper1.SetInput(normals1.GetOutput())
    objActor1.SetMapper(objMapper1)


    # optional second object
    if mesh2!=0:        
        objActor2.GetProperty().SetColor([1.0, 0.49, 0.25])
        pdr2.SetInputString(mesh2)
        pdr2.ReadFromInputStringOn()
        normals2.SetInput(pdr2.GetOutput())
        normals2.FlipNormalsOn()
        objMapper2.SetInput(normals2.GetOutput())
        objActor2.SetMapper(objMapper2)

        
        
    ren1.ResetCamera()
    ren_win.Render()

    
def render(image1, image2=0):
    global root 
    #global application
    mesh1 = mcube(image1, 0, 0, 0)
    if image2!=0:
        mesh2 = mcube(image2, 0, 0, 0)
    else:
        mesh2 = 0

    render_images(mesh1, mesh2)
    print "Rendering"
    root.mainloop()



# LuM end of file
