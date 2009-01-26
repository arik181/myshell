#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>


/*** Integer Definitions ***/
#define FORMATSTRINGSIZE 	30
#define BYTESIZE 		8
#define CHARSIZE 		1
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
void stdiocp(int argc, char ** argv);
void syscallcp(int argc, char ** argv);

size_t fgetfilesize(FILE * cpfile);
off_t getfilesize(int cpfile);

int myread(int file, char * bufptr, int count);
int mywrite(int file, char * bufptr, int count);

size_t myfread(void * bufptr,size_t size,size_t nmemb,FILE * stream);
size_t myfwrite(const void * bufptr,size_t size,size_t nmemb,FILE * stream);
