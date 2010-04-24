#include "util.h"

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

