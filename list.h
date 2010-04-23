#ifndef LIST
#define LIST

#include "defs.h"

/*** Linked List Definition ***/
typedef struct node * nodeptr;
typedef struct list * listptr;

struct list
{
	nodeptr arrayhead;
	nodeptr	oldestcmd;
	nodeptr lastcmd;
};

struct node 
{ 
	char string[MAXLINESIZE];
	int  histnumber;
	nodeptr next; 
};

/*** List Functions ***/
listptr initlist(int N);
void addstring(char * newstring, listptr list);
void printstrings(listptr list);
void printcmd(int n, listptr list);
char * getcmd(int n, listptr list);
void destructlist(listptr * list);

#endif // LIST
