/******************************************
 * TP 7 - Bibliothèque Inode
 * @authors: Damien Calesse, Henri Larget
 * @file: inode.c
 ******************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "inode.h"
#include "drive.h"
#include "mbr.h"
#include "super.h"

void read_inode (unsigned int inumber, struct inode_s *inode) {
	read_bloc_n(current_volume, inumber, (unsigned char *)inode, sizeof(struct inode_s));
}

void write_inode (unsigned int inumber, const struct inode_s *inode) {
	fprintf(stdout, "inode_size: %u; sizeof inode: %u\n", inode->ind_size, sizeof(struct inode_s));
	, sizeof(struct inode_s)write_bloc_n(current_volume, inumber, (unsigned char *)inode, sizeof(struct inode_s));
}

unsigned int create_inode (enum file_type_e type) {
	struct inode_s inode;
	unsigned int i, inumber;
	inode.ind_type = type;
	inode.ind_size = 0;
	for (i =0; i < NDIRECT; i++) {
		inode.ind_direct[i] = BLOC_NULL;
	}
	inode.ind_indirect = BLOC_NULL;
	inode.ind_d_indirect = BLOC_NULL;
	inumber = new_bloc();
	if (inumber == 0)
		return 0;
	write_inode(inumber, &inode);
	return inumber;
}

int delete_inode (unsigned int inumber) {
	struct inode_s inode;
	unsigned blocs[NINDIRECT]/*, blocs2[NINDIRECT]*/;
	/*unsigned int i;*/
	/* On lit l'inode et on libère les blocs directs*/
	read_inode(inumber, &inode);
	if (inode.ind_direct != BLOC_NULL)
		free_blocs(inode.ind_direct, NDIRECT);
	/* On lit les blocs indirects et on les libère */
	read_bloc(current_volume, inode.ind_indirect, (unsigned char *)blocs);
	if (inode.ind_indirect != BLOC_NULL) {
		free_blocs(blocs, NINDIRECT);
		free_bloc(inode.ind_indirect);
	}
	/* On lit les blocs double indirects et on les libère */
	read_bloc(current_volume, inode.ind_d_indirect, (unsigned char *)blocs);
	if (inode.ind_d_indirect != BLOC_NULL) {
		free_blocs(blocs, NINDIRECT);
		free_bloc(inode.ind_indirect);
	}
	/*for (i=0; i < NINDIRECT; i++) {
		read_bloc(current_volume, inode.ind_d_indirect[i], (unsigned char *)blocs2);
		free_blocs(blocs2, NINDIRECT);
	}*/
	free_bloc(inumber);
	return 0;
}

unsigned int vbloc_of_fbloc (unsigned int inumber, unsigned int fbloc, bool_t do_allocate) {
	struct inode_s inode;
	unsigned blocs[NINDIRECT], blocs2[NINDIRECT];
	unsigned int index1, index2;

	read_inode(inumber, &inode);
	
	switch(do_allocate){

		case FALSE :
			if (fbloc < NDIRECT) {
				return inode.ind_direct[fbloc];
			}
			fbloc -= NDIRECT;
			if (fbloc < NINDIRECT) {
				if (inode.ind_indirect == BLOC_NULL)
					return BLOC_NULL;
				read_bloc(current_volume, inode.ind_indirect, (unsigned char *)blocs);
				return blocs[fbloc];
			}
			fbloc -= NINDIRECT;
			if (fbloc < NINDIRECT * NINDIRECT) {
				if (inode.ind_d_indirect == BLOC_NULL)
					return BLOC_NULL;
				read_bloc(current_volume, inode.ind_d_indirect, (unsigned char *)blocs);
				index1 = fbloc / NINDIRECT;
				index2 = fbloc % NINDIRECT;
				if (blocs[index1] == BLOC_NULL)
					return BLOC_NULL;
				read_bloc(current_volume, blocs[index1], (unsigned char *)blocs2);
				return blocs2[index2];
			}
			break;
		case TRUE :
			// TODO : Indirect & Double Indirect
			if (fbloc < NDIRECT) {
				if (inode.ind_direct[fbloc] == BLOC_NULL)
					inode.ind_direct[fbloc] = new_bloc();
				write_inode(inumber, &inode);
				return inode.ind_direct[fbloc];
			}
			fbloc -= NDIRECT;
			if (fbloc < NINDIRECT) {
				if (inode.ind_indirect == BLOC_NULL)
					return BLOC_NULL;
				read_bloc(current_volume, inode.ind_indirect, (unsigned char *)blocs);
				return blocs[fbloc];
			}
			fbloc -= NINDIRECT;
			if (fbloc < NINDIRECT * NINDIRECT) {
				if (inode.ind_d_indirect == BLOC_NULL)
					return BLOC_NULL;
				read_bloc(current_volume, inode.ind_d_indirect, (unsigned char *)blocs);
				index1 = fbloc / NINDIRECT;
				index2 = fbloc % NINDIRECT;
				if (blocs[index1] == BLOC_NULL)
					return BLOC_NULL;
				read_bloc(current_volume, blocs[index1], (unsigned char *)blocs2);
				return blocs2[index2];
			}
			break;
		default :
			break;
	}
	fprintf(stderr, "Erreur : Numéro de bloc trop grand");
	exit(EXIT_FAILURE);
}
