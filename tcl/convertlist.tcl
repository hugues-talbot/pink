#sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $@

#########################
#
# convertlist.tcl (linux)
#
# converts each image file in 'list' to the format 'outformat' using convert
# example: convertlist.tcl im*.pgm png /tmp
#
# Author: Andre Saude dec 2004
#
#########################

#parameters
if {$argc < 2} {
    puts "USAGE: convertlist.tcl filelist outformat \[output-dir\]"
    puts "\t converts each image file in 'list' to the format 'outformat'"
    puts "\t example: convertlist.tcl im*.pgm png /tmp"
}

set outdir [string trim [lindex $argv [expr $argc - 1]] "/"]

if [file isdirectory $outdir] {
    set argv [lrange $argv 0 [expr $argc - 2]]
    set argc [llength $argv]
    set nooutdir 0
} else {
    set nooutdir 1
}
set outformat [lindex $argv [expr $argc - 1]]
set argv [lrange $argv 0 [expr $argc - 2]]

#doit
foreach i $argv {
    #filenames
    set ext [file extension $i]
    set fname [string trim [file tail $i] .$ext]
    if {$nooutdir} {
	set outdir [file dirname $i]
    }
    set outf [file join $outdir $fname.$outformat]

    #verbose
    puts -nonewline "$i -> $outf \t\t"

    #seuil
    exec convert $i $outf

    #verbose
    puts "done"
}
