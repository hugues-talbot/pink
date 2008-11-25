# writes a point list in a "b" list-type file
#-----------------------------------
proc my_write_b_list {filename pointlist} {
  set output [open $filename w]
  set n [expr [llength $pointlist] / 2]
  puts $output "b $n"
  puts $output $pointlist
  close $output
}

# writes a curve in a text file in the form of list of segments
#-----------------------------------
proc my_write_curve {filename pointlist} {
  set output [open $filename w]
  set n [expr [llength $pointlist] / 2]
  puts $output "$n"
  for {set k 0} {$k < [expr $n - 1]} {incr k} {
    puts $output "[lindex $pointlist [expr $k*2]] [lindex $pointlist [expr ($k*2)+1]] [lindex $pointlist [expr ($k+1)*2]] [lindex $pointlist [expr (($k+1)*2)+1]]"
  }
  close $output
}

# writes a number list in a "gnuplot" graph file
#-----------------------------------
proc my_write_gp_list {filename list} {
  set output [open $filename w]
  set n [llength $list]
  for {set k 0} {$k < $n} {incr k} {
    puts $output "$k [lindex $list $k]"
  }
  close $output
}

# writes a number list in a pgm 1d image file
#-----------------------------------
proc my_write_pgm1d_list {filename list} {
  set output [open $filename w]
  puts $output "PA"
  set n [llength $list]
  puts $output "$n 1"
  puts $output "255"
  for {set k 0} {$k < $n} {incr k} {
    puts $output "[lindex $list $k]"
  }
  close $output
}

# writes a text in a text file
#-----------------------------------
proc my_write_text {filename text} {
  set output [open $filename w]
  puts $output $text
  close $output
}
