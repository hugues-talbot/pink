#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

for {set j 27} {$j <= 30} {incr j 5} {
    my_exec_q drawtorus null [expr $j*2] $j [expr ($j*3)+1] [expr ($j*3)+1] [expr $j+1] _1
    my_exec_v testprop _1
}

