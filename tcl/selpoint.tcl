# Simple 2d point selector
# Michel couprie - aout 2002

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Point select"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_scanvas.tcl"

if {$argc != 2} { 
  puts stderr "usage: selpoint in out"
  exit 
}

# declare globals
global SELPOINT
# infilename  : input file name
# outfilename : output file name
# connex : connectivity (4 or 8)
# x  : pixel position in row
# y  : pixel position in column
# im : image structure
# rs : row size
# cs : col size
# name : command name
set SELPOINT(x) 0
set SELPOINT(y) 0 
set SELPOINT(name) "selpoint"

# get input image file name as first argument
set SELPOINT(infilename) [lindex $argv 0]

# get output image file name as second argument
set SELPOINT(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side top -fill x

# create the quit button
button .but.quit -text Quit -command selpoint_quit
pack .but.quit -side right

# create the reset button
button .but.reset -text Reset -command selpoint_reset
pack .but.reset -side right

# create the inverse image button
button .but.invima -text "Inv. Image" -command selpoint_invima
pack .but.invima -side right

# create an image and load contents from file 
my_exec_f cp $SELPOINT(infilename) "/tmp/$SELPOINT(name)_0"
my_exec_f cp $SELPOINT(infilename) "/tmp/$SELPOINT(name)_1"
set SELPOINT(im) [image create photo imname -file "/tmp/$SELPOINT(name)_1"]
my_exec_f cp "/tmp/$SELPOINT(name)_1" "/tmp/$SELPOINT(name)_3"

# get image size
set SELPOINT(cs) [image height $SELPOINT(im)]
set SELPOINT(rs) [image width $SELPOINT(im)]
set ca_w $SELPOINT(rs)
set ca_h $SELPOINT(cs)
if {$ca_w > 800} {set ca_w 800}
if {$ca_h > 500} {set ca_h 500}

my_exec genimage $SELPOINT(rs) $SELPOINT(cs) 1 0 "/tmp/$SELPOINT(name)_2"

# create a frame for the image
frame .imframe -borderwidth 10 -width $ca_w -height $ca_h
pack .imframe -side top

# create canvas with the right sizes
set c [my_scanvas .imframe.c -width $ca_w -height $ca_h \
	-scrollregion "0 0 $SELPOINT(rs) $SELPOINT(cs)"]

pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $SELPOINT(im)

# bind actions to mouse events
bind $c <Button> { 
  set SELPOINT(x) [$c canvasx %x] 
  set SELPOINT(y) [$c canvasy %y]
  my_exec point "/tmp/$SELPOINT(name)_2" $SELPOINT(x) $SELPOINT(y) 0 255 "/tmp/$SELPOINT(name)_2"
  my_exec surimp "/tmp/$SELPOINT(name)_1" "/tmp/$SELPOINT(name)_2" "/tmp/$SELPOINT(name)_3"  
  $SELPOINT(im) read "/tmp/$SELPOINT(name)_3"
}

# action associated to reset widget
proc selpoint_reset {} {
  global SELPOINT
  my_exec cp "/tmp/$SELPOINT(name)_0" "/tmp/$SELPOINT(name)_1"
  my_exec genimage $SELPOINT(rs) $SELPOINT(cs) 1 0 "/tmp/$SELPOINT(name)_2"
  my_exec cp "/tmp/$SELPOINT(name)_1" "/tmp/$SELPOINT(name)_3"
  $SELPOINT(im) read "/tmp/$SELPOINT(name)_3"
}

# action associated to inverse image widget
proc selpoint_invima {} {
  global SELPOINT
  my_exec inverse "/tmp/$SELPOINT(name)_0" "/tmp/$SELPOINT(name)_0"
  my_exec cp "/tmp/$SELPOINT(name)_0" "/tmp/$SELPOINT(name)_1"
  my_exec surimp "/tmp/$SELPOINT(name)_1" "/tmp/$SELPOINT(name)_2" "/tmp/$SELPOINT(name)_3"  
  $SELPOINT(im) read "/tmp/$SELPOINT(name)_3"
}

# action associated to quit widget
proc selpoint_quit {} {
  global SELPOINT
  my_exec mv "/tmp/$SELPOINT(name)_2" $SELPOINT(outfilename)
  foreach file [glob -nocomplain "/tmp/$SELPOINT(name)_?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW selpoint_quit
bind . "q" selpoint_quit
