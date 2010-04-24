#include "defs.h"

void havechildren(char * strptr, unsigned * stateptr);

void ordinaryfork(char * tokenptr, char ** remainder, unsigned * stateptr);
void pipefork(char * tokenptr, char ** remainder, unsigned * stateptr);
void redirectionfork(char * tokenptr, char ** remainder, unsigned * stateptr, unsigned operation);
