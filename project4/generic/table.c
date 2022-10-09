#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "set.h"

#define EMPTY 0
#define FILLED 1
#define DELETED 2

typedef struct set {
	int length;
	int count;
	char *flags;
	void **data;
	int (*compare)();
	unsigned (*hash)();
}SET;

//Search function that will return the index value of a requested element. If it is not present it returns as a -1. If the search goes over a deleted spot it will mark it to return as a spot to be returned if the function wants to insert.
//Runtime of O(n)
static int search(SET *sp, void *elt, bool *found) {
	assert(sp != NULL);
	int index, i, dindex, mark;
	mark = -1;

	unsigned inital = (*sp->hash)(elt);

	for(i = 0; i < sp->length; i++) {
		index = (inital+i) % (sp->length);
		if(sp->flags[index] == EMPTY) {
			if(mark == 1) {
				return dindex;
			} else {
				return index;
			}
		} else if (sp->flags[index] == FILLED) {
			if((*sp->compare)(sp->data[index],elt) == 0) {
				*found = true;
				return index;
			}
		} else {
			if(mark == -1) {
				dindex = index;
				mark = 1;
			}
		}
	}
	return -1;
}

//Function that allocates memory for the set of data. Allocates data for flags.
//Runtime of O(n)
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	int i;
	SET *sp;
	
	sp = malloc(sizeof(SET));
	assert(sp != NULL);

	sp->compare = compare;
	sp->hash = hash;

	sp->data = malloc(sizeof(void*)*maxElts);
	assert(sp->data != NULL);

	sp->flags = malloc(sizeof(char)*maxElts);
	assert(sp->flags != NULL);
	
	sp->length = maxElts;
	sp->count = 0;

	for(i = 0; i < maxElts; i++) {
		sp->flags[i] = EMPTY;
	}
	
	return sp;
}

//Function that removes the set by freeing the memory it uses. Will only free filled spots fist
//Runtime of O(1)
void destroySet(SET *sp) {
	assert(sp != NULL);

	free(sp->data);
	free(sp->flags);
	free(sp);
}

//Function that returns the number of elements in the set
//Runtime of O(1)
int numElements(SET *sp) {
	assert(sp != NULL);
	return(sp->count);
}

//Function that adds an element to the set. After searching, it will insert the desired element into the array. The flag array will then indicate the spot is filled.
//Runtime of O(n)
void addElement(SET *sp, void *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);

	if(found == false) {
		sp->data[sindex] = elt;
		sp->flags[sindex] = FILLED;
		sp->count++;
	}
}

//Function that removes an element from the set. After searching, it will free the memory of that element. The flag array will then indicate the spot has been deleted for future searches
//Runtime of O(n)
void removeElement(SET *sp, void *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);

	if(found == true) {
		sp->flags[sindex] = DELETED;
		sp->count--;
	}
}

//Function that finds an element in the set and returns it. After searching, it it finds what is requested, it will return the element.
//Runetime of O(n)
void *findElement(SET *sp, void *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	
	if(found == true) {
		return elt;
	}
	return NULL;
}

//Function that will create another set for use. It will copy all spots currently filled into the new set of data
//Runetime of O(n)
void *getElements(SET *sp) {
	assert(sp != NULL);
	void **newdata = malloc(sizeof(void)*(sp->count));
	int i, j;
	j = 0;

	for(i = 0; i < sp->length; i++) {
		if(sp->flags[i] == FILLED) {
			newdata[j] = sp->data[i];
			j++;
		}
	}
	return(newdata);
}

