# Copyright ESIEE (2009) 
#
# m.couprie@esiee.fr
#
# This software is an image processing library whose purpose is to be
# used primarily for research and teaching.
#
# This software is governed by the CeCILL  license under French law and
# abiding by the rules of distribution of free software. You can  use, 
# modify and/ or redistribute the software under the terms of the CeCILL
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info". 
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability. 
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or 
# data to be ensured and,  more generally, to use and operate it in the 
# same conditions as regards security. 
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL license and that you accept its terms.
#
#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1

# This example shows how to combine data from both the imaging
# and graphics pipelines. The vtkMergeData filter is used to
# merge the data from each together.

package require vtk
package require vtkinteraction

# get input image file name as first argument
if {$argc != 1} { 
  puts stderr "usage: relief in.ppm"
  exit 
}
set infilename [lindex $argv 0]

# Read in an image and compute a luminance value. The image is extracted
# as a set of polygons (vtkImageDataGeometryFilter). We then will
# warp the plane using the scalar (luminance) values.
#
vtkPNMReader reader
  reader SetFileName $infilename
vtkImageLuminance luminance
  luminance SetInput [reader GetOutput]
vtkImageDataGeometryFilter geometry
  geometry SetInput [luminance GetOutput]
vtkWarpScalar warp
  warp SetInput [geometry GetOutput]
  warp SetScaleFactor -0.2

# Use vtkMergeFilter to combine the original image with the warped geometry.
#
vtkMergeFilter merge
  merge SetGeometry [warp GetOutput]
  merge SetScalars  [reader GetOutput]
vtkDataSetMapper mapper
  mapper SetInput [merge GetOutput]
  mapper SetScalarRange 0 255
  mapper ImmediateModeRenderingOff
vtkActor actor
  actor SetMapper mapper

# Create renderer stuff
#
vtkRenderer ren1
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin

# Add the actors to the renderer, set the background and size
#
ren1 AddActor actor
[ren1 GetActiveCamera] Azimuth 20
[ren1 GetActiveCamera] Elevation 30
ren1 SetBackground 0.1 0.2 0.4
ren1 ResetCameraClippingRange

renWin SetSize 250 250

# render the image
#
iren AddObserver UserEvent {wm deiconify .vtkInteract}
set cam1 [ren1 GetActiveCamera]
$cam1 Zoom 1.4
renWin Render

# prevent the tk window from showing up then start the event loop
wm withdraw .


