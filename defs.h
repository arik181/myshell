#ifndef DEFS
#define DEFS


/*** Includes ***/
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
#define SYM_LT          0x1
#define SYM_LT_APPEND   0x2
#define SYM_GT          0x4
#define SYM_GT_APPEND   0x8
#define SYM_PIPE        0x10

#define DEFAULT         0x0
#define QUIT            0x8
#define BUILTIN         0x4
#define BACKGROUND      0x2
#define HISTERROR       0x1
#define REDIRECTION     0x10
#define PIPE            0x20

/*** Flag Related Macros ***/
#define IS(X,Y)         (X & Y)
#define BUILTIN_ON *stateptr |= BUILTIN;
#define HISTERROR_ON *stateptr |= HISTERROR;
#define TURN_BACKGROUND_ON *stateptr |= BACKGROUND;
#define TURN_REDIRECTION_ON *stateptr |= REDIRECTION;
#define TURN_PIPE_ON *stateptr |= PIPE;

#define BUILTIN_OFF *stateptr = *stateptr & ~(BUILTIN);
#define HISTERROR_OFF *stateptr = *stateptr & ~(HISTERROR);
#define TURN_BACKGROUND_OFF *stateptr = *stateptr & ~(BACKGROUND);
#define TURN_REDIRECTION_OFF *stateptr = *stateptr & ~(REDIRECTION);
#define TURN_PIPE_OFF *stateptr = *stateptr & ~(PIPE);

#define NO_HISTERROR !(HISTERROR & *stateptr)
#define IS_IN_BACKGROUND (BACKGROUND & *stateptr)
#define IS_REDIRECT (REDIRECTION & *stateptr)
#define IS_PIPED (PIPE & *stateptr)

#endif // DEFS
