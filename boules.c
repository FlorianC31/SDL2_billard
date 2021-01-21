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

    // Calcul des frotements
    float k1 = 1.0, k2 = 0.05, treshold = 0.001;

    float acceleration = k1 * pow(VectResult(boule->speed), 2) + k2;

    Coord vect_unit;
    UnitVect(&vect_unit, boule->speed);

    // Calcul des nouvelles vitesses
    boule->speed.x -= vect_unit.x * acceleration * delta_t / 1000;
    boule->speed.y -= vect_unit.y * acceleration * delta_t / 1000;



    // Si la vitesse passe sous la valeur de seuil, on arrête la boule
    if (VectResult(boule->speed) <= treshold){
        boule->speed.x = 0;
        boule->speed.y = 0;
    }

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



/*int ContactWall(Boule* boule){

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

}*/



void NewWall(Wall* wall, int xa, int ya, int xb, int yb){
    wall->point_A.x = xa;
    wall->point_A.y = ya;
    wall->point_B.x = xb;
    wall->point_B.y = yb;
    wall->length = sqrt(pow(xb - xa, 2) + pow(yb - ya, 2));
    wall->vect_u.x = (xb - xa) / wall->length ;
    wall->vect_u.y = (yb - ya) / wall->length ;
}



void ContactTable(Boule* boule, Wall walls[]){

    // Murs à gauche
    if (boule->position.x - boule->radius < walls[0].point_A.x){
        int list[3] = {0, 1, 10};
        int nb_wall = 3;
        ContactWallListe(boule, walls, list, nb_wall);
    }
    // Murs à droite
    else if (boule->position.x + boule->radius > walls[9].point_A.x){
        int list[3] = {9, 8, 17};
        int nb_wall = 3;
        ContactWallListe(boule, walls, list, nb_wall);
    }
    // Murs en haut
    else if (boule->position.y - boule->radius < walls[12].point_A.y){
        int list[6] = {12, 15, 11, 13, 14 ,16};
        int nb_wall = 6;
        ContactWallListe(boule, walls, list, nb_wall);
    }
    // Murs en bas
    else if (boule->position.y + boule->radius > walls[3].point_A.y){
        int list[6] = {3, 6, 2, 4, 5 ,7};
        int nb_wall = 6;
        ContactWallListe(boule, walls, list, nb_wall);
    }

}

void ContactWallListe(Boule* boule, Wall walls[], int list[], int nb_wall){
    for (int i = 0; i < nb_wall; i++){
        if (ContactWall(boule, walls[list[i]])){
            break;
        }
    }
}


int ContactWall(Boule* boule, Wall wall){

    // Vérification si il y a contact
    Coord vect_boule;
    vect_boule.x = boule->position.x - wall.point_A.x;
    vect_boule.y = boule->position.y - wall.point_A.y;

    // projection de la position de la boule sur le vecteur unitaire de la ligne (mur)
    float projection = ScalProdVect(vect_boule, wall.vect_u);

    // Si la projection de la boule est sur la ligne
    if (projection>=0 && projection <= wall.length){

        float distance = sqrt(pow(VectResult(vect_boule), 2)- pow(projection, 2));

        // Calcul de la position du point de contact
        Coord contact;
        contact.x = wall.vect_u.x * projection + wall.point_A.x;
        contact.y = wall.vect_u.y * projection + wall.point_A.y;

        //float distance

        // Si la distance entre la boule et la ligne est inférieure au rayon de la boule -> alors contact
        if (distance <= boule->radius){

            // Calcul du vecteur normal unitaire à la ligne passant par le centre de la boule
            Coord vect_norm;
            vect_norm.x = boule->position.x - contact.x;
            vect_norm.y = boule->position.y - contact.y;
            float d = VectResult(vect_norm);
            vect_norm.x = vect_norm.x / d;
            vect_norm.y = vect_norm.y / d;

            // Replacement de la boule en contact pour compenser le clash dans le mur
            boule->position.x = contact.x + vect_norm.x * boule->radius;
            boule->position.y = contact.y + vect_norm.y * boule->radius;

            // Projection de la vitesse de la boule sur le vecteur normal unitaire
            Coord proj_speed;
            ProjVect(&proj_speed, boule->speed, vect_norm);

            // Correction de la vitesse de la boule après contact
            boule->speed.x -= proj_speed.x * 2;
            boule->speed.y -= proj_speed.y * 2;

            return 1;

        }

    }

    return 0;

}

