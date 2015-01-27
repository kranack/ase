/******************************************
 * TP 5 - Bibliothèque Drive
 * @authors: Damien Calesse, Henri Larget
 * @file: drive.h
 ******************************************/

void dump (unsigned char *buffer, unsigned int buffer_size,
     		int ascii_dump, int octal_dump);

void read_sector (unsigned int cylinder, unsigned int sector,
					unsigned char *buffer);

void read_sector_n (unsigned int cylinder, unsigned int sector, 
						unsigned int nsector, unsigned char *buffer);

void write_sector (unsigned int cylinder, unsigned int sector,
					const unsigned char *buffer);

void write_sector_n (unsigned int cylinder, unsigned int sector,
						unsigned int nsector, const unsigned char *buffer);

void format_sector (unsigned int cylinder, unsigned int sector,
					unsigned int nsector, unsigned int value);

void goto_sector (unsigned int cylinder, unsigned int sector);

void check_hardware();
