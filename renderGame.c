#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "./game.h"
#include "./renderGame.h"

const SDL_Color GRILLE_COLOR = { .r = 0, .g = 0, .b = 0 };
const SDL_Color X_COLOR = { .r = 255, .g = 50, .b = 50 };
const SDL_Color O_COLOR = { .r = 50, .g = 100, .b = 255 };
const SDL_Color DRAWN_COLOR = { .r = 100, .g = 100, .b = 100 };


void render_x(SDL_Renderer *renderer,int row, int column,
              const SDL_Color *color)
{
    const float interior_box = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
    const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

    thickLineRGBA(renderer,
                  center_x - interior_box,
                  center_y - interior_box,
                  center_x + interior_box,
                  center_y + interior_box,
                  10,
                  color->r,
                  color->g,
                  color->b,
                  255);
    thickLineRGBA(renderer,
                  center_x + interior_box,
                  center_y - interior_box,
                  center_x - interior_box,
                  center_y + interior_box,
                  10,
                  color->r,
                  color->g,
                  color->b,
                  255);
}

void render_o(SDL_Renderer *renderer,
              int row, int column,
              const SDL_Color *color)
{
    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
    const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

    filledCircleRGBA(renderer,
                     center_x, center_y, half_box_side + 5,
                     color->r, color->g, color->b, 255);
    filledCircleRGBA(renderer,
                     center_x, center_y, half_box_side - 5,
                     255, 255, 255, 255);
}


void render_grille(SDL_Renderer *renderer, const SDL_Color *color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    for (int i = 1; i < N; ++i) {
        SDL_RenderDrawLine(renderer,i * CELL_WIDTH, 0,i * CELL_WIDTH, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer,0,i * CELL_HEIGHT,SCREEN_WIDTH, i * CELL_HEIGHT);
    }
}


void render_board(SDL_Renderer *renderer,const int *board,
                  const SDL_Color *x_color,const SDL_Color *o_color)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            switch (board[i * N + j]) {
            case PLAYER_X:
                render_x(renderer, i, j, x_color);
                break;

            case PLAYER_O:
                render_o(renderer, i, j, o_color);
                break;

            default: {}
            }
        }
    }
}


void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
    render_grille(renderer, &GRILLE_COLOR);
    render_board(renderer,game->board,&X_COLOR,&O_COLOR);
}


void render_end_game(SDL_Renderer *renderer,const game_t *game,const SDL_Color *color)
{
    render_grille(renderer, color);
    render_board(renderer,game->board,color,color);
}

void render_game(SDL_Renderer *renderer, const game_t *game)
{
    switch (game->state) {
    case PLAYING_STATE:
        render_running_state(renderer, game);
        break;

    case X_WIN_STATE:
        render_end_game(renderer, game, &X_COLOR);
        break;

    case O_WIN_STATE:
        render_end_game(renderer, game, &O_COLOR);
        break;

    case DRAWN_STATE:
        render_end_game(renderer, game, &DRAWN_COLOR);
        break;

    default: {}
    }
}
