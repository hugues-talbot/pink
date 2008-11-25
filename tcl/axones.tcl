wm title . "Main window"

# pour la portabilite LINUX - WINDOWS(MinGW)
# LINUX:
set PORTABLE(axone1) "axone1.tcl"
# WINDOWS:
#set PORTABLE(axone1) "axone1.bat"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"

global MAIN
set MAIN(name) main

# ============================================================
# MENU DEFINITION
# ============================================================

set modifier Alt
menu .menubar -tearoff 0
. config -menu .menubar
foreach m {File Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Open" -command main_open -underline 0
$File      add command -label "Quit" -command main_quit -underline 0

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top
pack .top -side top -fill x

# create the image
set SEG2D(im) [image create photo imxyname -file "axones.gif"]

# get image size
set SEG2D(cs) [image height $SEG2D(im)]
set SEG2D(rs) [image width $SEG2D(im)]

# create frame for the image
frame .top.imframe -borderwidth 10 -width $SEG2D(rs) -height $SEG2D(cs)
pack .top.imframe -side top

# create canvas with the right size
set SEG2D(can) [canvas .top.imframe.can -width $SEG2D(rs) -height $SEG2D(cs)]
pack .top.imframe.can -fill both -expand true

# put the images into the canvas
$SEG2D(can) create image 1 1 -anchor nw -image $SEG2D(im)

#===========================================
# PROCEDURES
#===========================================

proc main_quit {} {
  exit
}

proc main_open {} {
  global MAIN
  global PINK
  global PORTABLE
  set MAIN(filename) [tk_getOpenFile ]
  set filename [file tail $MAIN(filename)]
  set pathname [file dirname $MAIN(filename)]
  set patientname [string range $filename 0 3]
  cd $pathname
  exec $PORTABLE(axone1) $MAIN(filename) $patientname &
}

wm protocol . WM_DELETE_WINDOW main_quit
bind . "q" main_quit



