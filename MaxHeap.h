/*
 * MaxHeap.h
 *
 *  Created on: Dec 11, 2021
 *      Author: ise
 */

#ifndef MAXHEAP_H_
#define MAXHEAP_H_
#include "Defs.h"

typedef struct MaxHeap_t* MaxHeap;

MaxHeap CreateHeap(int maxNumOfElements, char* heapName,
		copyFunction copyFunc,printFunction printFunc, freeFunction freeFunc,equalFunction equalElement);
status insertToHeap(MaxHeap MH, element e);
status printHeap(MaxHeap MH);
element PopMaxHeap(MaxHeap MH);
element TopMaxHeap(MaxHeap MH);
char* getHeapID(MaxHeap MH);
int getHeapCurrentSize(MaxHeap MH);
status destroyheap(MaxHeap MH);
element HeapCopy(element elem);
int compareHeaps(element heap1, element heap2);
int compareByCategory(element heapElem, element category);

#endif /* MAXHEAP_H_ */
