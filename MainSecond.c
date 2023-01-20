/*
 * MainSecond.c
 *
 *  Created on: Dec 12, 2021
 *      Author: ise
 */

#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pokemon.h"
#include "MaxHeap.h"
#include "LinkedList.h"
#include "BattleByCategory.h"

int i;
status st;

/* deleting all types from array and frees the array memory */
void deletTypesArray(PokemonType **arr, int numOfTypes){
	int i;
	for(i=0;i<numOfTypes;i++){
		deleteType(arr[i]);
	}
	free(arr);
}
/* function to free memory when run into a failure */
void Exit(Battle battle, PokemonType** typesArray, int numOfTypes){
	destroyBattleByCategory(battle);
	deletTypesArray(typesArray, numOfTypes);
}

/* creates the types array out of the file */
status getTypesArray(PokemonType **typesArray, char* lines, int numOfTypes){
	if(numOfTypes == 0)
		return success;
	int i;

	PokemonType* pt;

	char* type = strtok(lines, ",");
	//cuts the names of types from line and creating the type object
	//iterate all types - 1 to be able to cut it properly
	for (i = 0; i < numOfTypes-1; i++) {
		pt = creatPokemonType(type);
		if(pt==NULL){
			return nomemory;
		}
		typesArray[i] = pt;

		if (i<numOfTypes-1)
			type = strtok(NULL, ",");
	}
	//last type cut
	type = strtok(type, "\r");
	pt = creatPokemonType(type);
	if(pt==NULL){
		return nomemory;
	}
	typesArray[numOfTypes-1] = pt;

	return success;
}

/* inserting all types of a row to relevant list by againstWho */
status insertIntoEffectiveList(char* typeA, char* line, PokemonType **typesArray, int numOfTypes,int againtsWho){
	char* typeB = strtok(NULL, ",");
	typeB=strtok(line,",");
	PokemonType *A = getType(typesArray, numOfTypes, typeA);
	PokemonType *B;

	//1 for effective against me
	//not 1 for effective against others
	if(againtsWho == 1){
		while(typeB != NULL){
			B=getType(typesArray, numOfTypes, typeB);
			if(A == NULL || B == NULL)
				return failure;
			st = addAgainstMe(A, B);

			if(st == nomemory){
				return nomemory;
			}
			typeB = strtok(NULL, ",");
		}
	}
	else{//against is for other
		while(typeB != NULL){
			B = getType(typesArray, numOfTypes, typeB);
			if(A == NULL || B == NULL)
				return failure;
			st = addAgainstOthers(A, B);

			if(st == nomemory){
				return nomemory;
			}
			typeB = strtok(NULL, ",");
		}
	}
	return success;
}

/* prints array of types */
status printTypesArray(PokemonType **typesArray, int numberOfTypes){
	int i;
	for(i=0;i<numberOfTypes;i++){
	  st = printType(typesArray[i]);
	  if(st == failure)
		  return failure;
	  printf("\n");
	}
	return success;
}

Pokemon* initPokemon(char* line,PokemonType** typesArray,int num){
	char *pokemonName = strtok(line, ",");
	char *species = strtok(NULL, ",");
	char *height = strtok(NULL, ",");
	double Dheight;
	sscanf(height,"%lf",&Dheight);
	char *weight = strtok(NULL, ",");
	double Dweight;
	sscanf(weight,"%lf",&Dweight);
	char *attack = strtok(NULL, ",");
	int attackI;
	sscanf(attack,"%d",&attackI);
	char *Type = strtok(NULL, "\r");
	PokemonType* pType=getType(typesArray, num, Type);
	if(pType == NULL){
		deletTypesArray(typesArray, num);
		return NULL;
	}
	Pokemon *pt = createPokemon(pokemonName, species, pType, Dheight, Dweight, attackI);
	if(pt==NULL){
		deletTypesArray(typesArray, num);
		printf("Memory Problem");
		return NULL;
	}
	pType->count_same_type++;
	return pt;
}

