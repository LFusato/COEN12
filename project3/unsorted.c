#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "set.h"

typedef struct set {
	int length;
	int count;
	char **data;
}SET;

//Search function that will return the location of the element that is requested. Will return -1 if it is not found
//Runtime of O(n)
static int search(SET *sp, char *elt){
        int i;
        assert(sp != NULL);
        for(i = 0; i < sp->count; i++) {
                if (strcmp(sp->data[i], elt) == 0) {
                        return i;
                }
        }
        return -1;
}

//Function that creates an empty set. It will allocate memory according to the maximum elements given
//Runtime of O(1) 
SET *createSet(int maxElts) {
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data != NULL);
	return sp;
}

//Function that destorys the indicated set by freeing the memory.
//Runtime of O(n)
void destroySet(SET *sp) {
	int i;
	assert(sp != NULL);
	for(i = 0; i < sp->count; i++) {
		free(sp->data[i]);
	}
	free(sp->data);
	free(sp);
}

//Function that returns the number of elements in the data array of the set
//Runtime of O(1)
int numElements(SET *sp) {
	assert(sp != NULL);
	return(sp->count);
}

//Function that adds an element into the array of the set. Will search first to see whether or not an idential element already exists.
//Runtime of O(n)
void addElement(SET *sp, char *elt) {
	assert(sp != NULL);
	if(search(sp,elt) == -1) {
		sp->data[sp->count] = strdup(elt);
		sp->count++;
	} else {
		return;
	}
}

//Function that removes an element from the array of the set. It will search first to find the element and will replace that element with the one at the end of the set.
//Runtime of O(n) 
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL);
	int i = search(sp,elt);
	if(i == -1) {
		return;	
	}
	free(sp->data[i]);

	sp->data[i] = sp->data[sp->count-1];
	sp->count--;
}

//Function that finds and returns the index of the element requested. The search function will determine whether or not the element exists first.
//Runtime of O(n)
char *findElement(SET *sp, char *elt) {
	assert(sp != NULL);
	int i = search(sp,elt);
	if(i == -1) {
		return NULL;
	} else {
		return sp->data[i];
	}
}

//Function that creates a new set of data from the existing set. It will allocate memory for the new, abreviatd set.
//Runtime O(1)
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **newdata = malloc(sizeof(char*)*(sp->count));
	newdata = sp->data;
	return(newdata);
}
