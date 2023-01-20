/*
 * Pokemon.h
 *
 * createPokemon returns NULL if memory problem occurs
 */

#ifndef POKEMON_H_
#define POKEMON_H_

#include "Defs.h"

typedef struct PokemonType_t
{
	char *name;
	int count_same_type;
	struct PokemonType_t** effective_against_me;
	int against_me_counter;
	struct PokemonType_t** effective_against_others;
	int against_others_counter;
}PokemonType;

typedef struct PokemonBio_t
{
	double height;
	double weight;
	int attack;
}PokemonBio;

typedef struct Pokemon_t
{
	char* name;
	char* species;
	PokemonType* ppt;
	PokemonBio* ppb;
}Pokemon;

Pokemon* createPokemon(char *pName, char *species, PokemonType* pt, double height, double weight, int attack);
PokemonType* creatPokemonType(char* tname);
PokemonBio* biologicalInfo(double height, double weight, int attack);
status addAgainstMe(PokemonType *A, PokemonType *B);
status addAgainstOthers(PokemonType *A, PokemonType *B);
status printPokemon(Pokemon *pokemon);
status printType(PokemonType *Type);
status printEffectivness(PokemonType** Type, int numOfEffective,char *name,int me_other);
status deletePokemon(Pokemon* p);
status deleteType(PokemonType* pt);
status deleteBio(PokemonBio* pb);
PokemonType* getType(PokemonType** typesArray, int numOfTypes,char* nameToGet);
char* getTypeFunc(Pokemon* e);
int equalPokemon(Pokemon* p1, Pokemon* p2);
Pokemon* copyFunc(Pokemon* e);
int getAttackFunc(Pokemon* sysPokemon ,Pokemon* oppPokemon ,int* sysAttack,int* oppAttack);
#endif /* POKEMON_H_ */
