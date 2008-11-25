# Simple 2d component selector
# Michel couprie - aout 2002

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Component select"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_scanvas.tcl"

if {$argc != 2} { 
  puts stderr "usage: selcomp in out"
  exit 
}

# declare globals
global SELCOMP
# infilename  : input file name
# outfilename : output file name
# connex : connectivity (4 or 8)
# x  : pixel position in row
# y  : pixel position in column
# im : image structure
# rs : row size
# cs : col size
# name : command name
set SELCOMP(connex) 4
set SELCOMP(x) 0
set SELCOMP(y) 0 
set SELCOMP(name) "selcomp"

# get input image file name as first argument
set SELCOMP(infilename) [lindex $argv 0]

# get output image file name as second argument
set SELCOMP(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side top -fill x

# create the quit button
button .but.quit -text Quit -command selcomp_quit
pack .but.quit -side right

# create the reset button
button .but.reset -text Reset -command selcomp_reset
pack .but.reset -side right

# create the inverse image button
button .but.invima -text "Inv. Image" -command selcomp_invima
pack .but.invima -side right

# create the inverse selection button
button .but.invsel -text "Inv. Selection" -command selcomp_invsel
pack .but.invsel -side right

# create the connex buttons
radiobutton .but.connex4 -variable SELCOMP(connex) -text "4" -value 4
pack .but.connex4 -side left
radiobutton .but.connex8 -variable SELCOMP(connex) -text "8" -value 8
pack .but.connex8 -side left

# create an image and load contents from file 
my_exec_f cp $SELCOMP(infilename) "/tmp/$SELCOMP(name)_0"
my_exec_f cp $SELCOMP(infilename) "/tmp/$SELCOMP(name)_1"
set SELCOMP(im) [image create photo imname -file "/tmp/$SELCOMP(name)_1"]
my_exec_f cp "/tmp/$SELCOMP(name)_1" "/tmp/$SELCOMP(name)_3"

# get image size
set SELCOMP(cs) [image height $SELCOMP(im)]
set SELCOMP(rs) [image width $SELCOMP(im)]
set ca_w $SELCOMP(rs)
set ca_h $SELCOMP(cs)
if {$ca_w > 800} {set ca_w 800}
if {$ca_h > 500} {set ca_h 500}

# create a frame for the image
frame .imframe -borderwidth 10 -width $ca_w -height $ca_h
pack .imframe -side top

# create canvas with the right sizes
set c [my_scanvas .imframe.c -width $ca_w -height $ca_h \
	-scrollregion "0 0 $SELCOMP(rs) $SELCOMP(cs)"]

pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $SELCOMP(im)

# bind actions to mouse events
bind $c <Button> { 
  set SELCOMP(x) [$c canvasx %x] 
  set SELCOMP(y) [$c canvasy %y]
  my_exec genimage $SELCOMP(rs) $SELCOMP(cs) 1 0 "/tmp/$SELCOMP(name)_2"
  my_exec point "/tmp/$SELCOMP(name)_2" $SELCOMP(x) $SELCOMP(y) 0 255 "/tmp/$SELCOMP(name)_2"
  my_exec geodilat "/tmp/$SELCOMP(name)_2" "/tmp/$SELCOMP(name)_1" $SELCOMP(connex) -1 "/tmp/$SELCOMP(name)_2"
  my_exec sub "/tmp/$SELCOMP(name)_1" "/tmp/$SELCOMP(name)_2" "/tmp/$SELCOMP(name)_1"
  my_exec scale "/tmp/$SELCOMP(name)_2" 0.5 "/tmp/$SELCOMP(name)_2"
  my_exec sub "/tmp/$SELCOMP(name)_3" "/tmp/$SELCOMP(name)_2" "/tmp/$SELCOMP(name)_3"
  $SELCOMP(im) read "/tmp/$SELCOMP(name)_3"
}

# action associated to reset widget
proc selcomp_reset {} {
  global SELCOMP
  my_exec cp "/tmp/$SELCOMP(name)_0" "/tmp/$SELCOMP(name)_1"
  my_exec cp "/tmp/$SELCOMP(name)_1" "/tmp/$SELCOMP(name)_3"
  $SELCOMP(im) read "/tmp/$SELCOMP(name)_3"
}

# action associated to inverse image widget
proc selcomp_invima {} {
  global SELCOMP
  my_exec inverse "/tmp/$SELCOMP(name)_0" "/tmp/$SELCOMP(name)_0"
  my_exec cp "/tmp/$SELCOMP(name)_0" "/tmp/$SELCOMP(name)_1"
  my_exec cp "/tmp/$SELCOMP(name)_1" "/tmp/$SELCOMP(name)_3"
  $SELCOMP(im) read "/tmp/$SELCOMP(name)_3"
}

# action associated to inverse selection widget
proc selcomp_invsel {} {
  global SELCOMP
  my_exec scale "/tmp/$SELCOMP(name)_1" 0.5 "/tmp/$SELCOMP(name)_2"
  my_exec sub "/tmp/$SELCOMP(name)_0" "/tmp/$SELCOMP(name)_2" "/tmp/$SELCOMP(name)_3"
  my_exec sub "/tmp/$SELCOMP(name)_0" "/tmp/$SELCOMP(name)_1" "/tmp/$SELCOMP(name)_1"
  $SELCOMP(im) read "/tmp/$SELCOMP(name)_3"
}

# action associated to quit widget
proc selcomp_quit {} {
  global SELCOMP
  my_exec sub "/tmp/$SELCOMP(name)_0" "/tmp/$SELCOMP(name)_1" $SELCOMP(outfilename)
  foreach file [glob -nocomplain "/tmp/$SELCOMP(name)_?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW selcomp_quit
bind . "q" selcomp_quit
