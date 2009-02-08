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

		/*** If the user simply hits return, do nothing. ***/
		if (string[0] != '\n')
		{
			token = strsep(&dupstring, " ");
			handleinput(token,stateptr,dupstring,history);
		}

		history = strdup(string);
	}
}



void handleinput(char * name, unsigned * stateptr, 
		 char * stringremainder, char * history)
{

#ifndef BUILTIN_ON
#define BUILTIN_ON *stateptr |= BUILTIN;
#endif
#ifndef BUILTIN_OFF
#define BUILTIN_OFF *stateptr &= ~(BUILTIN);
#endif

	/*** Eliminate trailing newline characters. ***/
	chomp(history);
	chomp(stringremainder);

	/*** Check the first argument for special cases and builtins. ***/
	if (!strncmp(name, "exit\n", 5) || 
		 !strncmp(name, "x\n", 2) ||
		 !strncmp(name, "logout\n", 7))
	{
		*stateptr |= QUIT;
		BUILTIN_ON
	}
	else if (!strncmp(name, "mydebug\n", 7) &&
		 history)
	{
		if (*stateptr & BUILTIN) 
				printf("Builtin command:\n");
		printf("%s\n", history);
		BUILTIN_ON
	}
	else if (!strncmp(name, "myshell", 7))
	{
		system(stringremainder);
		BUILTIN_ON
	}
	else if (!strncmp(name, "chdir", 5))
	{
		chdir(stringremainder);
		BUILTIN_ON
	}
	else 
	{
		havechildren(name,stringremainder);
		BUILTIN_OFF
	}

#undef BUILTIN_ON
#undef BUILTIN_OFF

}


void havechildren(char * name, char * stringremainder)
{
	char * argptr[MAXARGS]; 
	char * token = NULL;
	char * buf = NULL;


	/*** Eliminate trailing newline characters. ***/
	chomp(name);
	chomp(stringremainder);

	/*** Create the command string to execute. ***/
	buf = (char*) malloc (sizeof(char)*MAXLINESIZE);
	argptr[0] = buf;
	strncat(argptr[0],name,MAXLINESIZE);
	int i = 1;
	while ((token = strsep(&stringremainder, " ")) != NULL)
	{
		buf = (char*) malloc (sizeof(char)*MAXLINESIZE);
		argptr[i] = buf;
		strncat(argptr[i],token,MAXLINESIZE);
		strncat(argptr[i],"\0",MAXLINESIZE);
		++i;
	}

	/*** Terminate the argument array with NULL. ***/
	argptr[i] = NULL;

	/*** Create a new child process. ***/
	if (fork() == 0) {
		if (execvp(argptr[0], argptr) < 0)
			perror("execvp error");
		exit(0);
	}
	else
	{
		/*** Parent waits for child. ***/
		if (wait(0) < 0)
			perror("wait error");
	}

	/*** Free all malloc'd memory. ***/
	int j = 0;
	while(argptr[j])
	{
		free(argptr[j]);
		++j;
	}
}

void initialize(int argc, char ** argv, unsigned * stateptr)
{
	system("clear");
}


void cleanup()
{
	system("clear");
}


int chomp(char * chompstring)
{
	if (!chompstring)
		return -1;

	int lastchar = strlen(chompstring) - 1;

	if (chompstring[lastchar] == '\n')
	{
		chompstring[lastchar] = '\0';
		return 1;
	}
	else 
		return 0;
}
