#include "list.h"

/*** Create a circular linked list which stores a set number of data nodes, 
 * and allocates memory for each node. This means that all memory in the
 * list is still array accessible, and is thus easier to free up. 
 * Returns a pointer to the list.***/
listptr initlist(int N)
{
	/*** Allocate memory for a single list construct ***/
	listptr list  = calloc(1, (sizeof *list));
	/*** Allocate memory for an list of N nodes for the list***/
	nodeptr array = calloc((N+1), (sizeof *array));
	
	int i;
	for(i=0;i<=N;++i)
	{
		array[i].next = &array[i+1];
	}

	array[N].next = list -> lastcmd = list -> arrayhead = \
	list -> oldestcmd = &array[0];

	array[N+1].next = NULL;

	return list;
}

/*** Add a string to the list, increment its history number ***/
void addstring(char * laststring, listptr list)
{
	static int count = 0;
	++count;

	if (count > HISTORYSIZE || count <= 1)
		list -> oldestcmd = list -> oldestcmd -> next;

	list -> lastcmd = list -> lastcmd -> next;
	list -> lastcmd -> histnumber = count;
	strncpy(list -> lastcmd -> string, laststring, MAXLINESIZE);
}

/*** Print all the strings in the list ***/
void printstrings(listptr list)
{
	nodeptr printptr = list -> oldestcmd;

	if(printptr)
	{
		do
		{
			printf("%d %s\n", printptr -> histnumber,
					printptr -> string);
			printptr = printptr -> next;
		}
		while((strncmp(printptr -> string,"\0",1)) &&
		      (printptr != list -> lastcmd -> next));
	}
}

/*** Print one string from the list, based on its history number ***/
void printcmd(int n, listptr list)
{
	nodeptr printptr = list -> oldestcmd;

	if(printptr)
	{
		do
		{
			if (printptr -> histnumber == n)
			{
				printf("%d %s", printptr -> histnumber,
						printptr -> string);
				break;
			}
			printptr = printptr -> next;
		}
		while((strncmp(printptr -> string,"\0",1)) &&
		      (printptr != list -> lastcmd -> next)); 
	}
}

/*** Execute one string from the list, based on its history number ***/
char * getcmd(int n, listptr list)
{
	nodeptr printptr = list -> oldestcmd;
	int wehaveacommand = 0;

	if(printptr)
	{
		do
		{
			if (printptr -> histnumber == n)
			{
				wehaveacommand = 1;
				break;
			}
			
			printptr = printptr -> next;
		}
		while((strncmp(printptr -> string,"\0",1)) &&
		      (printptr != list -> lastcmd -> next)); 
	}

	if(wehaveacommand)
		return(printptr -> string);
	else 
		return NULL;
}

/*** Free up all memory used by the list ***/
void destructlist(listptr * list)
{
 	free((*(list)) -> arrayhead);
 	free((*list));
}

