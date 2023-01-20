#ifndef DEFS_H_
#define DEFS_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

typedef enum e_bool { false, true } bool;

typedef enum e_status { success, failure, nomemory } status;

typedef void* element;

typedef element (*copyFunction)(element);
typedef status(*freeFunction) (element);
typedef status(*printFunction) (element);

/*
 * 1 first bigger, 0 - same, -1 second bigger
 */
typedef int(*equalFunction)(element,element);

typedef char*(*getCategoryFunction)(element);

/*
 * 	return the first attack - second attack .
 * 	also produce the attack of each element,
 * 	The function transmits the information
 * 	through the pointers received in the function.
*/
typedef int(*getAttackFunction)(element firstElem ,element secondElem ,int* attackFirst,int* attackSecond);

#endif /* DEFS_H_ */

