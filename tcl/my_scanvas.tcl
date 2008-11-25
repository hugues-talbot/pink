proc my_scanvas { c args } {
	frame $c
	eval {canvas $c.canvas \
		-xscrollcommand [list $c.xscroll set] \
		-yscrollcommand [list $c.yscroll set] \
		-highlightthickness 0 \
		-borderwidth 0} $args
	scrollbar $c.xscroll -orient horizontal \
		-command [list $c.canvas xview]
	scrollbar $c.yscroll -orient vertical \
		-command [list $c.canvas yview]
	grid $c.canvas $c.yscroll -sticky news
	grid $c.xscroll -sticky ew
	grid rowconfigure $c 0 -weight 1
	grid columnconfigure $c 0 -weight 1
	return $c.canvas
}
