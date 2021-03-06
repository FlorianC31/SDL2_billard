#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "struct.h"
#include "boules.h"
#include "wall.h"


Color WHITE = {255, 255, 255};
Color BLACK = {0, 0, 0};
Color YELLOW = {255, 255, 0};
Color RED = {255, 0, 0};
Color BLUE = {0, 0, 255};
Color GREEN = {0, 255, 0};
Color GREY = {127, 127, 127};

void CreationBoules(Boule[]);
void CreationHoles(Boule[]);
void CreationWalls(Wall[]);

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;
    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }


    // creation des objets

    SDL_Surface* fond = IMG_Load("ressources/fond.png");
    if(!fond)
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }
    SDL_Texture* background = SDL_CreateTextureFromSurface(pRenderer,fond);
    SDL_Rect bg_position = {0, 0};
    SDL_QueryTexture(background, NULL, NULL, &bg_position.w, &bg_position.h);

    Boule boules[NB_BOULES], holes[6];
    Wall walls[NB_WALLS];
    CreationBoules(boules);
    CreationHoles(holes);
    CreationWalls(walls);
    Queue queue;
    int queue_length = 200;
    DefineQueue(&queue, &boules[0], BLACK, queue_length, 45);


    int last_time = SDL_GetTicks();
    SDL_Event events;
    int run = 1;
    int nb_contact = 0;

    Coord click_pos;
    int d_init, clicked = 0;

    while (run && nb_contact>=0) {
        SDL_Delay(ITER_TIME);
        while (SDL_PollEvent(&events)) {
            switch(events.type){
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE){
                    run = 0;
                    break;
                }
            case SDL_KEYDOWN:

                switch (events.key.keysym.sym)
                {
                    case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                        run = 0;
                        break;
                    default:break;
                }

                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION: // Déplacement de souris
                if(clicked){
                    click_pos.x = events.button.x;
                    click_pos.y = events.button.y;
                    MoveQueue(&queue, d_init, click_pos);
                }
                break;
            case SDL_MOUSEBUTTONDOWN: // Click de souris
                if (queue.displayed){
                    click_pos.x = events.button.x;
                    click_pos.y = events.button.y;

                    d_init = (int)GetDistance(click_pos, boules[0].position);

                    MoveQueue(&queue, d_init, click_pos);

                    clicked = 1;
                }

                break;
            case SDL_MOUSEBUTTONUP: // Click de souris relâché
                if (queue.displayed){
                    clicked = 0;
                    ShotQueue(&queue);
                }
                break;
            case SDL_MOUSEWHEEL: // Scroll de la molette
                break;
            }
        }

        int new_time = SDL_GetTicks();

        if (new_time - last_time >= ITER_TIME){
            // Displacements
            int nb_move = 0;
            for (int i=0; i < NB_BOULES; i++){
                if (boules[i].speed.x != 0 || boules[i].speed.x !=0){
                    MoveBoule(&boules[i], new_time - last_time);
                    nb_move++;
                }
            }
            if (nb_move ==0){
                queue.displayed = 1;}

            // Contacts
            for (int i=0; i < NB_BOULES; i++){
                if (boules[i].displayed){
                    if (boules[i].speed.x != 0 || boules[i].speed.x !=0)
                        ContactTable(&boules[i], walls);
                    for (int j = i+1; j < NB_BOULES; j++){
                        if (boules[i].speed.x != 0 || boules[i].speed.x !=0 || boules[j].speed.y != 0 || boules[j].speed.y !=0)
                            nb_contact += ContactBoule(&boules[i], &boules[j]);
                    }
                    for (int j = 0; j < 6; j++){
                        ContactHole(&boules[i], holes[j]);
                    }
                }
            }


            last_time = new_time;


            // Rendering

            //SDL_SetRenderDrawColor(pRenderer, GREEN.r, GREEN.g, GREEN.b, 255);
            //SDL_RenderClear(pRenderer);

            SDL_RenderCopy(pRenderer, background, NULL, &bg_position);



            //    DrawBoule(holes[i], pRenderer);}
            for (int i=0; i < NB_BOULES; i++){
                if (boules[i].displayed){
                    DrawBoule(boules[i], pRenderer);
                }
            }

            if (queue.displayed)
                DrawQueue(&queue, pRenderer);

            //for (int i=0; i < NB_WALLS; i++){
            //    DrawWall(walls[i], pRenderer, 0);}

            //for (int i=0; i < 6; i++){
            //    DrawBoule(holes[i], pRenderer);}

            SDL_RenderPresent(pRenderer);
        }
    }


    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}


