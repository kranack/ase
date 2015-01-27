/* ------------------------------
   $Id: vm-skel.c,v 1.1 2002/10/21 07:16:29 marquet Exp $
   ------------------------------------------------------------

   Volume manager skeleton.
   Philippe Marquet, october 2002

   1- you must complete the NYI (not yet implemented) functions
   2- you may add commands (format, etc.)
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware.h"
#include "config.h"
#include "drive.h"
#include "mbr.h"
#include "super.h"
#include "ifile.h"
#include "dir.h"

/* ------------------------------
   command list
   ------------------------------------------------------------*/
struct _cmd {
    char *name;
    void (*fun) (struct _cmd *c);
    char *comment;
};

struct _args {
	char **argv;
	int argc;
};

static void pwd(struct _cmd *c);
static void ls(struct _cmd *c);
static void mkdir(struct _cmd *c);
static void rm(struct _cmd *c);
static void touch(struct _cmd *c);
static void cat(struct _cmd *c);
static void cd(struct _cmd *c);
static void help(struct _cmd *c) ;
static void xit(struct _cmd *c);
static void none(struct _cmd *c) ;

static struct _cmd commands [] = {
	{"pwd",		pwd,	"display current directory"},
	{"ls", 		ls, 	"display directory content"},
    {"mkdir", 	mkdir, 	"create a new directory"},
    {"rm",  	rm,		"remove file"},
    {"touch", 	touch,	"create empty file"},
    {"cat", 	cat,	"display file content"},
	{"cd",		cd,		"change directory"},
    {"exit", 	xit,	"exit"},
    {"help",	help,	"display this help"},
    {0,			none, 	"unknown command, try help"}
} ;

static struct _args *args;

static char *path;
static char *current_directory;

/* ------------------------------
   dialog and execute 
   ------------------------------------------------------------*/

static void
execute(char *name)
{
    struct _cmd *c = commands;
	args = (struct _args *) malloc(sizeof(struct _args));
	int argc;
	char *argv[10];
	char *tmp;
	char *n;

	tmp = strtok(name, " \n");
	argc = 0;
	n = name;
	while(tmp != NULL)
	{
		if (argc == 0){
			sprintf(n, "%s", tmp);
		}
		if (tmp != " \n") {
			argv[argc] = tmp;
		}
		tmp = strtok(NULL, " ");
		argc++;
	}
	args->argv = argv;
	args->argc = argc;
	printf("name:%s; n:%s\n", name, n);
    while (c->name && strcmp (n, c->name))
	c++;
    (*c->fun)(c);

	free(args);
}

static void
loop(void)
{
    char name[64];
    
    while (printf("root@vm:%s$ ", path), fgets(name, 64, stdin))
	execute(name);
}

/* ------------------------------
   command execution 
   ------------------------------------------------------------*/

static void
pwd(struct _cmd *c)
{
	if (current_directory != path)
		printf("%s%s\n", path, current_directory);
	else
		printf("%s\n", path);
}

static void
ls(struct _cmd *c)
{
	unsigned int inumber;
	inumber = inumber_of_path(current_directory);
	printf("inode of directory %u\n", inumber);
	if (inumber >= 0)
		//printf("inode of directory %u\n", inumber);
		list_entries(inumber, path);
	else
		fprintf(stderr, "Error during listing directory\n");
}

static void
mkdir(struct _cmd *c)
{
	if (args == NULL)
		fprintf(stderr, "[Error] args not set\n");
	make_directory(inumber_of_path(current_directory), args->argv[1]);	
	//	fprintf(stderr, "[Error] Usage: mkdir dir_name\n");
    //printf("%s NYI\n", c->name); 
}

static void
rm(struct _cmd *c)
{
	printf("%s NYI\n", c->name);
}

static void
touch(struct _cmd *c)
{
	unsigned int inumber;
	inumber = create_ifile(ordinary);
	fprintf(stdout, "inumber of new file: %u;name: %s\n", inumber, args->argv[1]);
	int r = add_entry(inumber_of_path(current_directory), inumber, args->argv[1]);
	fprintf(stdout, "return value: %i\n", r);
	save_super();
	//printf("%s NYI\n", c->name);
}

static void
cat(struct _cmd *c)
{
	    printf("%s NYI\n", c->name);
}

static void
cd(struct _cmd *c)
{
	printf("%s NYI\n", c->name); 
}

static void
do_xit()
{
 	exit(EXIT_SUCCESS);
}

static void
xit(struct _cmd *dummy)
{
    do_xit(); 
}

static void
help(struct _cmd *dummy)
{
    struct _cmd *c = commands;
  
    for (; c->name; c++) 
	printf ("%s\t-- %s\n", c->name, c->comment);
}

static void
none(struct _cmd *c)
{
    printf ("%s\n", c->comment) ;
}


static void
empty_it()
{
    return;
}

int
main(int argc, char **argv)
{
    unsigned int i;

	fprintf(stdout, "\t\t\t#################################\n"	\
					"\t\t\t-------- \tBash\t --------\n"	\
					"\t\t\t#################################\n");

    if(init_hardware("hardware.ini") == 0) {
	    fprintf(stderr, "Error in hardware initialization\n");
	    exit(EXIT_FAILURE);
    }
    /* Interreupt handlers */
    for(i=0; i<16; i++)
	    IRQVECTOR[i] = empty_it;

    /* Allows all IT */
    _mask(1);

    /* On charge le MBR */
    check_hardware();
	load_mbr();

	/* On charge le volume */
	load_super(current_volume);
	
	path = "/";
	current_directory = "/";
    /* dialog with user */ 
    loop();
	
	save_super();
    /* abnormal end of dialog (cause EOF for xample) */
    do_xit();

    /* make gcc -W happy */
    exit(EXIT_SUCCESS);
}
