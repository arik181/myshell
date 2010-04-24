#include "redir.h"

int backgroundtest(char * strptr, unsigned * stateptr)
{
	/*** Test for Background symbol '&' ***/
	/*** If something is backgrounded, all arguments after
	 * the ampersand are ignored. For the time being this 
	 * is desired behavior, as it simplifies the code 
	 * tremendously. ***/
	char * substr;
	if ((substr = strstr(strptr,"&")))
	{
		TURN_BACKGROUND_ON
		*substr = '\0';
        return 1;
	}
    else
        return 0;
}

/*** returns a predefined value representing a specific symbol. 
 * ***/
unsigned operatortest(char * strptr, unsigned * stateptr)
{
    /*** Currently we only test for the redir and pipe symbols here.
     * No string modification takes place. This is subject
     * to change. ***/
    int symbolflag = 0;
    char * substr;

    /*** Test for redirection symbols '<', '<<', '>', '>>', and
     * set the symbolflag. ***/
    substr = strptr;
    int i;
	for (i = 0;i<strlen(substr);++i)
    {
        if (substr[i] == '<')
        {
            TURN_REDIRECTION_ON 
            symbolflag = SYM_LT;
            ++i;
            // >>
            if (substr[i] == '<')
            {
                symbolflag = SYM_LT_APPEND;
            }
            break;
        }
        else if (substr[i] == '>')
        {
            TURN_REDIRECTION_ON 
            symbolflag = SYM_GT;
            ++i;
            // <<
            if (substr[i] == '>')
            {
                symbolflag = SYM_GT_APPEND;
            }
            break;
        }
	    else if (substr[i] == '|')
        {
            TURN_PIPE_ON
            symbolflag = SYM_PIPE;
            break;
        }
    }
    return symbolflag;
}

