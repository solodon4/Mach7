/* An implementation of Red-Black Trees. Refer to 
"Introduction to Algorithms" - by Cormen, Leiserson, Rivest, Stein */

/* if the constant DOT_OUTPUT is defined, the program outputs a textfile
(mentioned as the argument to the program) that describes the tree built, 
in a language suitable for use with the "dot" program. */

#include <stdio.h>
#include <stdlib.h>

#define DOT_OUTPUT

#define red 0
#define black 1

struct Node {
	int label;
	int data;
	int colour;
	struct Node *father;
	struct Node *leftSon;
	struct Node *rightSon;
};

void rightRotate (struct Node *nodePtr, struct Node **root, struct Node *sentinel) {
	
	struct Node *p;
	
	if (nodePtr == sentinel) {
		fprintf (stderr, "Unexpected error...#1\n");
		exit (EXIT_FAILURE);
	}
	p = nodePtr->leftSon;
	
	if (p == sentinel) {
		/* nodePtr MUST have a leftSon */
		fprintf (stderr, "Unexpected error...#2\n");
		exit (EXIT_FAILURE);
	}
		
	p->father = nodePtr->father;
	if (nodePtr != (*root)) {
		if ((nodePtr->father)->leftSon == nodePtr)
			(nodePtr->father)->leftSon = p;
		else
			(nodePtr->father)->rightSon = p;
	}
	else
		*root = p;
	
	nodePtr->leftSon = p->rightSon;
	if ((p->rightSon) != sentinel)
		(p->rightSon)->father = nodePtr;
	
	p->rightSon = nodePtr;
	nodePtr->father = p;
	
	return;
}

void leftRotate (struct Node *nodePtr, struct Node **root, struct Node *sentinel) {
	
	struct Node *p;
	
	if (nodePtr == sentinel) {
		fprintf (stderr, "Unexpected error...#1\n");
		exit (EXIT_FAILURE);
	}
	p = nodePtr->rightSon;
	
	if (p == sentinel) {
		/* nodePtr MUST have a rightSon */
		fprintf (stderr, "Unexpected error...#2\n");
		exit (EXIT_FAILURE);
	}

	p->father = nodePtr->father;
	if (nodePtr != (*root)) {
		if ((nodePtr->father)->leftSon == nodePtr)
			(nodePtr->father)->leftSon = p;
		else
			(nodePtr->father)->rightSon = p;
	}
	else
		*root = p;
	
	nodePtr->rightSon = p->leftSon;
	if ((p->leftSon) != sentinel)
		(p->leftSon)->father = nodePtr;
	
	p->leftSon = nodePtr;
	nodePtr->father = p;
	
	return;
}

void insertNode (struct Node *nodePtr, struct Node **root, struct Node *sentinel) {
	
	struct Node *p = *root, *q = sentinel;
		
	if (nodePtr == sentinel || nodePtr == NULL) {
		fprintf (stderr, "Unexpected error...#3\n");
		exit (EXIT_FAILURE);
	}
	
	while (p != sentinel) {
		q = p;
		if (nodePtr->data < p->data)
			p = p->leftSon;
		else
			p = p->rightSon;
	}
	
	if (q == sentinel)
		*root = nodePtr;
	else {
		if (nodePtr->data < q->data) {
			q->leftSon = nodePtr;
			nodePtr->father = q;
		}
		else {
			q->rightSon = nodePtr;
			nodePtr->father = q;
		}
	}
	nodePtr->colour = red; /* function newNode() will have done this already */
	
	/* now the 'repair' of the R-B-tree starts.... */
	/* the tree might be breaking two of the following conditions for 
	a red black tree.
	
	A red black tree has to satisfy
	1. Root must be black.
	2. Leaf nodes (all sentinel here) must be black.
	3. Both children of a red node must be black.
	4. Number of black nodes from a node, downto the leaves
	   must be same taking any path.
	5. Every node can only be either red or black. 
	
	upon new insertion, 1. or 3. might be broken. Rectifiy. */
	
	/* sentinel is always a black node. */
	while ((nodePtr->father)->colour == red) {
		if (((nodePtr->father)->father)->leftSon == nodePtr->father) {
			/* nodePtr's father is leftSon to grandFather */
			
			/* If no uncle exists, the sentinel will be taking its 
			place. So no illegal memory access is made. Also, colour
			of the uncle is black and hence case 2 or 3 will fit in. */
			
			/* case 1 */
			if ((((nodePtr->father)->father)->rightSon)->colour == red) {
				(((nodePtr->father)->father)->rightSon)->colour = black;
				(nodePtr->father)->colour = black;
				((nodePtr->father)->father)->colour = red;
				nodePtr = (nodePtr->father)->father;
			}
			/* else the uncle is black */
			else {
				/* case 2 as described by cormen */
				if (nodePtr == (nodePtr->father)->rightSon) {
					nodePtr = nodePtr->father;
					leftRotate (nodePtr, root, sentinel);
				}
				/* case 3 */
				((nodePtr->father)->father)->colour = red;
				(nodePtr->father)->colour = black;
				rightRotate (((nodePtr)->father)->father, root, sentinel);
			}				
		}
		else  { /* nodePtr's father is rightSon to grandFather */
			
			/* case 1 for next direction */
			if ((((nodePtr->father)->father)->leftSon)->colour == red) {
				(((nodePtr->father)->father)->leftSon)->colour = black;
				(nodePtr->father)->colour = black;
				((nodePtr->father)->father)->colour = red;
				nodePtr = (nodePtr->father)->father;
			}
			else {
				/* case 2 */
				if (nodePtr == (nodePtr->father)->leftSon) {
					nodePtr = nodePtr->father;
					rightRotate (nodePtr, root, sentinel);
				}
				/* case 3 */
				((nodePtr->father)->father)->colour = red;
				(nodePtr->father)->colour = black;
				leftRotate (((nodePtr->father)->father), root, sentinel);
			}
		}
	}
	(*root)->colour = black;
}

