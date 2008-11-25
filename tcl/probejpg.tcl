#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

set PROBE(name) "probe"

proc tmpfile {tmpname} {
  global PROBE
  return [file join "/tmp" "$PROBE(name)_$tmpname"]
}

proc probefile {f} {
  global PROBE
  my_exec_q seuilhisto $f 0.5 [tmpfile 1]
  my_exec_q encadre [tmpfile 1] 0 [tmpfile 1]
  my_exec_v testprop [tmpfile 1]
}

#set PROBE(filelist) [glob "*.JPG"]
#foreach file $PROBE(filelist) {
#  set name [file rootname $file]
#  exec mv "$name.JPG" "$name.jpg" 
#}

set PROBE(filelist) [glob "*.jpg"]

foreach file $PROBE(filelist) {
  my_exec_q convert $file "$file.ppm"
  my_exec_q rgb2hls "$file.ppm" "/dev/null" "$file.l" "/dev/null"
  probefile "$file.l"
  my_exec_q rm "$file.ppm" "$file.l"
}

catch { exec rm [tmpfile "?"] } mes
