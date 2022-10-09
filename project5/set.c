#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "set.h"

#define AVG_CHAIN_LENGTH 20

typedef struct set {
	int count;
	int length;
	LIST **lists;
	int(*compare)();
	unsigned(*hash)();
} SET;

typedef struct node {
	void *data;
	struct node *next;
	struct node *prev;
} NODE;

typedef struct list {
	int count;
	NODE *head;
	int (*compare)();
} LIST;

//Allocates memory for lists, then calls function to allocate each individual list
//Runtime O(n), average and worst
SET *createSet(int maxElts, int(*compare)(), unsigned (*hash)()) {
	int i;
	SET *sp;
	
	assert(compare != NULL && hash != NULL);

	sp = malloc(sizeof(SET));
	assert(sp != NULL);

	sp->length = maxElts/AVG_CHAIN_LENGTH;
	sp->compare = compare;
	sp->hash = hash;
	sp->count = 0;

	sp->lists = malloc(sizeof(LIST*) * sp->length);
	assert(sp->lists != NULL);

	for(i = 0; i < sp->length; i++) {
		sp->lists[i] = createList(compare);
	}
	
	return sp;
}

//Deallocates memory from lists by calling function, then deallocates lists
//Runtime O(n^2), average and worst
void destroySet(SET *sp) {
	int i;
        assert(sp != NULL);
        for(i = 0; i < sp->length; i++) {
                destroyList(sp->lists[i]);
        }
        free(sp->lists);
        free(sp);
}

//Gives the number of elements in the set
//Runtime O(n)
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->count;
}

//Adds element to the list if it is not already found in the list, and increments the counter
//Runtime O(n) 
void addElement(SET *sp, void *elt) {
	assert(sp != NULL);
	int index = (*sp->hash)(elt) % sp->length;
	if(findItem(sp->lists[index], elt) == NULL) {
		addFirst(sp->lists[index], elt);
		sp->count++;
	}
}

//Removes element from a designated list by calling the functions
//Runtime O(n)
void removeElement(SET *sp, void *elt) {
	assert(sp != NULL);
	int index = (*sp->hash)(elt) % sp->length;

	if(findItem(sp->lists[index], elt) != NULL) {
		removeItem(sp->lists[index], elt);
		sp->count--;
	}
}

//Find element in a list, returns it if found and NULL if not
//Runtime O(n)
void *findElement(SET *sp, void *elt) {
	assert(sp != NULL);

	int index = (*sp->hash)(elt) % sp->length;
	void *itemSearch = findItem(sp->lists[index], elt);
        if(itemSearch != NULL) {
                return itemSearch;
        }
	else {
		return NULL;
	}
}

//Creates an array from the linked lists
//Runtime O(n^2)
void *getElements(SET *sp) {
	assert(sp != NULL);
	int i;
	int acount = 0;
	void **array = malloc(sizeof(void)*sp->count);
	NODE *np;
	
	for(i = 0; i < sp->length; i++) {	//runs through each list
		if (numItems(sp->lists[i]) > 0) {	//If its not empty
			np = sp->lists[i]->head->next;
			while (np != sp->lists[i]->head) {
				array[acount] = np->data;
				np = np->next;
				acount++;
			}			
		}		
	}

	return array;
}
