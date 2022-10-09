#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "list.h"

typedef struct list {
	struct node *head;
	struct node *tail;
	int nodenum;
	int totalElements;
}LIST;

typedef struct node {
	int length;
	int count;
	int first;
	void **data;
	struct node *next;
	struct node *prev;
}NODE;

static NODE *createNode(LIST *lp);

//Function creates the list and then its head and tail pointers and their data, Total elements is set to 0 since the list is empty.
//O(1)
LIST *createList(void) {
	LIST *lp;
	lp = malloc(sizeof(LIST));
	assert(lp != NULL);

	lp->nodenum = 0;
	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	
	//Im aware that the length need to be dynamic, to a power, but I couldn't get my code to work at a oonstant length let alone a dynamic one	
	lp->head->length = 2;
	lp->head->count = 0;
	lp->head->first = 0;
//	lp->head->data = malloc(sizeof(void)*lp->head->length);
	
	lp->tail = malloc(sizeof(NODE));
	lp->tail->first = 0;
	lp->tail->length = 2;
	lp->tail->count = 0;
//	lp->tail->data = malloc(sizeof(void)*lp->tail->length);

	lp->totalElements = 0;

	return(lp);
}

//Destroys nodes in list and then list itself
//O(logn)
void destroyList(LIST *lp) {
	assert(lp != NULL);
	NODE *np = lp->head;

	while(np != NULL) {
		np = lp->head;
		lp->head = lp->head->next;
		free(np);
	}
	free(lp);
}

//gives the total number of items in the list, count is incremented when items are added to the nodes
//O(1)
int numItems(LIST *lp){
	assert(lp != NULL);
	return lp->totalElements;
}

//Adds an element to the head of the list. If there are no nodes yet it creates one, if the node it full it craetes a new one, and then adds the item
//O(1)
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);
        int index;

        if(lp->nodenum == 0) {
                NODE *newNode = createNode(lp);
                lp->head = newNode;
                lp->head->data[lp->head->length - 1] = item;
                lp->head->first = lp->head->length - 1;
		lp->totalElements++;
		lp->head->count++;
		return;
        }
        else if(lp->head->count == lp->head->length) {
                NODE *newNode = createNode(lp);
                lp->head->prev = newNode;
                newNode->next = lp->head;
                lp->head = newNode;
        }
        index = (lp->head->first + lp->head->count)%lp->head->length;
        lp->head->data[index] = item;
	lp->totalElements++;
        lp->head->count++;
}

//Adds an element to the tail of the list. If there are no nodes yet it creates one, and if the node is full it creates a new one, and then adds the item
//O(1)
void addLast(LIST *lp, void *item) {
        assert(lp != NULL && item != NULL);
        int index;

        if(lp->nodenum == 0) {
                NODE *newNode = createNode(lp);
                lp->tail = newNode;
                lp->tail->data[lp->head->length - 1] = item;
                lp->tail->first = lp->head->length - 1;
                lp->totalElements++;
                lp->tail->count++;
		lp->head = lp->tail;
                return;
        }
        else if(lp->tail->count == lp->tail->length) {
                NODE *newNode = createNode(lp);
                lp->tail->next = newNode;
                newNode->prev = lp->head;
                lp->tail = newNode;
        }
        index = (lp->tail->first + lp->tail->count)%(lp->tail->length);

        lp->tail->data[index] = item;
        lp->totalElements++;
        lp->tail->count++;
}

//Removes the oldest item that was added to the head of the list. If there is a newer node that is empty it will deallocate that node and move to the next. It will then mark the new oldest item
//O(1)
void *removeFirst(LIST *lp) {
	assert(lp != NULL);

	int delIndex = lp->head->first;
	void *returnData = lp->head->data[delIndex];
	
	if(lp->head->count == 0) {
		NODE *emptyNode = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(emptyNode);
		lp->nodenum--;
	}
	lp->head->first = (lp->head->first + 1)%(lp->head->length);

	lp->head->count--;
	lp->totalElements--;
	return returnData;
}
//Removes the oldest item that was added to the tail of the list. If there is a newer node that is empty it will deallocate that node and move to the next. It will then mark the new oldest item
//O(1)
void *removeLast(LIST *lp) {
	assert(lp != NULL);

        int delIndex = lp->tail->first;
        void *returnData = lp->head->data[delIndex];

        if(lp->tail->count == 0) {
                NODE *emptyNode = lp->tail;
                lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
                free(emptyNode);
		lp->nodenum--;
        }
        lp->tail->first = (lp->tail->first + 1)%(lp->tail->length);
        lp->tail->count--;
        lp->totalElements--;
        return returnData;
}

//gets the item that is currently occupying a certain index in the list, finds by skipping nodes that are too small for the index and adjusts accordingly
//O(logn)
void *getItem(LIST *lp, int index) {
	assert(lp != NULL && index >= 0);

	NODE *searchNode = lp->head;

	while (searchNode->count < index) {
		index = index - searchNode->count;
		searchNode = searchNode->next;	
	}

	return(searchNode->data[index]);
}
//Updates a item to a new item at the index given. It will skip nodes that are too small for the index to be in and adjusts index accordingly.
//O(logn)
void setItem(LIST *lp, int index, void *item) {
	assert(lp != NULL && item != NULL && index >= 0);

	NODE *searchNode = lp->head;
        while (searchNode->length > index) {
                index = index - searchNode->length;
                searchNode = searchNode->next;
        }
        searchNode->data[index] = item;
}

//private function that will create new nodes that are to be used in cases in the add functions, allocates the node and space for its data
//O(1)
static NODE *createNode(LIST *lp) {
	assert(lp != NULL);
	
	NODE *makeNode = malloc(sizeof(NODE));
	assert(makeNode != NULL);
	makeNode->length = 2;
	makeNode->count = 0;
	makeNode->first = 0;

	makeNode->data = malloc(sizeof(void)*makeNode->length);
	assert(makeNode->data != NULL);
	lp->nodenum++;
	return makeNode;
}
