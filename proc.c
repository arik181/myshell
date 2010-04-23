#include "proc.h"

void havechildren(char * strptr, unsigned * stateptr)
{
	char sepptr[MAXLINESIZE];
	char * remainder[MAXARGS];

	int i;
	for(i=0;i<MAXARGS;++i)
		remainder[i] = (char *) 0x0;

	/*** Test for various symbols and set flags ***/
    backgroundtest(strptr,stateptr);
    redirectiontest(strptr,stateptr);


	chomp(strptr);

	strcpy(sepptr,strptr);

	tokenize(sepptr, remainder);


	/*** Create a new child process. ***/
	int pid;
	if ((pid = fork()) == 0) 
	{
		if (execvp(sepptr, remainder) < 0)
			perror("execvp error");
		exit(0);
	}
	else if(pid == -1)
	{
		perror("execvp error");
	}

	int status;
	if (IS_NOT_IN_BACKGROUND)
	{
		/*** Parent waits for child, if it exists, 
		* otherwise, it doesn't. ***/
		waitpid(pid, &status, WNOHANG); 
	} 

	TURN_BACKGROUND_OFF
}


