#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"

/*
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
*/

/*** Integer Definitions ***/
#define FORMATSTRINGSIZE 	30
#define BYTESIZE 		8
#define CHARSIZE 		1
#define BUFSIZE 		1
#define MAXLINESIZE 		1024
#define MAXARGS 		64
#define ARGNAME			1
#define NAMEPLUSFLAG		2
#define CHARSINARG		2
#define HISTORYSIZE		4

/*** Flags ***/
#define DEFAULT			0x0
#define QUIT			0x8
#define BUILTIN			0x4
#define BACKGROUND		0x2


/*** Flow Control Functions ***/
void initialize(int argc, char ** argv, unsigned * stateptr,listptr * historyptr);
void mainloop(unsigned * stateptr,listptr historyptr);
void handleinput(char * name, unsigned * stateptr, 
		 char * stringremainder, listptr historyptr);

void havechildren(char * name,unsigned * stateptr,char * stringremainder);
void cleanup(listptr historyptr);

/*** Utility Functions ***/
int chomp(char * chompstring);

/*** Signal Functions ***/
void reapz();

