PINK = .
# Version LINUX
#  XLIB = -L/usr/X11R6/lib -lX11 -lXext
  XINCL = /usr/include/X11R6
  CC = cc
  CPP = g++
  CCFLAGS = -g -Wall -DPC -DUNIXIO
  ODIR = $(PINK)/linux/obj
  BDIR = $(PINK)/linux/bin
  LIBS = -lm
  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/mcchrono.o
#  OBJ = $(ODIR)/mcliste.o $(ODIR)/mcset.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o $(ODIR)/mcchrono.o $(ODIR)/mcxbib.o $(ODIR)/mctopo3d_table.o $(ODIR)/mcdrawps.o
  OBJ = $(ODIR)/mcliste.o $(ODIR)/mcset.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o $(ODIR)/mcchrono.o $(ODIR)/mctopo3d_table.o $(ODIR)/mcdrawps.o
