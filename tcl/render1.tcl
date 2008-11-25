#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1

# example from the tutorial by W.J. Shroeder & al
package require vtk

# set window title
wm title . "Simple Renderer"

# get input image file name as first argument
if {$argc != 1} { 
  puts stderr "usage: render in"
  exit 
}
set infilename [lindex $argv 0]

# create renderer and window
vtkRenderer ren1
    ren1 SetViewport 0 0 1 1
    ren1 SetBackground 0 0 0.7
vtkRenderWindow renWin
    renWin SetSize 600 600
    renWin AddRenderer ren1 

# load geometry
vtkPolyDataReader reader
    reader SetFileName $infilename

# smoothing (Taubin method)
vtkWindowedSincPolyDataFilter smooth
    smooth SetInput [reader GetOutput]
    smooth SetNumberOfIterations 15
    smooth BoundarySmoothingOn
    smooth SetFeatureAngle 120
    smooth SetEdgeAngle 90
    smooth SetPassBand 0.1

# compute normals
vtkPolyDataNormals normals
    normals SetInput [smooth GetOutput]
    normals FlipNormalsOn
# create mapper and actor
vtkPolyDataMapper mapper
    mapper SetInput [normals GetOutput]
vtkActor actor
    actor SetMapper mapper
    eval [actor GetProperty] SetInterpolationToPhong
#    eval [actor GetProperty] SetColor 1.0 0.59 0.35

ren1 AddProp actor
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
