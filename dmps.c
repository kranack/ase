/******************************************
 * TP 5 - Dump Sector
 * @authors: Damien Calesse, Henri Larget
 * @file: dmps.c
 ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "config.h"
#include "tools.h"
#include "drive.h"



static void
empty_it()
{
    return;
}

void dmps (unsigned int cylinder, unsigned int sector) {
	check_hardware();
	
	read_sector(cylinder, sector, MASTERBUFFER);
	dump(MASTERBUFFER, HDA_SECTORSIZE, TRUE, TRUE);
}

int main (int argc, char **argv) {
	unsigned int i;
	if (argc < 2) {
		fprintf(stderr, "Bad usage for dmps : dmps cylinder sector\n");
		return 1;
	}
	else {
	 	if(init_hardware("hardware.ini") == 0) {
			fprintf(stderr, "Error in hardware initialization\n");
			exit(EXIT_FAILURE);
		}
		 /* Interreupt handlers */
		for(i=0; i<16; i++)
			IRQVECTOR[i] = empty_it;

		/* Allows all IT */
		_mask(1);
		dmps(atoi(argv[1]), atoi(argv[2]));
		return 0;
	}
}
