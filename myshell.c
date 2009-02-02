#include "myshell.h"

/*
 * Roughly your shell should do the following:
 *
 * 1. take an input string and parse it into words separated
 * by white space (tabs, blanks, and the like).  It should
 * read one line at a time (newline terminated).  Your
 * shell should exit at EOF (control-d).
 * 
 * 2. execute the command using fork, execvp, wait, and exit.
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

	/*** Clear screen and set default state ***/
	initialize(argc,argv,stateptr);

	mainloop(stateptr);

	cleanup();
	return 0;
}





void mainloop(unsigned * stateptr)
{
	char string[MAXLINESIZE];
	char * history = NULL;
	char * dupstring = NULL;
	char * token = NULL;

	/*** Main loop: 
	 * Print a prompt, 
	 * get input, 
	 * tokenize ***/
	while(!feof(stdin) && !(*stateptr & QUIT))
	{
		printf("%%");
		fgets(string, MAXLINESIZE, stdin);
		dupstring = strdup(string);

		int tokencount = 0;
		/*** If the user simply hits return, do nothing. ***/
		if (string[0] != '\n')
		{
			/*** For each token ***/
			while(token = strsep(&dupstring, " "))
			{
				handleinput(tokencount,token,stateptr,
						dupstring,history);
				++tokencount;
			}
		}

		history = strdup(string);
	}
}



void handleinput(int tokencount, char * token, unsigned * stateptr, 
		 char * stringremainder, char * history)
{

#ifndef BUILTIN_ON
#define BUILTIN_ON *stateptr |= BUILTIN;
#endif
#ifndef BUILTIN_OFF
#define BUILTIN_OFF *stateptr = *stateptr & ~(BUILTIN);
#endif

	/*** Check the first argument for special cases and builtins. ***/
	if (tokencount == 0)
	{
		if (!strncmp(token, "debug\n", 6))
		{
			printf("%s", token);
			BUILTIN_ON
		}
		else if (!strncmp(token, "exit\n", 5) || 
			 !strncmp(token, "x\n", 2) ||
			 !strncmp(token, "logout\n", 7))
		{
			*stateptr |= QUIT;
			BUILTIN_ON
		}
		else if (!strncmp(token, "myshell\n", 7))
		{
			system(stringremainder);
			BUILTIN_ON
		}
		else if (!strncmp(token, "mydebug\n", 7) &&
			 history)
		{
			if (*stateptr & BUILTIN) 
					printf("Builtin command:\n");
			printf("%s", history);
			BUILTIN_ON
		}
		else 
			BUILTIN_OFF
	}

#undef BUILTIN_ON
#undef BUILTIN_OFF

}



void initialize(int argc, char ** argv, unsigned * stateptr)
{
	system("clear");
}


void cleanup()
{
	system("clear");
}



