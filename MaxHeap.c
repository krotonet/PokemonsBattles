/*
 * MaxHeap.c
 *
 *  Created on: Dec 11, 2019
 *      Author: ise
 */
#include "MaxHeap.h"

struct MaxHeap_t
{
	int capacity;
	int size;
	char* name;
	element* elements;

	copyFunction copyFunc;
	printFunction printFunc;
	freeFunction freeFunc;
	equalFunction equalFunc;
};
/*
 * creates heap with given data
 * if one of data invalid returns NULL
 */
MaxHeap CreateHeap(int capacity, char* name, copyFunction copyFunc, printFunction printFunc,
		freeFunction freeFunc, equalFunction equalFunc){
	if (copyFunc==NULL || printFunc== NULL || freeFunc==NULL ||equalFunc==NULL)
		return NULL;

	if (name== NULL)
		return NULL;

	MaxHeap heap = (MaxHeap) malloc(sizeof(struct MaxHeap_t));
	if (heap == NULL)
		return NULL;

	heap->copyFunc= copyFunc;
	heap->equalFunc= equalFunc;
	heap->freeFunc= freeFunc;
	heap->printFunc = printFunc;

	char* n = (char*)malloc(strlen(name)+1);
	if(n == NULL){
		free(heap);
		return NULL;
	}
	strcpy(n,name);
	heap->name = n;

	heap->capacity=capacity;
	element* array = (element*)malloc(sizeof(element)*capacity);
	if(array == NULL){
		free(n);
		free(heap);
		return NULL;
	}
	int i;
	for(i = 0; i < capacity; i++)
		array[i]=NULL;
	heap->elements = array;
	heap->size=0;

	return heap;
}

element HeapCopy(element elem){
	MaxHeap heap =(MaxHeap)elem;
	if(heap==NULL){
		return NULL;
	}

	MaxHeap newheap = CreateHeap(heap->capacity,heap->name,heap->copyFunc,heap->freeFunc,heap->printFunc,heap->equalFunc);
	if(newheap==NULL){//creating heap
			return NULL;
	}

	for(int i=0;i<heap->size;i++){
		insertToHeap(newheap, heap->elements[i]);//insert each element to the new copy
	}

	return (element)newheap; //return the copy heap
}

/*
 * switching two elements in array
 */
void swap(MaxHeap heap,int i, int j){
	element temp = heap->elements[i];
	heap->elements[i]=heap->elements[j];
	heap->elements[j]= temp;
}

/*
 * sorting array as a heap
 */
void MaxHeapify(MaxHeap heap,int i,int n){
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if ((left < n) && (heap->equalFunc(heap->elements[left],heap->elements[largest]) == 1))
		largest = left;

	if ((right < n) && (heap->equalFunc(heap->elements[right],heap->elements[largest]) == 1))
		largest = right;

	if (largest != i){
		swap(heap,i,largest);
		MaxHeapify(heap,largest,n);
	}
}

/*
 * insert element to heap
 * after insertion to last available place,
 *  sorting as heap again
 *  returns success in insertion went well
*/
status insertToHeap(MaxHeap heap, element elem){
	if (heap == NULL || elem == NULL)
		return failure;

	//checking if there is enough space for more element
	if ((heap->size)==(heap->capacity))
		return failure;

	heap->elements[heap->size] = heap->copyFunc(elem);
	heap->size++;
	int i;
	for(i = heap->size/2-1;i>=0;i--){
		MaxHeapify(heap,i,heap->size);
	}
	return success;
}

element PopMaxHeap(MaxHeap heap){
	if (heap== NULL)
			return NULL;

	if (heap->size==0)
		return NULL;
	element max = heap->elements[0];

	heap->elements[0] = heap->elements[heap->size-1];
	heap->elements[heap->size-1] = NULL;
	heap->size--;
	MaxHeapify(heap, 0, heap->size);

	return max;
}

element TopMaxHeap(MaxHeap heap){
	if(heap->size == 0)
		return NULL;
	return heap->elements[0];
}


/*
 * returns success if destroy of heap went well
 * otherwise if didn't succeed to free element
 */
status destroyheap(MaxHeap heap){
	if (heap == NULL)
		return success;

	int i;
	status res = success;
	for(i=0; i<heap->size;i++){
		if (heap->freeFunc(heap->elements[i]) != success)
			res = failure;
	}

	free(heap->elements);

	if (heap->name != NULL)
		free(heap->name);
	free(heap);
	return res;
}


status printHeap(MaxHeap heap){
	if (heap == NULL)
		return failure;

	printf("%s:\n",heap->name);

	if (heap->size == 0){
		printf("No elements.\n\n");
		return success;
	}
	element e = (element)heap;
	element copy = HeapCopy(e);
	MaxHeap heapCopy = (MaxHeap)copy;

	int i;
	int size = heap->size;
	for (i=0; i < size; i++)
	{
		printf("%d. ",i+1);
		element elem = PopMaxHeap(heapCopy);
		if(heap->printFunc(elem) != success)
			return failure;
		printf("\n\n");
	}

	destroyheap(heapCopy);
	return success;
}



/*
 *prints all elements in heap
 *prints using a copy
 */
status printHeap1(MaxHeap heap){
	if (heap == NULL)
		return failure;

	printf("%s:\n",heap->name);

	if (heap->size == 0){
		printf("No elements.\n\n");
		return success;
	}

	MaxHeap heap_copy = CreateHeap(heap->capacity,heap->name,heap->copyFunc, heap->printFunc, heap->freeFunc, heap->equalFunc);
	int i;
	int size = heap->size;
	for (i=0; i < size; i++)
	{
		printf("%d. ",i+1);
		element elem = PopMaxHeap(heap);
		if(heap->printFunc(elem) != success)
			return failure;
		insertToHeap(heap_copy,elem);
		printf("\n\n");
	}

	for (i=0; i < size; i++){
		element elem = PopMaxHeap(heap_copy);
		insertToHeap(heap,elem);
	}

	destroyheap(heap_copy);
	return success;
}

char* getHeapID(MaxHeap heap){
	if (heap== NULL)
			return NULL;

	return heap->name;
}

int getHeapCurrentSize(MaxHeap heap){
	if (heap== NULL)
		return -1;

	return heap->size;
}


int compareHeaps(element heap1, element heap2){
	if(heap1 == NULL || heap2 == NULL){
		printf("compareHeaps problem");
		return 0;
	}
	MaxHeap mHeap1 = (MaxHeap)heap1;
	MaxHeap mHeap2 = (MaxHeap)heap2;
	if(strcmp(getHeapID(mHeap1),getHeapID(mHeap2)) == 0)
		return 1;

	return 0;
}

int compareByCategory(element heapElem, element category){
	MaxHeap heap = (MaxHeap)heapElem;
	char *cat = (char*)category;
	if(strcmp(getHeapID(heap),cat) == 0)
		return 1;
	return 0;
}
