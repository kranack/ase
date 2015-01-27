#!/bin/sh

make realclean
make
./mkhd
./mkvol
export CURRENT_VOLUME=0
./mknfs
