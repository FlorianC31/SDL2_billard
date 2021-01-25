#ifndef BOULES_H_INCLUDED
#define BOULES_H_INCLUDED

#include "struct.h"



void DrawBoule(Boule, SDL_Renderer*);

void NewBoule(Boule*, Color, int, int, int);

void MoveBoule(Boule*, int);

int ContactBoule(Boule*, Boule*);

//int ContactWall(Boule*);

void GetVect(Coord*, Boule, Boule);

void DefineQueue(Queue*, Boule*, Color, int, float);

void DrawQueue(Queue*, SDL_Renderer*);

void ShotQueue(Queue*);

void MoveQueue(Queue*, int, Coord);

void ContactHole(Boule*, Boule);


#endif // BOULES_H_INCLUDED
