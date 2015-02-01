# $Id: Makefile,v 1.2 2004/10/12 09:06:17 marquet Exp $
##############################################################################

ROOTDIR=.

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic
CFLAGS	+= -std=gnu99
#CFLAGS  += -std=iso9899:1999
#CFLAGS 	+= -std=c99 -D_XOPEN_SOURCE>=500
CFLAGS  += -g -m32
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES= mkhd dmps frmt mkvol dvol mknfs if_pfile if_nfile if_cfile if_dfile vm bash
OBJECTS	= $(addsuffix .o,\
	  bash vm-skel current mkhd drive dmps dump frmt mkvol dvol mknfs mbr super inode ifile mount tools if_pfile if_nfile if_cfile if_dfile dir)

all: $(BINARIES) $(OBJECTS)


###------------------------------
### Main rules 
###------------------------------------------------------------
vm-skel.o: vm-skel.c
	$(CC) $(CFLAGS) -c vm-skel.c $(INCDIR)

bash.o: bash.c
	$(CC) $(CFLAGS) -c bash.c $(INCDIR)

bash: bash.o drive.o mbr.o dir.o ifile.o inode.o tools.o
	$(CC) $(CFLAGS) -o bash bash.o drive.o mbr.o super.o dir.o ifile.o inode.o tools.o $(LIBS)

vm: vm-skel.o drive.o mbr.o
	$(CC) $(CFLAGS) -o vm vm-skel.o drive.o mbr.o $(LIBS)

mkhd.o: mkhd.c
	$(CC) $(CFLAGS) -c mkhd.c $(INCDIR)

mkhd: mkhd.o
	$(CC) $(CFLAGS) -o mkhd mkhd.o $(LIBS)

dump.o: dump.c
	$(CC) $(CFLAGS) -c dump.c $(INCDIR)

drive.o: drive.c dump.c
	$(CC) $(CFLAGS) -c drive.c $(INCDIR)

mbr.o: mbr.c
	$(CC) $(CFLAGS) -c mbr.c $(INCDIR)

super.o: super.c
	$(CC) $(CFLAGS) -c super.c $(INCDIR)

dmps.o: dmps.c
	$(CC) $(CFLAGS) -c dmps.c $(INCDIR)

dmps: dmps.o drive.o dump.o
	$(CC) $(CFLAGS) -o dmps dmps.o drive.o dump.o $(LIBS)

frmt.o: frmt.c
	$(CC) $(CFLAGS) -c frmt.c $(INCDIR)

frmt: frmt.o drive.o dump.o
	$(CC) $(CFLAGS) -o frmt frmt.o drive.o dump.o $(LIBS)

mkvol.o: mkvol.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=500 -c mkvol.c $(INCDIR)

mkvol: mkvol.o drive.o dump.o mbr.o
	$(CC) $(CFLAGS) -o mkvol mkvol.o drive.o dump.o mbr.o $(LIBS)

dvol.o: dvol.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=500 -c dvol.c $(INCDIR)

dvol: dvol.o drive.o dump.o mbr.o
	$(CC) $(CFLAGS) -o dvol dvol.o drive.o dump.o mbr.o $(LIBS)

current.o: current.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=500 -c current.c $(INCDIR)

mknfs.o: mknfs.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=500 -c mknfs.c $(INCDIR)

mknfs: mknfs.o drive.o dump.o mbr.o super.o current.o ifile.o inode.o tools.o
	$(CC) $(CFLAGS) -o mknfs mknfs.o drive.o dump.o mbr.o super.o current.o ifile.o inode.o tools.o $(LIBS)

inode.o: inode.c
	$(CC) $(CFLAGS) -c inode.c $(INCDIR)

ifile.o: ifile.c 
	$(CC) $(CFLAGS) -c ifile.c $(INCDIR)

if_cfile.o: if_cfile.c
	$(CC) $(CFLAGS) -c if_cfile.c $(INCDIR)
if_dfile.o: if_dfile.c
	$(CC) $(CFLAGS) -c if_dfile.c $(INCDIR)
if_nfile.o: if_nfile.c
	$(CC) $(CFLAGS) -c if_nfile.c $(INCDIR)
if_pfile.o: if_pfile.c
	$(CC) $(CFLAGS) -c if_pfile.c $(INCDIR)

dir.o: dir.c
	$(CC) $(CFLAGS) -c dir.c $(INCDIR)

mount.o: mount.c tools.o
	$(CC) $(CFLAGS) -c mount.c $(INCDIR)
tools.o: tools.c
	$(CC) $(CFLAGS) -c tools.c $(INCDIR)

if_cfile : if_cfile.o mount.o ifile.o inode.o
	$(CC) $(CFLAGS) -o if_cfile if_cfile.o ifile.o drive.o dump.o mbr.o super.o inode.o mount.o tools.o $(LIBS)
if_nfile : if_nfile.o mount.o ifile.o inode.o
	$(CC) $(CFLAGS) -o if_nfile if_nfile.o ifile.o drive.o dump.o mbr.o super.o inode.o mount.o tools.o $(LIBS)
if_dfile : if_dfile.o mount.o ifile.o inode.o
	$(CC) $(CFLAGS) -o if_dfile if_dfile.o ifile.o drive.o dump.o mbr.o super.o inode.o mount.o tools.o $(LIBS)
if_pfile : if_pfile.o mount.o ifile.o inode.o
	$(CC) $(CFLAGS) -o if_pfile if_pfile.o ifile.o drive.o dump.o mbr.o super.o inode.o mount.o tools.o $(LIBS)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o *.gch $(BINARIES)
realclean: clean 
	$(RM) vdiskA.bin vdiskB.bin
