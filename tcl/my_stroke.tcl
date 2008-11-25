#
# Example 34-8
# A canvas stroke drawing example.
#

proc StrokeBegin { w x y } {
	global stroke
	catch {unset stroke}
	set stroke(N) 0
	set stroke(0) [list $x $y]
}
proc Stroke { w x y col } {
	global stroke
	set coords $stroke($stroke(N))
	lappend coords $x $y
	incr stroke(N)
	set stroke($stroke(N)) [list $x $y]
	# eval gets the coordinates into individual arguments
	eval {$w create line} $coords {-tag segments -fill $col -width 3}
}
proc StrokeEnd { w x y col } {
	global stroke
	set coords {}
	for {set i 0} {$i <= $stroke(N)} {incr i} {
		append coords $stroke($i) " "
	}
	$w delete segments
	eval {$w create line} $coords \
		{-tag "line $col" -joinstyle round -smooth true -fill $col -width 3}
        return $coords
}
