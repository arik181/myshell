#include "proc.h"
#include "redir.h"
#include "util.h"



void havechildren(char * strptr, unsigned * stateptr)
{
	char tokenptr[MAXLINESIZE];
	char * remainder[MAXARGS];
    unsigned operation;

	int i;
	for(i=0;i<MAXARGS;++i)
		remainder[i] = (char *) 0x0;

	/*** Test for various symbols and set flags ***/
    backgroundtest(strptr,stateptr);
    operation = operatortest(strptr,stateptr);

    /*** Clean up the string a bit ***/
    chomp(strptr);

    /*** Break the commands into tokens ***/
    strncpy(tokenptr,strptr,MAXLINESIZE);
    tokenize(tokenptr, remainder);

    if (IS_REDIRECT)
    {
        redirectionfork(tokenptr, remainder, stateptr, operation);
    }
    else if (IS_PIPED)
    {
        pipefork(tokenptr, remainder, stateptr);
    }
    else
    {
        ordinaryfork(tokenptr, remainder, stateptr);
    }

    operation = 0x0;
    TURN_BACKGROUND_OFF
    TURN_REDIRECTION_OFF
    TURN_PIPE_OFF
}




void ordinaryfork(char * tokenptr, char ** remainder, unsigned * stateptr)
{
    int status = 0;

    /*** Create a new child process. ***/
    int childpid = fork();
    if (childpid == 0)
    {
        if (execvp(tokenptr, remainder) < 0)
            perror("execvp error");
        exit(0);
    }
    else if(childpid == -1)
    {
        perror("fork error");
    }
    else
    {
        /*** Parent does not wait for child. ***/
        if (IS_IN_BACKGROUND)
        {
            waitpid(childpid, &status, WNOHANG);
            if(status < 0)
                perror("wait error");
        } 
        /*** Parent does wait for child. ***/
        else 
        {
            if(wait(0) < 0)
                perror("wait error");
        }
    }
}

void pipefork(char * tokenptr, char ** remainder, unsigned * stateptr)
{
    /*** Create a pipe. ***/
    /*** Create two children. ***/
    /*** In first child, redirect. ***/
    /*** In second child, redirect. ***/
}

void redirectionfork(char * tokenptr, char ** remainder, unsigned * stateptr, unsigned operation)
{
    /*** Open file handler. ***/
    /*** Create a child. ***/
    /*** In child, redirect. ***/
}
