#include "boules.h"

void NewBoule(Boule* boule, Color color, int x, int y, int radius){
    boule->color = color;
    boule->position.x = x;
    boule->position.y = y;
    boule->speed.x = 0;
    boule->speed.y = 0;
    boule->radius = radius;
}




void DrawBoule(Boule boule, SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, boule.color.r, boule.color.g, boule.color.b, 255);

    for (int x = - boule.radius; x <= boule.radius; x++){
        int limit_y = sqrt(pow(boule.radius,2) - pow(x, 2));
        for (int y = - limit_y; y <= limit_y; y++){
            SDL_RenderDrawPoint(renderer, x + boule.position.x, y + boule.position.y);
        }
    }
}

void MoveBoule(Boule* boule, int delta_t){
        boule->position.x += boule->speed.x * delta_t;
        boule->position.y += boule->speed.y * delta_t;
}


/*
void ContactBoule(Boule* boule1, Boule* boule2){



}

void ContactWall(Boule* boule, )*/
