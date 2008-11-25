#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1

set file_name [lindex $argv 0]

vtkDataSetReader reader
 reader SetFileName $file_name
set file [reader GetOutput]

vtkRenderer ren1
ren1  SetBackground 1 1 1

vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin

vtkDataSetMapper mapper
 mapper SetInput $file
vtkActor actor
 actor SetMapper mapper

[actor GetProperty] SetColor .2 .3 .4
#[actor GetProperty] SetRepresentationToWireframe

ren1 AddActor actor
iren Initialize


