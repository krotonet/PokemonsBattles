/*
 * Pokemon.c
 *
 *  Created on: Dec 20, 2021
 *      Author: ise
 */

/*
 * Pokemon.c
 */
#include "Pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pokemon* createPokemon(char *pName, char *species, PokemonType* pt, double height, double weight, int attack){
	status st = success;

	PokemonBio* pb = biologicalInfo(height, weight, attack);
	if (pb == NULL)
		st=failure;
	Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
	if (pokemon == NULL){
		st=failure;
	}

	char* name = (char*)malloc(strlen(pName) + 1);//+1 for end of char sequence \0
	if (name == NULL){
		st=failure;
	}

	char* spec = (char*)malloc(strlen(species) + 1);
	if (spec == NULL)
		st=failure;

	if(st == failure){
		free(pb);
		free(pokemon);
		free(spec);
		return NULL;
	}
	pokemon->name = name;
	strcpy(name, pName);

	pokemon->species = spec;
	strcpy(spec, species);

	pokemon->ppb = pb;
	pokemon->ppt = pt;

	return pokemon;
}

status deletePokemon(Pokemon* p){
	if(p == NULL)
		return success;
	if(p->name != NULL)
		free(p->name);
	if(p->species != NULL)
		free(p->species);
	deleteBio(p->ppb);
	free(p);
	p=NULL;

	return success;
}

PokemonType* creatPokemonType(char* tname)
{
	PokemonType* ptype = (PokemonType*)malloc(sizeof(PokemonType));
	if(ptype == NULL)
		return NULL;
	char* ptName=(char*)malloc(strlen(tname)+1);
	if(ptName == NULL){
		free(ptype);
		return NULL;
	}
	ptype->name = ptName;
	strcpy(ptName,tname);
	ptype->count_same_type = 0;
	ptype->effective_against_me = NULL;
	ptype->against_me_counter = 0;
	ptype->effective_against_others = NULL;
	ptype->against_others_counter = 0;
	return ptype;
}

status deleteType(PokemonType* pt){
	if(pt == NULL)
		return success;
	if(pt->name != NULL)
		free(pt->name);
	if(pt->effective_against_me != NULL)
		free(pt->effective_against_me);
	if(pt->effective_against_others != NULL)
		free(pt->effective_against_others);
	free(pt);
	pt = NULL;
	return success;
}

PokemonBio* biologicalInfo(double height, double weight, int attack)
{
	PokemonBio* pbio = (PokemonBio*)malloc(sizeof(PokemonBio));
	if (pbio == NULL)
		return NULL;
	pbio->attack = attack;
	pbio->height = height;
	pbio->weight = weight;
	return pbio;
}

status deleteBio(PokemonBio* pb){
	if(pb == NULL)
		return success;
	free(pb);
	pb = NULL;
	return success;
}

/*
 * while pointers are good
 * if effective list is empty create new memory for types array
 * else making the list bigger by one type and adding it
 * returns status of operation
 */
status addAgainstMe(PokemonType *A, PokemonType *B){
	if(A==NULL || B == NULL)
		return failure;

	if(A->against_me_counter == 0){//need to creat new memory
		A->effective_against_me = (PokemonType**)malloc(sizeof(PokemonType*));
	}
	else{
		A->effective_against_me = (PokemonType**)realloc(A->effective_against_me,
				(A->against_me_counter+1)*sizeof(PokemonType*));
	}
	if(A->effective_against_me == NULL){
				return nomemory;
	}
	(A->effective_against_me)[A->against_me_counter] = &(*B);
	A->against_me_counter++;

	return success;
}

/*
 * while pointers are good
 * if effective list is empty create new memory for types array
 * else making the list bigger by one type and adding it
 * returns status of operation
 */
status addAgainstOthers(PokemonType *A, PokemonType *B){
	if(A==NULL || B == NULL)
		return failure;

	if(A->against_others_counter == 0){//need to creat new memory
		A->effective_against_others = (PokemonType**)malloc(sizeof(PokemonType*));
	}
	else{//make more space for new type
		A->effective_against_others = (PokemonType**)realloc(A->effective_against_others,
				(A->against_others_counter+1)*sizeof(PokemonType*));
	}
	if(A->effective_against_others == NULL){
				return nomemory;
	}
	(A->effective_against_others)[A->against_others_counter] = &(*B);
	A->against_others_counter++;

	return success;
}

/*
 * prtins all Pokemon information
 */
