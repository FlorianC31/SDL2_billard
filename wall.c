#include "wall.h"


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


void DrawWall(Wall wall, SDL_Renderer* renderer, int draw_normal){

    Coord local_position;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = 0; i <= wall.length; i++){
        ScalProdFloat(&local_position, wall.vect_u, i);
        SDL_RenderDrawPoint(renderer, wall.point_A.x + (int)local_position.x, wall.point_A.y + (int)local_position.y);
    }

    if (draw_normal){

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        Coord vect_ortho, centre;

        // Création du vecteur orthonormal qui part vers la droite du mur (vecteur AB)
        vect_ortho.x = wall.vect_u.y;
        vect_ortho.y = -wall.vect_u.x;

        centre.x = (wall.point_A.x + wall.point_B.x ) / 2;
        centre.y = (wall.point_A.y + wall.point_B.y ) / 2;

        int d = 20;

        for (int i = 0; i <= d; i++){
            ScalProdFloat(&local_position, vect_ortho, i);
            SDL_RenderDrawPoint(renderer, centre.x + (int)local_position.x, centre.y + (int)local_position.y);
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


        Coord vect_norm;

        // Création du vecteur orthonormal qui part vers la droite du mur (vecteur AB)
        vect_norm.x = wall.vect_u.y;
        vect_norm.y = -wall.vect_u.x;

        // projection de la boule sur ce vecteur
        float distance = ScalProdVect(vect_norm, vect_boule);


        // Si la distance entre la boule et la ligne est inférieure au rayon de la boule -> alors contact
        if (distance <= boule->radius){

            // Calcul de la position du point de contact
            Coord contact;
            contact.x = wall.vect_u.x * projection + wall.point_A.x;
            contact.y = wall.vect_u.y * projection + wall.point_A.y;


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
