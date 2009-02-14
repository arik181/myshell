/*** Create a "freelist" which stores a set number of data nodes, 
 * and allocates memory for history. This means that all memory in the
 * list is still array accessible, and is thus easier to free up. ***/
void initlist(int N, nodeptr freelist)
{
	freelist = calloc((N+1), (sizeof *freelist));
	itemptr newitem  = calloc((N+1), (sizeof *newitem));
	
	int i;
	for(i=0;i<N+1;++i)
	{
		freelist[i].next = &freelist[i+1];
		freelist[i].item = &newitem[i+1];
		strcpy(freelist[i].item -> hosebag, "\0");
	}

	freelist[N].next = NULL;
}

/*** Create a new node by taking one node off the end of the freelist. ***/
nodeptr newnode(int i, nodeptr freelist)
{
	nodeptr x = deletenext(freelist);

	if(x && x -> item)
	{
		strcpy(x -> item -> hosebag, "\0");
		x -> next = x;
	}
	return x;
}

/*** Add a node to the freelist ***/
void freenode(nodeptr x, nodeptr freelist)
{
	insertnext(freelist, x);
}

/*** Insert a node at next.
 * takes the current node and the node to be inserted as arguments
 * ***/
void insertnext(nodeptr x, nodeptr t)
{
	t -> next = x -> next;
	x -> next = t;
}

/*** Delete the next node, and return a reference to it ***/
nodeptr deletenext(nodeptr x)
{
	nodeptr t = x -> next;
	x -> next = t -> next;

	return t;
}

/*** Return a reference to the next node ***/
nodeptr getnext(nodeptr x)
{
	return x -> next;
}

/*** Return a reference to the item stored in X ***/
itemptr getitem(nodeptr x)
{
	return x -> item;
}

/*** Free up all memory consumed by freelist ***/
void destructlist(nodeptr freelist)
{
	int i = 0;
	while(freelist[i] != NULL)
	{
		free(freelist[i].item);
	}
	free(freelist);
}
