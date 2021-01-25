#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define ITER_TIME 30
#define SCREEN_WIDTH 1267
#define SCREEN_HEIGHT 706
#define NB_BOULES 16
#define NB_WALLS 18

typedef struct COLOR{
    int r,g,b;
}Color;

typedef struct COORD{
    float x, y;
}Coord;


typedef struct BOULE{
    Color color;
    Coord position, speed;
    int radius, displayed;
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
    float length;
}Wall;



float Degres(float);

float Radians(float);

float VectResult(Coord);

float ScalProdVect(Coord, Coord);

void ScalProdFloat(Coord*, Coord, float);

void UnitVect(Coord*, Coord);

void ProjVect(Coord*, Coord, Coord);

void PrintVect(char title[], Coord);

float GetDistance(Coord, Coord);

float Signe(float);

#endif // STRUCT_H_INCLUDED
