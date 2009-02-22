#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include "definitions.h"

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

