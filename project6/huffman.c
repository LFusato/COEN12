#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "pqueue.h"
#include "pack.h"

#define NODE struct node

//Creates a new node, attaching any children if there are any
//O(1)
static NODE *mknode(int data, NODE *left, NODE *right) {
	NODE *newNode = malloc(sizeof(NODE));
	assert(newNode != NULL);

	newNode->parent = NULL;
	newNode->count = 0;

	if(left != NULL) {
		left->parent = newNode;
	}

	if(right != NULL) {
		right->parent = newNode;
	}

	return newNode;
}

//Compares two nodes and returns the one that occurs least frequently
//O(1)
static int nodecmp(NODE *t1, NODE *t2) {
	assert(t1 != NULL && t2 != NULL);
	if(t1->count < t2->count) {
		return t1->count;
	}
	else {
		return t2->count;
	}
}

//Gives the height of a node
//O(h)
static int depth(NODE *np) {
	assert(np != NULL);
	
	if(np->parent == NULL) {
		return 0;
	} else {
		return (1 + depth(np->parent));
	}
}

void pack(char *infile, char *outfile, struct node *leaves[257]);

int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[1], "rb");
	assert(fp != NULL);

	int carray[257];
	NODE *leaves[257];

	int i;
	for(i = 0; i < 257; i++) {
		carray[i] = 0;
		leaves[i] = NULL;
	}
	
	int j;
	while((j = getc(fp)) != EOF) {
		carray[j]++;
	}

	PQ *pq = createQueue(*nodecmp);
	int k;
	for (k = 0; k < 257; k++) {
		if(carray[k] > 0) {
			leaves[k] = mknode(carray[k], NULL, NULL);
			addEntry(pq, leaves[k]);
		}
	}

	while(numEntries(pq) > 1) {
		NODE *cl = removeEntry(pq);
		NODE *cr = removeEntry(pq);
		NODE *root = mknode(cl->count + cr->count, cl, cr);
		addEntry(pq, root);
	}

	//Not sure how to do step 5...

	fclose(fp);
	return 0;
}
