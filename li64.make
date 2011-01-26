PINK = .
# Version LINUX
#  XLIB = -L/usr/X11R6/lib -lX11 -lXext
  XINCL = /usr/include/X11R6
  CC = gcc
  CPP = g++
  CCFLAGS = -g -DUNIXIO -DMC_64_BITS -Wall
  ODIR = $(PINK)/linux/obj
  BDIR = $(PINK)/linux/bin
  LIBS = -lm
  OBJ_COMMON = $(ODIR)/mcimage.o $(ODIR)/mcchrono.o
  OBJ = $(ODIR)/mcliste.o $(ODIR)/mcset.o $(ODIR)/mckhalimsky2d.o $(ODIR)/mckhalimsky3d.o $(ODIR)/mckhalimskyNd.o $(ODIR)/mcchrono.o $(ODIR)/mctopo3d_table.o $(ODIR)/mcdrawps.o $(ODIR)/mcsegment.o