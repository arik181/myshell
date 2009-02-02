#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
#define MAXLINESIZE 		1024
#define ARGNAME			1
#define NAMEPLUSFLAG		2
#define CHARSINARG		2
#define FFILE1			2
#define FFILE2			3
#define SFILE1			1
#define SFILE2			2

/*** Flags ***/
#define DEFAULT			0x0
#define QUIT			0x8
#define BUILTIN			0x4

/*** Functions ***/
void initialize(int argc, char ** argv, unsigned * stateptr);
void mainloop(unsigned * stateptr);
void handleinput(int tokencount, char * token, unsigned * stateptr, 
		 char * stringremainder, char * history);
void cleanup();

