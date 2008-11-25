#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1 $2 $3

# A call to the VTK decimatePro algorithm - Michel Couprie 2002
package require vtk

# Hide the default . widget
wm withdraw .

# get arguments
if {$argc != 3} { 
  puts stderr "usage: decimate in reduction out"
  exit 
}
set infilename [lindex $argv 0]
set reduction [lindex $argv 1]
set outfilename [lindex $argv 2]

# load geometry
vtkPolyDataReader reader
    reader SetFileName $infilename

# We want to preserve topology (not let any cracks form). This may limit
# the total reduction possible, which we have specified at 90%.
#
vtkDecimate deci
    deci SetInput [reader GetOutput]
    deci SetTargetReduction $reduction
    deci PreserveTopologyOn
#    deci SetDegree 8
#    deci SetMaximumError 100
#    set maxerr [deci GetMaximumError]
#    puts "max. error = $maxerr"
#    set split [deci GetSplitting]
#    puts "splitting = $split"
#    set degree [deci GetDegree]
#    puts "degree = $degree"
#    set fangle [deci GetFeatureAngle]
#    puts "fangle = $fangle"
#    set sangle [deci GetSplitAngle]
#    puts "sangle = $sangle"
    deci SetAspectRatio 5
    set aspect [deci GetAspectRatio]
#    puts "aspect ratio = $aspect"

# save decimated mesh
vtkPolyDataWriter writer
    writer SetFileName $outfilename
    writer SetInput [deci GetOutput]
    writer Write

exit
