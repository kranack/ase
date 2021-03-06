/******************************************
 * TP 5 - Bibliothèque Config
 * @authors: Damien Calesse, Henri Larget
 * @file: config.h
 ******************************************/

/* On définie les valeurs relatives au HDA (hardware.ini) */
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DEFAULT_HW_CONFIG "hardware.ini"

#endif

#define HDA_DATAREGS 0x110
#define HDA_CMDREG 0x3F6
#define HDA_IRQ 14

#define HDA_SECTORSIZE 512
#define HDA_MAXCYLINDER 1024
#define HDA_MAXSECTOR 32

/* GLOBAL */

#define BLOC_NULL 0
#define NDIRECT 10
#define NINDIRECT HDA_SECTORSIZE/sizeof(unsigned)

#define BLOC_SIZE       HDA_SECTORSIZE  
#define DATA_BLOC_SIZE  BLOC_SIZE
