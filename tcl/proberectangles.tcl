#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

#for {set j 100} {$j <= 10000} {incr j 100} {
for {set j 20} {$j <= 1000} {incr j 20} {
#    my_exec_q genimage [expr $j+2] [expr $j+2] 1 0 _1
    my_exec_q genimage [expr $j+2] [expr $j+2] [expr ($j/2)+2] 0 _1
#    my_exec_q drawrect _1 1 1 0 $j $j 0 _1
    my_exec_q drawrect _1 1 1 1 $j $j [expr $j/2] _1
    my_exec_v testprop _1
}

