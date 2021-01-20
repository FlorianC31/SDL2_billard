#include "boules.h"

void NewBoule(Boule* boule, Color color, int x, int y, int radius){
    boule->color = color;
    boule->position.x = x;
    boule->position.y = y;
    boule->speed.x = 0;
    boule->speed.y = 0;
    boule->radius = radius;
}


void DefineQueue(Queue* queue, Boule* boule, Color color, int length, float angle){
    queue->color = color;
    queue->boule = boule;
    queue->length = length;
    queue->angle = angle;
    queue->force = 0;
    queue->displayed = 1;
    queue->vect.x = 0;
    queue->vect.y = 0;
}


void DrawQueue(Queue* queue, SDL_Renderer* renderer){

    // Calcul de la position des 2 points de la queue

    Coord point_A, point_B;
    point_A.x = queue->boule->position.x + (queue->boule->radius + queue->force) * cos(queue->angle);
    point_A.y = queue->boule->position.y + (queue->boule->radius + queue->force) * sin(queue->angle);

    point_B.x = queue->boule->position.x + (queue->boule->radius + queue->force + queue->length) * cos(queue->angle);
    point_B.y = queue->boule->position.y + (queue->boule->radius + queue->force + queue->length) * sin(queue->angle);

    queue->vect.x = point_B.x - point_A.x;
    queue->vect.y = point_B.y - point_A.y;


    SDL_SetRenderDrawColor(renderer, queue->color.r, queue->color.g, queue->color.b, 255);

    Coord unit_vect, local_position;
    UnitVect(&unit_vect, queue->vect);
    for (int i = 0; i <= VectResult(queue->vect); i++){
        ScalProdFloat(&local_position, unit_vect, i);
        SDL_RenderDrawPoint(renderer, point_A.x + (int)local_position.x, point_A.y + (int)local_position.y);
    }

}



void MoveQueue(Queue* queue, int d_init, Coord click_pos){
    Coord delta;
    delta.x = click_pos.x - queue->boule->position.x;
    delta.y = click_pos.y - queue->boule->position.y;

    queue->force = VectResult(delta) - d_init;
    if (queue->force < 0)
        queue->force = 0;
    queue->angle = atan2f(delta.y, delta.x);

}


void ShotQueue(Queue* queue){
    queue->displayed = 0;
    Coord vect_unit;
    UnitVect(&vect_unit, queue->vect);
    int factor = 100;

    queue->boule->speed.x = - vect_unit.x * (float)(queue->force) / factor;
    queue->boule->speed.y = - vect_unit.y * (float)(queue->force) / factor;

    queue->force = 0;
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

void MoveBoule(Boule* boule, int delta_t_int){

    float delta_t = (float)delta_t_int;

    printf("Test delta t = %f - %d\n", delta_t, delta_t_int);

    // Calcul des frotements
    float k1 = 1.0, k2 = 0.05, treshold = 0.001;

    float acceleration = k1 * pow(VectResult(boule->speed), 2) + k2;

    Coord vect_unit;
    UnitVect(&vect_unit, boule->speed);

    PrintVect("V0", boule->speed);
    printf("A : %f\n", acceleration);

    // Calcul des nouvelles vitesses
    boule->speed.x -= vect_unit.x * acceleration * delta_t / 1000;
    boule->speed.y -= vect_unit.y * acceleration * delta_t / 1000;

    PrintVect("V1", boule->speed);


    // Si la vitesse passe sous la valeur de seuil, on arrête la boule
    if (VectResult(boule->speed) <= treshold){
        boule->speed.x = 0;
        boule->speed.y = 0;
    }

    PrintVect("V2", boule->speed);
    printf("delta t = %f\n\n", delta_t);

    // Calcul des déplacements
    boule->position.x += boule->speed.x * delta_t;
    boule->position.y += boule->speed.y * delta_t;
}


void GetVect(Coord* vect, Boule boule1, Boule boule2){
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


        /*PrintVect("Vitesse A initiale", boule1->speed);
        PrintVect("Vitesse B initiale", boule2->speed);

        PrintVect("Vecteur AB", vect_U);*/

        // Projection de la vitesse de chaque boule sur le vecteur entre les 2 boules
        ProjVect(&vect_UA, boule1->speed, vect_U);
        ProjVect(&vect_UB, boule2->speed, vect_U);

        /*PrintVect("Vitesse A projetée", vect_UA);
        PrintVect("Vitesse B projetée", vect_UB);*/

        // Ajout de la vitesse projeté de la boule B sur la boule A et vice versa
        boule1->speed.x += vect_UB.x - vect_UA.x;
        boule1->speed.y += vect_UB.y - vect_UA.y;
        boule2->speed.x += vect_UA.x - vect_UB.x;
        boule2->speed.y += vect_UA.y - vect_UB.y;

        /*PrintVect("Vitesse A corrigée", boule1->speed);
        PrintVect("Vitesse B corrigée", boule2->speed);*/


        // Déplacement des boules pour compenser les clashs
        Coord vect_unit, vect_delta;
        UnitVect(&vect_unit, vect_U);
        ScalProdFloat(&vect_delta, vect_unit, delta_d / 2);

        boule1->position.x -= vect_delta.x;
        boule1->position.y -= vect_delta.y;

        boule2->position.x += vect_delta.x;
        boule2->position.y += vect_delta.y;

        GetVect(&vect_U, *boule1, *boule2);
        //float delta_d2 = boule1->radius + boule2->radius - VectResult(vect_U);
        //printf("%f -> %f\n", delta_d, delta_d2);

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
