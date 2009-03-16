#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Threshold"

if {$argc != 2} { 
  puts stderr "usage: gaussianfilter in out"
  exit 
}

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

# globals
#   GAUSSIANFILTER(infilename)
#   GAUSSIANFILTER(outfilename)
#   GAUSSIANFILTER(im)
#   GAUSSIANFILTER(im_rs)
#   GAUSSIANFILTER(im_cs)
set GAUSSIANFILTER(param) 1
set GAUSSIANFILTER(name) gaussianfilter

# get input image file name as first argument
set GAUSSIANFILTER(infilename) [lindex $argv 0]

# get output image file name as second argument
set GAUSSIANFILTER(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command gaussianfilter_quit
pack .top.quit -side right

# create the param button
scale .top.param -from 1 -to 50 -length 200 -variable GAUSSIANFILTER(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command gaussianfilter_run
pack .top.param -side left

# create an image and load contents from file 
set GAUSSIANFILTER(im) [image create photo imname -file "$GAUSSIANFILTER(infilename)"]

# get image size
set GAUSSIANFILTER(im_cs) [image height $GAUSSIANFILTER(im)]
set GAUSSIANFILTER(im_rs) [image width $GAUSSIANFILTER(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $GAUSSIANFILTER(im_rs) -height $GAUSSIANFILTER(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $GAUSSIANFILTER(im_rs) - 2] -height [expr $GAUSSIANFILTER(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $GAUSSIANFILTER(im)

# action associated to param. change
proc gaussianfilter_run {param} {
  global GAUSSIANFILTER
  global PINK
    my_exec_f $PINK/linux/bin/gaussianfilter $GAUSSIANFILTER(infilename) [expr 1.0 / $param] $GAUSSIANFILTER(outfilename)
  $GAUSSIANFILTER(im) read $GAUSSIANFILTER(outfilename)
}

# action associated to quit widget
proc gaussianfilter_quit {} {
  global GAUSSIANFILTER
  foreach file [glob -nocomplain "/tmp/$GAUSSIANFILTER(name)_?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW gaussianfilter_quit
bind . "q" gaussianfilter_quit
