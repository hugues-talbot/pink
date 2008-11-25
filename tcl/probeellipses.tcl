#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

for {set j 100} {$j <= 50000} {incr j 100} {
    my_exec_q genimage [expr $j+2] [expr $j+2] 1 0 _1
    my_exec_q drawellipse _1 1 [expr $j / 2] $j [expr $j / 2] [expr $j / 2] [expr $j / 6] [expr $j / 2] [expr (5*$j) / 6] 1 _1
    my_exec_v testprop _1
}

