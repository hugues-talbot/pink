#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $1 $2

set PROBE(name) "probe"

proc tmpfile {tmpname} {
  global PROBE
  return [file join "/tmp" "$PROBE(name)_$tmpname"]
}

if {($argc != 2)} { 
  puts stderr "usage: probepgm extension testprop"
  exit 
}

set PROBE(extension) [lindex $argv 0]
set PROBE(testprop) [lindex $argv 1]

proc probefile {f} {
  global PROBE
  catch { exec convert $f "/tmp/$f.ppm" } mes
  catch { exec mv "/tmp/$f.ppm" [tmpfile 0]} mes
  catch { exec rgb2hls [tmpfile 0] "/dev/null" [tmpfile 0] "/dev/null" } mes
#  catch { exec gaussianfilter [tmpfile 0] 0.1 [tmpfile 1] } mes
#  catch { exec sub [tmpfile 0] [tmpfile 1] [tmpfile 0] } mes
  catch { exec seuilhisto [tmpfile 0] 0.5 [tmpfile 1] } mes
  catch { exec encadre [tmpfile 1] 0 [tmpfile 1] } mes
  catch { exec cp [tmpfile 1] "/tmp/$f.pgm"} mes
  catch { exec $PROBE(testprop) [tmpfile 1] } mes
  puts "file $f : $mes"
}

set PROBE(filelist) [glob "*.$PROBE(extension)"]

foreach file $PROBE(filelist) {
  probefile "$file"
}

catch { exec rm [tmpfile "?"] } mes