struct Node *newNode (struct Node *sentinel) {
	
	struct Node *p;
	
	p = (struct Node *) malloc (sizeof (struct Node));
	
	if (p == NULL) {
		fprintf (stderr, "Memory allocation error...\n");
		exit (EXIT_FAILURE);
	}
	
	p->label = 0;
	p->data = 0;
	p->leftSon = sentinel;
	p->rightSon = sentinel;
	p->father = sentinel;
	p->colour = red; /* red chosen arbitrarily. make sure right colour is set before use */
	
	return p;
}

#ifndef DOT_OUTPUT
void inorderTraverse (struct Node *root, struct Node *sentinel) {
	
	if (root != sentinel) {
		inorderTraverse (root->leftSon, sentinel);
		printf ("%d ", root->data);
		inorderTraverse (root->rightSon, sentinel);
	}
}
#endif

#ifdef DOT_OUTPUT
void preorderDotDump (struct Node *root, struct Node *sentinel, FILE *outputFile) {
	
	if (root != sentinel) {
		if (root->colour == red)
			fprintf (outputFile, "%d [label=%d,color=red];\n",\
					root->label, root->data);
		else
			fprintf (outputFile, "%d [label=%d,color=black];\n",\
					root->label, root->data);
	
		if (root->leftSon != sentinel)
			fprintf (outputFile, "%d -> %d;\n", root->label, (root->leftSon)->label);
		if (root->rightSon != sentinel)
			fprintf (outputFile, "%d -> %d;\n", root->label, (root->rightSon)->label);
		preorderDotDump (root->leftSon, sentinel, outputFile);
		preorderDotDump (root->rightSon, sentinel, outputFile);
	}

}

void dotDump (struct Node *root, struct Node *sentinel, FILE *outputFile) {
	
	fprintf (outputFile, "digraph rbtree {\n");
	preorderDotDump (root, sentinel, outputFile);
	fprintf (outputFile, "}\n");
}

#endif

int main (int argc, char **argv) {
	
	struct Node *root, *sentinel, *p;
	 int n, i;
	 FILE *outputFile;
	
#ifdef DOT_OUTPUT	
	if (argc != 2) {
		fprintf (stderr, "Usage: rbtree filename.dot\n");
		exit (EXIT_FAILURE);
	}
#endif
	
	sentinel = newNode (NULL);  /* NULL is arbitrary here */
	sentinel->colour = black;  /* sentinel is always black node */
	root = sentinel;
	
	printf ("How many elements? ");
	scanf ("%d", &n);
	
	//srandom (n);
	for (i = 0; i < n; i++) {
		p = newNode (sentinel);
		p->data = rand() % (10*n);
		/* range chosen arbitrarily */
		p->label = i;
		insertNode (p, &root, sentinel);
	}
#ifndef DOT_OUTPUT
	
	inorderTraverse (root, sentinel);
	putc ('\n', stdout);
#endif
	
#ifdef DOT_OUTPUT
	outputFile = fopen (argv[1], "w");
	/* file will be replaced if already exists */
	if (outputFile != NULL) {
		dotDump (root, sentinel, outputFile);
		fclose (outputFile);
	}
	else
		fprintf (stderr, "Unable to output open file. Output not dumped...\n");
#endif
	
	return EXIT_SUCCESS;
}
