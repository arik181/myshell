#ifndef DEFS
#define DEFS


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/wait.h>

/*** Integer Definitions ***/
#define FORMATSTRINGSIZE    30
#define BYTESIZE            8
#define CHARSIZE            1
#define BUFSIZE             1
#define MAXLINESIZE         1024
#define MAXARGS             64
#define ARGNAME             1
#define NAMEPLUSFLAG        2
#define CHARSINARG          2
#define HISTORYSIZE         100
#define NUMLOCATION         1

#define EXIT                5
#define EX                  2
#define LOGOUT              7
#define CHDIR               6
#define CD                  2
#define MYSHELL             2
#define HISTORY             7

/*** Flags ***/
#define DEFAULT         0x0
#define QUIT            0x8
#define BUILTIN         0x4
#define BACKGROUND      0x2
#define HISTERROR       0x1

#define BUILTIN_ON *stateptr |= BUILTIN;
#define BUILTIN_OFF *stateptr = *stateptr & ~(BUILTIN);
#define TURN_BACKGROUND_ON *stateptr |= BACKGROUND;
#define TURN_BACKGROUND_OFF *stateptr = *stateptr & ~(BACKGROUND);
#define HISTERROR_ON *stateptr |= HISTERROR;
#define HISTERROR_OFF *stateptr = *stateptr & ~(HISTERROR);
#define NO_HISTERROR !(HISTERROR & *stateptr)
#define IS_NOT_IN_BACKGROUND !(BACKGROUND & *stateptr)

#endif // DEFS
