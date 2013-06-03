#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0

# Hide the default . widget
wm withdraw .

set VIEW(name) "view"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]
source [file join "$PINK" "tcl" "my_read.tcl"]
source "$PINK/tcl/my_feedback.tcl"

proc tmpfile {tmpname} {
  global VIEW
  return [file join "/tmp" "$VIEW(name)_$tmpname"]
}

set VIEW(filelist) [glob "*.jpg"]
set VIEW(zoom) 2
set VIEW(delay) 3000
set count 0

toplevel .waitmess -borderwidth 10
entry .waitmess.entry
pack .waitmess.entry -side top
update
foreach f $VIEW(filelist) {
  my_feedback "processing $f..."
  my_exec convert $f "_$f.ppm"
  set h [my_readpgmheader "_$f.ppm"]
  set rs [lindex $h 1]
  set cs [lindex $h 2]
  set w [expr $rs / 2]
  set h [expr $cs / 2]
  set filecount [format "%03d" $count]
  incr count
  my_exec cropppm "_$f.ppm" 0 0 $w $h "_$filecount"
  my_exec zoomppm "_$filecount" $VIEW(zoom) "_$filecount"
  set filecount [format "%03d" $count]
  incr count
  my_exec cropppm "_$f.ppm" $w 0 $w $h "_$filecount"
  my_exec zoomppm "_$filecount" $VIEW(zoom) "_$filecount"
  set filecount [format "%03d" $count]
  incr count
  my_exec cropppm "_$f.ppm" 0 $h $w $h "_$filecount"
  my_exec zoomppm "_$filecount" $VIEW(zoom) "_$filecount"
  set filecount [format "%03d" $count]
  incr count
  my_exec cropppm "_$f.ppm" $w $h $w $h "_$filecount"
  my_exec zoomppm "_$filecount" $VIEW(zoom) "_$filecount"
  file delete "_$f.ppm"
}
destroy .waitmess

set VIEW(filelist) [glob "_???"]
set VIEW(count) [llength $VIEW(filelist)]
set VIEW(index) 0
set VIEW(nima) 0
set VIEW(nsub) 0
toplevel .view

# create a frame for buttons
frame .view.top -borderwidth 10
pack .view.top -side top -fill x

# create the quit button
button .view.top.quit -text Quit -command view_quit
pack .view.top.quit -side right

# create the prev, next and auto button
button .view.top.prev -text prev -command view_prev
button .view.top.next -text next -command view_next
button .view.top.auto -text auto -command view_auto
pack .view.top.prev .view.top.next .view.top.auto -side left

# create the label for pixel value
label .view.top.nimage -textvariable VIEW(nima) -width 3
label .view.top.nsubim -textvariable VIEW(nsub) -width 3
pack .view.top.nimage -side left
pack .view.top.nsubim -side left

# create an image and load contents from file 
set ww [expr $w * $VIEW(zoom)]
set hh [expr $h * $VIEW(zoom)]
my_exec genimage $ww $hh 1 0 "_" 
set VIEW(im) [image create photo imname -file "_"]

# get image size
set VIEW(im_cs) [image height $VIEW(im)]
set VIEW(im_rs) [image width $VIEW(im)]

# create a frame for the image
frame .view.imframe -borderwidth 10 -width $VIEW(im_rs) -height $VIEW(im_cs)
pack .view.imframe -side top

# create canvas with the right sizes
set c [canvas .view.imframe.c -width [expr $VIEW(im_rs) - 2] -height [expr $VIEW(im_cs) - 2]]
pack .view.imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $VIEW(im)

# action associated to next widget
proc view_next {} {
  global VIEW
  if {$VIEW(index) < [expr $VIEW(count)-1]} {
    incr VIEW(index)
    set VIEW(nima) [expr ($VIEW(index) / 4) + 1]
    set VIEW(nsub) [expr ($VIEW(index) % 4) + 1]
    set filecount [format "%03d" $VIEW(index)]
    $VIEW(im) read "_$filecount" 
  }
}

# action associated to prev widget
proc view_prev {} {
  global VIEW
  if {$VIEW(index) > 0} {
    incr VIEW(index) -1
    set VIEW(nima) [expr ($VIEW(index) / 4) + 1]
    set VIEW(nsub) [expr ($VIEW(index) % 4) + 1]
    set filecount [format "%03d" $VIEW(index)]
    $VIEW(im) read "_$filecount" 
  }
}

# action associated to auto widget
proc view_auto {} {
  global VIEW
  .view config -cursor watch
  while {$VIEW(index) < $VIEW(count)} {
    set VIEW(nima) [expr ($VIEW(index) / 4) + 1]
    set VIEW(nsub) [expr ($VIEW(index) % 4) + 1]
    set filecount [format "%03d" $VIEW(index)]
    $VIEW(im) read "_$filecount" 
    update
    after $VIEW(delay)
    incr VIEW(index)
  }
  set VIEW(index) 0
  set filecount [format "%03d" $VIEW(index)]
  $VIEW(im) read "_$filecount" 
  update
  .view config -cursor left_ptr
}

# action associated to quit widget
proc view_quit {} {
  global VIEW
  foreach file [glob -nocomplain "_*"] {
    catch {exec rm $file} result
  }
  exit
}

wm protocol .view WM_DELETE_WINDOW view_quit
bind .view "q" view_quit

