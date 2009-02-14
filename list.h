#include <stdlib.h>
#include <string.h>

/*** Linked List Definition ***/
typedef struct node * nodeptr;
typedef struct item * itemptr;

struct item
{
	char hosebag[1024];
};

struct node 
{ 
	itemptr item; 
	nodeptr next; 
};

/*** List Functions ***/
void initlist(int N, nodeptr freelist);
nodeptr newnode(int i, nodeptr freelist);
void freenode(nodeptr x, nodeptr freelist);
void insertnext(nodeptr x, nodeptr t);
nodeptr deletenext(nodeptr x);
nodeptr getnext(nodeptr x);
int item(nodeptr x);
void destructlist(nodeptr freelist);
