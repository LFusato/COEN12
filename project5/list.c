#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

typedef struct node{
	void *data;
	struct node *next;
	struct node *prev;
} NODE;

typedef struct list{
	int count;
	NODE *head;
	int(*compare)();
} LIST;

//Allocates memory for the list and nodes, establishes head and head's next and prev pointers to itself
//Runtime O(1), average and worst
LIST *createList(int (*compare)()) {
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	lp->count = 0;
	lp->compare = compare;

	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);

	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	return lp;
}

//Deallocates memory for the nodes and then the list.
//Runtime O(n), average and worst
void destroyList(LIST *lp) {
	assert(lp != NULL);
	NODE *pPrev = lp->head->prev;
	NODE *pDel = NULL;
	assert(pPrev != NULL);
	while(pDel != lp->head) {
		pDel = pPrev;
		pPrev = pPrev->prev;
		free(pDel);
	}
	free(lp);
}

//Gives the current number of items in the linked list
//Runtime O(1), average and worst
int numItems(LIST *lp) {
	assert(lp != NULL);
	return (lp->count);
}

//Adds the given element to the front of the list, and adjusts the next and prev pointers accordingly
//Runtime O(1), average and worst
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL);
	NODE *np = malloc(sizeof(NODE));
	np->data = item;

	np->prev = lp->head;
	np->next = lp->head->next;
	lp->head->next->prev = np;
	lp->head->next = np;
	lp->count++;
}

//Adds the given element to the end of the list, and adjusts the next and prev pointers accordingly
//Runtime O(1), average and worst
void addLast(LIST *lp, void *item) {
	assert(lp != NULL);
        NODE *np = malloc(sizeof(NODE));
        np->data = item;

        np->prev = lp->head->prev;
        np->next = lp->head;
        lp->head->prev->next = np;
        lp->head->prev = np;
        lp->count++;
}

//Removes the first element at the front of the list, adjusts the next and prev pointers accordingly
//Runtime O(1), average and worst
void *removeFirst(LIST *lp) {
	assert(lp != NULL && lp->count > 0);
	NODE *np = lp->head->next;
	void *returnData = np->data;

	lp->head->next = np->next;
	np->next->prev = lp->head;
	
	lp->count--;
	free(np);
	return returnData;
}

//Removes the last element from the end of the the list, adjusts the next and prev pointers accordingly
//Runtime O(1), average and worst
void *removeLast(LIST *lp) {
	assert(lp != NULL && lp->count > 0);
        NODE *np = lp->head->prev;
        void *returnData = np->data;

        lp->head->prev = np->prev;
        np->prev->next = lp->head;

	lp->count--;
        free(np);
        return returnData;

}

//Returns the first element in the list
//Runtime O(1), average and worst
void *getFirst(LIST *lp) {
	assert(lp != NULL && lp->count > 0);
	return lp->head->next->data;
}

//Returns the last element in the list
//Runtime O(1), average and worst
void *getLast(LIST *lp) {
	assert(lp != NULL && lp->count > 0);
        return lp->head->prev->data;

}

//Finds and then removes a given element in the list
//Runtime O(n) average and  worst case
void removeItem(LIST *lp, void *item) {
	assert(lp != NULL && lp->count > 0);
	assert(lp->compare != NULL);
	NODE *pDel = lp->head->next;
	NODE *pPrev = pDel->prev;
	while(pDel != lp->head) {
		if((lp->compare)(pDel->data,item) == 0) {
			pPrev->next = pDel->next;
			pDel->next->prev = pPrev;

			free(pDel);
			lp->count--;
			return;
		}
		pPrev = pDel;
		pDel = pDel->next;
	}
	return;
}

//Finds and returns a given element in the list
//Runtime O(n) average and worst case
void *findItem(LIST *lp, void *item) {
        assert(lp != NULL);
        NODE *np = lp->head->next;
        while(np != lp->head) {
                if((lp->compare)(np->data,item) == 0) {
                        return np->data;
                } else {
                        np = np->next;
                }
        }
	return NULL;
}

//Takes linked list and copies data into an array of necessary length
//Runtime O(n), average and worst case
void *getItems(LIST *lp) {
	assert(lp != NULL);
	void **array = malloc(sizeof(void) * lp->count);
	assert(array != NULL);

	int i = 0;
	NODE *np = lp->head->next;
	while(np != lp->head){
		array[i] = np->data;

		i++;
		np = np->next;
	}
	return array;
}
