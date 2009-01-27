#include <stdlib.h>
#include <stdio.h>

/*
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

*/

/*** Integer Definitions ***/
#define FORMATSTRINGSIZE 	30
#define BYTESIZE 		8
#define CHARSIZE 		1
#define BUFSIZE 		1
#define ARGNAME			1
#define NAMEPLUSFLAG		2
#define CHARSINARG		2
#define FFILE1			2
#define FFILE2			3
#define SFILE1			1
#define SFILE2			2

/*** Flags ***/
#define DEFAULT			0x0
#define FLAG1			0x8

/*** Functions ***/
void stateset(int argc, char ** argv, unsigned state);
void buildcmd(char * buf);
void shprompt();
