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

#define CHDIR			5
#define MYSHELL			7

/*** Flags ***/
#define DEFAULT			0x0
#define QUIT			0x8
#define BUILTIN			0x4
#define BACKGROUND		0x2


/*** Flow Control Functions ***/
void initialize(int argc, char ** argv, unsigned * stateptr,listptr * historyptr);
void handleinput(char * strptr, unsigned * stateptr, listptr historyptr);

void havechildren(char * strptr, unsigned * stateptr);
void cleanup(listptr historyptr);

/*** Utility Functions ***/
void tokenize(char * strptr, char ** remainder);
int chomp(char * chompstring);

/*** Signal Functions ***/
void reapz();

