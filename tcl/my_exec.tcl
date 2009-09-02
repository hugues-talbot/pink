# external command call
#-----------------------------------
proc my_exec {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  if {$result != ""} {
    puts $result
  }
  return $result
}

# external command call with no error check
# (returns error message if any)
#-----------------------------------
proc my_exec_q {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  return $result
}

# external command call with no error check
# (displays error message if any)
#-----------------------------------
proc my_exec_v {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  puts $result
}

# external command call with fatal error test
#-----------------------------------
proc my_exec_f {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message "Fatal error: $result"
    exit
  }
}
