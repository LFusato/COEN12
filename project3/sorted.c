#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "set.h"

typedef struct set {
	int length;
	int count;
	char **data; 
}SET;

//declare search function
static int search(SET *sp, char *elt, bool *found);

//Function that allocates memory for the set of data.
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

//Function that removes the set by freeing up the memory it uses.
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

//function that returns the number of elements in the set.
//Runtime of O(1)
int numElements(SET *sp) {
	assert(sp != NULL);
	return(sp->count);
}

//Function that adds an element to the set. After searching, it will insert the desired element into the sorted array in the appropriate spot. It will shift all other elements according to its postion.
//Runtime of O(n)
void addElement(SET *sp, char *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	int i;
	if(found == false) {
		for(i = sp->count-1; i >= sindex; i--) {
			sp->data[i+1] = sp->data[i];
		}
		sp->data[sindex] = strdup(elt);
		sp->count++; 
	}
}

//Function that removes an element from the set. After searching, it will free the memory of that element and shift all remaining elements to cover the gap.
//Runtime of O(n)
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	int i;
	if(found == true) {
		free(sp->data[sindex]);
		for(i = sindex; i < sp->count; i++) {
			sp->data[i] = sp->data[i+1];
		}
		sp->count--;
	}
}

char *findElement(SET *sp, char *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	if(found == true) {
		return sp->data[sindex];
	}
	return NULL;
}

//function that creates a new set of memory with max space based on count.
//Runtime of O(1)
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **newdata = malloc(sizeof(char*)*(sp->count));
	newdata = sp->data;
	return(newdata);
}

//function that searches for index of requested element. Finds through binary search. If not found the variable will chance and return the first value.
//Run time of O(logn)
static int search(SET *sp, char *elt, bool *found){
        assert(sp != NULL);
        int low, high, mid, compare;
        low = 0;
        high = sp->count-1;
        while(low <= high) {
                mid = (high+low)/2;
		compare = strcmp(elt,sp->data[mid]);
                if(compare == 0) {
                        *found = true;
                        return mid;
                } else if (compare > 0) {
                        low = mid+1;
                } else {
                        high = mid-1;
                }
        }
        *found = false;
        return low;
}
