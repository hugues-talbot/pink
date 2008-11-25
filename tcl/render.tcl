#ATTENTION: BUGGE avec la version NVIDIA (carte graphique)
#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1

# A simple renderer - Michel Couprie 2002
package require vtk
package require vtkinteraction

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
    renWin AddRenderer ren1 

# load geometry
vtkPolyDataReader obj1
    obj1 SetFileName $infilename
vtkPolyDataNormals normals1
    normals1 SetInput [obj1 GetOutput]
    normals1 FlipNormalsOn
vtkPolyDataMapper obj1Mapper
    obj1Mapper SetInput [normals1 GetOutput]
vtkActor obj1Actor
    obj1Actor SetMapper obj1Mapper

ren1 AddProp obj1Actor
ren1 ResetCamera

# then binds rendering window to a tk widget
set vtkw [vtkTkRenderWidget .ren \
        -width 600 \
        -height 600 \
        -rw renWin]
BindTkRenderWidget $vtkw
# command zone
frame .commands
frame .buttons
set quitbutton [button .buttons.quit -text "Quit" -command RENDER_bye\
                -activebackground white]
# what to do in case of window closing:
wm protocol . WM_DELETE_WINDOW RENDER_bye
bind . "q" RENDER_bye
 
proc RENDER_bye {} {
    vtkCommand DeleteAllObjects
    exit
}
set wireframe 0
set wirebutton [checkbutton .buttons.wire -text "Wireframe" \
                -variable wireframe -command RENDER_togglewire\
                -activebackground white]
proc RENDER_togglewire {} {
  global wireframe
  if ($wireframe) {
    eval [obj1Actor GetProperty] SetRepresentationToWireframe
  } else {
    eval [obj1Actor GetProperty] SetRepresentationToSurface
  }
  renWin Render
}
set opacity [scale .commands.opacity \
        -from 0.0 -to 1.0 -res 0.05 \
        -orient horizontal \
        -label "Opacity:" \
        -command setOpacity]
$opacity set 1.0
proc setOpacity {opa} {
    eval [obj1Actor GetProperty] SetOpacity $opa
    renWin Render
}
pack $wirebutton -side left
pack $quitbutton -side right
pack $opacity .buttons -side top -anchor nw -fill both
pack $vtkw .commands -side top -fill both -expand yes
