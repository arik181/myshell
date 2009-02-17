#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef MAXLINESIZE
#define MAXLINESIZE 	1024
#endif

/*** Linked List Definition ***/
typedef struct node * nodeptr;
typedef struct list * listptr;

struct list
{
	nodeptr arrayhead;
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
void destructlist(listptr * list);
