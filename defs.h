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

#include "list.h"
#include "input.h"
#include "main.h"
#include "proc.h"
#include "redir.h"
#include "util.h"

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

#endif // DEFS
