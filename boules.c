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


float GetVect(Coord* vect, Boule boule1, Boule boule2){
    vect->x = boule2.position.x - boule1.position.x;
    vect->y = boule2.position.y - boule1.position.y;
}




int ContactBoule(Boule* boule1, Boule* boule2){
    Coord vect_U, vect_UA, vect_UB;

    // Récupération du vecteur entre les 2 boules
    GetVect(&vect_U, *boule1, *boule2);
    float delta_d = boule1->radius + boule2->radius - VectResult(vect_U);

    // Si il y a contact
    if (delta_d > 0){


        PrintVect("Vitesse A initiale", boule1->speed);
        PrintVect("Vitesse B initiale", boule2->speed);

        PrintVect("Vecteur AB", vect_U);

        // Projection de la vitesse de chaque boule sur le vecteur entre les 2 boules
        ProjVect(&vect_UA, boule1->speed, vect_U);
        ProjVect(&vect_UB, boule2->speed, vect_U);

        PrintVect("Vitesse A projetée", vect_UA);
        PrintVect("Vitesse B projetée", vect_UB);

        // Ajout de la vitesse projeté de la boule B sur la boule A et vice versa
        boule1->speed.x += vect_UB.x - vect_UA.x;
        boule1->speed.y += vect_UB.y - vect_UA.y;
        boule2->speed.x += vect_UA.x - vect_UB.x;
        boule2->speed.y += vect_UA.y - vect_UB.y;

        PrintVect("Vitesse A corrigée", boule1->speed);
        PrintVect("Vitesse B corrigée", boule2->speed);


        // Déplacement des boules pour compenser les clashs
        Coord vect_unit, vect_delta;
        UnitVect(&vect_unit, vect_U);
        ScalProdFloat(&vect_delta, vect_unit, delta_d / 2);

        boule1->position.x -= vect_delta.x;
        boule1->position.y -= vect_delta.y;

        boule2->position.x += vect_delta.x;
        boule2->position.y += vect_delta.y;

        GetVect(&vect_U, *boule1, *boule2);
        float delta_d2 = boule1->radius + boule2->radius - VectResult(vect_U);
        printf("%f -> %f\n", delta_d, delta_d2);

        return 1;
    }

    return 0;
}



int ContactWall(Boule* boule){

    if (boule->position.x - boule->radius < 0){
        boule->position.x = boule->radius;
        boule->speed.x = - boule->speed.x;
    }

    if (boule->position.y - boule->radius < 0){
        boule->position.y = boule->radius;
        boule->speed.y = - boule->speed.y;
    }

    if (boule->position.x + boule->radius > SCREEN_WIDTH){
        boule->position.x = SCREEN_WIDTH - boule->radius;
        boule->speed.x = - boule->speed.x;
    }

    if (boule->position.y + boule->radius > SCREEN_HEIGHT){
        boule->position.y = SCREEN_HEIGHT - boule->radius;
        boule->speed.y = - boule->speed.y;
    }

    return 0;

}
