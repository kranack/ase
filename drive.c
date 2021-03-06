/******************************************
 * TP 5 - Bibliothèque Drive
 * @authors: Damien Calesse, Henri Larget
 * @file: drive.c
 ******************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "config.h"
#include "tools.h"
#include "drive.h"

/*##########################
 #
 # SECTOR & CYLINDER
 ###########################*/

void read_sector (unsigned int cylinder, unsigned int sector,
					unsigned char *buffer) {
	goto_sector(cylinder, sector);
	/* On écrit les arguments pour read */
	_out(HDA_DATAREGS, 0);
	_out(HDA_DATAREGS+1, 1);
	/* On execute la commande CMD_READ */
	_out(HDA_CMDREG, CMD_READ);
	_sleep(HDA_IRQ);
	//dump(buffer, HDA_SECTORSIZE, TRUE, TRUE);
	memcpy(buffer, MASTERBUFFER, HDA_SECTORSIZE);memcpy(buffer, MASTERBUFFER, HDA_SECTORSIZE);
}

void read_sector_n (unsigned int cylinder, unsigned int sector, 
						unsigned int buf_size, unsigned char *buffer) {
	unsigned int i;

	goto_sector(cylinder, sector);
	/* On écrit les arguments pour read */
	_out(HDA_DATAREGS, 0 & 0xFF);
	_out(HDA_DATAREGS+1, 1 & 0xFF);
	/* On execute la commande CMD_READ */
	_out(HDA_CMDREG, CMD_READ);
	_sleep(HDA_IRQ);
	for(i=0; i<buf_size; i++) {
		buffer[i] = MASTERBUFFER[i];
	}
	
}

void write_sector (unsigned int cylinder, unsigned int sector,
					const unsigned char *buffer) {
	goto_sector(cylinder, sector);
	memcpy(MASTERBUFFER, buffer, HDA_SECTORSIZE);

	_out(HDA_DATAREGS, 0);
	_out(HDA_DATAREGS+1, 1);
	_out(HDA_CMDREG, CMD_WRITE);
	_sleep(HDA_IRQ);
}

void write_sector_n (unsigned int cylinder, unsigned int sector,
						unsigned int buf_size, const unsigned char *buffer) {
	goto_sector(cylinder, sector);
	memcpy(MASTERBUFFER, buffer, buf_size);

	_out(HDA_DATAREGS, 0 & 0xFF);
	_out(HDA_DATAREGS+1, 1 & 0xFF);
	_out(HDA_CMDREG, CMD_WRITE);
	_sleep(HDA_IRQ);

}

void format_sector (unsigned int cylinder, unsigned int sector,
					unsigned int nsector, unsigned int value) {
	unsigned int i;
	/* Se déplacer au bon cylindre */
	goto_sector(cylinder, sector);
	for(i=0;i<nsector;i++) {
		/* Nombre de secteurs à formatter */
		_out(HDA_DATAREGS, (0>> 8) & 0xFF);
		_out(HDA_DATAREGS+1, 1 & 0xFF);
		/* Valeur à écrire */
		_out(HDA_DATAREGS+2, (value>> 24) & 0xFF);
		_out(HDA_DATAREGS+3, (value>> 16) & 0xFF);
		_out(HDA_DATAREGS+4, (value>> 8) & 0xFF);
		_out(HDA_DATAREGS+5, value & 0xFF);
		_out(HDA_CMDREG, CMD_FORMAT);
	    _sleep(HDA_IRQ);
	}

}

void goto_sector (unsigned int cylinder, unsigned int sector) {
	
	/* On écrit les arguments pour seek */
	_out(HDA_DATAREGS, (cylinder >> 8) & 0xFF);
	_out(HDA_DATAREGS+1, cylinder & 0xFF);
	_out(HDA_DATAREGS+2, (sector >> 8) & 0xFF);
	_out(HDA_DATAREGS+3, sector & 0xFF);
	/* On execute la commande CMD_SEEK */
	_out(HDA_CMDREG, CMD_SEEK);
	_sleep(HDA_IRQ);
	
}


void check_hardware() {
	
	unsigned int max_cylinder, max_sector, sector_size;

	/* On execute la commande CMD_SEEK */
	_out(HDA_CMDREG, CMD_DSKINFO);
	
	max_sector = _in(HDA_DATAREGS) << 8;
	max_sector |= _in(HDA_DATAREGS+1);
	max_cylinder = _in(HDA_DATAREGS+2) << 8;
	max_cylinder |= _in(HDA_DATAREGS+3);
	sector_size = _in(HDA_DATAREGS+4) << 8;
	sector_size |= _in(HDA_DATAREGS+5);

	if ((max_cylinder != HDA_MAXCYLINDER) && (max_sector != HDA_MAXSECTOR) && (sector_size != HDA_SECTORSIZE))
		exit(1);
	
}

