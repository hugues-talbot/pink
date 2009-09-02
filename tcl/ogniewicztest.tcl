#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Ogniewick skeleton (Test)"

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
#   OGNIEWICZTEST(infilename)
#   OGNIEWICZTEST(outfilename)
#   OGNIEWICZTEST(im)
#   OGNIEWICZTEST(im_rs)
#   OGNIEWICZTEST(im_cs)
set OGNIEWICZTEST(combine) 0
set OGNIEWICZTEST(param) 1
set OGNIEWICZTEST(angle) 0
set OGNIEWICZTEST(name) ogniewicztest

if {$argc != 2} { 
  puts stderr "usage: ogniewicztest.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global OGNIEWICZTEST
  return [file join "/tmp" "$OGNIEWICZTEST(name)_$tmpname"]
}

# get input image file name as first argument
set OGNIEWICZTEST(infilename) [lindex $argv 0]

# get output image file name as second argument
set OGNIEWICZTEST(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command ogniewicztest_quit
pack .top.quit -side right

# create the combine button
set OGNIEWICZTEST(combine) 0
checkbutton .top.combine -text combine -variable OGNIEWICZTEST(combine) -command ogniewicztest_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from 1 -to 50 -length 100 -variable OGNIEWICZTEST(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command ogniewicztest_run
pack .top.radius -side left

# create the angle button
scale .top.angle -from 0 -to 89 -length 200 -variable OGNIEWICZTEST(angle) \
  -orient horizontal -tickinterval 15 -showvalue true -command rotation_run
pack .top.angle -side left

# create an image and load contents from file 
set OGNIEWICZTEST(im) [image create photo imname -file "$OGNIEWICZTEST(infilename)"]

# get image size
set OGNIEWICZTEST(im_cs) [image height $OGNIEWICZTEST(im)]
set OGNIEWICZTEST(im_rs) [image width $OGNIEWICZTEST(im)]
my_exec genimage $OGNIEWICZTEST(im_rs) $OGNIEWICZTEST(im_cs) 1 0 [tmpfile v]

# create a frame for the image
frame .imframe -borderwidth 10 -width $OGNIEWICZTEST(im_rs) -height $OGNIEWICZTEST(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $OGNIEWICZTEST(im_rs) - 2] -height [expr $OGNIEWICZTEST(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $OGNIEWICZTEST(im)

# action associated to radius change
proc ogniewicztest_run {radius} {
  global OGNIEWICZTEST
  global PINK
  my_exec $PINK/linux/bin/rotate $OGNIEWICZTEST(infilename) $OGNIEWICZTEST(angle) [expr $OGNIEWICZTEST(im_rs)/2] [expr $OGNIEWICZTEST(im_cs)/2] [tmpfile 0]
  my_exec $PINK/linux/bin/seuil [tmpfile 0] 128 [tmpfile 0]
  my_exec $PINK/linux/bin/areaclosing [tmpfile 0] 4 100 [tmpfile 0]


  my_exec_q $PINK/scripts/ogniewicz1 [tmpfile 0] $OGNIEWICZTEST(param)
  my_exec_q $PINK/linux/bin/drawlines [tmpfile v] /tmp/ogniewicz_temp [tmpfile 3]

  my_exec $PINK/linux/bin/distc $OGNIEWICZTEST(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $OGNIEWICZTEST(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$OGNIEWICZTEST(combine) == 0} {
    $OGNIEWICZTEST(im) read [tmpfile 3]
  } else {
    my_exec scale [tmpfile 0] 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $OGNIEWICZTEST(im) read [tmpfile 2]
  }
}

# action associated to angle change
proc rotation_run {angle} {
  global OGNIEWICZTEST
  global PINK
  my_exec $PINK/linux/bin/rotate $OGNIEWICZTEST(infilename) $angle [expr $OGNIEWICZTEST(im_rs)/2] [expr $OGNIEWICZTEST(im_cs)/2] [tmpfile 0]
  my_exec $PINK/linux/bin/seuil [tmpfile 0] 128 [tmpfile 0]
  my_exec $PINK/linux/bin/areaclosing [tmpfile 0] 4 100 [tmpfile 0]

  my_exec_q $PINK/scripts/ogniewicz1 [tmpfile 0] $OGNIEWICZTEST(param)
  my_exec_q $PINK/linux/bin/drawlines [tmpfile v] /tmp/ogniewicz_temp [tmpfile 3]

  if {$OGNIEWICZTEST(combine) == 0} {
    $OGNIEWICZTEST(im) read [tmpfile 3]
  } else {
    my_exec scale [tmpfile 0] 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $OGNIEWICZTEST(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc ogniewicztest_combine {} {
  global OGNIEWICZTEST
  if {$OGNIEWICZTEST(combine) == 0} {
    $OGNIEWICZTEST(im) read [tmpfile 3]
  } else {
    my_exec scale $OGNIEWICZTEST(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $OGNIEWICZTEST(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc ogniewicztest_quit {} {
  global OGNIEWICZTEST
  my_exec cp [tmpfile 3] $OGNIEWICZTEST(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW ogniewicztest_quit
bind . "q" ogniewicztest_quit
