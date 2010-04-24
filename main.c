#include "main.h"
#include "input.h"
#include "util.h"
#include "signal.h"

/*
 * Roughly, this shell will do the following:
 *
 * 1. take an input string and parse it into words separated
 * by white space (tabs, blanks, and the like).  It should
 * read one line at a time (newline terminated).  Your
 * shell should exit at EOF (control-d).
 * 
 * 2. execute the command using fork, execvp, waitpid, and exit.
 * I.e., the shell should fork a child, let the child call
 * exec, and wait for the child to terminate.  Be careful
 * about forking; i.e., run ps or ps -elf (or top) to make
 * sure you do not have vast amounts of escaped children.
 * This may be due to the parent exiting with the children
 * still running or for other reason where the parent terminates
 * before it can call wait.
 *
 * 3. add three built-in commands: 
 *
 * 	myshell <string>...
 *
 * 	mydebug
 *
 * 	chdir <somedir>
 *
 */

int main(int argc, char ** argv)
{
	unsigned state = DEFAULT;
	unsigned * stateptr = &state;
	listptr historyptr;

	initialize(argc,argv,stateptr,&historyptr);

	char inputstring[MAXLINESIZE];
	char * strptr = &inputstring[0];

	/*** Main loop: 
	 * Print a prompt, 
	 * get input, 
	 * tokenize ***/
	while(!feof(stdin) && !(*stateptr & QUIT))
	{
        prettyprompt();
        getinput(inputstring);

		/*** If the user simply hits return, do nothing. ***/
		if (inputstring[0] != '\n')
		{
			/*** Otherwise, deal with input ***/
			handleinput(strptr,stateptr,historyptr);

			chomp(inputstring);

			/*** Add the string to history ***/
			/*** If there was no history error ***/
			if (NO_HISTERROR)
				addstring(inputstring,historyptr);
		}
	}

	cleanup(historyptr);
	return 0;
}


/*** Disposes of Unwashed Zombie Hordes ***/
void reapz()
{
	int pid = 0;
	int status = 0;

	while(1) 
	{
		pid = waitpid(-1, &status, WNOHANG); 
		if (pid <= 0)
			break;
	}
}


/*** Overall System Initialization ***/
void initialize(int argc, char ** argv, unsigned * stateptr,listptr *historyptr)
{
	system("clear");

	*stateptr |= DEFAULT;

	signal(SIGCHLD, reapz);

	*historyptr = initlist(HISTORYSIZE);
}

/*** Cleanup Before Exiting Main ***/
void cleanup(listptr historyptr)
{
	destructlist(&historyptr);
}

void prettyprompt()
{
    char hostname[MAXLINESIZE];
    char cwd[MAXLINESIZE];
    size_t len = MAXLINESIZE;

    /*** Obtain hostname and current working directory:
     * We do this on every iteration of the loop because these
     * values are subject to change.
     * ***/
    if ((gethostname(hostname, len))) 
        strncpy(hostname, ".\0", MAXLINESIZE);

    if (!(strncpy(cwd, getenv("PWD"), MAXLINESIZE)))
        strncpy(cwd, ".\0", MAXLINESIZE);
    
    /*** Print prompt ***/
    printf("< %s:%s >%% ", hostname, cwd);
}

void getinput(char * inputstring)
{
    inputstring[0] = '\0';
    fgets(inputstring, MAXLINESIZE, stdin);

    if(feof(stdin))
        exit(0);
}
