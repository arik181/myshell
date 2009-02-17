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

	array[N].next = list -> lastcmd = list -> arrayhead = &array[0];
	array[N+1].next = NULL;

	return list;
}

/*** Add a string to the list, increment its history number ***/
void addstring(char * laststring, listptr list)
{
	static int count = 0;

	list -> lastcmd = list -> lastcmd -> next;
	list -> lastcmd -> histnumber = ++count;
	strncpy(list -> lastcmd -> string, laststring, MAXLINESIZE);
}

/*** Print all the strings in the list ***/
void printstrings(listptr list)
{
	nodeptr printptr = list -> arrayhead -> next;

	if(printptr)
	{
		while((strncmp(printptr -> string,"\0",1)) && 
		     (printptr -> next != list -> arrayhead)) 
		{
			printf("%d %s", printptr -> histnumber,
					printptr -> string);
			printptr = printptr -> next;
		}
	}
}

/*** Free up all memory used by the list ***/
void destructlist(listptr * list)
{
 	free((*(list)) -> arrayhead);
 	free((*list));
}

