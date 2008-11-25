#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Area opening"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   AREAOPEN(infilename)
#   AREAOPEN(outfilename)
#   AREAOPEN(im)
#   AREAOPEN(im_rs)
#   AREAOPEN(im_cs)
set AREAOPEN(combine) 0
set AREAOPEN(param) 0
set AREAOPEN(name) areaopen
set AREAOPEN(connex) 4

if {$argc != 2} { 
  puts stderr "usage: areaopen in out"
  exit 
}

proc tmpfile {tmpname} {
  global AREAOPEN
  return [file join "/tmp" "$AREAOPEN(name)_$tmpname"]
}

# get input image file name as first argument
set AREAOPEN(infilename) [lindex $argv 0]

# get output image file name as second argument
set AREAOPEN(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command areaopen_quit
pack .top.quit -side right

# create the combine button
set AREAOPEN(combine) 0
checkbutton .top.combine -text combine -variable AREAOPEN(combine) -command areaopen_combine
pack .top.combine -side right

# create the connex buttons
radiobutton .top.connex4 -variable AREAOPEN(connex) -text "4" -value 4
pack .top.connex4 -side right
radiobutton .top.connex8 -variable AREAOPEN(connex) -text "8" -value 8
pack .top.connex8 -side right

# create the area button
scale .top.area -from 0 -to 60 -length 300 -variable AREAOPEN(param) \
  -orient horizontal -tickinterval 1 -showvalue true -command areaopen_run
pack .top.area -side left

# create an image and load contents from file 
set AREAOPEN(im) [image create photo imname -file "$AREAOPEN(infilename)"]

# get image size
set AREAOPEN(im_cs) [image height $AREAOPEN(im)]
set AREAOPEN(im_rs) [image width $AREAOPEN(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $AREAOPEN(im_rs) -height $AREAOPEN(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $AREAOPEN(im_rs) - 2] -height [expr $AREAOPEN(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $AREAOPEN(im)

# action associated to area change
proc areaopen_run {area} {
  global AREAOPEN
  if {$area < 0} {
    my_exec areaclosing $AREAOPEN(infilename) $AREAOPEN(connex) [expr -$area]  [tmpfile 3]
  } else {
    my_exec areaopening $AREAOPEN(infilename) $AREAOPEN(connex) $area [tmpfile 3]
  }
  if {$AREAOPEN(combine) == 0} {
    $AREAOPEN(im) read [tmpfile 3]
  } else {
    my_exec scale $AREAOPEN(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $AREAOPEN(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc areaopen_combine {} {
  global AREAOPEN
  if {$AREAOPEN(combine) == 0} {
    $AREAOPEN(im) read [tmpfile 3]
  } else {
    my_exec scale $AREAOPEN(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $AREAOPEN(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc areaopen_quit {} {
  global AREAOPEN
  my_exec cp [tmpfile 3] $AREAOPEN(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW areaopen_quit
bind . "q" areaopen_quit
