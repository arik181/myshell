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



/*** Main ***/
int main(int argc, char ** argv)
{
	unsigned state = DEFAULT;
	unsigned * stateptr = &state;

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
		havechildren(name,stateptr,stringremainder);
		BUILTIN_OFF
	}

}


void havechildren(char * name, unsigned * stateptr, char * stringremainder)
{
#ifndef TURN_BACKGROUND_ON
#define TURN_BACKGROUND_ON *stateptr |= BACKGROUND;
#endif
#ifndef TURN_BACKGROUND_OFF
#define TURN_BACKGROUND_OFF *stateptr &= ~(BACKGROUND);
#endif
#ifndef IS_NOT_IN_BACKGROUND
#define IS_NOT_IN_BACKGROUND !(*stateptr & BACKGROUND)
#endif
	char * argptr[MAXARGS]; 
	char * token = NULL;
	char * buf = NULL;


	/*** Eliminate trailing newline characters. ***/
	chomp(name);
	chomp(stringremainder);

	/*** Create the command string to execute. ***/
	/*** Calloc is less efficient, but safer, and 
	 * 	in this case, I'd prefer error free to
	 * 	efficient. ***/
	buf = (char*) calloc (1,sizeof(char)*MAXLINESIZE);
	argptr[0] = buf;
	strncat(argptr[0],name,MAXLINESIZE);
	int i = 1;
	while ((token = strsep(&stringremainder, " ")) != NULL)
	{
		buf = (char*) calloc (1,sizeof(char)*MAXLINESIZE);
		argptr[i] = buf;
		strncat(argptr[i],token,MAXLINESIZE);
		strncat(argptr[i],"\0",MAXLINESIZE);
		++i;
	}

	/*** Terminate the argument array with NULL. ***/
	argptr[i] = NULL;

	/*** Test for Background symbol '&' ***/
	/*** Note: This will not work if there is a space 
	 * 	after the ampersand, because the space will
	 * 	constitute the final token, and we only search
	 * 	the final token for the ampersand. Also, the
	 * 	background will be flagged if there is an 
	 * 	ampersand *anywhere* in the final token, 
	 * 	since this is a simple blind search. ***/
	int lasttoken = i-1;
	int j = 0;
	while(argptr[lasttoken][j] != '\0')
	{
		if (argptr[lasttoken][j] == '&')
		{
			TURN_BACKGROUND_ON
			argptr[lasttoken] = NULL;
			--lasttoken;
			break;
		}
		++j;
	}

	/*** Create a new child process. ***/
	int pid;
	if ((pid = fork()) == 0) {
		if (execvp(argptr[0], argptr) < 0)
			perror("execvp error");
		exit(0);
	}

	int status;
	if (IS_NOT_IN_BACKGROUND)
	{
		/*** Parent waits for child, if it exists, 
		* otherwise, it doesn't. ***/
		waitpid(pid, &status, WNOHANG); 
	} 

	/*** Free all calloc'd memory. ***/
	int k = 0;
	while(argptr[k])
	{
		free(argptr[k]);
		++k;
	}

	TURN_BACKGROUND_OFF
}

void initialize(int argc, char ** argv, unsigned * stateptr)
{
	system("clear");
	*stateptr |= DEFAULT;
	signal(SIGCHLD, reapz);
}


void cleanup()
{
	system("clear");
}

/*** Removes trailing newline characters. ***/
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

/*** Disposes of unwashed zombie hordes ***/
void reapz()
{
	int pid = 0;
	int status = 0;

	while(1) {
		pid = waitpid(-1, &status, WNOHANG); 
		if (pid <= 0)
			break;
	}
}

/*** Begin Linked List Code ***/
void initlist(int N, nodeptr freelist)
{
	freelist = calloc((N+1), (sizeof *freelist));
	itemptr newitem;
	
	int i;
	for(i=0;i<N+1;++i)
	{
		newitem = calloc(1, (sizeof *newitem));
		freelist[i].next = &freelist[i+1];
		strcpy(freelist[i].item -> hosebag, "\0");
	}

	freelist[N].next = NULL;
}

nodeptr newnode(int i, nodeptr freelist)
{
	nodeptr x = deletenext(freelist);

	if(x && x -> item)
	{
		strcpy(x -> item -> hosebag, "\0");
		x -> next = x;
	}
	return x;
}

void freenode(nodeptr x, nodeptr freelist)
{
	if(x)
	{
		insertnext(freelist, x);
	}
}

void insertnext(nodeptr x, nodeptr t)
{
	if(x)
	{
		t -> next = x -> next;
		x -> next = t;
	}
}

nodeptr deletenext(nodeptr x)
{
	nodeptr t = NULL;

	if(x)
	{
		t = x -> next;
		x -> next = t -> next;
	}

	return t;
}

nodeptr next(nodeptr x)
{
	if(x)
		return x -> next;
	else 
		return NULL;
}

itemptr getitem(nodeptr x)
{
	if(x)
		return x -> item;
	else 
		return NULL;
}

#undef BUILTIN_ON
#undef BUILTIN_OFF
#undef TURN_BACKGROUND_ON
#undef TURN_BACKGROUND_OFF
#undef IS_NOT_IN_BACKGROUND
