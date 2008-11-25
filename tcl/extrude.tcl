#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $1 $2 $3

if {$argc != 3} { 
  puts stderr "usage: extrude in2d.pgm nplanes out3d.pgm"
  exit 
}

set EXTRUDE(name) "extrude"
set EXTRUDE(infile) [lindex $argv 0]
set EXTRUDE(nplanes) [lindex $argv 1]
set EXTRUDE(outfile) [lindex $argv 2]

for {set j 0} {$j < $EXTRUDE(nplanes)} {incr j 1} {
  set n [format "%04d" $j]
  set filename_out "$EXTRUDE(name)_$n.pgm"  
  catch { exec cp $EXTRUDE(infile) $filename_out } mes
}
catch { exec catpgm "$EXTRUDE(name)_" 0 [expr $EXTRUDE(nplanes)-1] $EXTRUDE(outfile) } mes
for {set j 0} {$j < $EXTRUDE(nplanes)} {incr j 1} {
  set n [format "%04d" $j]
  set filename_out "$EXTRUDE(name)_$n.pgm"  
  catch { exec rm -f $EXTRUDE(infile) $filename_out } mes
}




