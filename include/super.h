/******************************************
 * TP 5 - Bibliothèque SUPER
 * @authors: Damien Calesse, Henri Larget
 * @file: super.h
 ******************************************/

/* SUPERBLOC */
struct super_s {
	unsigned int super_magic;
	unsigned int super_serial;
	char		 super_name[32];
	unsigned int super_root;
	unsigned int super_first_free;
};

struct freebloc_s {
	unsigned	fb_nbloc;
	unsigned	fb_next;
};

struct super_s super;
unsigned int current_volume;
#define SUPER_MAGIC 0XDECAFE
#define SUPER 0

unsigned int cylinder_of_bloc (unsigned int nvol, unsigned int nbloc);

unsigned int sector_of_bloc (unsigned int nvol, unsigned int nbloc);

void read_bloc (unsigned int vol, unsigned int nbloc, 
					unsigned char *buffer);

void read_bloc_n (unsigned int vol, unsigned int nbloc, 
					unsigned char *buffer, unsigned int buf_size);

void write_bloc (unsigned int vol, unsigned int nbloc, 
					unsigned char *buffer);

void write_bloc_n (unsigned int vol, unsigned int nbloc, 
					const unsigned char *buffer, unsigned int buf_size);

unsigned int new_bloc();

void format_vol (unsigned int vol);

void init_super (unsigned int vol);

void save_super();

int load_super (unsigned int vol);

void free_bloc (unsigned int bloc);

void free_blocs(unsigned blocs[], unsigned n);