void CreationBoules(Boule boules[]){
    int radius = 14;

    NewBoule(&boules[0], WHITE, 800, 300, radius);
    //boules[0].speed.x = -2;

    int x = 300, y = 300;
    NewBoule(&boules[1], YELLOW, x, y, radius);

    x -= radius * sqrt(3);
    NewBoule(&boules[2], RED, x, y - radius, radius);
    NewBoule(&boules[3], YELLOW, x, y + radius, radius);

    x -= radius * sqrt(3);
    NewBoule(&boules[4], YELLOW, x, y - radius * 2, radius);
    NewBoule(&boules[5], BLACK, x, y, radius);
    NewBoule(&boules[6], RED, x, y + radius * 2, radius);

    x -= radius * sqrt(3);
    NewBoule(&boules[7], RED, x, y - radius * 3, radius);
    NewBoule(&boules[8], YELLOW, x, y - radius, radius);
    NewBoule(&boules[9], RED, x, y + radius, radius);
    NewBoule(&boules[10], YELLOW, x, y + radius * 3, radius);

    x -= radius * sqrt(3);
    NewBoule(&boules[11], YELLOW, x, y - radius * 4, radius);
    NewBoule(&boules[12], RED, x, y - radius * 2, radius);
    NewBoule(&boules[13], RED, x, y, radius);
    NewBoule(&boules[14], YELLOW, x, y + radius * 2, radius);
    NewBoule(&boules[15], RED, x, y + radius *4, radius);
}


void CreationHoles(Boule holes[]){
    int radius = 34;

    NewBoule(&holes[0], BLUE, 62, 57, radius);
    NewBoule(&holes[1], BLUE, 633, 37, radius);
    NewBoule(&holes[2], BLUE, 1204, 57, radius);
    NewBoule(&holes[3], BLUE, 62, 648, radius);
    NewBoule(&holes[4], BLUE, 633 ,668, radius);
    NewBoule(&holes[5], BLUE, 1204, 648, radius);
}

void CreationWalls(Wall wall[]){

    NewWall(&wall[0], 73, 106, 73, 599);
    NewWall(&wall[1], 73, 599, 41, 633);
    NewWall(&wall[2], 73, 666, 110, 633);
    NewWall(&wall[3], 110, 633, 603, 633);
    NewWall(&wall[4], 603, 633, 609, 651);
    NewWall(&wall[5], 658, 651, 664, 633);
    NewWall(&wall[6], 664, 633, 1157, 633);
    NewWall(&wall[7], 1157, 633, 1194, 666);
    NewWall(&wall[8], 1226, 633, 1194, 599);
    NewWall(&wall[9], 1194, 599, 1194, 106);
    NewWall(&wall[10], 41, 72, 73, 106);
    NewWall(&wall[11], 110, 72, 73, 39);
    NewWall(&wall[12], 603, 72, 110, 72);
    NewWall(&wall[13], 609, 54, 603, 72);
    NewWall(&wall[14], 664, 72, 658, 54);
    NewWall(&wall[15], 1157, 72, 664, 72);
    NewWall(&wall[16], 1194, 39, 1157, 72);
    NewWall(&wall[17], 1194, 106, 1226, 72);
}


