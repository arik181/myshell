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
	}
    return 0;
}

int redirectiontest(char * strptr, unsigned * stateptr)
{
    /*** Test for pipe symbol '|'***/
    /*** Test for redirection symbol '<'***/
    /*** Test for redirection symbol '<<'***/
    /*** Test for redirection symbol '>'***/
    /*** Test for redirection symbol '>>'***/
    return 0;
}

