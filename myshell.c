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

	initialize(argc,argv,state);

	char ** strptr;

	while(!feof(stdin))
	{
		strptr = NULL;
		strptr = getcmd();

	}

	cleanup();
	return 0;
}

void initialize(int argc, char ** argv, unsigned state)
{
	system("clear");
}

char ** getcmd()
{
	char buf = '\0';
	char * bufptr = &buf;

	char delim = ' ';
	const char * delimptr = &delim;

	printf("%%");
	buf = '\0';
	char string[LINESIZE];
	int i = 0;
	while (*bufptr != '\n' && !feof(stdin))
	{
		fread(bufptr,BUFSIZE,BUFSIZE,stdin);
		string[i] = *bufptr;
		++i;
	}

	return NULL;
}

void cleanup()
{
	system("clear");
}
