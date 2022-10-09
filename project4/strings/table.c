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
	char **data;
}SET;

static unsigned strhash(char *s);

static int search(SET *sp, char *elt, bool *found) {
	assert(sp != NULL);
	int index, i, dindex, mark;
	mark = -1;

	unsigned inital = strhash(elt);

	for(i = 0; i < sp->length; i++) {
		index = (inital+i) % (sp->length);
		if(sp->flags[index] == EMPTY) {
			if(mark == 1) {
				return dindex;
			} else {
				return index;
			}
		} else if (sp->flags[index] == FILLED) {
			if(strcmp(sp->data[index],elt) == 0) {
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

//hash function
static unsigned strhash(char *s) {
	unsigned hash = 0;
	
	while(*s != '\0') {
		hash = 31 *hash + *s++;
	}
	return hash;
}

//Function that allocates memory for the set of data. Allocates data for flags.
//Runtime of O(n)
SET *createSet(int maxElts) {
	int i;
	SET *sp;
	
	sp = malloc(sizeof(SET));
	assert(sp != NULL);

	sp->data = malloc(sizeof(char*)*maxElts);
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
//Runtime of O(n)
void destroySet(SET *sp) {
	assert(sp != NULL);
	int i;

	for(i = 0; i < sp->length; i++) {
		if(sp->flags[i] == FILLED) {
			free(sp->data[i]);
		}
	}
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
void addElement(SET *sp, char *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	int i;

	if(found == false) {
		sp->data[sindex] = strdup(elt);
		sp->flags[sindex] = FILLED;
		sp->count++;
	}
}

//Function that removes an element from the set. After searching, it will free the memory of that element. The flag array will then indicate the spot has been deleted for future searches
//Runtime of O(n)
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	int i;

	if(found == true) {
		sp->flags[sindex] = DELETED;
		free(sp->data[sindex]);
		sp->count--;
	}
}

//Function that finds an element in the set and returns it. After searching, it it finds what is requested, it will return the element.
//Runetime of O(n)
char *findElement(SET *sp, char *elt) {
	assert(sp != NULL);
	bool found = false;
	int sindex = search(sp,elt,&found);
	
	if(found == true) {
		return sp->data[sindex];
	}
	return NULL;
}

//Function that will create another set for use. It will copy all spots currently filled into the new set of data
//Runetime of O(n)
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **newdata = malloc(sizeof(char*)*(sp->count));
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

