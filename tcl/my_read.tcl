# reads a pgm image header
#-----------------------------------
proc my_readpgmheader {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  while {([string index $line 0] == "#") || ([string length $line] == 0)} {
    set line [gets $input]
  }
  scan $line "%d %d" rs cs
  close $input
  return [list $tag $rs $cs]
}

# reads a pgm 2d image data
#-----------------------------------
proc my_readpgmdata {filename} {
  set tmpfile "/tmp/myreadpgmdata_tmp"
  catch { exec pgmrawmode $filename $tmpfile } result
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
    return 1
  }
  set im [image create photo imname -file $tmpfile]
  set cs [image height $im]
  set rs [image width $im]
  set data [list]
  for {set j 0} {$j < $cs} {incr j} {
    for {set i 0} {$i < $rs} {incr i} {
      set data [concat $data [lindex [$im get $i $j] 0]]
    }
  }
  catch { exec rm "-f" $tmpfile } result
  return $data
}

# reads a pgm 3d image header
#-----------------------------------
proc my_readpgmheader3 {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  while {[string index $line 0] == "#"} {
    set line [gets $input]
  }
  scan $line "%d %d %d" rs cs ds
  close $input
  return [list $tag $rs $cs $ds]
}

# reads a pgm 3d image data
#-----------------------------------
proc my_readpgmdata3 {filename} {
  set h [my_readpgmheader3 $filename]
  set ds [lindex $h 2]
  set data [list]
  set tmpfile "/tmp/myreadpgmdata3_tmp"
  for {set k 0} {$k < $ds} {incr k} {
    catch { exec extractplane $filename $k xy $tmpfile } result
    if {$result != ""} {
      tk_messageBox -type ok -icon error -message $result
      return 1
    }
    catch { exec pgmrawmode $tmpfile } result
    if {$result != ""} {
      tk_messageBox -type ok -icon error -message $result
      return 1
    }
    set d [my_readpgmdata $tmpfile]
    set data [concat $data $d]
  }
  catch { exec rm "-f" $tmpfile } result
  return $data  
}

# reads a text in a text file
#-----------------------------------
proc my_read_text {filename} {
  set input [open $filename]
  set text [read $input]
  close $input
  return $text
}
