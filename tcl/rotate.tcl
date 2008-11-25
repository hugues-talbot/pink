#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Rotation"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   ROTATION(infilename)
#   ROTATION(outfilename)
#   ROTATION(im)
#   ROTATION(im_rs)
#   ROTATION(im_cs)
set ROTATION(mode) 1
set ROTATION(param) 0
set ROTATION(name) rotation

if {$argc != 2} { 
  puts stderr "usage: rotation.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global ROTATION
  return [file join "/tmp" "$ROTATION(name)_$tmpname"]
}

# get input image file name as first argument
set ROTATION(infilename) [lindex $argv 0]

# get output image file name as second argument
set ROTATION(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command rotation_quit
pack .top.quit -side right

# create the mode buttons
radiobutton .top.modeinter -variable ROTATION(mode) -text "inter" -value 1\
    -command rotation_changemode
pack .top.modeinter -side right
radiobutton .top.modetrunc -variable ROTATION(mode) -text "trunc" -value 0\
  -command rotation_changemode
pack .top.modetrunc -side right

# create the angle button
scale .top.angle -from 0 -to 359 -length 300 -variable ROTATION(param) \
  -orient horizontal -tickinterval 45 -showvalue true -command rotation_run
pack .top.angle -side left

# create an image and load contents from file 
set ROTATION(im) [image create photo imname -file "$ROTATION(infilename)"]

# get image size
set ROTATION(im_cs) [image height $ROTATION(im)]
set ROTATION(im_rs) [image width $ROTATION(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $ROTATION(im_rs) -height $ROTATION(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $ROTATION(im_rs) - 2] -height [expr $ROTATION(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $ROTATION(im)

# action associated to angle change
proc rotation_run {angle} {
  global ROTATION
  if {$ROTATION(mode) == 1} {
    my_exec rotate $ROTATION(infilename) $ROTATION(param) [expr $ROTATION(im_rs)/2] [expr $ROTATION(im_cs)/2] [tmpfile 3]
  } else {
    my_exec rotatebin $ROTATION(infilename) $ROTATION(param) [expr $ROTATION(im_rs)/2] [expr $ROTATION(im_cs)/2] [tmpfile 3]
  }
  $ROTATION(im) read [tmpfile 3]
}

# action associated to mode change
proc rotation_changemode {} {
  global ROTATION
  rotation_run $ROTATION(param)
}

# action associated to quit widget
proc rotation_quit {} {
  global ROTATION
  my_exec cp [tmpfile 3] $ROTATION(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW rotation_quit
bind . "q" rotation_quit
