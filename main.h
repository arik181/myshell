#include "defs.h"
#include "list.h"

/*** Flow Control Functions ***/
void initialize(int argc, char ** argv, unsigned * stateptr, listptr * historyptr);

void cleanup(listptr historyptr);

void reapz();
