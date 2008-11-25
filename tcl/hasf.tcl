#!tclsh

if {$argc != 4} { 
  puts stderr "usage: selcomp in n m out"
  exit 
}

set HASF(infilename) [lindex $argv 0]
set HASF(n) [lindex $argv 1]
set HASF(m) [lindex $argv 2]
set HASF(outfilename) [lindex $argv 3]

catch {exec cp $HASF(infilename) "/tmp/HASF_1"} result
for {set x 0} {$x<$HASF(m)} {incr x} {
  puts "step $x: shrink"
  catch {exec shrink "/tmp/HASF_1" $HASF(n) "/tmp/HASF_1"} result
  catch {exec inverse "/tmp/HASF_1" "/tmp/HASF_1"} result
  puts "step $x: grow"
  catch {exec shrinkc "/tmp/HASF_1" $HASF(n) "/tmp/HASF_1"} result
  catch {exec inverse "/tmp/HASF_1" "/tmp/HASF_1"} result
}
catch {exec cp "/tmp/HASF_1" $HASF(outfilename)} result
catch {exec rm "/tmp/HASF_1"} result

