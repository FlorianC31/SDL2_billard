#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include "struct.h"
#include "boules.h"


Color WHITE = {255, 255, 255};
Color BLACK = {0, 0, 0};
Color YELLOW = {255, 255, 0};
Color RED = {255, 0, 0};
Color BLUE = {0, 0, 255};
Color GREEN = {0, 255, 0};
Color GREY = {127, 127, 127};

void CreationBoules(Boule[]);
void CreationHoles(Boule[]);

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


    // Boules creation
    Boule boules[NB_BOULES], holes[6];
    CreationBoules(boules);
    CreationHoles(holes);


    int last_time = SDL_GetTicks();
    SDL_Event events;
    int run = 1;
    int nb_contact = 0;

    while (run && nb_contact>=0) {
        while (SDL_PollEvent(&events)) {
            switch(events.type){
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE){
                    run = 0;
                    break;
                }
            case SDL_KEYDOWN:
                SDL_Log("+key");

                switch (events.key.keysym.sym)
                {
                    case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                        run = 0;
                        break;
                    default:break;
                }

                break;
            case SDL_KEYUP:
                SDL_Log("-key");
                break;
            case SDL_MOUSEMOTION: // Déplacement de souris
                SDL_Log("Mouvement de souris");
                break;
            case SDL_MOUSEBUTTONDOWN: // Click de souris
                SDL_Log("+clic");
                break;
            case SDL_MOUSEBUTTONUP: // Click de souris relâché
                SDL_Log("-clic");
                break;
            case SDL_MOUSEWHEEL: // Scroll de la molette
                SDL_Log("wheel");
                break;
            }
        }

        int new_time = SDL_GetTicks();

        if (new_time - last_time >= 30){
            // Displacements
            for (int i=0; i < NB_BOULES; i++){
                if (boules[i].speed.x != 0 || boules[i].speed.x !=0){
                    MoveBoule(&boules[i], new_time - last_time);
                }
            }

            // Contacts
            for (int i=0; i < NB_BOULES; i++){
                if (boules[i].speed.x != 0 || boules[i].speed.x !=0)
                    nb_contact += ContactWall(&boules[i]);
                for (int j = i+1; j < NB_BOULES; j++){
                    if (boules[i].speed.x != 0 || boules[i].speed.x !=0 || boules[j].speed.y != 0 || boules[j].speed.y !=0)
                        nb_contact += ContactBoule(&boules[i], &boules[j]);
                }
            }


            last_time = new_time;


            // Rendering
            Color* color = &GREEN;
            SDL_SetRenderDrawColor(pRenderer, color->r, color->g, color->b, 255);
            SDL_RenderClear(pRenderer);

            for (int i=0; i < NB_BOULES; i++){
                DrawBoule(boules[i], pRenderer);}
            for (int i=0; i < 6; i++){
                DrawBoule(holes[i], pRenderer);}
            SDL_RenderPresent(pRenderer);
        }
    }


    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}


void CreationBoules(Boule boules[]){
    int radius = 20;

    NewBoule(&boules[0], WHITE, 800, 300, radius);
    boules[0].speed.x=-0.5;

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
    int radius = 26;

    NewBoule(&holes[0], BLACK, 0, 0, radius);
    NewBoule(&holes[1], BLACK, SCREEN_WIDTH / 2, 0, radius);
    NewBoule(&holes[2], BLACK, SCREEN_WIDTH, 0, radius);
    NewBoule(&holes[3], BLACK, 0, SCREEN_HEIGHT, radius);
    NewBoule(&holes[4], BLACK, SCREEN_WIDTH / 2, SCREEN_HEIGHT, radius);
    NewBoule(&holes[5], BLACK, SCREEN_WIDTH, SCREEN_HEIGHT, radius);
}

