/*
 * LinkedList.c
 *
 *  Created on: Dec 10, 2021
 *      Author: ise
 */
#include "LinkedList.h"
#include <stdlib.h>

struct Node_t{
	element data;
	struct Node_t *next;
};


struct LinkedList_t{
	Node head;
	Node end;
	int size;

	copyFunction copyFunc;
	freeFunction freeFunc;
	printFunction printFunc;
	equalFunction compareFunc;
	equalFunction compareByKey;
};

Node creatNode(element data){
	Node newNode = (Node)malloc(sizeof(struct Node_t));
	if(newNode == NULL){
		return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

status destroyNode(Node node,freeFunction freefunc){
	if(node == NULL)
		return success;
	freefunc(node->data);
	free(node);
	return success;
}

LinkedList createLinkedList(copyFunction copyFunc, freeFunction freeFunc,
		printFunction printFunc,equalFunction compFunc,equalFunction compareByKey){
	LinkedList list = (LinkedList)malloc(sizeof(struct LinkedList_t));
	if(list == NULL)
		return NULL;
	list->size=0;
	list->copyFunc=copyFunc;
	list->freeFunc=freeFunc;
	list->printFunc = printFunc;
	list->compareFunc = compFunc;
	list->compareByKey = compareByKey;
	list->head = NULL;
	list->end = NULL;
	return list;
}

status destroyList(LinkedList list){
	Node next = NULL;
	while(list->head != NULL){
		next = list->head->next;
		destroyNode(list->head, list->freeFunc);
		list->head = next;
	}
	free(list);
	return success;
}




status appendNode(LinkedList list,element e){
	Node newNode = creatNode(e);
	if(newNode == NULL){
		return failure;
	}
	if(list->head == NULL){
		list->head = newNode;
		list->end = newNode;
		list->size++;
		return success;
	}
	list->end->next = newNode;
	list->end = newNode;
	list->size++;
	return success;
}

status deleteNode(LinkedList list,element e){
	Node curr = list->head;
	Node prev = NULL;
	while(curr != NULL && curr->data != e){
		prev = curr;
		curr = curr->next;
	}
	//e doesn't exist in list
	if(curr == NULL)
		return failure;

	//now curr holds the node of data e
	if(prev == NULL)
		list->head = curr->next;
	else
		prev->next = curr->next;
	destroyNode(curr, list->freeFunc);
	list->size--;
	return success;

}

status displayList(LinkedList list){
	if(list == NULL)
		return failure;
	Node curr = list->head;
	while(curr != NULL){
		list->printFunc(curr->data);
		curr = curr->next;
	}
	return success;
}
/*e is the name of heap aka the category name*/
element searchByKeyInList(LinkedList list, element e){
	if(list == NULL || e == NULL)
		return NULL;
	if(list->size == 0)
		return NULL;
	Node curr = list->head;
	while(curr != NULL){
		if(list->compareByKey(curr->data,e) == 1){
			return curr->data;
		}
		curr = curr->next;
	}
	return NULL;
}

