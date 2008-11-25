wm title . "Main window"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"

global MAIN
set MAIN(name) main

proc tmpfile {tmpname} {
  global MAIN
  return [file join "/tmp" "$MAIN(name)_$tmpname"]
}

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

$File      add command -label "Quit" -command main_quit -underline 0
$File      add command -label "Open" -command main_open -underline 0

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top -borderwidth 10 -width 100 -height 40
pack .top -side top -fill x

#===========================================
# PROCEDURES
#===========================================

proc main_quit {} {
  global MAIN
  foreach file [glob -nocomplain [tmpfile *]] {
    exec rm $file
  }
  exit
}

proc main_open {} {
  global MAIN
  set MAIN(filename) [tk_getOpenFile ]
  puts $MAIN(filename)
}

wm protocol . WM_DELETE_WINDOW main_quit
bind . "q" main_quit

