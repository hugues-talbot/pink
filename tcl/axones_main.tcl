#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $1

if {$argc == 1} {
  set nom_patient [lindex $argv 0]
} else {
  puts stderr "usage: axones_main nom_patient"
  exit 
}
set filelist [glob "$nom_patient*.pgm"]
foreach infilename $filelist {
  exec axones.tcl $infilename $nom_patient
}
