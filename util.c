#include "util.h"

/*** Create an array of executable commands, by replacing
 * the critical operator with a null character. ***/
/*** Returns a pointer to an array of all strings ***/
void tokenize(char * tokenptr, char ** returnptr)
{
	int i = 0;
	int j = 0;

	returnptr[0] = &tokenptr[0];
	++j;

	while(tokenptr[i] != '\0')
	{
		if (tokenptr[i] == '<')
        {
			tokenptr[i] = '\0';
            ++i;
            if (tokenptr[i] == '<')
            {
                tokenptr[i] = '\0';
                returnptr[j] = &tokenptr[i+2];
            }
            else
                returnptr[j] = &tokenptr[i+1];
			chomp(returnptr[j]);
			++j;
        }
        else if (tokenptr[i] == '>')
        {
			tokenptr[i] = '\0';
            ++i;
            if (tokenptr[i] == '>')
            {
                tokenptr[i] = '\0';
                returnptr[j] = &tokenptr[i+2];
            }
            else
                returnptr[j] = &tokenptr[i+1];
			chomp(returnptr[j]);
			++j;
        }
        else if (tokenptr[i] == '|')
		{
			tokenptr[i] = '\0';
            ++i;
			returnptr[j] = &tokenptr[i+1];
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

