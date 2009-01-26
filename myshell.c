#include "mycp.h"

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
	/*** Start Timing ***/
	struct timeval starttime;
	struct timeval endtime;
	struct timeval finaltime;
	gettimeofday(&starttime, NULL);

	/*** Determine number of files ***/
	int numberoffiles = argc - ARGNAME;

	/*** Set default state ***/
	unsigned state = DEFAULT;

	/*** Determine library preference ***/
	/*** (Check for the "-s" flag...) ***/
	if (argc >= NAMEPLUSFLAG && !strncmp(argv[1],"-s",CHARSINARG))
	{
		state |= USESTDIO;

		/*** Subtract the flag from the file count ***/
		--numberoffiles;
	}

	if (numberoffiles != 2)
	{
		printf("\nUsage: %s -s <filename 1> <filename 2> \n\n", argv[0]);
		exit(0);
	}

	/*** If we're using stdio ***/
	if (state & USESTDIO)
	{
		stdiocp(argc, argv);
	}
	/*** Else we're using straight sys calls ***/
	else
	{
		syscallcp(argc, argv);
	}

	gettimeofday(&endtime, NULL);
	timersub(&endtime, &starttime, &finaltime);
	if (&finaltime)
		printf("total execution time:  %d (secs) : %d (usecs)\n", finaltime.tv_sec, finaltime.tv_usec);

	return 0;
}


/*** The stdio version of the cp code ***/
void stdiocp(int argc, char ** argv)
{
	/*** Init file handle ***/
	FILE * cpfile = NULL;
	FILE * outfile = NULL;
	size_t filesize = 0;

	/*** File buffer ***/
	char buf = '\0';
	char * bufptr = &buf;

	/*** Open file for reading ***/
	cpfile = fopen(argv[FFILE1],"r");

	if (cpfile == NULL)
	{
		perror("\nBad filename or nonexistent file\n");
	}
	else
	{
		/*** Open file for writing ***/
		outfile = fopen(argv[FFILE2],"w");

		/*** Read file ***/
		int i = 0;
		while (!feof(cpfile))
		{
			if (i != 0)
			{
				/*** Dump file1 to file2 ***/
				myfwrite(bufptr,CHARSIZE,CHARSIZE,outfile);
			}

			myfread(bufptr,CHARSIZE,CHARSIZE,cpfile);
			++i;
		}

		/*** Close up shop ***/
		fclose(outfile);
		fclose(cpfile);
	}
}


/*** The system call version of the cp code ***/
void syscallcp(int argc, char ** argv)
{
	/*** Init file descriptor ***/
	int cpfile = 0;
	int outfile = 0;
	off_t filesize = 0;

	/*** File buffer ***/
	char buf = '\0';
	char * bufptr = &buf;

	/*** Open file for reading ***/
	cpfile = open(argv[SFILE1], O_RDONLY);

	if (cpfile == 0)
	{
		perror("\nBad filename or nonexistent file\n");
	}
	else
	{
		/*** Open file for writing ***/
		outfile = open(argv[SFILE2], O_WRONLY|O_CREAT, 
				S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

		/*** Read file  ***/
		while (myread(cpfile,bufptr,CHARSIZE))
		{
			/*** Dump file1 to file2 ***/
			if(bufptr)
				mywrite(outfile,bufptr,CHARSIZE);
		}
			
		/*** Close up shop ***/
		close(outfile);
		close(cpfile);
	}
}

int myread(int file, char * bufptr, int count)
{
	int retval;
	retval = read(file, bufptr, count);
	return retval;
}

int mywrite(int file, char * bufptr, int count)
{
	int retval;
	retval = write(file, bufptr, count);
	return retval;
}

size_t myfread(void * bufptr,size_t size,size_t nmemb,FILE * stream)
{
	size_t retval;
	fread(bufptr,size,nmemb,stream);
	return retval;
}

size_t myfwrite(const void * bufptr,size_t size,size_t nmemb,FILE * stream)
{
	size_t retval;
	fwrite(bufptr,size,nmemb,stream);
	return retval;
}

