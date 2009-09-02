#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $1

# ============================================================
# ============================================================
# pinktest.tcl
# executes regression tests for Pink
# Michel Couprie - July 2009
# ============================================================
# ============================================================

if {($argc != 0)} { 
  puts stderr "usage: pinktest"
  exit 
}

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

set PINKTEST(name)     "pinktest"
set PINKTEST(testtag)  "%TEST"
set PINKTEST(testdir)  [file join "$env(HOME)" "Pinktests"]
set PINKTEST(comdir)   [file join "$PINK" "src" "com"]
set PINKTEST(filelist) [glob "$PINKTEST(comdir)/*.c"]

proc tmpfile {tmpname} {
  global PINKTEST
  return [file join "/tmp" "$PINKTEST(name)_$tmpname"]
}

my_exec_q rm -f [tmpfile file1] [tmpfile file2]

foreach file $PINKTEST(filelist) {
  my_exec_q grep -h --color=never $PINKTEST(testtag) $file >> [tmpfile file1]
}

set sedcom1 "s|%TEST||g"
set sedcom2 "s|%IMAGES|$PINKTEST(testdir)/images|g"
set sedcom3 "s|%RESULTS|$PINKTEST(testdir)/results|g"

my_exec_v sed -e $sedcom1 -e $sedcom2 -e $sedcom3 [tmpfile file1] > [tmpfile file2]

set PINKTEST(input) [open [tmpfile file2]]

while {[gets $PINKTEST(input) comexec] >= 0} {
#  puts "$comexec"
  set res [my_exec_q $comexec]
  if {$res != ""} {
    puts "EXECUTION ERROR: $comexec"
    puts $res
  }
  set tag "results/"
  set ltag [string length $tag]
  set ind [string first $tag $comexec]
  set filename [string range $comexec [expr $ind+$ltag] end]
#  puts "$filename"
  set rescheck "diff $PINKTEST(testdir)/results/$filename $PINKTEST(testdir)/results_prev/$filename"
  set res [my_exec_q $rescheck]
  if {$res != ""} {
    puts "CHECK FAILED: $rescheck"
    puts $res
  } else {
    puts "CHECK OK: $filename"
  }
  
}

close $PINKTEST(input)