int main(int argc, char* argv[]) {

	//get arguments
	int numberOfTypes = atoi(argv[1]);
	int capacityInType = atoi(argv[2]);
	char* path = argv[3];


	//initialize types array of size numOfTypes
	PokemonType** typesArray = (PokemonType**)malloc(numberOfTypes*sizeof(PokemonType*));
	if (typesArray == NULL && numberOfTypes != 0) {//unintentionally empty array
		if(st == nomemory)
			printf("Memory Problem");
		return failure;
	}
	//init array
	for (i=0; i < numberOfTypes; i++) {
		typesArray[i] = NULL;
	}

	//file read
	FILE * file;
	file = fopen(path, "r");
	if (file == NULL){
		deletTypesArray(typesArray, numberOfTypes);
		return failure;
	}
	char line[300];

	//content manipulations
	fgets(line, 300, file);//take Types row out
	fgets(line, 300, file);
	char *typesLine = (char*)malloc(strlen(line)+1);///*****free******/
	strcpy(typesLine,line);
	char* types = strtok(typesLine,"\r");
	Battle battle = createBattleByCategory(capacityInType,numberOfTypes,types,(equalFunction)equalPokemon,(copyFunction)copyFunc,
			(freeFunction)deletePokemon,(getCategoryFunction)getTypeFunc,(getAttackFunction)getAttackFunc,(printFunction)printPokemon);
	if(battle == NULL){
		deletTypesArray(typesArray, numberOfTypes);
		printf("Memory Problem");
		free(typesLine);
		return nomemory;
	}
	st = getTypesArray(typesArray, line,numberOfTypes);
	if(st == nomemory){
		Exit(battle, typesArray, numberOfTypes);
		free(typesLine);
		printf("Memory Problem");
		return nomemory;
	}
	free(typesLine);
	char* A;
	char* temp;
	int againtsWho;
	//create types array
	while (fgets(line, 300, file) != NULL && !strstr(line,"Pokemons")) {
		againtsWho = strstr(line,"other") ?  0 :  1;//0=against others , 1=against me

		A = strtok(line,"\r");
		temp=strtok(A,"\t");
		temp = strtok(line,":");
		temp=strtok(NULL," ");

		A=strtok(A,"\t");//source type
		A=strtok(A," ");
		st = insertIntoEffectiveList(A, temp, typesArray,numberOfTypes,againtsWho);
		if(st == nomemory){
			Exit(battle, typesArray, numberOfTypes);
			printf("Memory Problem");
			return nomemory;
		}
		if (st == failure){
			Exit(battle, typesArray, numberOfTypes);
			return failure;
		}
	}

	Pokemon *pt;
	//create pokemons array

	while (fgets(line, 300, file) != NULL){
		pt = initPokemon(line, typesArray, numberOfTypes);
		if(pt == NULL){
			deletePokemon(pt);
			Exit(battle, typesArray, numberOfTypes);
			printf("Memory Problem");
			return failure;
		}
		st = insertObject(battle, (element)pt);
		if(st == failure){
			deletePokemon(pt);
			Exit(battle, typesArray, numberOfTypes);
			printf("Memory Problem");
			return failure;
		}
	}

	fclose(file);

	char ch = '1';

	PokemonType *newType = NULL;
	char pokemonName[300];
	char species[300];
	double height;
	double weight;
	int attack;

	do{
	  printf("Please choose one of the following numbers:\n");
	  printf("1 : Print all Pokemons by types\n");
	  printf("2 : Print all Pokemons types\n");
	  printf("3 : Insert Pokemon to battles training camp\n");
	  printf("4 : Remove strongest Pokemon by type\n");
	  printf("5 : Fight\n");
	  printf("6 : Exit\n");
	  scanf("%s", &ch);
	  switch( ch )
	  {
		  case '1':{  /* Print all Pokemons */
			  displayObjectsByCategories(battle);
			  break;
		  }
		  case '2':{  /* Print all Pokemons types */
			  st = printTypesArray(typesArray, numberOfTypes);
			  if(st == failure){
				  Exit(battle, typesArray, numberOfTypes);
				  return failure;
			  }
			  break;
		  }
		  case '3':{  /* Insert Pokemon to battles training camp */

			  	printf("Please enter Pokemon type name:\n");
			  	scanf("%s", line );
			  	newType = getType(typesArray, numberOfTypes, line);
			  	if(newType == NULL){
			  		printf("Type name doesn't exist.\n");
			  		break;
			  	}

			  	if(getNumberOfObjectsInCategory(battle, line) == capacityInType){
			  		newType = NULL;
			  		printf("Type at full capacity.\n");
			  		break;
			  	}

			  	printf("Please enter Pokemon name:\n");
			  	scanf("%s", pokemonName);

			  	printf("Please enter Pokemon species:\n");
			  	scanf("%s", species );

			  	printf("Please enter Pokemon height:\n");
			  	scanf("%lf", &height );

			  	printf("Please enter Pokemon weight:\n");
			  	scanf("%lf", &weight );

			  	printf("Please enter Pokemon attack:\n");
			  	scanf("%d", &attack );

			  	pt = createPokemon(pokemonName, species, newType, height, weight, attack);
			  	if (pt == NULL){
			  		Exit(battle,typesArray,numberOfTypes);
			  		return nomemory;
			  	}

			  st = insertObject(battle,pt);
			  if (st == success){
				  printf("The Pokemon was successfully added.\n");
				  printPokemon(pt);
				  printf("\n\n");
			  }
			  newType->count_same_type++;
			  break;
		  }
		  case '4':{ /* Remove strongest Pokemon by type */
			  printf("Please enter type name:\n");
			  scanf("%s", line);
			  newType = getType(typesArray, numberOfTypes, line);
			  if (newType == NULL){
				printf("Type name doesn't exist.\n");
				break;
			  }
			  pt = removeMaxByCategory(battle,newType->name);
			  if (pt == NULL){
				printf("There is no Pokemon to remove.\n");
				deletePokemon(pt);
				break;

			  }
			  printf("The strongest Pokemon was removed:\n");
			  newType->count_same_type--;
			  printPokemon(pt);
			  printf("\n\n");
			  deletePokemon(pt);
			  break;
		  }
		  case '5':{/* Fight */
				printf("Please enter Pokemon type name:\n");
				scanf("%s", line );
				newType = getType(typesArray, numberOfTypes, line);
				if (newType == NULL){
					printf("Type name doesn't exist.\n");
					break;
				}

				printf("Please enter Pokemon name:\n");
				scanf("%s", pokemonName);

				printf("Please enter Pokemon species:\n");
				scanf("%s", species );

				printf("Please enter Pokemon height:\n");
				scanf("%lf", &height );

				printf("Please enter Pokemon weight:\n");
				scanf("%lf", &weight );

				printf("Please enter Pokemon attack:\n");
				scanf("%d", &attack );

				pt = createPokemon(pokemonName, species, newType, height, weight, attack);
				if (pt == NULL){
					Exit(battle,typesArray,numberOfTypes);
					return nomemory;
				}

			  printf("You choose to fight with:\n");
			  printPokemon(pt);
			  printf("\n\n");
			  /* FIGHT */
			  element res = fight(battle, pt);
			  if(res == NULL){
				  printf("There are no Pokemons to fight against you.\n");
				  deletePokemon(pt);
				  break;
			  }
			  deletePokemon(pt);
			  break;
		  }
		  case '6':{
			  Exit(battle, typesArray, numberOfTypes);
			  printf("All the memory cleaned and the program is safely closed.\n\n");
			  break;
		  }
		  default:{
			  printf("Please choose a valid number.\n");
		  }
	  }//switch
	}while(ch!='6');



	return 0;
}


