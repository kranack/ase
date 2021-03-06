/******************************************
 * TP 7 - Bibliothèque SUPER
 * @authors: Damien Calesse, Henri Larget
 * @file: super.c
 ******************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware.h"
#include "config.h"
#include "drive.h"
#include "mbr.h"
#include "super.h"

unsigned int cylinder_of_bloc(unsigned int nvol, unsigned int nbloc) {
	/* On récupère le premier cylindre & le premier secteur */
	unsigned int fc = mbr.mbr_vol[nvol].vol_first_cylinder;
	unsigned int fs = mbr.mbr_vol[nvol].vol_first_sector;
	assert(nvol < MAX_VOL);
	assert(mbr.mbr_vol[nvol].vol_type == base);
	fprintf(stderr, "nvol:%u\tnbloc:%u\n", nvol, nbloc);
	fflush(stderr);
	/*fprintf(stderr, "nvol:%u;nbloc:%u;\nmbr.mbr_vol[nvol].vol_first_sector:%u;mbr.mbr_vol[nvol].vol_n_bloc:%u;\n", nvol, nbloc, mbr.mbr_vol[nvol].vol_first_sector, mbr.mbr_vol[nvol].vol_n_bloc);
	fflush(stderr);*/
	assert(nbloc < (mbr.mbr_vol[nvol].vol_first_sector + mbr.mbr_vol[nvol].vol_n_bloc));
	return fc+(fs+nbloc)/HDA_MAXSECTOR;
}

unsigned int sector_of_bloc(unsigned int nvol, unsigned int nbloc) {
	unsigned int fs = mbr.mbr_vol[nvol].vol_first_sector;
	return (fs+nbloc)%HDA_MAXSECTOR;
}

void read_bloc (unsigned int vol, unsigned int nbloc,
					unsigned char *buffer) {
	read_sector(cylinder_of_bloc(vol, nbloc), sector_of_bloc(vol, nbloc), buffer);
}

void read_bloc_n (unsigned int vol, unsigned int nbloc, 
					unsigned char *buffer, unsigned int buf_size) {
	read_sector_n(cylinder_of_bloc(vol, nbloc), sector_of_bloc(vol, nbloc),
						buf_size, buffer);
}

void write_bloc (unsigned int vol, unsigned int nbloc,
					unsigned char *buffer) {
	write_sector(cylinder_of_bloc(vol, nbloc), sector_of_bloc(vol, nbloc), buffer);
}

void write_bloc_n (unsigned int vol, unsigned int nbloc, 
					const unsigned char *buffer, unsigned int buf_size) {
	write_sector_n(cylinder_of_bloc(vol, nbloc), sector_of_bloc(vol, nbloc),
						buf_size, buffer);
}

void init_super(unsigned int vol) {
	struct freebloc_s fb;
	super.super_magic = SUPER_MAGIC;
	super.super_root = 0;
	super.super_first_free = SUPER+1;

	write_bloc_n(vol, SUPER, (unsigned char*)&super, sizeof(struct super_s));
	fb.fb_nbloc = mbr.mbr_vol[vol].vol_n_bloc-1;
	fb.fb_next = 0;
	write_bloc_n(vol, SUPER+1, (unsigned char*)&fb, sizeof(struct freebloc_s));
	current_volume = vol;
}

void save_super() {
	write_bloc_n(current_volume, SUPER, (unsigned char*)&super, sizeof(struct super_s));
}

int load_super(unsigned int vol) {
	read_bloc_n(vol, SUPER, (unsigned char*)&super, sizeof(struct super_s));
	assert(super.super_magic == SUPER_MAGIC);
	current_volume = vol;
	return EXIT_SUCCESS;
}

unsigned int new_bloc() {
	unsigned int new;
	struct freebloc_s fb;
	
	if (super.super_first_free == 0)
		return 0;

	new = super.super_first_free;
	printf("root: %u; first: %u\n", super.super_root, super.super_first_free);
	read_bloc_n(current_volume, new, (unsigned char*)&fb, sizeof(struct freebloc_s));

	if (fb.fb_nbloc == 1) {
		super.super_first_free = fb.fb_next;
	} else {
		super.super_first_free++;
		fb.fb_next = new;
		fb.fb_nbloc--;
		write_bloc_n(current_volume,new+1, (unsigned char*)&fb, sizeof(struct freebloc_s));
	}
	return new;
}

void free_bloc(unsigned int bloc) {
	struct freebloc_s fb;
	fb.fb_nbloc = 1;
	fb.fb_next = super.super_first_free;
	super.super_first_free = bloc;
	write_bloc_n(current_volume, bloc, (unsigned char*) &fb, sizeof(struct freebloc_s));
}

void free_blocs(unsigned blocs[], unsigned n) {
	unsigned int i;
	for (i =0; i < n; i++) {
		if (blocs[i])
			free_bloc(blocs[i]);
	}
}
