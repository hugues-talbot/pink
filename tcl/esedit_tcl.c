/*! \file esedit.tcl

\brief structuring element editor

<B>Usage:</B> esedit.tcl <width height filename | filename>

<B>Description:</B>
If given alone, parameter \b filename indicates a file which contains the 
structuring element to be edited.

Otherwise, \b width and \b height indicate the dimensions of the
element to be created.

Check \b origin to specify the origin of the structuring element.

Check \b revert to set all blue points in the structuring element,
and all red points in its complementary.

Use the \b Abort button to quit without saving.

Use the \b Save/Quit button to save and quit.

<B>Types supported:</B> byte 2d

<B>Category:</B> inter
\ingroup inter

\author Michel Couprie
*/
