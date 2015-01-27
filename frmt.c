/******************************************
 * TP 5 - Format Sector
 * @authors: Damien Calesse, Henri Larget
 * @file: frmt.c
 ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "drive.h"
#include "config.h"

static void
empty_it()
{
    return;
}

void frmt(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value) {
	
	check_hardware();
	format_sector(cylinder, sector, nsector, value);

}

int main (int argc, char **argv) {
	unsigned int i;
	if (argc < 4) {
		fprintf(stderr, "Bad usage for frmt : frmt cylinder sector nsector value\n");
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
		frmt(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		return 0;
	}
	
}
