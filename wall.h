#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED

#include "struct.h"



void ContactWallListe(Boule*, Wall[], int[], int);

int ContactWall(Boule*, Wall);

void DrawWall(Wall, SDL_Renderer*, int);

void NewWall(Wall*, int, int, int, int);

void ContactTable(Boule*, Wall[]);


#endif // WALL_H_INCLUDED
