#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

for {set j 0.5} {$j <= 12.0} {set j [expr $j * 1.2]} {
#    my_exec_q zoom engr1_bin.pgm $j _1
    my_exec_q zoom v.pgm $j _1
    my_exec_q seuil _1 128 _1
    my_exec_v testprop _1
}

