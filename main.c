#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "./game.h"
#include "./play.h"
#include "./renderGame.h"
#include "./user.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500


int main(int argc, char *argv[])
{  
  int x=1;
  int quit=0;
        SDL_Surface *picture=NULL;
      SDL_Texture *texture=NULL;

  SDL_Surface *pictureUser=NULL;
  SDL_Texture *textureUser=NULL;
  SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 50;
    r.h = 50;  
  //initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Can't initialize sdl: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

   /*************************************************user interface********************/
   //the user interface window

   SDL_Window *window2 = SDL_CreateWindow("Tic-Tac-Toe",100, 100,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
   if (window2 == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
   //renderer

   SDL_Renderer *renderer2 = SDL_CreateRenderer(window2, -1,SDL_RENDERER_ACCELERATED); 
   if (renderer2 == NULL) {
        SDL_DestroyWindow(window2);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } 
   //upload image

    pictureUser=SDL_LoadBMP("./logmdp.bmp");
    if (pictureUser == NULL) {
        SDL_DestroyWindow(window2);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    textureUser=SDL_CreateTextureFromSurface(renderer2,pictureUser);
    SDL_FreeSurface(pictureUser);
    if (textureUser == NULL) {
        SDL_DestroyWindow(window2);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

   if(SDL_QueryTexture(textureUser,NULL,NULL,&r.w,&r.h)!=0){
        SDL_DestroyWindow(window2);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
 

   SDL_Rect T;
    T.x = 0;
    T.y = 0;
    T.w = 150;
    T.h = 150;

   char text[200]="\0";
   SDL_StartTextInput(); 
   SDL_SetTextInputRect(&T); 

   //switch window's events for the user interface
    SDL_Event event2;
    while (quit==0) {
        while (SDL_PollEvent(&event2)) {
            switch (event2.type) {
            case SDL_QUIT:
                quit=1;
                break;
            case SDL_TEXTINPUT:                 
                  strcat(text,event2.text.text);
                  for(int i;i<strlen(text);i++)
                  printf("%s",&text[i]);
                  break;
            case SDL_MOUSEBUTTONDOWN:    
                 if(357 < event2.button.y && event2.button.y < 408 && 156 < event2.button.x && event2.button.x < 344)
                  {  
                     if(login(text) == 1)
                   { x=0; 
                     quit=1; }
                  }
            break;
            default: {}
            }
        }

       SDL_RenderClear(renderer2);
       if(SDL_RenderCopy(renderer2,textureUser,NULL,&r)!=0){
           SDL_DestroyWindow(window2);
           fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
           return EXIT_FAILURE;  }
       SDL_RenderPresent(renderer2);
      }
   SDL_StopTextInput();  

  //************************************************game window********************************************************

  //create window

    if(x==0) //login true
 {
    SDL_DestroyWindow(window2);
    SDL_Window *window = SDL_CreateWindow("Tic-Tac-Toe",100, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);    
        if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //create renderer

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED |
                                                           SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //upload button image

    picture=SDL_LoadBMP("./restart.bmp");
    if (picture == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    texture=SDL_CreateTextureFromSurface(renderer,picture);
    SDL_FreeSurface(picture);
    if (texture == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(SDL_QueryTexture(texture,NULL,NULL,&r.w,&r.h)!=0){
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
   

   //initial game

    game_t game = {
        .board = { EMPTY, EMPTY, EMPTY,
                   EMPTY, EMPTY, EMPTY,
                   EMPTY, EMPTY, EMPTY },
        .player = PLAYER_X,
        .state = PLAYING_STATE
    };
   
    //switch window events for the game
    SDL_Event event;
    while (game.state!=QUIT_STATE) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                game.state=QUIT_STATE;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.y < r.h && event.button.x < r.w)
                replay_game(&game);
                else
                click(&game,event.button.y/CELL_HEIGHT,event.button.x/CELL_WIDTH);
                break;
            default: {}
            }
          
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        if(SDL_RenderCopy(renderer,texture,NULL,&r)!=0){
          SDL_DestroyWindow(window);
          fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
          return EXIT_FAILURE; }
       
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
       }
     SDL_DestroyWindow(window);
    }

    SDL_DestroyWindow(window2);
    SDL_Quit();

    return EXIT_SUCCESS;
}

