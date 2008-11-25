#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $@

#########################
#
# minctopgm.tcl (linux with minc installed)
#
# converts each minc image file in 'filelist' to the Pink pgm format
# requires minc software
# example: minctopgm.tcl im*.mnc /tmp
#
# Author: Andre Saude dec 2004
#
#########################

#parameters
set l [llength $argv]

if {$l == 0} {
    puts "USAGE: minctopgm.tcl filelist \[output-dir\]"
    puts "\t converts each minc image file in 'filelist' to the Pink pgm format"
    puts "\t requires minc software"
    puts "\t example: minctopgm.tcl im*.mnc /tmp"
}

set outdir [string trim [lindex $argv [expr $l - 1]] "/"]
if [file isdirectory $outdir] {
    set argv [lrange $argv 0 [expr $l - 2]]
} else {
    set outdir "."
}

#doit
foreach i $argv {
    #filenames
    regsub ".mnc" $i ".raw" auxf
    if {[string first ".raw" $auxf] < 0} {set auxf $auxf.raw}
    set auxf $outdir/$auxf

    regsub ".mnc" $i ".pgm" outf
    if {[string first ".pgm" $outf] < 0} {set outf $outf.pgm}
    set outf $outdir/$outf

    #verbose
    puts -nonewline "$i -> $auxf -> $outf \t\t"

    #dimensions
    set inf [exec mincinfo $i]

    set aux [expr [string last xspace $inf] + 10]
    set xspace [string trim [string range $inf $aux [expr $aux + 30]]]
    set aux [expr [string last yspace $inf] + 10]
    set yspace [string trim [string range $inf $aux [expr $aux + 30]]]
    set aux [expr [string last zspace $inf] + 10]
    set zspace [string trim [string range $inf $aux [expr $aux + 30]]]

    #minctoraw
    exec minctoraw -normalize $i > $auxf

    #raw2pgm
    exec raw2pgm $auxf $zspace $yspace $xspace 0 1 0 $outf

    #verbose
    puts "done"
}
