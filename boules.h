#ifndef BOULES_H_INCLUDED
#define BOULES_H_INCLUDED

#include "struct.h"


typedef struct BOULE{
    Color color;
    Coord position;
    Coord speed;
    int radius;
}Boule;

typedef struct QUEUE{
    Boule* boule;
    Coord vect;
    Color color;
    int length, force, displayed;
    float angle;
}Queue;

typedef struct WALL{
    Coord point_A, point_B, vect_u;
    int length;
}Wall;

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

void NewWall(Wall*, int, int, int, int);

void ContactTable(Boule*, Wall[]);

void ContactWallListe(Boule*, Wall[], int[], int);

int ContactWall(Boule*, Wall);


#endif // BOULES_H_INCLUDED
