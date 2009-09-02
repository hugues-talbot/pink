#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Ogniewicz Medial Axis"

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
#   OGNIEWICZMEDIALAXIS(infilename)
#   OGNIEWICZMEDIALAXIS(outfilename)
#   OGNIEWICZMEDIALAXIS(im)
#   OGNIEWICZMEDIALAXIS(im_rs)
#   OGNIEWICZMEDIALAXIS(im_cs)
set OGNIEWICZMEDIALAXIS(combine) 0
set OGNIEWICZMEDIALAXIS(param) 1
set OGNIEWICZMEDIALAXIS(name) ogniewiczmedialaxis

if {$argc != 2} { 
  puts stderr "usage: ogniewiczmedialaxis.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global OGNIEWICZMEDIALAXIS
  return [file join "/tmp" "$OGNIEWICZMEDIALAXIS(name)_$tmpname"]
}

# get input image file name as first argument
set OGNIEWICZMEDIALAXIS(infilename) [lindex $argv 0]

# get output image file name as second argument
set OGNIEWICZMEDIALAXIS(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command ogniewiczmedialaxis_quit
pack .top.quit -side right

# create the combine button
set OGNIEWICZMEDIALAXIS(combine) 0
checkbutton .top.combine -text combine -variable OGNIEWICZMEDIALAXIS(combine) -command ogniewiczmedialaxis_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from 1 -to 50 -length 200 -variable OGNIEWICZMEDIALAXIS(param) \
  -orient horizontal -tickinterval 5 -showvalue true -command ogniewiczmedialaxis_run
pack .top.radius -side left

# create an image and load contents from file 
set OGNIEWICZMEDIALAXIS(im) [image create photo imname -file "$OGNIEWICZMEDIALAXIS(infilename)"]

# get image size
set OGNIEWICZMEDIALAXIS(im_cs) [image height $OGNIEWICZMEDIALAXIS(im)]
set OGNIEWICZMEDIALAXIS(im_rs) [image width $OGNIEWICZMEDIALAXIS(im)]
my_exec genimage $OGNIEWICZMEDIALAXIS(im_rs) $OGNIEWICZMEDIALAXIS(im_cs) 1 0 [tmpfile v]

# create a frame for the image
frame .imframe -borderwidth 10 -width $OGNIEWICZMEDIALAXIS(im_rs) -height $OGNIEWICZMEDIALAXIS(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $OGNIEWICZMEDIALAXIS(im_rs) - 2] -height [expr $OGNIEWICZMEDIALAXIS(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $OGNIEWICZMEDIALAXIS(im)

# action associated to param change
proc ogniewiczmedialaxis_run {param} {
  global OGNIEWICZMEDIALAXIS
  global PINK
  my_exec_q $PINK/scripts/ogniewicz1 $OGNIEWICZMEDIALAXIS(infilename) $OGNIEWICZMEDIALAXIS(param)
  my_exec_q $PINK/linux/bin/drawlines [tmpfile v] /tmp/ogniewicz_temp [tmpfile 3]

  my_exec $PINK/linux/bin/distc $OGNIEWICZMEDIALAXIS(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $OGNIEWICZMEDIALAXIS(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$OGNIEWICZMEDIALAXIS(combine) == 0} {
    $OGNIEWICZMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $OGNIEWICZMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $OGNIEWICZMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc ogniewiczmedialaxis_combine {} {
  global OGNIEWICZMEDIALAXIS
  if {$OGNIEWICZMEDIALAXIS(combine) == 0} {
    $OGNIEWICZMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $OGNIEWICZMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $OGNIEWICZMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc ogniewiczmedialaxis_quit {} {
  global OGNIEWICZMEDIALAXIS
  my_exec cp [tmpfile 3] $OGNIEWICZMEDIALAXIS(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW ogniewiczmedialaxis_quit
bind . "q" ogniewiczmedialaxis_quit