status printPokemon(Pokemon *pokemon){
	if(pokemon == NULL)
		return failure;
	printf("%s :\n", pokemon->name);
	printf("%s, %s Type.\n", pokemon->species,pokemon->ppt->name);
	printf("Height: %.2lf m    Weight: %.2lf kg    Attack: %d", pokemon->ppb->height, pokemon->ppb->weight, pokemon->ppb->attack);
	return success;
}


/*
 * prints all Type information
 */
status printType(PokemonType *Type){
	if(Type == NULL)
		return failure;
	printf("Type %s -- %d pokemons\n",Type->name, Type->count_same_type);
	printEffectivness(Type->effective_against_me,Type->against_me_counter,Type->name,1);
	printEffectivness(Type->effective_against_others,Type->against_others_counter,Type->name,2);
	return success;
}

/*print effectiveness lists,
 * me_other determine if its effective_against_me list(=1)
 * or effective_against_other(not =1) list
 */
status printEffectivness(PokemonType** Type, int numOfEffective,char *name,int me_other){
	int i;
	if(numOfEffective != 0){
		if(me_other == 1){
			printf("	These types are super-effective against %s:",name);
			for(i=0; i < numOfEffective; i++){
				if(i < numOfEffective - 1)
					printf("%s ,",Type[i]->name);
				else
					printf("%s\n",Type[i]->name);
			}
		}
		else{
			printf("	%s moves are super-effective against:",name);
			for(i=0; i < numOfEffective; i++){
				if(i < numOfEffective - 1)
					printf("%s ,",Type[i]->name);
				else
					printf("%s\n",Type[i]->name);
			}
		}
	}
	return success;
}

/* search for "nameToGet type in array */
PokemonType* getType(PokemonType** typesArray, int numOfTypes,char* nameToGet){
	int i;
	for(i=0;i<numOfTypes;i++){
		if(strcmp(typesArray[i]->name,nameToGet)==0)
			return typesArray[i];
	}
	return NULL;
}


Pokemon* copyFunc(Pokemon* e){
	Pokemon *pokemonTosend = e;
	return pokemonTosend;
}


char* getTypeFunc(Pokemon* e){
	return e->ppt->name;
}


int equalPokemon(Pokemon* p1, Pokemon* p2){
	if(p1 == NULL || p2 == NULL)
		return 0;
	int attack1 = p1->ppb->attack;
	int attack2 = p2->ppb->attack;
	if(attack1 > attack2)
		return 1;
	if(attack1 < attack2)
		return -1;
	return 0;
}

/*
 * calculate the subtraction of two pokemons attack's
 *
 */
int getAttackFunc(Pokemon* sysPokemon ,Pokemon *oppPokemon ,int* sysAttack,int* oppAttack){
	*sysAttack = sysPokemon->ppb->attack;
	*oppAttack = oppPokemon->ppb->attack;

	PokemonType *sysType = sysPokemon->ppt;
	PokemonType *oppType = oppPokemon->ppt;

	PokemonType *sysMeEffectiveness = getType(sysType->effective_against_me, sysType->against_me_counter, oppType->name);
	PokemonType *oppMeEffectiveness = getType(oppType->effective_against_me, oppType->against_me_counter, sysType->name);

	PokemonType *sysOthersEffectiveness = getType(sysType->effective_against_others, sysType->against_others_counter, oppType->name);
	PokemonType *oppOthersEffectiveness = getType(oppType->effective_against_others, oppType->against_others_counter, sysType->name);

	/*
	 * if sys is effective against opp -> oppattack-=10
	 * if opp is effective against sys -> sysattack-=10
	 */
	bool sysBoth = false;
	bool oppBoth = false;

	if(sysMeEffectiveness != NULL && oppOthersEffectiveness != NULL){
		*sysAttack -= 10;
		sysBoth = true;
	}
	if(oppMeEffectiveness != NULL && sysOthersEffectiveness != NULL){
		*oppAttack -= 10;
		oppBoth = true;
	}

	if(sysBoth == false){
		if(sysMeEffectiveness != NULL){
			*sysAttack -= 10;
		}
		if(oppOthersEffectiveness != NULL)
			*sysAttack -= 10;
	}

	if(oppBoth == false){
		if(oppMeEffectiveness != NULL){
			*oppAttack -= 10;
		}
		if(sysOthersEffectiveness != NULL)
			*oppAttack -= 10;
	}

	return *sysAttack - *oppAttack;
}

