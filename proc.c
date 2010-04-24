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
    pid_t childpid = fork();

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

void pipefork(char * tokenptr, char ** cmdarray, unsigned * stateptr)
{
    int status = 0;
    int fd[2];
    int readsize = 0;
    pid_t firstpid;
    pid_t secondpid;

    /*** Create a pipe. ***/
    if (pipe(fd) < 0)
        perror("pipe error");

    /*** Create two children. ***/
    firstpid = fork();
    if (firstpid < 0)
        perror("fork error");
    else if (firstpid > 0) 
    {
        waitpid(firstpid, &status, WNOHANG); /*** Parent ***/

        secondpid = fork();
        if (secondpid < 0)
            perror("fork error");
        else if (secondpid > 0)
        {
            waitpid(firstpid, &status, WNOHANG); /*** Parent ***/
        }
        else
        {
            /*** Second child. ***/
            close(fd[1]);

            /*** Exec Second Command ***/
            if (execvp(cmdarray[1], remainder) < 0)
                perror("execvp error");
            exit(0);
        }

    }
    else
    {
        /*** First child... ***/
        close(fd[0]);

        /*** Exec First Command ***/
        if (execvp(cmdarray[0], cmdarray) < 0)
            perror("execvp error");
        exit(0);
    }
}

void redirectionfork(char * tokenptr, char ** remainder, unsigned * stateptr, unsigned operation)
{
    /*** Open file handler. ***/
    /*** Create a child. ***/
    /*** In child, redirect. ***/
}
