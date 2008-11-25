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
  my_exec_q testprop [tmpfile 1]
}

my_exec_q randimage 1000 1000 1 200 _1
my_exec_q seuil _1 193 _2
my_exec_q seuil2 _2 0 1 1 1 _2
my_exec_q genimage 1000 1000 1 0 _3
my_exec_q byte2long _3 _3
my_exec_q bruite _3 1 12000 1 _3
my_exec_q frame _1 _4
my_exec_q seuil2 _4 0 1 1 1 _4
my_exec_q mult _3 _4 _5
my_exec_q mult _2 _5 _6
my_exec_q redt _6 _7
my_exec_q frame _1 _4
my_exec_q max _7 _4 _8
my_exec_q inverse _8 _8

my_exec_q rm [tmpfile "?"]





