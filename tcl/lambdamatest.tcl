#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Lambda Medial Axis (Test)"

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
set LAMBDAMATEST(infilename) [lindex $argv 0]
set LAMBDAMATEST(outfilename) [lindex $argv 1]
#   LAMBDAMATEST(im)
#   LAMBDAMATEST(im_rs)
#   LAMBDAMATEST(im_cs)
set LAMBDAMATEST(combine) 0
set LAMBDAMATEST(param) 0
set LAMBDAMATEST(angle) 0
set LAMBDAMATEST(name) lambdamatest

if {$argc != 2} { 
  puts stderr "usage: lambdamedialaxis.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global LAMBDAMATEST
  return [file join "/tmp" "$LAMBDAMATEST(name)_$tmpname"]
}

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command lambdamedialaxis_quit
pack .top.quit -side right

# create the combine button
set LAMBDAMATEST(combine) 0
checkbutton .top.combine -text combine -variable LAMBDAMATEST(combine) -command lambdamedialaxis_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from 0 -to 100 -length 400 -variable LAMBDAMATEST(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command lambdamedialaxis_run
pack .top.radius -side left

# create the angle button
scale .top.angle -from 0 -to 359 -length 300 -variable LAMBDAMATEST(angle) \
  -orient horizontal -tickinterval 45 -showvalue true -command rotation_run
pack .top.angle -side left

# create an image and load contents from file 
set LAMBDAMATEST(im) [image create photo imname -file "$LAMBDAMATEST(infilename)"]

# get image size
set LAMBDAMATEST(im_cs) [image height $LAMBDAMATEST(im)]
set LAMBDAMATEST(im_rs) [image width $LAMBDAMATEST(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $LAMBDAMATEST(im_rs) -height $LAMBDAMATEST(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $LAMBDAMATEST(im_rs) - 2] -height [expr $LAMBDAMATEST(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $LAMBDAMATEST(im)

# action associated to radius change
proc lambdamedialaxis_run {radius} {
  global LAMBDAMATEST
  global PINK
  my_exec $PINK/linux/bin/rotate $LAMBDAMATEST(infilename) $LAMBDAMATEST(angle) [expr $LAMBDAMATEST(im_rs)/2] [expr $LAMBDAMATEST(im_cs)/2] [tmpfile 0]
  my_exec $PINK/linux/bin/seuil [tmpfile 0] 128 [tmpfile 0]
  my_exec $PINK/linux/bin/areaclosing [tmpfile 0] 4 100 [tmpfile 0]
  my_exec $PINK/linux/bin/lambdamedialaxis [tmpfile 0] $radius [tmpfile 3]

  my_exec $PINK/linux/bin/distc $LAMBDAMATEST(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $LAMBDAMATEST(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$LAMBDAMATEST(combine) == 0} {
    $LAMBDAMATEST(im) read [tmpfile 3]
  } else {
    my_exec scale [tmpfile 0] 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMATEST(im) read [tmpfile 2]
  }
}

# action associated to angle change
proc rotation_run {angle} {
  global LAMBDAMATEST
  global PINK
  my_exec $PINK/linux/bin/rotate $LAMBDAMATEST(infilename) $angle [expr $LAMBDAMATEST(im_rs)/2] [expr $LAMBDAMATEST(im_cs)/2] [tmpfile 0]
  my_exec $PINK/linux/bin/seuil [tmpfile 0] 128 [tmpfile 0]
  my_exec $PINK/linux/bin/areaclosing [tmpfile 0] 4 100 [tmpfile 0]
  my_exec $PINK/linux/bin/lambdamedialaxis [tmpfile 0] $LAMBDAMATEST(param) [tmpfile 3]
  if {$LAMBDAMATEST(combine) == 0} {
    $LAMBDAMATEST(im) read [tmpfile 3]
  } else {
    my_exec scale [tmpfile 0] 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMATEST(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc lambdamedialaxis_combine {} {
  global LAMBDAMATEST
  if {$LAMBDAMATEST(combine) == 0} {
    $LAMBDAMATEST(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMATEST(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMATEST(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc lambdamedialaxis_quit {} {
  global LAMBDAMATEST
  my_exec cp [tmpfile 3] $LAMBDAMATEST(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW lambdamedialaxis_quit
bind . "q" lambdamedialaxis_quit
