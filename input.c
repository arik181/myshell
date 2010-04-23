void handleinput(char * strptr, unsigned * stateptr, listptr historyptr)
{
	/*** Clear former history errors ***/
	HISTERROR_OFF

	int cmdnum = 0;

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


	/*** Check the first argument for builtins ***/
	if (!strncmp(strptr, "exit\n", EXIT) || 
		 !strncmp(strptr, "x\n", EX) ||
		 !strncmp(strptr, "logout\n", LOGOUT))
	{
		*stateptr |= QUIT;
		BUILTIN_ON
	}
	else if (!strncmp(strptr, "myshell ", MYSHELL) && NO_HISTERROR)
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
	else 
		perror("History out of range");
}


