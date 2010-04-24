#include "proc.h"
#include "redir.h"
#include "util.h"

void havechildren(char * strptr, unsigned * stateptr)
{
	char tokenptr[MAXLINESIZE];
	char * remainder[MAXARGS];
    unsigned operator;
    int status = 0;

	int i;
	for(i=0;i<MAXARGS;++i)
		remainder[i] = (char *) 0x0;

	/*** Test for various symbols and set flags ***/
    backgroundtest(strptr,stateptr);
    operator = operatortest(strptr,stateptr);

    /*** Clean up the string a bit ***/
    chomp(strptr);

    /*** Break the commands into tokens ***/
    strncpy(tokenptr,strptr,MAXLINESIZE);
    tokenize(tokenptr, remainder);

    if (IS(operator,SYM_LT))
    {
        printf("symbol is less than\n");
    }
    else if (IS(operator,SYM_GT))
    {
        printf("symbol is greater than\n");
    }
    else if (IS(operator,SYM_LT_APPEND))
    {
        printf("symbol is less than append\n");
    }
    else if (IS(operator,SYM_GT_APPEND))
    {
        printf("symbol is greater than append\n");
    }
    else if (IS(operator,SYM_PIPE))
    {
        printf("symbol is pipe\n");
    }

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

    operator = 0x0;
    TURN_BACKGROUND_OFF
    TURN_REDIRECTION_OFF
    TURN_PIPE_OFF
}

