#include "myshell.h"

/*** Flag Setting Macros ***/
#ifndef BUILTIN_ON
#define BUILTIN_ON *stateptr |= BUILTIN;
#endif
#ifndef BUILTIN_OFF
#define BUILTIN_OFF *stateptr &= ~(BUILTIN);
#endif
#ifndef HISTERROR_ON
#define HISTERROR_ON *stateptr |= HISTERROR;
#endif
#ifndef HISTERROR_OFF
#define HISTERROR_OFF *stateptr &= ~(HISTERROR);
#endif
#ifndef NO_HISTERROR
#define NO_HISTERROR !(*stateptr & HISTERROR)
#endif
#ifndef TURN_BACKGROUND_ON
#define TURN_BACKGROUND_ON *stateptr |= BACKGROUND;
#endif
#ifndef TURN_BACKGROUND_OFF
#define TURN_BACKGROUND_OFF *stateptr &= ~(BACKGROUND);
#endif
#ifndef IS_NOT_IN_BACKGROUND
#define IS_NOT_IN_BACKGROUND !(*stateptr & BACKGROUND)
#endif

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



/*** Main ***/
int main(int argc, char ** argv)
{
	unsigned state = DEFAULT;
	unsigned * stateptr = &state;
	listptr historyptr;

	initialize(argc,argv,stateptr,&historyptr);

	char string[MAXLINESIZE];
	char * strptr = &string[0];

	/*** Main loop: 
	 * Print a prompt, 
	 * get input, 
	 * tokenize ***/
	while(!feof(stdin) && !(*stateptr & QUIT))
	{
		printf("%%");
		fgets(string, MAXLINESIZE, stdin);

		if(feof(stdin))
			break;

		/*** If the user simply hits return, do nothing. ***/
		if (string[0] != '\n')
		{
			/*** Otherwise, deal with input ***/
			handleinput(strptr,stateptr,historyptr);

			chomp(string);

			/*** Add the string to history ***/
			/*** If there was no history error ***/
			if (NO_HISTERROR)
				addstring(string,historyptr);
		}
	}

	cleanup(historyptr);
	return 0;
}







void handleinput(char * strptr, unsigned * stateptr, listptr historyptr)
{
	/*** Clear former history errors ***/
	HISTERROR_OFF

	/*** Eliminate trailing newline characters ***/
	int cmdnum = 0;

	/*** Check the first argument for exit ***/
	if (!strncmp(strptr, "exit\n", EXIT) || 
		 !strncmp(strptr, "x\n", EX) ||
		 !strncmp(strptr, "logout\n", LOGOUT))
	{
		*stateptr |= QUIT;
		BUILTIN_ON
	}

	/*** Find out if we need to do some history replacement ***/
	/*** If we do, replace our string with the correct command ***/
	if (strptr[0] == '!')
	{
		cmdnum = atoi(&strptr[NUMLOCATION]);

		if (getcmd(cmdnum,historyptr))
		{
			/*** Get the command from history ***/
			strncpy(strptr, getcmd(cmdnum,historyptr), MAXLINESIZE);

		}
		else 
		{
			HISTERROR_ON
		}
	}

	/*** Test for other builtins ***/
	if (!strncmp(strptr, "myshell ", MYSHELL) && NO_HISTERROR)
	{
		/*** Skip over the builtin part of the input string ***/
		strptr += MYSHELL;

		system(strptr);

		/*** Now come back to it ***/
		strptr -= MYSHELL;

		BUILTIN_ON
	}
	else if (!strncmp(strptr, "chdir ", CHDIR) && NO_HISTERROR)
	{
		chomp(strptr);

		/*** Skip over the builtin part of the input string ***/
		strptr += CHDIR;

		if(chdir(strptr) == -1)
		{
			perror("chdir error");
		}

		/*** Now come back to it ***/
		strptr -= CHDIR;

		BUILTIN_ON
	}
	else if (!strncmp(strptr, "cd ", CD) && NO_HISTERROR)
	{
		chomp(strptr);

		/*** Skip over the builtin part of the input string ***/
		strptr += CD;

		if(chdir(strptr) == -1)
		{
			perror("cd error");
		}

		/*** Now come back to it ***/
		strptr -= CD;

		BUILTIN_ON
	}
	else if (!strncmp(strptr, "history", HISTORY))
	{ 
		printstrings(historyptr);
		BUILTIN_ON
	}
	else if (NO_HISTERROR)
	{
		havechildren(strptr,stateptr);
		BUILTIN_OFF
	}
	else if (!(NO_HISTERROR))
		perror("History out of range");

}


void havechildren(char * strptr, unsigned * stateptr)
{
	char sepptr[MAXLINESIZE];
	char * remainder[MAXARGS];

	int i;
	for(i=0;i<MAXARGS;++i)
		remainder[i] = (char *) 0x0;

	/*** Test for Background symbol '&' ***/
	int lastchar = strlen(strptr);
	while(strptr[lastchar] != '\0')
	{
		if (strptr[lastchar] == '&')
		{
			TURN_BACKGROUND_ON
			strptr[lastchar] = '\0';
			--lastchar;
			break;
		}
	}

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

/*** System Initialization ***/
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
	system("clear");
}

/*** Create a series of null separated tokens from a string 
 * containing spaces ***/
/*** Returns a pointer to an array of all strings ***/
void tokenize(char * strptr, char ** returnptr)
{
	int i = 0;
	int j = 0;

	returnptr[0] = &strptr[0];
	++j;

	while(strptr[i] != '\0')
	{
		if (strptr[i] == ' ')
		{
			strptr[i] = '\0';
			returnptr[j] = &strptr[i+1];
			chomp(returnptr[j]);
			++j;
		}
		++i;
	}
}

/*** Removes Trailing Newline Characters and spaces. ***/
int chomp(char * chompstring)
{
	if (!chompstring)
		return -1;

	int lastchar = strlen(chompstring) - 1;

	while (chompstring[lastchar] == '\n' || 
		    chompstring[lastchar] == ' ')
	{
		chompstring[lastchar] = '\0';
		--lastchar;
	}

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

#undef BUILTIN_ON
#undef BUILTIN_OFF
#undef HISTERROR_ON
#undef HISTERROR_OFF
#undef TURN_BACKGROUND_ON
#undef TURN_BACKGROUND_OFF
#undef IS_NOT_IN_BACKGROUND

