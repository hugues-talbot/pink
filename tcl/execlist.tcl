#!/bin/sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $@

#########################
# 
# execlist.tcl pinkCommand inputFileList [options]
#
# executes the Pink command 'pinkCommand' for the list of files in 'filelist'
# and write the output for each image in filename'outputSuffix'.pgm
#
# Arguments:
#    pinkCommand:   a Pink executable
#    inputFileList: the list of input files
#    [options]:      optional arguments
#        -a args:         the arguments for the Pink command
#        -o outputSuffix: the suffix of the output files; default: _$pinkCommand_$arg1_$arg2_...
#        -d outdir:       the output directory
#
# example: execlist.tcl seuil im*.pgm -a 12 -o _seuil12 -d /tmp"
#
# Author: Andre Saude dec 2004
#
#########################

#usage
proc printUsage {} {
    puts "\nUSAGE: execlist.tcl pinkCommand inputFileList \[options\]"
    puts "\nExecutes the Pink command 'pinkCommand' for the list of files in 'filelist'"
    puts "and write the output for each image in filename'outputSuffix'.pgm"
    puts "\n\[options\]:      optional arguments"
    puts "\t -a args:         the arguments for the Pink command"
    puts "\t -o outputSuffix: the suffix of the output files; default: _pinkCommand_arg1_arg2_..."
    puts "\t -d outdir:       the output directory"
    puts "\nExample: execlist.tcl seuil im*.pgm -a 12 -o _seuil12 -d /tmp"
    puts "\t executes a threshold at 12 for each file in im*.pgm"
    puts "\t and write the output for each image in filename_seuil12.pgm"
}

#parameters
if {$argc < 2} {
    printUsage
    exit
}

set order [list $argc $argc $argc $argc]
set occurence 0

set ai 0
set oi 0
set di 0
set a $argc
set o $argc
set d $argc

set nooutdir 1
set nooutsuf 1

for {set i 0} {$i < $argc} {incr i} {
    switch -exact -- [lindex $argv $i] {
	-a {
	    if {$a != $argc} {
		puts "ERROR: -a option given twice"
		printUsage
		exit
	    }
	    set a $i
	    set ai $occurence
	    lset order $ai $a
	    incr occurence
	}
	-o {
	    if {$o != $argc} {
		puts "ERROR: -o option given twice"
		printUsage
		exit
	    }
	    set o $i
	    set oi $occurence
	    lset order $oi $o
	    set nooutsuf 0
	    incr occurence
	}
	-d {
	    if {$d != $argc} {
		puts "ERROR: -d option given twice"
		printUsage
		exit
	    }
	    set d $i
	    set di $occurence
	    lset order $di $d
	    set nooutdir 0
	    incr occurence
	}
    }
}

#the parameters, finally
set pinkCommand [lindex $argv 0]
set filelist [lrange $argv 1 [expr [lindex $order 0] - 1]]
set args [lrange $argv [expr $a + 1] [expr [lindex $order [expr $ai + 1]] - 1]]
set outsuf [lrange $argv [expr $o + 1] [expr [lindex $order [expr $oi + 1]] - 1]]
set outdir [lrange $argv [expr $d + 1] [expr [lindex $order [expr $di + 1]] - 1]]

#test them
if {[llength $filelist] < 1} {
    puts "ERROR: no input file given"
    printUsage
    exit
}
if {[llength $outsuf] > 1} {
    puts "ERROR: more than one output suffix given"
    printUsage
    exit
}
if {[llength $outdir] > 1} {
    puts "ERROR: more than one output directory given"
    printUsage
    exit
}

#correct optional parameters
if {!$nooutdir} {
    puts carai
    if {![file isdirectory $outdir]} {
	puts "ERROR: -d option given is not a directory"
	printUsage
	exit
    }
}
if {!$nooutsuf} {
    if {[llength $outsuf] != 1} {
	set nooutsuf 1
    }
}
if {$nooutsuf} {
    set outsuf "_$pinkCommand"
    foreach i $args {
	set outsuf "$outsuf\_$i"
    }
}

#doit
foreach i $filelist {
    #filenames
    set ext [file extension $i]
    set fname [file rootname [file tail $i]]
    if {$nooutdir} {
	set outdir [file dirname $i]
    }
    set outf [file join $outdir $fname$outsuf$ext]
    set inf [file join [file dirname $i] [file tail $i]]

    #verbose
    puts -nonewline "\n$pinkCommand $i $args $outf ... \t\t"

    #exec
     set eargs [concat $inf $args $outf]
     set call "catch { exec $pinkCommand $eargs } result"
     eval $call
     if {$result != ""} {
 	puts "\n\nERROR in execution:"
 	puts "\t$result"
 	puts "\nAborted!"
 	exit
     }

    #verbose
    puts "done"
}
