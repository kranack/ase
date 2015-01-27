#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "config.h"
#include "drive.h"
#include "mbr.h"
#include "super.h"

#include "ifile.h"

static void
empty_it()
{
    return;
}

int
main (int argc, char **argv)
{
	unsigned int i;
	unsigned int inumber;
	struct freebloc_s fb;
    if(init_hardware("hardware.ini") == 0) {
		fprintf(stderr, "Error in hardware initialization\n");
		exit(EXIT_FAILURE);
	}
	 /* Interreupt handlers */
	for(i=0; i<16; i++)
		IRQVECTOR[i] = empty_it;

	/* Allows all IT */
	_mask(1);

	/* On rempli le MBR */
	check_hardware();
	load_mbr();
	init_super(current_volume);
	save_super();
	load_super(current_volume);
	
	//inumber = create_ifile(directory);
	//fprintf(stdout, "inumber of root dir: %u\n", inumber);
	fprintf(stdout, "vol : %d\t super_magic: %d\t super_first_free: %d\n",
				current_volume, super.super_magic, super.super_first_free);
	fflush(stdout);
	read_bloc_n(current_volume, super.super_first_free, (unsigned char*)&fb, sizeof(struct freebloc_s));
	fprintf(stdout, "fb_nbloc : %d\t fb_next : %d\n", fb.fb_nbloc, fb.fb_next);
	while (fb.fb_next != 0) {
		read_bloc_n(current_volume, fb.fb_next, (unsigned char*)&fb, sizeof(struct freebloc_s));
		fprintf(stdout, "fb_nbloc : %d\t fb_next : %d\n", fb.fb_nbloc, fb.fb_next);
	}
	
	inumber = create_ifile(directory);
	fprintf(stdout, "inumber of root dir: %u\n", inumber);
	super.super_root = inumber;
	save_super();
    exit(EXIT_SUCCESS);
}
