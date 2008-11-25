#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1

# A simple renderer - Michel Couprie 2002
package require vtk
#package require vtkinteraction

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
vtkPolyDataReader obj1
    obj1 SetFileName $infilename
# compute normals
vtkPolyDataNormals normals1
    normals1 SetInput [obj1 GetOutput]
    normals1 FlipNormalsOn
# create mapper and actor
vtkPolyDataMapper obj1Mapper
    obj1Mapper SetInput [normals1 GetOutput]
vtkActor obj1Actor
    obj1Actor SetMapper obj1Mapper

ren1 AddProp obj1Actor
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

# command zone
toplevel .commandwin
frame .commandwin.commands
frame .commandwin.buttons
set quitbutton [button .commandwin.buttons.quit -text "Quit" -command RENDER_bye\
                -activebackground white]
# what to do in case of window closing:
wm protocol .commandwin WM_DELETE_WINDOW RENDER_bye 
wm protocol . WM_DELETE_WINDOW RENDER_bye 
bind . "q" RENDER_bye

proc RENDER_bye {} {
    vtkCommand DeleteAllObjects
    exit
}
set wireframe 0
set wirebutton [checkbutton .commandwin.buttons.wire -text "Wireframe" \
                -variable wireframe -command RENDER_togglewire\
                -activebackground white]
proc RENDER_togglewire {} {
  global wireframe
  global renwin
  if ($wireframe) {
    eval [obj1Actor GetProperty] SetRepresentationToWireframe
  } else {
    eval [obj1Actor GetProperty] SetRepresentationToSurface
  }
  renWin Render
}
set opacity [scale .commandwin.commands.opacity \
        -from 0.0 -to 1.0 -res 0.05 \
        -orient horizontal \
        -label "Opacity:" \
        -command setOpacity]
$opacity set 1.0
proc setOpacity {opa} {
  global renwin
  eval [obj1Actor GetProperty] SetOpacity $opa
  renWin Render
}
pack .commandwin.buttons.wire -side left
pack .commandwin.buttons.quit -side right
pack .commandwin.commands.opacity -side top
pack .commandwin.commands .commandwin.buttons -side top -anchor nw -fill both
