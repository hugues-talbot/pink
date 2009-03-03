#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Integer Medial Axis"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# reads a file
#-----------------------------------
proc my_read_val {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  scan $line "%g" v1
  close $input
  return $v1
}

# globals
#   INTEGERMEDIALAXIS(infilename)
#   INTEGERMEDIALAXIS(outfilename)
#   INTEGERMEDIALAXIS(im)
#   INTEGERMEDIALAXIS(im_rs)
#   INTEGERMEDIALAXIS(im_cs)
set INTEGERMEDIALAXIS(combine) 0
set INTEGERMEDIALAXIS(param) 1
set INTEGERMEDIALAXIS(name) integermedialaxis

if {$argc != 2} { 
  puts stderr "usage: integermedialaxis.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global INTEGERMEDIALAXIS
  return [file join "/tmp" "$INTEGERMEDIALAXIS(name)_$tmpname"]
}

# get input image file name as first argument
set INTEGERMEDIALAXIS(infilename) [lindex $argv 0]

# get output image file name as second argument
set INTEGERMEDIALAXIS(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command integermedialaxis_quit
pack .top.quit -side right

# create the combine button
set INTEGERMEDIALAXIS(combine) 0
checkbutton .top.combine -text combine -variable INTEGERMEDIALAXIS(combine) -command integermedialaxis_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from 1 -to 100 -length 400 -variable INTEGERMEDIALAXIS(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command integermedialaxis_run
pack .top.radius -side left

# create an image and load contents from file 
set INTEGERMEDIALAXIS(im) [image create photo imname -file "$INTEGERMEDIALAXIS(infilename)"]

# get image size
set INTEGERMEDIALAXIS(im_cs) [image height $INTEGERMEDIALAXIS(im)]
set INTEGERMEDIALAXIS(im_rs) [image width $INTEGERMEDIALAXIS(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $INTEGERMEDIALAXIS(im_rs) -height $INTEGERMEDIALAXIS(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $INTEGERMEDIALAXIS(im_rs) - 2] -height [expr $INTEGERMEDIALAXIS(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $INTEGERMEDIALAXIS(im)

# action associated to radius change
proc integermedialaxis_run {radius} {
  global INTEGERMEDIALAXIS
  global PINK
  my_exec $PINK/linux/bin/integermedialaxis $INTEGERMEDIALAXIS(infilename) $radius [tmpfile 3]

  my_exec $PINK/linux/bin/distc $INTEGERMEDIALAXIS(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $INTEGERMEDIALAXIS(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"


  if {$INTEGERMEDIALAXIS(combine) == 0} {
    $INTEGERMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $INTEGERMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $INTEGERMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc integermedialaxis_combine {} {
  global INTEGERMEDIALAXIS
  if {$INTEGERMEDIALAXIS(combine) == 0} {
    $INTEGERMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $INTEGERMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $INTEGERMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc integermedialaxis_quit {} {
  global INTEGERMEDIALAXIS
  my_exec cp [tmpfile 3] $INTEGERMEDIALAXIS(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW integermedialaxis_quit
bind . "q" integermedialaxis_quit
