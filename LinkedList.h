/*
 * LinkedList.h
 *
 *  Created on: Dec 10, 2021
 *      Author: ise
 */
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include "Defs.h"
typedef struct Node_t* Node;
typedef struct LinkedList_t* LinkedList;

/*
 * Linked List implementation using List struct and Node struct
 * to store the data in it.
 * list struct holds only functions for element using it
 * and pointer to head and end to list.
 */
LinkedList createLinkedList(copyFunction copyFunc, freeFunction freeFunc,
		printFunction printFunc,equalFunction compFunc,equalFunction compareByKey);


/*
 *create node with dada in it
 */
Node creatNode(element data);
/*
 * deletes all nodes in list with destroyNode function
 * lastly free the list
 */
status destroyList(LinkedList list);


/*
 * deletes the node and free it with free function delivered by user
 * and the element holds by node
 * returns status of operation
 */
status destroyNode(Node node,freeFunction freefunc);


/*
 * insert new Node to the end of the linked list
 * returns status of operation
 */
status appendNode(LinkedList list,element e);


/*
 * delete the node which holds data e from list
 * using free function delivered by user
 */
status deleteNode(LinkedList list,element e);


/*
 * print list Nodes
 * using print function delivered by user
 */
status displayList(LinkedList list);


/*
 * return the element if e exist in list, NULL otherwise
 * using compareByKey function delivered by user to find e in list
 */
element searchByKeyInList(LinkedList list, element e);

#endif /* LINKEDLIST_H_ */
