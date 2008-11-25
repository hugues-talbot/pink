
# feedback procedure
#-----------------------------------
proc my_feedback { message } {
	.waitmess.entry config -state normal
	.waitmess.entry delete 0 end
	.waitmess.entry insert 0 $message
	# Leave the entry in a read-only state
	.waitmess.entry config -state disabled
	# Force a display update
	update idletasks
}
