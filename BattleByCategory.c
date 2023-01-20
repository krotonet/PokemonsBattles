/*
 * BattleByCategory.c
 *
 *  Created on: Dec 14, 2021
 *      Author: ise
 */

#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"
#include "Pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct battle_s
{
	char** categoriesArray;
	int capacity;
	int numOfCategories;
	LinkedList categoriesList;

	printFunction printElement;
	getCategoryFunction getcatFunc;
	getAttackFunction getattackFunc;
};

Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,copyFunction copyElement,
		freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement){
	status st;
	if (equalElement == NULL || copyElement == NULL || freeElement == NULL || printElement == NULL)
		return NULL;
	if (getCategory == NULL || getAttack == NULL)
		return NULL;

	if (categories == NULL)
		return NULL;

	Battle battle = (Battle)malloc(sizeof(struct battle_s));
	if (battle == NULL)
		return NULL;

	battle->getcatFunc = getCategory;
	battle->getattackFunc = getAttack;
	battle->printElement = printElement;

	battle->capacity = capacity;
	battle->numOfCategories = numberOfCategories;

	battle->categoriesArray = (char**)malloc(sizeof(char*)*numberOfCategories);
	if(battle->categoriesArray == NULL){
		free(battle);
		return NULL;
	}

	battle->categoriesList = createLinkedList((copyFunction)HeapCopy, (freeFunction)destroyheap,
			(printFunction)printHeap, compareHeaps,compareByCategory);
	if(battle->categoriesList == NULL){
		free(battle);
		free(battle->categoriesArray);
		return NULL;
	}
	int i;
	char* cat = strtok(categories, ",");
	for(i = 0; i < numberOfCategories; i++){
		MaxHeap elementsHeap = CreateHeap(capacity, cat, copyElement, printElement,freeElement, equalElement);
		if(elementsHeap == NULL){
			destroyheap(elementsHeap);
			if(i>0){
				int j;
				for(j=0;j<i;j++){
					free(battle->categoriesArray[j]);
				}
			}
			free(battle->categoriesArray);
			destroyList(battle->categoriesList);
			free(battle);
			return NULL;
		}
		battle->categoriesArray[i] = (char*)malloc(strlen(cat)+1);
		if(battle->categoriesArray[i] == NULL){
			for(i = 0; i < battle->numOfCategories && battle->categoriesArray[i] != NULL; i++){
				free(battle->categoriesArray[i]);
			}
			destroyList(battle->categoriesList);
			destroyheap(elementsHeap);
			free(battle);
			return NULL;
		}
		strcpy(battle->categoriesArray[i],cat);
		st = appendNode(battle->categoriesList, elementsHeap);
		if(st == failure){
			for(i = 0; i < battle->numOfCategories && battle->categoriesArray[i] != NULL; i++){
					free(battle->categoriesArray[i]);
				}
				destroyList(battle->categoriesList);
				destroyheap(elementsHeap);
				free(battle);
				return NULL;
		}
		cat = strtok(NULL, ",");
	}
	return battle;
}

status destroyBattleByCategory(Battle b){
	if(b == NULL)
		return success;
	int i;
	for(i = 0; i < b->numOfCategories && b->categoriesArray[i] != NULL; i++){
		free(b->categoriesArray[i]);
	}
	free(b->categoriesArray);
	destroyList(b->categoriesList);
	free(b);
	return success;
}

status insertObject(Battle b, element elem){
	if(b == NULL || elem == NULL)
		return failure;
	char* categoryToINsert = b->getcatFunc(elem);
	element heap = searchByKeyInList(b->categoriesList, (element)categoryToINsert);

	if(heap == NULL)
		return failure;

	/*no more free space in heap*/
	if(getHeapCurrentSize((MaxHeap)heap) == b->capacity)
		return failure;

	status st;

	st = insertToHeap((MaxHeap)heap, elem);
	if(st == failure)
		return failure;

	return success;
}

void displayObjectsByCategories(Battle b){
	if(b == NULL)
		return;
	displayList(b->categoriesList);
}

element removeMaxByCategory(Battle b,char* category){
	if(b == NULL)
		return NULL;

	MaxHeap heap = searchByKeyInList(b->categoriesList, (element)category);
	if(heap == NULL)
		return NULL;

	element e = PopMaxHeap(heap);
	if(e == NULL)
		return NULL;

	return e;
}

int getNumberOfObjectsInCategory(Battle b,char* category){
	if(b == NULL)
		return 0;

	MaxHeap heap = (MaxHeap)searchByKeyInList(b->categoriesList, (element)category);
	if(heap == NULL)
		return 0;
	return getHeapCurrentSize(heap);

}

/* elem is the pokemon of opponent */
element fight(Battle b,element elem){
	if(b == NULL || elem == NULL)
		return NULL;

	int i;

	Pokemon* oppPokemon = (Pokemon*)elem;
	int oppAttack = oppPokemon->ppb->attack;

	int sysAttack, result = 0, maxAttack = (-oppAttack);
	MaxHeap heap;
	element sysElem = NULL;

	for(i = 0; i < b->numOfCategories; i++){

		heap = searchByKeyInList(b->categoriesList, b->categoriesArray[i]);

		element temp = PopMaxHeap(heap);
		if(temp == NULL){//no items in this heap
			continue;
		}
		Pokemon* sysPokemon = (Pokemon*)temp;
		result = b->getattackFunc(sysPokemon ,oppPokemon , &sysAttack, &oppAttack);

		if(sysAttack > maxAttack){
			maxAttack = sysAttack;
			sysElem = temp;
		}

		insertToHeap(heap, temp);
	}

	/* no pokemons for battle */
	if(sysElem == NULL){
		return NULL;
	}

	result = b->getattackFunc(sysElem ,elem , &sysAttack, &oppAttack);

	printf("The final battle between:\n");
	b->printElement(elem);
	printf("\n\n");
	printf("In this battle his attack is :%d\n\n",oppAttack);

	printf("against ");

	b->printElement(sysElem);
	printf("\n\n");
	printf("In this battle his attack is :%d\n\n",sysAttack);

	element winner;

	/* system wins */
	if(result > 0){
		winner =  sysElem;
	}
	/* system wins */
	else if(result < 0){
		winner = elem;
	}
	/* its a draw */
	else{
		printf("IT IS A DRAW.\n");
		return sysElem;
	}
	printf("THE WINNER IS:\n");
	b->printElement(winner);
	printf("\n\n");
	return winner;
}

