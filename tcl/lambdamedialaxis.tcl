#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Lambda Medial Axis"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   LAMBDAMEDIALAXIS(infilename)
#   LAMBDAMEDIALAXIS(outfilename)
#   LAMBDAMEDIALAXIS(im)
#   LAMBDAMEDIALAXIS(im_rs)
#   LAMBDAMEDIALAXIS(im_cs)
set LAMBDAMEDIALAXIS(combine) 1
set LAMBDAMEDIALAXIS(topo) 0
set LAMBDAMEDIALAXIS(param) 0
set LAMBDAMEDIALAXIS(name) lambdamedialaxis

if {$argc != 2} { 
  puts stderr "usage: lambdamedialaxis.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global LAMBDAMEDIALAXIS
  return [file join "/tmp" "$LAMBDAMEDIALAXIS(name)_$tmpname"]
}

# get input image file name as first argument
set LAMBDAMEDIALAXIS(infilename) [lindex $argv 0]

# get output image file name as second argument
set LAMBDAMEDIALAXIS(outfilename) [lindex $argv 1]

# compute the lambda map - store in tmpfile 0
my_exec $PINK/linux/bin/lambdamedialaxis $LAMBDAMEDIALAXIS(infilename) [tmpfile 0]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command lambdamedialaxis_quit
pack .top.quit -side right

# create the combine button
checkbutton .top.combine -text combine -variable LAMBDAMEDIALAXIS(combine) -command lambdamedialaxis_combine
pack .top.combine -side right

# create the topology button
set LAMBDAMEDIALAXIS(topo) 0
checkbutton .top.topo -text topo -variable LAMBDAMEDIALAXIS(topo) -command lambdamedialaxis_topo
pack .top.topo -side right

# create the radius button
scale .top.radius -from 0 -to 100 -length 400 -variable LAMBDAMEDIALAXIS(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command lambdamedialaxis_run
pack .top.radius -side left

# create an image and load contents from file 
set LAMBDAMEDIALAXIS(im) [image create photo imname -file "$LAMBDAMEDIALAXIS(infilename)"]

# get image size
set LAMBDAMEDIALAXIS(im_cs) [image height $LAMBDAMEDIALAXIS(im)]
set LAMBDAMEDIALAXIS(im_rs) [image width $LAMBDAMEDIALAXIS(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $LAMBDAMEDIALAXIS(im_rs) -height $LAMBDAMEDIALAXIS(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $LAMBDAMEDIALAXIS(im_rs) - 2] -height [expr $LAMBDAMEDIALAXIS(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $LAMBDAMEDIALAXIS(im)

# action associated to radius change
proc lambdamedialaxis_run {radius} {
  global LAMBDAMEDIALAXIS
  global PINK
  my_exec $PINK/linux/bin/seuil [tmpfile 0] $radius [tmpfile 3]
  if {$LAMBDAMEDIALAXIS(topo) != 0} {
    my_exec $PINK/linux/bin/skeleton $LAMBDAMEDIALAXIS(infilename) [tmpfile 0] 8 [tmpfile 3] [tmpfile 3]
  }
  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc lambdamedialaxis_combine {} {
  global LAMBDAMEDIALAXIS
  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to topo button
proc lambdamedialaxis_topo {} {
  global LAMBDAMEDIALAXIS
  global PINK
  my_exec $PINK/linux/bin/seuil [tmpfile 0] $LAMBDAMEDIALAXIS(param) [tmpfile 3]
  if {$LAMBDAMEDIALAXIS(topo) != 0} {
    my_exec $PINK/linux/bin/skeleton $LAMBDAMEDIALAXIS(infilename) [tmpfile 0] 8 [tmpfile 3] [tmpfile 3]
  }
  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc lambdamedialaxis_quit {} {
  global LAMBDAMEDIALAXIS
  my_exec cp [tmpfile 3] $LAMBDAMEDIALAXIS(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW lambdamedialaxis_quit
bind . "q" lambdamedialaxis_quit
