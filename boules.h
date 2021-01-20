#ifndef BOULES_H_INCLUDED
#define BOULES_H_INCLUDED

#include "struct.h"


typedef struct BOULE{
    Color color;
    Coord position;
    Coord speed;
    int radius;
}Boule;

void DrawBoule(Boule, SDL_Renderer*);

void NewBoule(Boule*, Color, int, int, int);

void MoveBoule(Boule*, int);

int ContactBoule(Boule*, Boule*);

int ContactWall(Boule*);

float GetVect(Coord*, Boule, Boule);


#endif // BOULES_H_INCLUDED
