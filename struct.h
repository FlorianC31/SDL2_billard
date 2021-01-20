#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <SDL2/SDL.h>
#include <math.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define NB_BOULES 16

typedef struct COLOR{
    int r,g,b;
}Color;

typedef struct COORD{
    float x, y;
}Coord;


float VectResult(Coord);

float ScalProdVect(Coord, Coord);

void ScalProdFloat(Coord*, Coord, float);

void UnitVect(Coord*, Coord);

void ProjVect(Coord*, Coord, Coord);

void PrintVect(char title[], Coord);

#endif // STRUCT_H_INCLUDED
