/******************************************
 * TP 7 - Bibliothèque MBR
 * @authors: Damien Calesse, Henri Larget
 * @file: mbr.c
 ******************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "config.h"
#include "drive.h"
#include "mbr.h"

struct mbr_descr_s mbr;

void save_mbr() {
	write_sector_n(0, 0, sizeof(struct mbr_descr_s), (unsigned char*)&mbr);
}

void load_mbr() {
	unsigned int i;
	assert(sizeof(mbr) <= HDA_SECTORSIZE);
	read_sector_n(0, 0, sizeof(struct mbr_descr_s), (unsigned char *)&mbr);

	if (mbr.mbr_magic != MBR_MAGIC) {
		mbr.mbr_magic = MBR_MAGIC;
		for (i=0; i<MAX_VOL;i++)
			mbr.mbr_vol[i].vol_type = other;
	}
}
