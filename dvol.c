#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "config.h"
#include "drive.h"
#include "mbr.h"

static void
empty_it()
{
    return;
}

int
main (int argc, char **argv)
{
	unsigned int i;
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
	printf ("-------------------\n Liste des volumes \n-------------------\n");
	for (i=0; i<MAX_VOL;i++) {
		if (mbr.mbr_vol[i].vol_type != other) {
			printf ("numéro vol: %d\t fc: %d\t fs: %d\t nsector: %d\t voltype: %d\n", 				i, 
						mbr.mbr_vol[i].vol_first_cylinder,
						mbr.mbr_vol[i].vol_first_sector,
						mbr.mbr_vol[i].vol_n_bloc,
						mbr.mbr_vol[i].vol_type);
		}
	}

    exit(EXIT_SUCCESS);
}
