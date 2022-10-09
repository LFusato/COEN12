#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "pqueue.h"

#define p(x) (((x)-1)/2)
#define l(x) ((x)*2+1)
#define r(x) ((x)*2+2)
#define INIT_LENGTH 10

struct pqueue {
	int count;
	int length;
	void **data;
	int (*compare)();
};

//creates a queue using inital values
//O(1)
PQ *createQueue(int (*compare)()) {
	PQ *pq;

	assert(compare != NULL);

	pq = malloc(sizeof(PQ));
	assert(pq != NULL);

	pq->compare = compare;
	pq->length = INIT_LENGTH;
	pq->count = 0;

	pq->data = malloc(sizeof(void*) * pq->length);
	assert(pq->data != NULL);
	
	return pq;
}

//Destroys the queue's data, then the queue itself
//O(1)
void destroyQueue(PQ *pq) {
	assert(pq != NULL);
	free(pq->data);
	free(pq);
	return;	
}

//Returns the number of entires in a given queue
//O(1)
int numEntries(PQ *pq) {
	assert(pq != NULL);
	return pq->count;
}

//Adds an entry to the queue, if it is smaller than it parent it swaps places, and continues until its parent is no longer larger than it. If the array is full, it allocates a new array with double the previous length
//O(logn)
void addEntry(PQ *pq, void *entry) {
	assert(pq != NULL && entry != NULL);
	
	if(pq->count == pq->length) {
		pq->length = pq->length*2;
		pq->data = realloc(pq->data,sizeof(void) * pq->length);
		assert(pq->data != NULL);
	}

	int index = pq->count;
	pq->data[index] = entry;
	int pindex = p(index);

	while((*pq->compare)(pq->data[index], pq->data[pindex]) < 0) {
		void *temp = pq->data[pindex];
		pq->data[pindex] = pq->data[index];
		pq->data[index] = temp;
		index = pindex;
		pindex = p(pindex);
	}

	pq->count++;
	return;
}

//Removes the entry fron the given queue at the top of the tree. It then moves the remaining nodes around accordingly and returns the node removed.
//O(logn)
void *removeEntry(PQ *pq) {
	assert(pq != NULL);
	PQ *pDel = pq->data[0];
	assert(pDel != NULL);

	int index = 0;
	int lindex, rindex;
	void *temp;
	pq->data[0] = pq->data[pq->count-1];

	while(r(index) < pq->count) {
		lindex = l(index);
		rindex = r(index);
		if(((*pq->compare)(pq->data[index], pq->data[lindex]) < 0) && (*pq->compare)(pq->data[index],pq->data[rindex]) < 0) {
			index = lindex;
			break;
		} else {
			if((*pq->compare)(pq->data[rindex], pq->data[lindex]) < 0) {
				temp = pq->data[index];
				pq->data[index] = pq->data[rindex];
				pq->data[rindex] = temp;
				index = rindex; 		
			} else {
				temp = pq->data[index];
				pq->data[index] = pq->data[lindex];
				pq->data[lindex] = temp;
				index = lindex;
			}
		}
	}
	pq->count--;
	return pDel; 
}



