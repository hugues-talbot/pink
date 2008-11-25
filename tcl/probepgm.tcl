#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $1

set PROBE(name) "probe"

proc tmpfile {tmpname} {
  global PROBE
  return [file join "/tmp" "$PROBE(name)_$tmpname"]
}

if {($argc != 1)} { 
  puts stderr "usage: probepgm testprop" 
  exit 
}

set PROBE(testprop) [lindex $argv 0]

proc probefile {f} {
  global PROBE
#  catch { exec seuilhisto $f 0.5 "$f.seuil" } mes
#  catch { exec encadre "$f.seuil" 0 "$f.seuil" } mes
#  catch { exec $PROBE(testprop) "$f.seuil" } mes
  catch { exec $PROBE(testprop) "$f" } mes
  puts "$f"
  puts "$mes"
}

set PROBE(filelist) [glob "*.pgm"]

foreach file $PROBE(filelist) {
  probefile "$file"
}

#catch { exec rm [tmpfile "?"] } mes


