#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $1 $2

if {$argc != 2} { 
  puts stderr "usage: renumber extension prefix"
  exit 
}

set RENUMBER(name) "renumber"
set RENUMBER(extension) [lindex $argv 0]
set RENUMBER(prefix) [lindex $argv 1]
set RENUMBER(filelist) [glob "*.$RENUMBER(extension)"]

set l1 ""
foreach f $RENUMBER(filelist) {
    set filenum [file rootname $f]
    lappend l1 $filenum
}

set l2 [lsort -integer $l1]

set filecount 0
foreach f $l2 {
  set n [format "%04d" $filecount]
  set filename_in "$f.$RENUMBER(extension)"  
  set filename_out "$RENUMBER(prefix)$n.$RENUMBER(extension)"  
  puts "mv $filename_in $filename_out"
  catch { exec mv $filename_in $filename_out } mes
  incr filecount
}





