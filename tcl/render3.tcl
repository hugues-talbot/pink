#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1 $2 $3

# example from the tutorial by W.J. Shroeder & al
package require vtk

# set window title
wm title . "Simple Renderer"

# get input image file name as first argument
if {$argc != 3} { 
  puts stderr "usage: render2 in1 in2 in3"
  exit 
}
set infilename1 [lindex $argv 0]
set infilename2 [lindex $argv 1]
set infilename3 [lindex $argv 2]

# create renderer and window
vtkRenderer ren1
    ren1 SetViewport 0 0 1 1
    ren1 SetBackground 0 0 0.7
vtkRenderWindow renWin
    renWin SetSize 600 600
    renWin AddRenderer ren1 

# load geometry
vtkPolyDataReader obj1
    obj1 SetFileName $infilename1
vtkPolyDataReader obj2
    obj2 SetFileName $infilename2
vtkPolyDataReader obj3
    obj3 SetFileName $infilename3

# compute normals
vtkPolyDataNormals normals1
    normals1 SetInput [obj1 GetOutput]
    normals1 FlipNormalsOn
vtkPolyDataNormals normals2
    normals2 SetInput [obj2 GetOutput]
    normals2 FlipNormalsOn
vtkPolyDataNormals normals3
    normals3 SetInput [obj3 GetOutput]
    normals3 FlipNormalsOn

vtkPolyDataMapper obj1Mapper
    obj1Mapper SetInput [normals1 GetOutput]
vtkPolyDataMapper obj2Mapper
    obj2Mapper SetInput [normals2 GetOutput]
vtkPolyDataMapper obj3Mapper
    obj3Mapper SetInput [normals3 GetOutput]

vtkActor obj1Actor
    obj1Actor SetMapper obj1Mapper
    eval [obj1Actor GetProperty] SetOpacity 0.5
vtkActor obj2Actor
    obj2Actor SetMapper obj2Mapper
    eval [obj2Actor GetProperty] SetColor 1.0 0.49 0.25
vtkActor obj3Actor
    obj3Actor SetMapper obj3Mapper
    eval [obj3Actor GetProperty] SetColor 0.25 0.25 0.25

ren1 AddProp obj1Actor
ren1 AddProp obj2Actor
ren1 AddProp obj3Actor
ren1 ResetCamera

# add interactor to the rendering window
vtkRenderWindowInteractor iren
iren SetRenderWindow renWin
iren Initialize
# add this for a normal exit
iren AddObserver ExitEvent {exit} 

renWin Render

# 
# Hide the default . widget
#
wm withdraw .

