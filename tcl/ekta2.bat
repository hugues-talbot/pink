echo OFF
set PINK=e:\Pink
for /f %%x in ('dir /os /b *.bmp') do tclsh e:\Pink\tcl\ekta1 %%x

